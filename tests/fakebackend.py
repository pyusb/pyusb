import usb.backend
import array
import devinfo
import usb.util
from usb.core import USBError

class Pipe(object):
    def __init__(self):
        self._buff = array.array('B')
    def write(self, data):
        self._buff.extend(data)
        return len(data)
    def read(self, read):
        data = self._buff[0:size]
        del self._buff[0:size]
        return data

class EndpointDescriptor(object):
    def __init__(self):
        self.bLength = 9
        self.bDescriptorType = 5
        self.bEndpointAddress = 1
        self.bmAttributes = 0
        self.wMaxPacketSize = 64
        self.bInterval= 0
        self.bRefresh = 0
        self.bSyncAddress = 0

class InterfaceDescriptor(object):
    def __init__(self):
        self.bLength = 9
        self.bDescriptorType = 4
        self.bInterfaceNumber = 0
        self.bAlternateSetting = 0
        self.bNumEndpoints = 0
        self.bInterfaceClass = 0
        self.bInterfaceSubClass = 0
        self.bInterfaceProtocol = 0
        self.iInterface = 0
        self.endpoints = []
        self.pipes = {}
    def add_pipe(self, address, type):
        epo = Endpoint()
        epo.bEndpointAddress = address | util.ENDPOINT_OUT
        epo.bmAttributes = type
        epi = Endpoint()
        epi.bEndpointAddress = address | util.ENDPOINT_IN
        epi.bmAttributes = type
        self.endpoints.extend((epo, epi))
        self.pipes[address, type] = Pipe()
    def write(self, address, type, data):
        key = (usb.util.endpoint_address(address), usb.util.endpoint_type(type))
        return self.pipes[key].write(data)
    def read(self, address, type, size):
        key = (usb.util.endpoint_address(address), usb.util.endpoint_type(type))
        return self.pipes[key].read(size)

class ConfigurationDescriptor(object):
    def __init__(self):
        self.bLength = 9
        self.bDescriptorType = 2
        self.wTotalLength = 0
        self.bNumInterfaces = 0
        self.bConfigurationValue = 1
        self.iConfiguration = 0
        self.bmAttributes = 0
        self.bMaxPower = 100
        self.interfaces = []
    def add_interface(self, intf):
        self.interfaces.append(intf)

def DeviceDescriptor(object):
    def __init__(self):
        self.bLength = 18
        self.bDescriptorType = 1
        self.bcdUSB = 0x0200
        self.bDeviceClass = 0
        self.bDeviceSubClass = 0
        self.bDeviceProtocol = 0
        self.bMaxPacketSize = 0
        self.idVendor = devinfo.ID_VENDOR
        self.idProduct = devinfo.ID_PRODUCT
        self.bcdDevice = 0x0100
        self.iManufacturer = 0
        self.iProduct = 0
        self.iSerialNumber = 0
        self.bNumConfigurations = 0

class Device(object):
    def __init__(self):
        self.dsc = DeviceDescriptor()
        self.configurations = []
        self.active_configuration = None
        self.claimed_interfaces = set()
        self.alternate_settings = {}
    def add_configuration(self, cfg):
        self.configurations.append(cfg)
    def set_configuration(self, config):
        self.active_configuration = usb.util.find_descriptor(self.configurations, bConfigurationValue=config)
        if self.active_configuration is None:
            raise USBError('Invalid configuration value')
    def set_interface(self, interface, altsetting):
        key = (interface, self.active_configuration.bConfigurationValue)
        self.alternate_settings[key] = altsetting
    def claim_interface(self, interface):
        i = usb.util.find_descriptor(self.active_configuration.interfaces, bInterfaceNumber=interface)
        if i is None:
            raise USBError('Invalid interface number')
        if interface in self.claimed_interfaces:
            raise USBError('Interface already claimed')
        self.claimed_interfaces.add(interface)
    def release_interface(self, interface):
        if interface not in self.claimed_interfaces:
            raise USBError('Interface not claimed')
        self.claimed_interfaces.remove(interface)

class FakeBackend(usb.backend.IBackend):
    def __init__(self):
        self.devices = []

    def add_device(self, dev):
        self.devices.append(dev)

    def enumerate_devices(self):
        pass

    def get_device_descriptor(self, dev):
        pass

    def get_configuration_descriptor(self, dev, config):
        pass

    def get_interface_descriptor(self, dev, intf, alt, config):
        pass

    def get_endpoint_descriptor(self, dev, ep, intf, alt, config):
        pass

    def open_device(self, dev):
        pass

    def close_device(self, dev_handle):
        pass

    def set_configuration(self, dev_handle, config_value):
        pass

    def set_interface_altsetting(self, dev_handle, intf, altsetting):
        pass

    def claim_interface(self, dev_handle, intf):
        pass

    def release_interface(self, dev_handle, intf):
        pass

    def bulk_write(self, dev_handle, ep, intf, data, timeout):
        pass

    def bulk_read(self, dev_handle, ep, intf, size, timeout):
        pass

    def intr_write(self, dev_handle, ep, intf, data, timeout):
        pass

    def intr_read(self, dev_handle, ep, intf, size, timeout):
        pass

    def iso_write(self, dev_handle, ep, intf, data, timeout):
        pass

    def iso_read(self, dev_handle, ep, intf, size, timeout):
        pass

    def ctrl_transfer(self, dev_handle, bmRequestType, bRequest, wValue, wIndex, data_or_wLength, timeout):
        pass

    def reset_device(self, dev_handle):
        pass

    def is_kernel_driver_active(self, dev_handle, intf):
        pass

    def detach_kernel_driver(self, dev_handle, intf):
        pass

    def attach_kernel_driver(self, dev_handle, intf):
        pass

def get_backend():
    return FakeBackend()
