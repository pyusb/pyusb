# Copyright (C) 2009-2014 Wander Lairson Costa
#
# The following terms apply to all files associated
# with the software unless explicitly disclaimed in individual files.
#
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.
#
# IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
# DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
# IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
# NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
# MODIFICATIONS.

# Integration tests

import utils
import unittest
import usb.core
import devinfo
import usb._interop
from usb._debug import methodtrace
import usb.util
import usb.backend.libusb0 as libusb0
import usb.backend.libusb1 as libusb1
import usb.backend.openusb as openusb
import time
import sys

def make_data_list(length = 8):
    return (utils.get_array_data1(length),
            utils.get_array_data2(length),
            utils.get_list_data1(length),
            utils.get_list_data2(length),
            utils.get_str_data1(length),
            utils.get_str_data1(length))

class DeviceTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.test_attributes()
            self.test_timeout()
            self.test_set_configuration()
            self.test_set_interface_altsetting()
            self.test_write_read()
            self.test_write_array()
            self.test_ctrl_transfer()
            self.test_clear_halt()
            #self.test_reset()
        finally:
            usb.util.dispose_resources(self.dev)

    @methodtrace(utils.logger)
    def test_attributes(self):
        self.assertEqual(self.dev.bLength, 18)
        self.assertEqual(self.dev.bDescriptorType, usb.util.DESC_TYPE_DEVICE)
        self.assertEqual(self.dev.bcdUSB, 0x0200)
        self.assertEqual(self.dev.idVendor, devinfo.ID_VENDOR)
        self.assertEqual(self.dev.idProduct, devinfo.ID_PRODUCT)
        self.assertEqual(self.dev.bcdDevice, 0x0001)
        self.assertEqual(self.dev.iManufacturer, 0x01)
        self.assertEqual(self.dev.iProduct, 0x02)
        self.assertEqual(self.dev.iSerialNumber, 0x03)
        self.assertEqual(self.dev.bNumConfigurations, 0x01)
        self.assertEqual(self.dev.bMaxPacketSize0, 8)
        self.assertEqual(self.dev.bDeviceClass, 0x00)
        self.assertEqual(self.dev.bDeviceSubClass, 0x00)
        self.assertEqual(self.dev.bDeviceProtocol, 0x00)

    @methodtrace(utils.logger)
    def test_timeout(self):
        def set_invalid_timeout():
            self.dev.default_timeout = -1
        tmo = self.dev.default_timeout
        self.dev.default_timeout = 1
        self.assertEqual(self.dev.default_timeout, 1)
        self.dev.default_timeout = tmo
        self.assertEqual(self.dev.default_timeout, tmo)
        self.assertRaises(ValueError, set_invalid_timeout)
        self.assertEqual(self.dev.default_timeout, tmo)

    @methodtrace(utils.logger)
    def test_set_configuration(self):
        cfg = self.dev[0].bConfigurationValue
        self.dev.set_configuration(cfg)
        self.dev.set_configuration()
        self.assertEqual(cfg, self.dev.get_active_configuration().bConfigurationValue)

    @methodtrace(utils.logger)
    def test_set_interface_altsetting(self):
        intf = self.dev.get_active_configuration()[(0,0)]
        self.dev.set_interface_altsetting(intf.bInterfaceNumber, intf.bAlternateSetting)
        self.dev.set_interface_altsetting()

    @methodtrace(utils.logger)
    def test_reset(self):
        self.dev.reset()
        utils.delay_after_reset()

    @methodtrace(utils.logger)
    def test_write_read(self):
        altsettings = [devinfo.INTF_BULK, devinfo.INTF_INTR]
        eps = [devinfo.EP_BULK, devinfo.EP_INTR]
        data_len = [8, 8]

        if utils.is_iso_test_allowed():
            altsettings.append(devinfo.INTF_ISO)
            eps.append(devinfo.EP_ISO)
            data_len.append(64)

        def delay(alt):
            # Hack to avoid two consecutive isochronous transfers to fail
            if alt == devinfo.INTF_ISO and utils.is_windows():
                time.sleep(0.5)

        for alt, length in zip(altsettings, data_len):
            self.dev.set_interface_altsetting(0, alt)
            for data in make_data_list(length):
                adata = utils.to_array(data)
                length = utils.data_len(data)
                buff = usb.util.create_buffer(length)

                try:
                    ret = self.dev.write(eps[alt], data)
                except NotImplementedError:
                    continue

                self.assertEqual(ret, length)

                self.assertEqual(
                    ret,
                    length,
                    'Failed to write data: ' + \
                        str(data) + ', in interface = ' + \
                        str(alt))

                try:
                    ret = self.dev.read(eps[alt] | usb.util.ENDPOINT_IN, length)
                except NotImplementedError:
                    continue

                self.assertTrue(
                    utils.array_equals(ret, adata),
                    str(ret) + ' != ' + \
                        str(adata) + ', in interface = ' + \
                        str(alt))

                delay(alt)

                try:
                    ret = self.dev.write(eps[alt], data)
                except NotImplementedError:
                    continue

                self.assertEqual(ret, length)

                self.assertEqual(
                    ret,
                    length,
                    'Failed to write data: ' + \
                        str(data) + ', in interface = ' + \
                        str(alt))

                try:
                    ret = self.dev.read(eps[alt] | usb.util.ENDPOINT_IN, buff)
                except NotImplementedError:
                    continue

                self.assertEqual(ret, length)

                self.assertTrue(
                    utils.array_equals(buff, adata),
                     str(buff) + ' != ' + \
                        str(adata) + ', in interface = ' + \
                        str(alt))

                delay(alt)

    @methodtrace(utils.logger)
    def test_write_array(self):
        a = usb._interop.as_array('test')
        self.dev.set_interface_altsetting(0, devinfo.INTF_BULK)

        self.assertEquals(self.dev.write(devinfo.EP_BULK, a), len(a))

        self.assertTrue(utils.array_equals(
            self.dev.read(devinfo.EP_BULK | usb.util.ENDPOINT_IN, len(a)),
            a))

    @methodtrace(utils.logger)
    def test_ctrl_transfer(self):
        for data in make_data_list():
            length = utils.data_len(data)
            adata = utils.to_array(data)

            ret = self.dev.ctrl_transfer(
                    0x40,
                    devinfo.PICFW_SET_VENDOR_BUFFER,
                    0,
                    0,
                    data)

            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data))

            ret = utils.to_array(self.dev.ctrl_transfer(
                        0xC0,
                        devinfo.PICFW_GET_VENDOR_BUFFER,
                        0,
                        0,
                        length))

            self.assertTrue(utils.array_equals(ret, adata),
                             str(ret) + ' != ' + str(adata))

            buff = usb.util.create_buffer(length)

            ret = self.dev.ctrl_transfer(
                    0x40,
                    devinfo.PICFW_SET_VENDOR_BUFFER,
                    0,
                    0,
                    data)

            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data))

            ret = self.dev.ctrl_transfer(
                        0xC0,
                        devinfo.PICFW_GET_VENDOR_BUFFER,
                        0,
                        0,
                        buff)

            self.assertEqual(ret, length)

            self.assertTrue(utils.array_equals(buff, adata),
                             str(buff) + ' != ' + str(adata))

    @methodtrace(utils.logger)
    def test_clear_halt(self):
        self.dev.set_interface_altsetting(0, 0)
        self.dev.clear_halt(0x01)
        self.dev.clear_halt(0x81)

class ConfigurationTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.cfg = dev[0]

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.test_attributes()
            self.test_set()
        finally:
            usb.util.dispose_resources(self.cfg.device)

    @methodtrace(utils.logger)
    def test_attributes(self):
        self.assertEqual(self.cfg.bLength, 9)
        self.assertEqual(self.cfg.bDescriptorType, usb.util.DESC_TYPE_CONFIG)
        self.assertEqual(self.cfg.wTotalLength, 78)
        self.assertEqual(self.cfg.bNumInterfaces, 0x01)
        self.assertEqual(self.cfg.bConfigurationValue, 0x01)
        self.assertEqual(self.cfg.iConfiguration, 0x00)
        self.assertEqual(self.cfg.bmAttributes, 0xC0)
        self.assertEqual(self.cfg.bMaxPower, 50)

    @methodtrace(utils.logger)
    def test_set(self):
        self.cfg.set()

class InterfaceTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev
        self.intf = dev[0][(0,0)]

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.dev.set_configuration()
            self.test_attributes()
            self.test_set_altsetting()
        finally:
            usb.util.dispose_resources(self.intf.device)

    @methodtrace(utils.logger)
    def test_attributes(self):
        self.assertEqual(self.intf.bLength, 9)
        self.assertEqual(self.intf.bDescriptorType, usb.util.DESC_TYPE_INTERFACE)
        self.assertEqual(self.intf.bInterfaceNumber, 0)
        self.assertEqual(self.intf.bAlternateSetting, 0)
        self.assertEqual(self.intf.bNumEndpoints, 2)
        self.assertEqual(self.intf.bInterfaceClass, 0x00)
        self.assertEqual(self.intf.bInterfaceSubClass, 0x00)
        self.assertEqual(self.intf.bInterfaceProtocol, 0x00)
        self.assertEqual(self.intf.iInterface, 0x00)

    @methodtrace(utils.logger)
    def test_set_altsetting(self):
        self.intf.set_altsetting()

class EndpointTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev
        intf = dev[0][(0,0)]
        self.ep_out = usb.util.find_descriptor(intf, bEndpointAddress=0x01)
        self.ep_in = usb.util.find_descriptor(intf, bEndpointAddress=0x81)

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.dev.set_configuration()
            self.test_attributes()
            self.test_write_read()
        finally:
            usb.util.dispose_resources(self.dev)

    @methodtrace(utils.logger)
    def test_attributes(self):
        self.assertEqual(self.ep_out.bLength, 7)
        self.assertEqual(self.ep_out.bDescriptorType, usb.util.DESC_TYPE_ENDPOINT)
        self.assertEqual(self.ep_out.bEndpointAddress, 0x01)
        self.assertEqual(self.ep_out.bmAttributes, 0x02)
        self.assertEqual(self.ep_out.wMaxPacketSize, 16)
        self.assertEqual(self.ep_out.bInterval, 0)

    @methodtrace(utils.logger)
    def test_write_read(self):
        self.dev.set_interface_altsetting(0, 0)
        for data in make_data_list():
            adata = utils.to_array(data)
            length = utils.data_len(data)
            buff = usb.util.create_buffer(length)

            ret = self.ep_out.write(data)
            self.assertEqual(ret, length, 'Failed to write data: ' + str(data))
            ret = self.ep_in.read(length)
            self.assertTrue(utils.array_equals(ret, adata), str(ret) + ' != ' + str(adata))

            ret = self.ep_out.write(data)
            self.assertEqual(ret, length, 'Failed to write data: ' + str(data))
            ret = self.ep_in.read(buff)
            self.assertEqual(ret, length)
            self.assertTrue(utils.array_equals(buff, adata), str(buff) + ' != ' + str(adata))

def get_suite():
    suite = unittest.TestSuite()
    test_cases = (DeviceTest, ConfigurationTest, InterfaceTest, EndpointTest)
    for m in (libusb1, libusb0, openusb):
        b = m.get_backend()
        if b is None:
            continue
        dev = utils.find_my_device(b)
        if dev is None:
            utils.logger.warning('Test hardware not found for backend %s', m.__name__)
            continue

        for ObjectTestCase in test_cases:
            utils.logger.info('Adding %s(%s) to test suite...', ObjectTestCase.__name__, m.__name__)
            suite.addTest(ObjectTestCase(dev))

    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
