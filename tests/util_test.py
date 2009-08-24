import unittest
from usb.util import *
from device_info import *

class UtilTestCase(unittest.TestCase):
    def runTest(self):
        self.test_endpoint_address()
        self.test_endpoint_direction()
        self.test_endpoint_type()
        self.test_ctrl_direction()
    def test_endpoint_address(self):
        self.assertEqual(endpoint_address(EP_BULK_OUT), 0x01)
        self.assertEqual(endpoint_address(EP_BULK_IN), 0x01)
    def test_endpoint_direction(self):
        self.assertEqual(endpoint_direction(EP_BULK_OUT), ENDPOINT_OUT)
        self.assertEqual(endpoint_direction(EP_BULK_IN), ENDPOINT_IN)
    def test_endpoint_type(self):
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_CTRL), ENDPOINT_TYPE_CTRL)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_ISO), ENDPOINT_TYPE_ISO)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_INTR), ENDPOINT_TYPE_INTR)
        self.assertEqual(endpoint_type(ENDPOINT_TYPE_BULK), ENDPOINT_TYPE_BULK)
    def test_ctrl_direction(self):
        self.assertEqual(ctrl_direction(CTRL_OUT), CTRL_OUT)
        self.assertEqual(ctrl_direction(CTRL_IN), CTRL_IN)

def get_testsuite():
    suite = unittest.TestSuite()
    suite.addTest(UtilTestCase())
    return suite
