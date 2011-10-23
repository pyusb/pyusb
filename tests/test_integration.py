# Copyright (C) 2009-2011 Wander Lairson Costa 
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
import usb.util
import usb.backend.libusb01 as libusb01
import usb.backend.libusb10 as libusb10
import usb.backend.openusb as openusb

data_list = (utils.get_array_data1(),
             utils.get_array_data2(),
             utils.get_list_data1(),
             utils.get_list_data2(),
             utils.get_str_data1(),
             utils.get_str_data1())

class DeviceTest(unittest.TestCase):
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev

    def runTest(self):
        try:
            self.test_attributes()
            self.test_timeout()
            self.test_set_configuration()
            self.test_set_interface_altsetting()
            self.test_write_read()
            self.test_ctrl_transfer()
            #self.test_reset()
        finally:
            usb.util.dispose_resources(self.dev)

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

    def test_set_configuration(self):
        cfg = self.dev[0].bConfigurationValue
        self.dev.set_configuration(0)
        self.dev.set_configuration(cfg)
        self.dev.set_configuration()
        self.assertEqual(cfg, self.dev.get_active_configuration().bConfigurationValue)
        self.dev.set_configuration(0)
        self.assertRaises(usb.core.USBError, self.dev.get_active_configuration)
        self.dev.set_configuration()

    def test_set_interface_altsetting(self):
        intf = self.dev.get_active_configuration()[(0,0)]
        self.dev.set_interface_altsetting(intf.bInterfaceNumber, intf.bAlternateSetting)
        self.dev.set_interface_altsetting()

    def test_reset(self):
        self.dev.reset()
        utils.delay_after_reset()

    def test_write_read(self):
        altsettings = (0, 1)

        for alt in altsettings:
            self.dev.set_interface_altsetting(0, alt)
            for data in data_list:
                adata = utils.to_array(data)
                length = utils.data_len(data)
                ret = self.dev.write(0x01, data)
                self.assertEqual(ret,
                                 length,
                                 'Failed to write data: ' + \
                                    str(data) + ', in interface = ' + \
                                    str(alt)
                                )
                ret = self.dev.read(0x81, length)
                self.assertTrue(utils.array_equals(ret, adata),
                                 str(ret) + ' != ' + \
                                    str(adata) + ', in interface = ' + \
                                    str(alt)
                                )

    def test_ctrl_transfer(self):
        for data in data_list:
            length = utils.data_len(data)
            adata = utils.to_array(data)
            ret = self.dev.ctrl_transfer(
                    0x40,
                    devinfo.PICFW_SET_VENDOR_BUFFER,
                    0,
                    0,
                    data
                )
            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data))
            ret = utils.to_array(self.dev.ctrl_transfer(
                        0xC0,
                        devinfo.PICFW_GET_VENDOR_BUFFER,
                        0,
                        0,
                        length
                    ))
            self.assertTrue(utils.array_equals(ret, adata),
                             str(ret) + ' != ' + str(adata))

class ConfigurationTest(unittest.TestCase):
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.cfg = dev[0]
    def runTest(self):
        try:
            self.test_attributes()
            self.test_set()
        finally:
            usb.util.dispose_resources(self.cfg.device)
    def test_attributes(self):
        self.assertEqual(self.cfg.bLength, 9)
        self.assertEqual(self.cfg.bDescriptorType, usb.util.DESC_TYPE_CONFIG)
        self.assertEqual(self.cfg.wTotalLength, 78)
        self.assertEqual(self.cfg.bNumInterfaces, 0x01)
        self.assertEqual(self.cfg.bConfigurationValue, 0x01)
        self.assertEqual(self.cfg.iConfiguration, 0x00)
        self.assertEqual(self.cfg.bmAttributes, 0xC0)
        self.assertEqual(self.cfg.bMaxPower, 50)
    def test_set(self):
        self.cfg.set()

class InterfaceTest(unittest.TestCase):
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev
        self.intf = dev[0][(0,0)]
    def runTest(self):
        try:
            self.dev.set_configuration()
            self.test_attributes()
            self.test_set_altsetting()
        finally:
            usb.util.dispose_resources(self.intf.device)
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
    def test_set_altsetting(self):
        self.intf.set_altsetting()

class EndpointTest(unittest.TestCase):
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev
        intf = dev[0][(0,0)]
        self.ep_out = usb.util.find_descriptor(intf, bEndpointAddress=0x01)
        self.ep_in = usb.util.find_descriptor(intf, bEndpointAddress=0x81)
    def runTest(self):
        try:
            self.dev.set_configuration()
            self.test_attributes()
            self.test_write_read()
        finally:
            usb.util.dispose_resources(self.dev)
    def test_attributes(self):
        self.assertEqual(self.ep_out.bLength, 7)
        self.assertEqual(self.ep_out.bDescriptorType, usb.util.DESC_TYPE_ENDPOINT)
        self.assertEqual(self.ep_out.bEndpointAddress, 0x01)
        self.assertEqual(self.ep_out.bmAttributes, 0x02)
        self.assertEqual(self.ep_out.wMaxPacketSize, 16)
        self.assertEqual(self.ep_out.bInterval, 0)
    def test_write_read(self):
        self.dev.set_interface_altsetting(0, 0)
        for data in data_list:
            adata = utils.to_array(data)
            ret = self.ep_out.write(data)
            length = utils.data_len(data)
            self.assertEqual(ret, length, 'Failed to write data: ' + str(data))
            ret = self.ep_in.read(length)
            self.assertTrue(utils.array_equals(ret, adata), str(ret) + ' != ' + str(adata))

def get_suite():
    suite = unittest.TestSuite()
    test_cases = (DeviceTest, ConfigurationTest, InterfaceTest, EndpointTest)
    for m in (libusb10, libusb01, openusb):
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
