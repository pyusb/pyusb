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
import usb.backend
from usb.core import find
from usb._debug import methodtrace
import usb.util
import unittest
import devinfo

class _DeviceDescriptor(object):
    def __init__(self, idVendor, idProduct):
        self.bLength = 18
        self.bDescriptorType = usb.util.DESC_TYPE_DEVICE
        self.bcdUSB = 0x0200
        self.idVendor = idVendor
        self.idProduct = idProduct
        self.bcdDevice = 0x0001
        self.iManufacturer = 0
        self.iProduct = 0
        self.iSerialNumber = 0
        self.bNumConfigurations = 0
        self.bMaxPacketSize0 = 64
        self.bDeviceClass = 0xff
        self.bDeviceSubClass = 0xff
        self.bDeviceProtocol = 0xff
        self.bus = 1
        self.address = 1
        self.port_number = None
        self.port_numbers = None
        self.speed = None

# We are only interested in test usb.find() function, we don't need
# to implement all IBackend stuff
class _MyBackend(usb.backend.IBackend):
    def __init__(self):
        self.devices = [_DeviceDescriptor(devinfo.ID_VENDOR, p) for p in range(4)]
    def enumerate_devices(self):
        return range(len(self.devices))
    def get_device_descriptor(self, dev):
        return self.devices[dev]

class FindTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def test_find(self):
        b = _MyBackend()
        self.assertEqual(find(backend=b, idVendor=1), None)
        self.assertNotEqual(find(backend=b, idProduct=1), None)
        self.assertEqual(len(tuple(find(find_all=True, backend=b))), len(b.devices))
        self.assertEqual(len(tuple(find(find_all=True, backend=b, idProduct=1))), 1)
        self.assertEqual(len(tuple(find(find_all=True, backend=b, idVendor=1))), 0)

        self.assertEqual(
                len(tuple(find(
                        find_all=True,
                        backend=b,
                        custom_match = lambda d: d.idProduct==1))),
                    1)

        self.assertEqual(
                len(tuple(
                    find(
                        find_all=True,
                        backend=b,
                        custom_match = lambda d: d.idVendor==devinfo.ID_VENDOR,
                        idProduct=1))),
                    1)

def get_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.defaultTestLoader.loadTestsFromTestCase(FindTest))
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
