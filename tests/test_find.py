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
