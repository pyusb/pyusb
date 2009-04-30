import ctypes
import os
import usb.backend
import usb.util
import array

# usb.h

_PC_PATH_MAX = 4
_PATH_MAX = os.pathconf('.', _PC_PATH_MAX)

class _usb_descriptor_header(ctypes.Structure):
    _pack_ = 1
    _fields_ = [('blength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8)]

class _usb_string_descriptor(ctypes.Structure):
    _pack_ = 1
    _fields_ = [('bLength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8),
                ('wData', ctypes.c_uint16)]

class _usb_endpoint_descriptor(ctypes.Structure):
    _fields_ = [('bLength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8),
                ('bEndpointAddress', ctypes.c_uint8),
                ('bmAttributes', ctypes.c_uint8),
                ('wMaxPacketSize', ctypes.c_uint8),
                ('bInterval', ctypes.c_uint8),
                ('bRefresh', ctypes.c_uint8),
                ('bSynchAddress', ctypes.c_uint8),
                ('extra', ctypes.POINTER(ctypes.c_uint8)),
                ('extralen', ctypes.c_int)]

class _usb_interface_descriptor(ctypes.Structure):
    _fields_ = [('bLength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8),
                ('bInterfaceNumber', ctypes.c_uint8),
                ('bAlternateSetting', ctypes.c_uint8),
                ('bNumEndpoints', ctypes.c_uint8),
                ('bInterfaceClass', ctypes.c_uint8),
                ('bInterfaceSubClass', ctypes.c_uint8),
                ('bInterfaceProtocol', ctypes.c_uint8),
                ('iInterface', ctypes.c_uint8),
                ('endpoint', ctypes.POINTER(_usb_endpoint_descriptor)),
                ('extra', ctypes.POINTER(ctypes.c_uint8)),
                ('extralen', ctypes.c_int)]

class _usb_interface(ctypes.Structure):
    _fields_ = [('altsetting', ctypes.POINTER(_usb_interface_descriptor)),
                ('num_altsetting', ctypes.c_int)]

class _usb_config_descriptor(ctypes.Structure):
    _fields_ = [('bLength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8),
                ('wTotalLength', ctypes.c_uint16),
                ('bNumInterfaces', ctypes.c_uint8),
                ('bConfigurationValue', ctypes.c_uint8),
                ('iConfiguration', ctypes.c_uint8),
                ('bmAttributes', ctypes.c_uint8),
                ('MaxPower', ctypes.c_uint8),
                ('interface', ctypes.POINTER(_usb_interface)),
                ('extra', ctypes.POINTER(ctypes.c_uint8)),
                ('extralen', ctypes.c_int)]

class _usb_device_descriptor(ctypes.Structure):
    _pack_ = 1
    _fields_ = [('bLength', ctypes.c_uint8),
                ('bDescriptorType', ctypes.c_uint8),
                ('bcdUSB', ctypes.c_uint16),
                ('bDeviceClass', ctypes.c_uint8),
                ('bDeviceSubClass', ctypes.c_uint8),
                ('bDeviceProtocol', ctypes.c_uint8),
                ('bMaxPacketSize0', ctypes.c_uint8),
                ('idVendor', ctypes.c_uint16),
                ('idProduct', ctypes.c_uint16),
                ('bcdDevice', ctypes.c_uint16),
                ('iManufacturer', ctypes.c_uint8),
                ('iProduct', ctypes.c_uint8),
                ('iSerialNumber', ctypes.c_uint8),
                ('bNumConfigurations', ctypes.c_uint8)]

class _usb_device(ctypes.Structure):
    pass

class _usb_bus(ctypes.Structure):
    pass

_usb_device._fields_ = [('next', ctypes.POINTER(_usb_device)),
                        ('prev', ctypes.POINTER(_usb_device)),
                        ('filename', ctypes.c_int8 * (_PATH_MAX + 1)),
                        ('bus', ctypes.POINTER(_usb_bus)),
                        ('descriptor', _usb_device_descriptor),
                        ('config', ctypes.POINTER(_usb_config_descriptor)),
                        ('dev', ctypes.c_void_p),
                        ('devnum', ctypes.c_uint8),
                        ('num_children', ctypes.c_ubyte),
                        ('children', ctypes.POINTER(ctypes.POINTER(_usb_device)))]

_usb_bus._fields_ = [('next', ctypes.POINTER(_usb_bus)),
                    ('prev', ctypes.POINTER(_usb_bus)),
                    ('dirname', ctypes.c_char * (_PATH_MAX + 1)),
                    ('devices', ctypes.POINTER(_usb_device)),
                    ('location', ctypes.c_uint32),
                    ('root_dev', ctypes.POINTER(_usb_device))]

_usb_dev_handle = ctypes.c_void_p

_dll = ctypes.CDLL('libusb.so')

# usb_dev_handle *usb_open(struct usb_device *dev);
_dll.usb_open.argtypes = [ctypes.POINTER(_usb_device)]
_dll.usb_open.restype = _usb_dev_handle

# int usb_close(usb_dev_handle *dev);
_dll.usb_close.argtypes = [_usb_dev_handle]

# int usb_get_string(usb_dev_handle *dev, int index, int langid, char *buf,size_t buflen);
_dll.usb_get_string.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_int,
                                ctypes.c_char_p, ctypes.c_size_t]


# int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf, size_t buflen);
_dll.usb_get_string_simple.argtypes = [_usb_dev_handle, ctypes.c_int,
                                        ctypes.c_char_p, ctypes.c_size_t]


# int usb_get_descriptor_by_endpoint(usb_dev_handle *udev, int ep,
#	unsigned char type, unsigned char index, void *buf, int size);
_dll.usb_get_descriptor_by_endpoint.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_ubyte,
                                                ctypes.c_ubyte, ctypes.c_void_p, ctypes.c_int]


# int usb_get_descriptor(usb_dev_handle *udev, unsigned char type,
#	unsigned char index, void *buf, int size);
_dll.usb_get_descriptor.argtypes = [_usb_dev_handle, ctypes.c_ubyte,
                                    ctypes.c_ubyte, ctypes.c_void_p, ctypes.c_int]


# int usb_bulk_write(usb_dev_handle *dev, int ep, const char *bytes, int size,
#	int timeout);
_dll.usb_bulk_write.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_char_p,
                                ctypes.c_int, ctypes.c_int]


# int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
#	int timeout);
_dll.usb_bulk_read.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_char_p,
                                ctypes.c_int, ctypes.c_int]

# int usb_interrupt_write(usb_dev_handle *dev, int ep, const char *bytes, int size,
#         int timeout);
_dll.usb_interrupt_write.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_char_p,
                                    ctypes.c_int, ctypes.c_int]

# int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size,
#         int timeout);
_dll.usb_interrupt_read.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_char_p,
                                    ctypes.c_int, ctypes.c_int]

