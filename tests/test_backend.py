# Copyright 2009-2017 Wander Lairson Costa
# Copyright 2009-2021 PyUSB contributors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import utils
import unittest
from types import SimpleNamespace
from unittest.mock import Mock, call, sentinel
import devinfo
import usb.core
import usb.util
import usb.backend.libusb0 as libusb0
import usb.backend.libusb1 as libusb1
import usb.backend.openusb as openusb
from usb._debug import methodtrace
import time
import sys

class BackendTest(unittest.TestCase):
    __test__ = False
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


class AutoDetachKernelDriverTest(unittest.TestCase):
    def setUp(self):
        self.library = Mock()
        self.library.libusb_init.return_value = 0
        self.library.libusb_set_auto_detach_kernel_driver.return_value = 0
        self.backend = libusb1._LibUSB(self.library)
        descriptor = libusb1._libusb_device_descriptor()
        descriptor.bus = descriptor.address = descriptor.port_number = None
        descriptor.port_numbers = descriptor.speed = None
        self.backend.get_device_descriptor = Mock(return_value=descriptor)
        self.backend.open_device = Mock(return_value=SimpleNamespace(handle=sentinel.handle))
        self.backend.close_device = Mock()
        self.device = usb.core.Device(sentinel.device, self.backend)
        self.addCleanup(usb.util.dispose_resources, self.device)

    def test_toggle_uses_current_device_handle(self):
        libusb1._setup_prototypes(self.library)
        toggle = self.library.libusb_set_auto_detach_kernel_driver
        self.assertEqual(toggle.argtypes, [libusb1.c_void_p, libusb1.c_int])
        self.assertIs(toggle.restype, libusb1.c_int)

        self.device.set_auto_detach_kernel_driver(True)
        self.device.set_auto_detach_kernel_driver(False)
        self.backend.open_device.assert_called_once_with(sentinel.device)
        self.assertEqual(toggle.call_args_list, [
            call(sentinel.handle, True), call(sentinel.handle, False)
        ])

        usb.util.dispose_resources(self.device)
        self.backend.open_device.return_value = SimpleNamespace(handle=sentinel.reopened_handle)
        self.device.set_auto_detach_kernel_driver(True)
        toggle.assert_called_with(sentinel.reopened_handle, True)

    def test_older_libusb_can_load_without_optional_symbol(self):
        del self.library.libusb_set_auto_detach_kernel_driver
        libusb1._setup_prototypes(self.library)
        with self.assertRaises(NotImplementedError):
            self.device.set_auto_detach_kernel_driver(True)

    def test_libusb_error_translation(self):
        toggle = self.library.libusb_set_auto_detach_kernel_driver
        toggle.return_value = libusb1.LIBUSB_ERROR_NOT_SUPPORTED
        with self.assertRaises(NotImplementedError):
            self.device.set_auto_detach_kernel_driver(True)

        toggle.return_value = libusb1.LIBUSB_ERROR_ACCESS
        with self.assertRaises(usb.core.USBError) as error:
            self.device.set_auto_detach_kernel_driver(True)
        self.assertEqual(error.exception.backend_error_code, libusb1.LIBUSB_ERROR_ACCESS)

    def test_legacy_backend_without_method(self):
        backend = SimpleNamespace(get_device_descriptor=self.backend.get_device_descriptor)
        device = usb.core.Device(sentinel.device, backend)
        with self.assertRaises(NotImplementedError):
            device.set_auto_detach_kernel_driver(True)


def get_suite():
    suite = unittest.TestSuite()
    suite.addTests(unittest.defaultTestLoader.loadTestsFromTestCase(AutoDetachKernelDriverTest))
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
