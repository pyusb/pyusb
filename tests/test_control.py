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
import struct
import usb.util
import usb.core
import usb.control
import usb.backend.libusb0 as libusb0
import usb.backend.libusb1 as libusb1
import usb.backend.openusb as openusb
from usb._debug import methodtrace
import sys

class ControlTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self, dev):
        unittest.TestCase.__init__(self)
        self.dev = dev

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.dev.set_configuration()
            self.test_getset_configuration()
            self.test_get_status()
            self.test_getset_descriptor()
            self.test_getset_interface()
            # this test case is problematic in Windows, and nobody could
            # figure out why. Let's disable it for now.
            if sys.platform not in ('win32', 'cygwin'):
                self.test_clearset_feature()
            self.test_get_string()
        finally:
            usb.util.dispose_resources(self.dev)

    @methodtrace(utils.logger)
    def test_get_status(self):
        self.assertEqual(usb.control.get_status(self.dev), 1)
        self.assertEqual(usb.control.get_status(self.dev, self.dev[0][0,0]), 0)
        self.assertEqual(usb.control.get_status(self.dev, self.dev[0][0,0][0]), 0)
        self.assertRaises(ValueError, usb.control.get_status, (self.dev, 0), 0)

    @methodtrace(utils.logger)
    def test_clearset_feature(self):
        e = self.dev[0][0,0][0]
        self.dev.set_interface_altsetting(0, 0)
        self.assertEqual(usb.control.get_status(self.dev, e), 0)
        usb.control.set_feature(self.dev, usb.control.ENDPOINT_HALT, e)
        self.assertEqual(usb.control.get_status(self.dev, e), 1)
        usb.control.clear_feature(self.dev, usb.control.ENDPOINT_HALT, e)
        self.assertEqual(usb.control.get_status(self.dev, e), 0)

    @methodtrace(utils.logger)
    def test_getset_descriptor(self):
        # TODO: test set_descriptor
        dev_fmt = 'BBHBBBBHHHBBBB'
        dev_descr = (self.dev.bLength,
                     self.dev.bDescriptorType,
                     self.dev.bcdUSB,
                     self.dev.bDeviceClass,
                     self.dev.bDeviceSubClass,
                     self.dev.bDeviceProtocol,
                     self.dev.bMaxPacketSize0,
                     self.dev.idVendor,
                     self.dev.idProduct,
                     self.dev.bcdDevice,
                     self.dev.iManufacturer,
                     self.dev.iProduct,
                     self.dev.iSerialNumber,
                     self.dev.bNumConfigurations)
        ret = usb.control.get_descriptor(
                    self.dev,
                    struct.calcsize(dev_fmt),
                    self.dev.bDescriptorType,
                    0
                )
        self.assertEqual(struct.unpack(dev_fmt, ret.tostring()), dev_descr)

    @methodtrace(utils.logger)
    def test_getset_configuration(self):
        usb.control.set_configuration(self.dev, 1)
        self.assertEqual(usb.control.get_configuration(self.dev), 1)

    @methodtrace(utils.logger)
    def test_getset_interface(self):
        i = self.dev[0][0,0]
        usb.control.set_interface(
            self.dev,
            i.bInterfaceNumber,
            i.bAlternateSetting
        )
        self.assertEqual(usb.control.get_interface(
                            self.dev,
                            i.bInterfaceNumber),
                            i.bAlternateSetting
                        )

    # Although get_string is implemented in the util module,
    # we test it here for convenience
    @methodtrace(utils.logger)
    def test_get_string(self):
        manufacturer_str = 'Travis Robinson'.encode('utf-16-le').decode('utf-16-le')
        product_str = 'Benchmark Device'.encode('utf-16-le').decode('utf-16-le')
        self.assertEqual(usb.util.get_string(self.dev, self.dev.iManufacturer), manufacturer_str)
        self.assertEqual(usb.util.get_string(self.dev, self.dev.iProduct), product_str)

def get_suite():
    suite = unittest.TestSuite()
    for m in (libusb1, libusb0, openusb):
        b = m.get_backend()
        if b is None:
            continue
        dev = utils.find_my_device(b)
        if dev is None:
            utils.logger.warning('Test hardware not found for backend %s', m.__name__)
            continue
        suite.addTest(ControlTest(dev))
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