# int usb_control_msg(usb_dev_handle *dev, int requesttype, int request,
# 	int value, int index, char *bytes, int size, int timeout);
_dll.usb_control_msg.argtypes = [_usb_dev_handle, ctypes.c_int, ctypes.c_int, ctypes.c_int,
                                ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_int]

# int usb_set_configuration(usb_dev_handle *dev, int configuration);
_dll.usb_set_configuration.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_claim_interface(usb_dev_handle *dev, int interface);
_dll.usb_claim_interface.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_release_interface(usb_dev_handle *dev, int interface);
_dll.usb_release_interface.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_set_altinterface(usb_dev_handle *dev, int alternate);
_dll.usb_set_altinterface.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_resetep(usb_dev_handle *dev, unsigned int ep);
_dll.usb_resetep.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_clear_halt(usb_dev_handle *dev, unsigned int ep);
_dll.usb_clear_halt.argtypes = [_usb_dev_handle, ctypes.c_int]

# int usb_reset(usb_dev_handle *dev);
_dll.usb_reset.argtypes = [_usb_dev_handle]

# char *usb_strerror(void);
_dll.usb_strerror.argtypes = []
_dll.usb_strerror.restype = ctypes.c_char_p

# void usb_set_debug(int level);
_dll.usb_set_debug.argtypes = [ctypes.c_int]

# struct usb_device *usb_device(usb_dev_handle *dev);
_dll.usb_device.argtypes = [_usb_dev_handle]
_dll.usb_device.restype = ctypes.POINTER(_usb_device)

# struct usb_bus *usb_get_busses(void);
_dll.usb_get_busses.restype = ctypes.POINTER(_usb_bus)

def _check(retval):
    from usb.core import USBError
    if isinstance(retval, ctypes.c_int):
        if retval < 0:
            raise USBError(_dll.usb_strerror())
    elif retval == None:
        raise USBError(_dll.usb_strerror())
    else:
        return retval

