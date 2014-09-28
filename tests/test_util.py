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
import unittest
from usb.util import *
from devinfo import *
from usb._debug import methodtrace
import usb.backend

class _ConfigurationDescriptor(object):
    def __init__(self, bConfigurationValue):
        self.bLength = 9
        self.bDescriptorType = DESC_TYPE_CONFIG
        self.wTotalLength = 18
        self.bNumInterfaces = 0
        self.bConfigurationValue = bConfigurationValue
        self.iConfiguration = 0
        self.bmAttributes = 0xc0
        self.bMaxPower = 50

class _DeviceDescriptor(object):
    def __init__(self):
        self.configurations = (_ConfigurationDescriptor(1), _ConfigurationDescriptor(2))
        self.bLength = 18
        self.bDescriptorType = usb.util.DESC_TYPE_DEVICE
        self.bcdUSB = 0x0200
        self.idVendor = ID_VENDOR
        self.idProduct = ID_PRODUCT
        self.bcdDevice = 0x0001
        self.iManufacturer = 0
        self.iProduct = 0
        self.iSerialNumber = 0
        self.bNumConfigurations = len(self.configurations)
        self.bMaxPacketSize0 = 64
        self.bDeviceClass = 0xff
        self.bDeviceSubClass = 0xff
        self.bDeviceProtocol = 0xff

class FindDescriptorTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def runTest(self):
        d = usb.core.find(idVendor=ID_VENDOR)
        if d is None:
            return

        self.assertEqual(find_descriptor(d, bConfigurationValue=10), None)
        self.assertNotEqual(find_descriptor(d, bConfigurationValue=1), None)
        self.assertEqual(len(list(find_descriptor(d, find_all=True, bConfigurationValue=10))), 0)
        self.assertEqual(len(list(find_descriptor(d, find_all=True, bConfigurationValue=1))), 1)
        self.assertEqual(len(list(find_descriptor(d, find_all=True))), d.bNumConfigurations)
        self.assertEqual(find_descriptor(d, custom_match = lambda c: c.bConfigurationValue == 10), None)
        self.assertNotEqual(find_descriptor(d, custom_match = lambda c: c.bConfigurationValue == 1), None)
        self.assertEqual(len(list(find_descriptor(d, find_all=True, custom_match = lambda c: c.bConfigurationValue == 10))), 0)
        self.assertEqual(len(list(find_descriptor(d, find_all=True, custom_match = lambda c: c.bConfigurationValue == 1))), 1)
        self.assertEqual(find_descriptor(d, custom_match = lambda c: c.bConfigurationValue == 10, bLength=9), None)
        self.assertNotEqual(find_descriptor(d, custom_match = lambda c: c.bConfigurationValue == 1, bLength=9), None)

        cfg = find_descriptor(d)
        self.assertTrue(isinstance(cfg, usb.core.Configuration))
        intf = find_descriptor(cfg)
        self.assertTrue(isinstance(intf, usb.core.Interface))

class UtilTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def test_endpoint_address(self):
        self.assertEqual(endpoint_address(0x01), 0x01)
        self.assertEqual(endpoint_address(0x81), 0x01)

    @methodtrace(utils.logger)
    def test_endpoint_direction(self):
        self.assertEqual(endpoint_direction(0x01), ENDPOINT_OUT)
        self.assertEqual(endpoint_direction(0x81), ENDPOINT_IN)

    @methodtrace(utils.logger)
    def test_endpoint_type(self):
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_CTRL), ENDPOINT_TYPE_CTRL)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_ISO), ENDPOINT_TYPE_ISO)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_INTR), ENDPOINT_TYPE_INTR)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_BULK), ENDPOINT_TYPE_BULK)

    @methodtrace(utils.logger)
    def test_ctrl_direction(self):
        self.assertEqual(ctrl_direction(CTRL_OUT), CTRL_OUT)
        self.assertEqual(ctrl_direction(CTRL_IN), CTRL_IN)

    @methodtrace(utils.logger)
    def test_build_request_type(self):
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_DEVICE), 0x00)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_INTERFACE), 0x01)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_ENDPOINT), 0x02)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_OTHER), 0x03)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_CLASS, CTRL_RECIPIENT_DEVICE), 0x20)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_CLASS, CTRL_RECIPIENT_INTERFACE), 0x21)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_CLASS, CTRL_RECIPIENT_ENDPOINT), 0x22)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_CLASS, CTRL_RECIPIENT_OTHER), 0x23)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_DEVICE), 0x40)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_INTERFACE), 0x41)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_ENDPOINT), 0x42)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_OTHER), 0x43)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_DEVICE), 0x60)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_INTERFACE), 0x61)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_ENDPOINT), 0x62)
        self.assertEqual(build_request_type(CTRL_OUT, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_OTHER), 0x63)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_DEVICE), 0x80)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_INTERFACE), 0x81)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_ENDPOINT), 0x82)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_STANDARD, CTRL_RECIPIENT_OTHER), 0x83)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_CLASS, CTRL_RECIPIENT_DEVICE), 0xa0)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_CLASS, CTRL_RECIPIENT_INTERFACE), 0xa1)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_CLASS, CTRL_RECIPIENT_ENDPOINT), 0xa2)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_CLASS, CTRL_RECIPIENT_OTHER), 0xa3)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_DEVICE), 0xc0)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_INTERFACE), 0xc1)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_ENDPOINT), 0xc2)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_VENDOR, CTRL_RECIPIENT_OTHER), 0xc3)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_DEVICE), 0xe0)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_INTERFACE), 0xe1)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_ENDPOINT), 0xe2)
        self.assertEqual(build_request_type(CTRL_IN, CTRL_TYPE_RESERVED, CTRL_RECIPIENT_OTHER), 0xe3)

def get_suite():
    suite = unittest.TestSuite()
    suite.addTest(FindDescriptorTest())
    suite.addTest(unittest.defaultTestLoader.loadTestsFromTestCase(UtilTest))
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
