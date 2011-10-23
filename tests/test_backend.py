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

import utils
import unittest
import devinfo
import usb.util
import usb.backend.libusb01 as libusb01
import usb.backend.libusb10 as libusb10
import usb.backend.openusb as openusb

class BackendTest(unittest.TestCase):
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.backend = backend

    def runTest(self):
        try:
            self.test_enumerate_devices()
            self.test_get_device_descriptor()
            self.test_get_configuration_descriptor()
            self.test_get_interface_descriptor()
            self.test_get_endpoint_descriptor()
            self.test_open_device()
            self.test_set_configuration()
            self.test_claim_interface()
            self.test_set_interface_altsetting()
            self.test_bulk_write_read()
            self.test_intr_write_read()
            self.test_iso_write_read()
            self.test_ctrl_transfer()
        except:
            # do this to not influence other tests upon error
            intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
            self.backend.release_interface(self.handle, intf.bInterfaceNumber)
            self.backend.close_device(self.handle)
            raise
        self.test_release_interface()
        #self.test_reset_device()
        self.test_close_device()
        #utils.delay_after_reset()

    def test_enumerate_devices(self):
        for d in self.backend.enumerate_devices():
            desc = self.backend.get_device_descriptor(d)
            if desc.idVendor == devinfo.ID_VENDOR and desc.idProduct == devinfo.ID_PRODUCT:
                self.dev = d
                return
        self.fail('PyUSB test device not found')

    def test_get_device_descriptor(self):
        dsc = self.backend.get_device_descriptor(self.dev)
        self.assertEqual(dsc.bLength, 18)
        self.assertEqual(dsc.bDescriptorType, usb.util.DESC_TYPE_DEVICE)
        self.assertEqual(dsc.bcdUSB, 0x0200)
        self.assertEqual(dsc.idVendor, devinfo.ID_VENDOR)
        self.assertEqual(dsc.idProduct, devinfo.ID_PRODUCT)
        self.assertEqual(dsc.bcdDevice, 0x0001)
        self.assertEqual(dsc.iManufacturer, 0x01)
        self.assertEqual(dsc.iProduct, 0x02)
        self.assertEqual(dsc.iSerialNumber, 0x03)
        self.assertEqual(dsc.bNumConfigurations, 0x01)
        self.assertEqual(dsc.bMaxPacketSize0, 8)
        self.assertEqual(dsc.bDeviceClass, 0x00)
        self.assertEqual(dsc.bDeviceSubClass, 0x00)
        self.assertEqual(dsc.bDeviceProtocol, 0x00)

    def test_get_configuration_descriptor(self):
        cfg = self.backend.get_configuration_descriptor(self.dev, 0)
        self.assertEqual(cfg.bLength, 9)
        self.assertEqual(cfg.bDescriptorType, usb.util.DESC_TYPE_CONFIG)
        self.assertEqual(cfg.wTotalLength, 78)
        self.assertEqual(cfg.bNumInterfaces, 0x01)
        self.assertEqual(cfg.bConfigurationValue, 0x01)
        self.assertEqual(cfg.iConfiguration, 0x00)
        self.assertEqual(cfg.bmAttributes, 0xC0)
        self.assertEqual(cfg.bMaxPower, 50)

    def test_get_interface_descriptor(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.assertEqual(intf.bLength, 9)
        self.assertEqual(intf.bDescriptorType, usb.util.DESC_TYPE_INTERFACE)
        self.assertEqual(intf.bInterfaceNumber, 0)
        self.assertEqual(intf.bAlternateSetting, 0)
        self.assertEqual(intf.bNumEndpoints, 2)
        self.assertEqual(intf.bInterfaceClass, 0x00)
        self.assertEqual(intf.bInterfaceSubClass, 0x00)
        self.assertEqual(intf.bInterfaceProtocol, 0x00)
        self.assertEqual(intf.iInterface, 0x00)

    def test_get_endpoint_descriptor(self):
        ep = self.backend.get_endpoint_descriptor(self.dev, 0, 0, 0, 0)
        self.assertEqual(ep.bLength, 7)
        self.assertEqual(ep.bDescriptorType, usb.util.DESC_TYPE_ENDPOINT)
        self.assertEqual(ep.bEndpointAddress, 0x01)
        self.assertEqual(ep.bmAttributes, 0x02)
        self.assertEqual(ep.wMaxPacketSize, 16)
        self.assertEqual(ep.bInterval, 0)

    def test_open_device(self):
        self.handle = self.backend.open_device(self.dev)

    def test_close_device(self):
        self.backend.close_device(self.handle)

    def test_set_configuration(self):
        cfg = self.backend.get_configuration_descriptor(self.dev, 0)
        self.backend.set_configuration(self.handle, 0)
        self.backend.set_configuration(self.handle, cfg.bConfigurationValue)

    def test_set_interface_altsetting(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.set_interface_altsetting(self.handle,
                                              intf.bInterfaceNumber,
                                              intf.bAlternateSetting)

    def test_claim_interface(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.claim_interface(self.handle, intf.bInterfaceNumber)

    def test_release_interface(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.release_interface(self.handle, intf.bInterfaceNumber)

    def test_bulk_write_read(self):
        self.backend.set_interface_altsetting(
                self.handle,
                0,
                devinfo.INTF_BULK
            )

        self.__write_read(
                self.backend.bulk_write,
                self.backend.bulk_read
            )

    def test_intr_write_read(self):
        self.backend.set_interface_altsetting(
                self.handle,
                0,
                devinfo.INTF_INTR
            )

        self.__write_read(
                self.backend.intr_write,
                self.backend.intr_read
            )

    def test_iso_write_read(self):
        pass

    def test_ctrl_transfer(self):
        for data in (utils.get_array_data1(), utils.get_array_data2()):
            length = len(data) * data.itemsize
            ret = self.backend.ctrl_transfer(self.handle,
                                             0x40,
                                             devinfo.PICFW_SET_VENDOR_BUFFER,
                                             0,
                                             0,
                                             data,
                                             1000)
            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data))
            ret = self.backend.ctrl_transfer(self.handle,
                                             0xC0,
                                             devinfo.PICFW_GET_VENDOR_BUFFER,
                                             0,
                                             0,
                                             length,
                                             1000)
            self.assertEqual(ret,
                             data,
                             'Failed to read data: ' + str(data) + ' != ' + str(ret))

    def test_reset_device(self):
        self.backend.reset_device(self.handle)

    def __write_read(self, write_fn, read_fn, ep_out = 0x01, ep_in = 0x81):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0).bInterfaceNumber
        for data in (utils.get_array_data1(), utils.get_array_data2()):
            length = len(data) * data.itemsize
            ret = write_fn(self.handle, ep_out, intf, data, 1000)
            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep_out))
            ret = read_fn(self.handle, ep_in, intf, length, 1000)
            self.assertEqual(ret,
                             data,
                             'Failed to read data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep_in))

def get_suite():
    suite = unittest.TestSuite()
    for m in (libusb10, libusb01, openusb):
        b = m.get_backend()
        if b is not None and utils.find_my_device(b):
            utils.logger.info('Adding %s(%s) to test suite...', BackendTest.__name__, m.__name__)
            suite.addTest(BackendTest(b))
        else:
            utils.logger.warning('%s(%s) is not available', BackendTest.__name__, m.__name__)
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