_dll.usb_init()

# implementation of libusb 0.1.x backend
class LibUSB(usb.backend.IBackend):
    def enumerate_devices(self):
        _check(_dll.usb_find_busses())
        _check(_dll.usb_find_devices())

        bus = _dll.usb_get_busses()

        while bool(bus):
            dev = bus[0].devices
            while bool(dev):
                yield dev[0]
                dev = dev[0].next
            bus = bus[0].next

    def get_device_descriptor(self, dev):
        return dev.descriptor

    def get_configuration_descriptor(self, dev, config):
        if config >= dev.descriptor.bNumConfigurations:
            raise IndexError('Invalid configuration index %d' % (config))
        return dev.config[config]

    def get_interface_descriptor(self, dev, intf, alt, config):
        cfgdesc = self.get_configuration_descriptor(dev, config)
        if intf >= cfgdesc.bNumInterfaces:
            raise IndexError('Invalid interface index %d' % (interface))
        interface = cfgdesc.interface[intf]
        if alt >= interface.num_altsetting:
            raise IndexError('Invalid alternate setting index %d' % (alt))
        return interface.altsetting[alt]

    def get_endpoint_descriptor(self, dev, ep, intf, alt, config):
        interface = self.get_interface_descriptor(dev, intf, alt, config)
        if ep >= interface.bNumEndpoints:
            raise IndexError('Invalid endpoint index %d' % (ep))
        return interface.endpoint[ep]

    def open_device(self, dev):
        return _check(_dll.usb_open(dev))

    def close_device(self, dev_handle):
        _check(_dll.usb_close(dev_handle))

    def set_configuration(self, dev_handle, config_value):
        _check(_dll.usb_set_configuration(dev_handle, config_value))

    def set_interface_altsetting(self, dev_handle, intf, altsetting):
        _check(_dll.usb_set_altinterface(dev_handle, altsetting))

    def claim_interface(self, dev_handle, intf):
        _check(_dll.usb_claim_interface(dev_handle, intf))

    def release_interface(self, dev_handle, intf):
        _check(_dll.usb_release_interface(dev_handle, intf))

    def bulk_transfer(self, dev_handle, ep, intf, data_or_length, timeout):
        return self.__transfer(_dll.usb_bulk_write, _dll.usb_bulk_read, dev_handle, ep,
                                intf, data_or_legnth, timeout)
    def interrupt_transfer(self, dev_handle, ep, data_or_length, intf, timeout):
        return self.__transfer(_dll.usb_interrupt_write, _dll.usb_interrupt_read, dev_handle, ep,
                                intf, data_or_legnth, timeout)

    def isochronous_transfer(self, dev_handle, ep, data_or_length, intf, timeout):
        return self.__transfer(_dll.usb_isochronous_write, _dll.usb_isochronous_read,
                                dev_handle, ep, intf, data_or_legnth, timeout)

    def ctrl_transfer(self, dev_handle, bmRequestType, bRequest, wValue, wIndex, data_or_wLength, timeout):
        if usb.util.ctrl_direction(bmRequestType) == usb.util.CTRL_OUT:
            address, length = data_or_wLength.buffer_info()
            length *= data_or_wLength.itemsize()
            return _check(_dll.usb_control_msg(dev_handle, bmRequestType, bRequest, wValue,
                                                wIndex, address, length, timeout))
        else:
            buffer = array.array('B', '\x00' * data_or_wLength)
            read = int(_check(_dll.usb_control_msg(dev_handle, bmRequestType, bRequest, wValue,
                                            wIndex, buffer.buffer_info()[0], data_or_wLength, timeout)))
            return buffer[:read]

    def reset_device(self, dev_handle):
        _check(_dll.usb_reset(dev_handle))

    def detach_kernel_driver(self, dev_handle, intf):
        _check(_dll.usb_detach_kernel_driver_np(dev_handle, intf))

    def __transfer(write_func, read_func, dev_handle, ep, intf, data_or_length,  timeout):
        if usb.util.endpoint_direction == usb.util.ENDPOINT_OUT:
            address, length = data_or_length.buffer_info()
            length *= data_or_length.itemsize()
            return int(_check(write_func(dev_handle, ep, address, length, timeout)))
        else:
            buffer = array.array('B', '\x00' * data_or_length)
            read = int(_check(read_func(dev_handle, ep,
                        buffer.buffer_info()[0], data_or_length, timeout)))
            return buffer[:read]

