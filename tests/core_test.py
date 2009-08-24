import unittest
import usb.core
import device_info as di

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
    def __init__(self, backend_name):
        unittest.TestCase.__init__(self)
        self.backend_module = __import__(backend_name, fromlist=['dummy'])
        self.backend = self.backend_module.get_backend()
        self.dev = usb.core.find(backend=self.backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)

    def runTest(self):
        self.test_attributes()
        self.test_set_configuration()
        self.test_set_interface_altsetting()
        self.test_write_read()
        self.test_ctrl_transfer()
        self.test_reset()

    def test_attributes(self):
        self.assertEqual(self.dev.bLength, 18)
        self.assertEqual(self.dev.bDescriptorType, 0x01)
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

    def test_set_configuration(self):
        cfg = iter(self.dev).next().bConfigurationValue
        self.dev.set_configuration(cfg)

    def test_set_interface_altsetting(self):
        cfg = iter(self.dev).next()
        intf = iter(cfg).next()
        self.dev.set_interface_altsetting(intf.bInterfaceNumber, intf.bAlternateSetting)

    def test_reset(self):
        self.dev.reset()

    def test_write_read(self):
        pass

    def test_ctrl_transfer(self):
        pass

class ConfigurationTest(unittest.TestCase):
    def __init__(self, backend_name):
        unittest.TestCase.__init__(self)
        self.backend_module = __import__(backend_name, fromlist=['dummy'])
        self.backend = self.backend_module.get_backend()
        self.dev = usb.core.find(backend=self.backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)
        self.cfg = iter(self.dev).next()
    def runTest(self):
        self.test_attributes()
        self.test_set()
    def test_attributes(self):
        self.assertEqual(self.cfg.bLength, 9)
        self.assertEqual(self.cfg.bDescriptorType, 0x02)
        self.assertEqual(self.cfg.wTotalLength, 60)
        self.assertEqual(self.cfg.bNumInterfaces, 0x01)
        self.assertEqual(self.cfg.bConfigurationValue, 0x01)
        self.assertEqual(self.cfg.iConfiguration, 0x00)
        self.assertEqual(self.cfg.bmAttributes, 0xC0)
        self.assertEqual(self.cfg.bMaxPower, 50)
    def test_set(self):
        self.cfg.set()

class InterfaceTest(unittest.TestCase):
    def __init__(self, backend_name):
        unittest.TestCase.__init__(self)
        self.backend_module = __import__(backend_name, fromlist=['dummy'])
        self.backend = self.backend_module.get_backend()
        self.dev = usb.core.find(backend=self.backend, idVendor=di.ID_VENDOR, idProduct=di.ID_PRODUCT)
        cfg = iter(self.dev).next()
        self.intf = iter(cfg).next()
    def runTest(self):
        self.test_attributes()
        self.test_set_altsetting()
    def test_attributes(self):
        self.assertEqual(self.intf.bLength, 9)
        self.assertEqual(self.intf.bDescriptorType, 0x04)
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
    def __init__(self, backend_name):
        unittest.TestCase.__init__(self)
    def runTest(self):
        pass
    def test_attributes(self):
        pass
    def test_write_read(self):
        pass

def get_testsuite():
    suite = unittest.TestSuite()
    suite.addTest(FindTest())

    for c in [DeviceTest, ConfigurationTest, InterfaceTest, EndpointTest]:
        libusb01 = c('usb.backend.libusb01')
        libusb10 = c('usb.backend.libusb10')
        #openusb = c('usb.backend.openusb')
        suite.addTest(libusb01)
        suite.addTest(libusb10)
        #suite.addTest(openusb)

    return suite
