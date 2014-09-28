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
import devinfo
import struct
import usb
import usb.core
from usb._debug import methodtrace

class LegacyTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def __init__(self):
        unittest.TestCase.__init__(self)

    @methodtrace(utils.logger)
    def runTest(self):
        try:
            self.test_find_device()
            self.test_open_device()
            self.test_device_descriptor()
            self.test_configuration_descriptor()
            self.test_interface_descriptor()
            self.test_endpoint_descriptor()
            self.test_open_device()
            self.test_set_configuration()
            self.test_claim_interface()
            self.test_set_interface_altsetting()
            self.test_clear_halt()
            self.test_bulk_write_read()
            self.test_intr_write_read()
            self.test_ctrl_transfer()
            self.test_get_string()
            self.test_get_descriptor()
        except:
            # do this to not influence other tests upon error
            if hasattr(self, 'handle'):
                self.handle.releaseInterface()
                del self.handle
            raise

        self.test_release_interface()
        self.test_close_device()

    @methodtrace(utils.logger)
    def test_find_device(self):
        busses = usb.busses()
        for bus in busses:
            for dev in bus.devices:
                if dev.idVendor == devinfo.ID_VENDOR and dev.idProduct == devinfo.ID_PRODUCT:
                    self.dev = dev
                    return
        self.fail('PyUSB test device not found')

    @methodtrace(utils.logger)
    def test_device_descriptor(self):
        self.assertEqual(self.dev.usbVersion, '02.00')
        self.assertEqual(self.dev.idVendor, devinfo.ID_VENDOR)
        self.assertEqual(self.dev.idProduct, devinfo.ID_PRODUCT)
        self.assertEqual(self.dev.deviceVersion, '00.01')
        self.assertEqual(self.dev.iManufacturer, 0x01)
        self.assertEqual(self.dev.iProduct, 0x02)
        self.assertEqual(self.dev.iSerialNumber, 0x03)
        self.assertEqual(len(self.dev.configurations), 0x01)
        self.assertEqual(self.dev.maxPacketSize, 8)
        self.assertEqual(self.dev.deviceClass, 0x00)
        self.assertEqual(self.dev.deviceSubClass, 0x00)
        self.assertEqual(self.dev.deviceProtocol, 0x00)

    @methodtrace(utils.logger)
    def test_configuration_descriptor(self):
        cfg = self.dev.configurations[0]
        self.assertEqual(cfg.totalLength, 78)
        self.assertEqual(len(cfg.interfaces), 0x03)
        self.assertEqual(cfg.value, 0x01)
        self.assertEqual(cfg.iConfiguration, 0x00)
        self.assertEqual(cfg.remoteWakeup, 0)
        self.assertEqual(cfg.selfPowered, 1)
        self.assertEqual(cfg.maxPower, 100)

    @methodtrace(utils.logger)
    def test_interface_descriptor(self):
        intf = self.dev.configurations[0].interfaces[0][0]
        self.assertEqual(intf.interfaceNumber, 0)
        self.assertEqual(intf.alternateSetting, 0)
        self.assertEqual(len(intf.endpoints), 2)
        self.assertEqual(intf.interfaceClass, 0x00)
        self.assertEqual(intf.interfaceSubClass, 0x00)
        self.assertEqual(intf.interfaceProtocol, 0x00)
        self.assertEqual(intf.iInterface, 0x00)

    @methodtrace(utils.logger)
    def test_endpoint_descriptor(self):
        ep = self.dev.configurations[0].interfaces[0][0].endpoints[0]
        self.assertEqual(ep.address, 0x01)
        self.assertEqual(ep.type, 0x02)
        self.assertEqual(ep.maxPacketSize, 16)
        self.assertEqual(ep.interval, 0)

    @methodtrace(utils.logger)
    def test_open_device(self):
        self.handle = self.dev.open()
        self.assertNotEquals(self.handle, None)

    @methodtrace(utils.logger)
    def test_close_device(self):
        del self.handle

    @methodtrace(utils.logger)
    def test_set_configuration(self):
        self.handle.setConfiguration(1)

    @methodtrace(utils.logger)
    def test_set_interface_altsetting(self):
        self.handle.setAltInterface(0)

    @methodtrace(utils.logger)
    def test_claim_interface(self):
        self.handle.claimInterface(0)

    @methodtrace(utils.logger)
    def test_release_interface(self):
        self.handle.releaseInterface()

    @methodtrace(utils.logger)
    def test_bulk_write_read(self):
        self.handle.setAltInterface(devinfo.INTF_BULK)

        self.__write_read(
                self.handle.bulkWrite,
                self.handle.bulkRead,
                devinfo.EP_BULK)

    @methodtrace(utils.logger)
    def test_intr_write_read(self):
        self.handle.setAltInterface(devinfo.INTF_INTR)

        self.__write_read(
                self.handle.interruptWrite,
                self.handle.interruptRead,
                devinfo.EP_INTR)

    @methodtrace(utils.logger)
    def test_clear_halt(self):
        self.handle.clearHalt(0x01)
        self.handle.clearHalt(0x81)

    @methodtrace(utils.logger)
    def test_ctrl_transfer(self):
        for data in (utils.get_array_data1(), utils.get_array_data2()):
            length = len(data) * data.itemsize

            ret = self.handle.controlMsg(
                             0x40,
                             devinfo.PICFW_SET_VENDOR_BUFFER,
                             data,
                             0,
                             0,
                             1000)

            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + str(data) + ', ' + str(length) + ' != ' + str(ret))

            ret = self.handle.controlMsg(
                             0xC0,
                             devinfo.PICFW_GET_VENDOR_BUFFER,
                             length,
                             0,
                             0,
                             1000)

            self.assertEqual(ret,
                             data,
                             'Failed to read data: ' + str(data) + ' != ' + str(ret))

    @methodtrace(utils.logger)
    def test_get_string(self):
        manufacturer_str = 'Travis Robinson'.encode('ascii')
        product_str = 'Benchmark Device'.encode('ascii')
        self.assertEqual(self.handle.getString(self.dev.iManufacturer, 0), manufacturer_str)
        self.assertEqual(self.handle.getString(self.dev.iProduct, 0), product_str)

    @methodtrace(utils.logger)
    def test_get_descriptor(self):
        dev = usb.core.find(idVendor=devinfo.ID_VENDOR,
                            idProduct=devinfo.ID_PRODUCT)

        assert dev is not None

        dev_fmt = 'BBHBBBBHHHBBBB'
        dev_descr = (dev.bLength,
                     dev.bDescriptorType,
                     dev.bcdUSB,
                     dev.bDeviceClass,
                     dev.bDeviceSubClass,
                     dev.bDeviceProtocol,
                     dev.bMaxPacketSize0,
                     dev.idVendor,
                     dev.idProduct,
                     dev.bcdDevice,
                     dev.iManufacturer,
                     dev.iProduct,
                     dev.iSerialNumber,
                     dev.bNumConfigurations)

        ret = self.handle.getDescriptor(
                    dev.bDescriptorType,
                    0,
                    struct.calcsize(dev_fmt))

        self.assertEqual(struct.unpack(dev_fmt, ret.tostring()), dev_descr)

    def __write_read(self, write_fn, read_fn, ep):
        for data in (utils.get_array_data1(), utils.get_array_data2()):
            length = len(data) * data.itemsize

            try:
                ret = write_fn(ep, data, 1000)
            except NotImplementedError:
                return

            self.assertEqual(ret,
                             length,
                             'Failed to write data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep))

            try:
                ret = read_fn(ep | usb.util.ENDPOINT_IN, length, 1000)
            except NotImplementedError:
                return

            self.assertEqual(ret,
                             data,
                             'Failed to read data: ' + \
                                str(data) + \
                                ', in EP = ' + \
                                str(ep))

def get_suite():
    suite = unittest.TestSuite()

    if utils.find_my_device():
        suite.addTest(LegacyTest())

    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
