import unittest
import usb.core
import device_info as di
import utils
import usb.util
import sys
import usb.backend.libusb01 as libusb01
import usb.backend.libusb10 as libusb10
import usb.backend.openusb as openusb

data_list = (utils.get_array_data1(),
             utils.get_array_data2(),
             utils.get_list_data1(),
             utils.get_list_data2(),
             utils.get_str_data1(),
             utils.get_str_data1())

class FindTest(unittest.TestCase):
    def runTest(self):
        # TODO: more tests
        self.assertEqual(usb.core.find(idVendor=di.ID_VENDOR, idProduct=0xFFFF), None)
        self.assertEqual(len([i for i in usb.core.find(find_all=True, idVendor=di.ID_VENDOR, idProduct=2)]), 0)
        self.assertNotEqual(usb.core.find(idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT), None)
        self.assertEqual(len([i for i in usb.core.find(find_all=True, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)]), 1)
        self.assertEqual(usb.core.find(predicate = lambda d: d.idVendor==di.ID_VENDOR and d.idProduct==0xFFFF), None)
        self.assertNotEqual(usb.core.find(predicate = lambda d: d.idVendor==di.ID_VENDOR and d.idProduct==di.ID_PRODUCT), None)

class DeviceTest(unittest.TestCase):
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.dev = usb.core.find(backend=backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)

    def runTest(self):
        self.test_attributes()
        self.test_timeout()
        self.test_set_configuration()
        self.test_set_interface_altsetting()
        self.test_write_read()
        self.test_ctrl_transfer()
        self.test_reset()

    def test_attributes(self):
        self.assertEqual(self.dev.bLength, 18)
        self.assertEqual(self.dev.bDescriptorType, usb.util.DESC_TYPE_DEVICE)
        self.assertEqual(self.dev.bcdUSB, 0x0200)
        self.assertEqual(self.dev.idVendor, di.ID_VENDOR)
        self.assertEqual(self.dev.idProduct, di.ID_PRODUCT)
        self.assertEqual(self.dev.bcdDevice, 0x0001)
        self.assertEqual(self.dev.iManufacturer, 0x01)
        self.assertEqual(self.dev.iProduct, 0x02)
        self.assertEqual(self.dev.iSerialNumber, 0x00)
        self.assertEqual(self.dev.bNumConfigurations, 0x01)
        self.assertEqual(self.dev.bMaxPacketSize0, 64)
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
        cfg = usb.core.Configuration(self.dev).bConfigurationValue
        self.dev.set_configuration(cfg)
        self.dev.set_configuration()

    def test_set_interface_altsetting(self):
        intf = usb.core.Interface(self.dev)
        self.dev.set_interface_altsetting(intf.bInterfaceNumber, intf.bAlternateSetting)
        self.dev.set_interface_altsetting()

    def test_reset(self):
        self.dev.reset()

    def test_write_read(self):
        ep_list = ((di.EP_BULK_OUT, di.EP_BULK_IN),
                   (di.EP_INTR_OUT, di.EP_INTR_IN))

        for ep in ep_list:
            for data in data_list:
                ret = self.dev.write(ep[0], data)
                self.assertEqual(ret, len(data), 'Failed to write data: ' + str(data) + ', in EP = ' + str(ep[0]))
                ret = utils.to_array(self.dev.read(ep[1], len(data)))
                self.assertEqual(ret, utils.to_array(data), 'Failed to read data: ' + str(data) + ', in EP = ' + str(ep[1]))

    def test_ctrl_transfer(self):
        for data in data_list:
            ret = self.dev.ctrl_transfer(0x40, di.CTRL_LOOPBACK_WRITE, 0, 0, data)
            self.assertEqual(ret, len(data), 'Failed to write data: ' + str(data))
            ret = utils.to_array(self.dev.ctrl_transfer(0xC0, di.CTRL_LOOPBACK_READ, 0, 0, len(data)))
            self.assertEqual(ret, utils.to_array(data), 'Failed to read data: ' + str(data))

class ConfigurationTest(unittest.TestCase):
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.dev = usb.core.find(backend=backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)
        self.cfg = usb.core.Configuration(self.dev)
    def runTest(self):
        self.test_attributes()
        self.test_set()
    def test_attributes(self):
        self.assertEqual(self.cfg.bLength, 9)
        self.assertEqual(self.cfg.bDescriptorType, usb.util.DESC_TYPE_CONFIG)
        self.assertEqual(self.cfg.wTotalLength, 60)
        self.assertEqual(self.cfg.bNumInterfaces, 0x01)
        self.assertEqual(self.cfg.bConfigurationValue, 0x01)
        self.assertEqual(self.cfg.iConfiguration, 0x00)
        self.assertEqual(self.cfg.bmAttributes, 0xC0)
        self.assertEqual(self.cfg.bMaxPower, 50)
    def test_set(self):
        self.cfg.set()

class InterfaceTest(unittest.TestCase):
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.dev = usb.core.find(backend=backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)
        self.intf = usb.core.Interface(self.dev)
    def runTest(self):
        self.test_attributes()
        self.test_set_altsetting()
    def test_attributes(self):
        self.assertEqual(self.intf.bLength, 9)
        self.assertEqual(self.intf.bDescriptorType, usb.util.DESC_TYPE_INTERFACE)
        self.assertEqual(self.intf.bInterfaceNumber, 0)
        self.assertEqual(self.intf.bAlternateSetting, 0)
        self.assertEqual(self.intf.bNumEndpoints, 6)
        self.assertEqual(self.intf.bInterfaceClass, 0xFF)
        self.assertEqual(self.intf.bInterfaceSubClass, 0xFF)
        self.assertEqual(self.intf.bInterfaceProtocol, 0xFF)
        self.assertEqual(self.intf.iInterface, 0x00)
    def test_set_altsetting(self):
        self.intf.set_altsetting()

class EndpointTest(unittest.TestCase):
    def __init__(self, backend):
        unittest.TestCase.__init__(self)
        self.dev = usb.core.find(backend=backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)
        self.dev.set_configuration()
        self.ep = usb.core.Endpoint(self.dev, 0)
    def runTest(self):
        self.test_attributes()
        self.test_write_read()
    def test_attributes(self):
        self.assertEqual(self.ep.bLength, 7)
        self.assertEqual(self.ep.bDescriptorType, usb.util.DESC_TYPE_ENDPOINT)
        self.assertEqual(self.ep.bEndpointAddress, 0x01)
        self.assertEqual(self.ep.bmAttributes, 0x02)
        self.assertEqual(self.ep.wMaxPacketSize, 64)
        self.assertEqual(self.ep.bInterval, 32)
    def test_write_read(self):
        for data in data_list:
            ret = self.ep.write(data)
            self.assertEqual(ret, len(data), 'Failed to write data: ' + str(data))
            ret = utils.to_array(self.ep.read(len(data)))
            self.assertEqual(ret, utils.to_array(data), 'Failed to read data: ' + str(data))

def get_testsuite():
    suite = unittest.TestSuite()
    suite.addTest(FindTest())
    for m in (libusb10, libusb01, openusb):
        b = m.get_backend()
        if b is not None:
            for ObjectTestCase in (DeviceTest, ConfigurationTest, InterfaceTest, EndpointTest):
                sys.stdout.write('Adding %s(%s) to test suite...\n' % (ObjectTestCase.__name__, m.__name__))
                suite.addTest(ObjectTestCase(b))
    return suite
