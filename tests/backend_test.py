import unittest
import array
import device_info as di

class BackendTestCase(unittest.TestCase):
    def __init__(self, backend_module_name):
        unittest.TestCase.__init__(self)
        self.module = __import__(backend_module_name, fromlist=['dummy'])
        self.backend = self.module.get_backend()

    def runTest(self):
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
        self.test_release_interface()
        self.test_reset_device()
        self.test_close_device()

    def test_enumerate_devices(self):
        for d in self.backend.enumerate_devices():
            desc = self.backend.get_device_descriptor(d)
            if desc.idVendor == di.ID_VENDOR and desc.idProduct == di.ID_PRODUCT:
                self.dev = d
                return
        self.fail('PyUSB test device not found')

    def test_get_device_descriptor(self):
        dsc = self.backend.get_device_descriptor(self.dev)
        self.assertEqual(dsc.bLength, 18)
        self.assertEqual(dsc.bDescriptorType, 0x01)
        self.assertEqual(dsc.bcdUSB, 0x0200)
        self.assertEqual(dsc.idVendor, di.ID_VENDOR)
        self.assertEqual(dsc.idProduct, di.ID_PRODUCT)
        self.assertEqual(dsc.bcdDevice, 0x0001)
        self.assertEqual(dsc.iManufacturer, 0x01)
        self.assertEqual(dsc.iProduct, 0x02)
        self.assertEqual(dsc.iSerialNumber, 0x00)
        self.assertEqual(dsc.bNumConfigurations, 0x01)
        self.assertEqual(dsc.bMaxPacketSize0, 64)
        self.assertEqual(dsc.bDeviceClass, 0x00)
        self.assertEqual(dsc.bDeviceSubClass, 0x00)
        self.assertEqual(dsc.bDeviceProtocol, 0x00)

    def test_get_configuration_descriptor(self):
        cfg = self.backend.get_configuration_descriptor(self.dev, 0)
        self.assertEqual(cfg.bLength, 9)
        self.assertEqual(cfg.bDescriptorType, 0x02)
        self.assertEqual(cfg.wTotalLength, 60)
        self.assertEqual(cfg.bNumInterfaces, 0x01)
        self.assertEqual(cfg.bConfigurationValue, 0x01)
        self.assertEqual(cfg.iConfiguration, 0x00)
        self.assertEqual(cfg.bmAttributes, 0xC0)
        self.assertEqual(cfg.bMaxPower, 50)

    def test_get_interface_descriptor(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0)
        self.assertEqual(intf.bLength, 9)
        self.assertEqual(intf.bDescriptorType, 0x04)
        self.assertEqual(intf.bInterfaceNumber, 0)
        self.assertEqual(intf.bAlternateSetting, 0)
        self.assertEqual(intf.bNumEndpoints, 6)
        self.assertEqual(intf.bInterfaceClass, 0xFF)
        self.assertEqual(intf.bInterfaceSubClass, 0xFF)
        self.assertEqual(intf.bInterfaceProtocol, 0xFF)
        self.assertEqual(intf.iInterface, 0x00)

    def test_get_endpoint_descriptor(self):
        ep = self.backend.get_endpoint_descriptor(self.dev, 0, 0, 0, 0)
        self.assertEqual(ep.bLength, 7)
        self.assertEqual(ep.bDescriptorType, 0x05)
        self.assertEqual(ep.bEndpointAddress, 0x01)
        self.assertEqual(ep.bmAttributes, 0x02)
        self.assertEqual(ep.wMaxPacketSize, 64)
        self.assertEqual(ep.bInterval, 32)

    def test_open_device(self):
        self.handle = self.backend.open_device(self.dev)

    def test_close_device(self):
        self.backend.close_device(self.handle)

    def test_set_configuration(self):
        cfg = self.backend.get_configuration_descriptor(self.dev, 0)
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
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0).bInterfaceNumber

        data = self.get_data1()

        self.assertEqual(self.backend.bulk_write(self.handle,
                                                 di.EP_BULK_OUT,
                                                 intf,
                                                 data,
                                                 1000),
                                             len(data))

        self.assertEqual(self.backend.bulk_read(self.handle,
                                                di.EP_BULK_IN,
                                                intf,
                                                len(data),
                                                1000),
                                             data)

        data = self.get_data2()

        self.assertEqual(self.backend.bulk_write(self.handle,
                                                 di.EP_BULK_OUT,
                                                 intf,
                                                 data,
                                                 1000),
                                             len(data))

        self.assertEqual(self.backend.bulk_read(self.handle,
                                                di.EP_BULK_IN,
                                                intf,
                                                len(data),
                                                1000),
                                             data)

    def test_intr_write_read(self):
        intf = self.backend.get_interface_descriptor(self.dev, 0, 0, 0).bInterfaceNumber

        data = self.get_data1()

        self.assertEqual(self.backend.intr_write(self.handle,
                                                 di.EP_INTR_OUT,
                                                 intf,
                                                 data,
                                                 1000),
                                             len(data))

        self.assertEqual(self.backend.intr_read(self.handle,
                                                di.EP_INTR_IN,
                                                intf,
                                                len(data),
                                                1000),
                                             data)

        data = self.get_data2()

        self.assertEqual(self.backend.intr_write(self.handle,
                                                 di.EP_INTR_OUT,
                                                 intf,
                                                 data,
                                                 1000),
                                             len(data))

        self.assertEqual(self.backend.intr_read(self.handle,
                                                di.EP_INTR_IN,
                                                intf,
                                                len(data),
                                                1000),
                                             data)

    def test_iso_write_read(self):
        pass

    def test_ctrl_transfer(self):
        data = self.get_data1()

        self.assertEqual(self.backend.ctrl_transfer(self.handle,
                                                    0x40,
                                                    di.CTRL_LOOPBACK_WRITE,
                                                    0,
                                                    0,
                                                    data,
                                                    1000),
                                                 len(data))

        self.assertEqual(self.backend.ctrl_transfer(self.handle,
                                                    0xC0,
                                                    di.CTRL_LOOPBACK_READ,
                                                    0,
                                                    0,
                                                    len(data),
                                                    1000),
                                                 data)

        data = self.get_data2()

        self.assertEqual(self.backend.ctrl_transfer(self.handle,
                                                    0x40,
                                                    di.CTRL_LOOPBACK_WRITE,
                                                    0,
                                                    0,
                                                    data,
                                                    1000),
                                                 len(data))

        self.assertEqual(self.backend.ctrl_transfer(self.handle,
                                                    0xC0,
                                                    di.CTRL_LOOPBACK_READ,
                                                    0,
                                                    0,
                                                    len(data),
                                                    1000),
                                                 data)

    def test_reset_device(self):
        self.backend.reset_device(self.handle)

    def get_data1(self):
        return array.array('B', range(10))

    def get_data2(self):
        return array.array('B', reversed(range(10)))

def get_testsuite():
    libusb10_testcase = BackendTestCase('usb.backend.libusb10')
    libusb01_testcase = BackendTestCase('usb.backend.libusb01')
    #openusb_testcase = BackendTestCase('usb.backend.openusb')
    suite = unittest.TestSuite()
    suite.addTest(libusb10_testcase)
    suite.addTest(libusb01_testcase)
    #suite.addTest(openusb_testcase)
    return suite
