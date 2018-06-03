# Copyright (C) 2009-2017 Wander Lairson Costa
# Copyright (C) 2017-2018 Robert Wlodarczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import utils
import unittest
import devinfo
import usb.util
import usb.backend.libusb0 as libusb0
import usb.backend.libusb1 as libusb1
import usb.backend.openusb as openusb
from usb._debug import methodtrace
import time
import sys

class BackendTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.backend = backend

    @methodtrace(utils.logger)
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
            self.test_clear_halt()
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

    @methodtrace(utils.logger)
    def test_enumerate_devices(self):
        for d in self.backend.enumerate_devices():
            desc = self.backend.get_device_descriptor(d)
            if desc.idVendor == devinfo.ID_VENDOR and desc.idProduct == devinfo.ID_PRODUCT:
                self.dev = d
                return
        self.fail('PyUSB test device not found')

    @methodtrace(utils.logger)
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

    @methodtrace(utils.logger)
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

    @methodtrace(utils.logger)
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

    @methodtrace(utils.logger)
    def test_get_endpoint_descriptor(self):
        ep = self.backend.get_endpoint_descriptor(self.dev, 0, 0, 0, 0)
        self.assertEqual(ep.bLength, 7)
        self.assertEqual(ep.bDescriptorType, usb.util.DESC_TYPE_ENDPOINT)
        self.assertEqual(ep.bEndpointAddress, 0x01)
        self.assertEqual(ep.bmAttributes, 0x02)
        self.assertEqual(ep.wMaxPacketSize, 16)
        self.assertEqual(ep.bInterval, 0)

    @methodtrace(utils.logger)
    def test_open_device(self):
        self.handle = self.backend.open_device(self.dev)

    @methodtrace(utils.logger)
    def test_close_device(self):
        self.backend.close_device(self.handle)

    @methodtrace(utils.logger)
    def test_set_configuration(self):
        cfg = self.backend.get_configuration_descriptor(self.dev, 0)
        self.backend.set_configuration(self.handle, cfg.bConfigurationValue)

    @methodtrace(utils.logger)
    def test_set_interface_altsetting(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.set_interface_altsetting(self.handle,
                                              intf.bInterfaceNumber,
                                              intf.bAlternateSetting)

    @methodtrace(utils.logger)
    def test_claim_interface(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.claim_interface(self.handle, intf.bInterfaceNumber)

    @methodtrace(utils.logger)
    def test_release_interface(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.backend.release_interface(self.handle, intf.bInterfaceNumber)

    @methodtrace(utils.logger)
    def test_bulk_write_read(self):
        self.backend.set_interface_altsetting(
                self.handle,
                0,
                devinfo.INTF_BULK
            )

        self.__write_read(
                self.backend.bulk_write,
                self.backend.bulk_read,
                devinfo.EP_BULK
            )

    @methodtrace(utils.logger)
    def test_intr_write_read(self):
        self.backend.set_interface_altsetting(
                self.handle,
                0,
                devinfo.INTF_INTR
            )

        self.__write_read(
                self.backend.intr_write,
                self.backend.intr_read,
                devinfo.EP_INTR
            )

    @methodtrace(utils.logger)
    def test_iso_write_read(self):
        if utils.is_iso_test_allowed():
            self.backend.set_interface_altsetting(
                self.handle,
                0,
                devinfo.INTF_ISO)

            self.__write_read(
                self.backend.iso_write,
                self.backend.iso_read,
                devinfo.EP_ISO,
                64)

    @methodtrace(utils.logger)
    def test_clear_halt(self):
        self.backend.clear_halt(self.handle, 0x01)
        self.backend.clear_halt(self.handle, 0x81)

    @methodtrace(utils.logger)
    def test_ctrl_transfer(self):
        for data in (utils.get_array_data1(), utils.get_array_data2()):
            length = len(data) * data.itemsize
            buff = usb.util.create_buffer(length)

            ret = self.backend.ctrl_transfer(self.handle,
                                             0x40,
                                             devinfo.PICFW_SET_VENDOR_BUFFER,
                                             0,
                                             0,
                                             data,
                                             1000)
            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data) + ', ' + str(length) + ' != ' + str(ret))

            ret = self.backend.ctrl_transfer(self.handle,
                                             0xC0,
                                             devinfo.PICFW_GET_VENDOR_BUFFER,
                                             0,
                                             0,
                                             buff,
                                             1000)

            self.assertEqual(ret, length)

            self.assertEqual(buff,
                             data,
                             'Failed to read data: ' + str(data) + ' != ' + str(ret))

    @methodtrace(utils.logger)
    def test_reset_device(self):
        self.backend.reset_device(self.handle)

    def __write_read(self, write_fn, read_fn, ep, length = 8):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0).bInterfaceNumber
        for data in (utils.get_array_data1(length), utils.get_array_data2(length)):
            length = len(data) * data.itemsize

            try:
                ret = write_fn(self.handle, ep, intf, data, 1000)
            except NotImplementedError:
                return

            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep))

            buff = usb.util.create_buffer(length)

            try:
                ret = read_fn(self.handle, ep | usb.util.ENDPOINT_IN, intf, buff, 1000)
            except NotImplementedError:
                return

            self.assertEqual(ret, length, str(ret) + ' != ' + str(length))

            self.assertEqual(buff,
                             data,
                             'Failed to read data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep))

            if utils.is_windows():
                time.sleep(0.5)

def get_suite():
    suite = unittest.TestSuite()
    for m in (libusb1, libusb0, openusb):
        b = m.get_backend()
        if b is not None and utils.find_my_device(b):
            utils.logger.info('Adding %s(%s) to test suite...', BackendTest.__name__, m.__name__)
            suite.addTest(BackendTest(b))
        else:
            utils.logger.warning('%s(%s) is not available', BackendTest.__name__, m.__name__)
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
