from ctypes import *
import ctypes.util
import os
import usb.backend
import usb.util
import array
import sys
from usb.core import USBError

__author__ = 'Wander Lairson Costa'

__all__ = ['get_backend']

# usb.h

_PC_PATH_MAX = 4

if sys.platform != 'win32':
    _PATH_MAX = os.pathconf('.', _PC_PATH_MAX)
else:
    _PATH_MAX = 511

# Data structures

class _usb_descriptor_header(Structure):
    _pack_ = 1
    _fields_ = [('blength', c_uint8),
                ('bDescriptorType', c_uint8)]

class _usb_string_descriptor(Structure):
    _pack_ = 1
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('wData', c_uint16)]

class _usb_endpoint_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('bEndpointAddress', c_uint8),
                ('bmAttributes', c_uint8),
                ('wMaxPacketSize', c_uint16),
                ('bInterval', c_uint8),
                ('bRefresh', c_uint8),
                ('bSynchAddress', c_uint8),
                ('extra', POINTER(c_uint8)),
                ('extralen', c_int)]

class _usb_interface_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('bInterfaceNumber', c_uint8),
                ('bAlternateSetting', c_uint8),
                ('bNumEndpoints', c_uint8),
                ('bInterfaceClass', c_uint8),
                ('bInterfaceSubClass', c_uint8),
                ('bInterfaceProtocol', c_uint8),
                ('iInterface', c_uint8),
                ('endpoint', POINTER(_usb_endpoint_descriptor)),
                ('extra', POINTER(c_uint8)),
                ('extralen', c_int)]

class _usb_interface(Structure):
    _fields_ = [('altsetting', POINTER(_usb_interface_descriptor)),
                ('num_altsetting', c_int)]

class _usb_config_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('wTotalLength', c_uint16),
                ('bNumInterfaces', c_uint8),
                ('bConfigurationValue', c_uint8),
                ('iConfiguration', c_uint8),
                ('bmAttributes', c_uint8),
                ('bMaxPower', c_uint8),
                ('interface', POINTER(_usb_interface)),
                ('extra', POINTER(c_uint8)),
                ('extralen', c_int)]

class _usb_device_descriptor(Structure):
    _pack_ = 1
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('bcdUSB', c_uint16),
                ('bDeviceClass', c_uint8),
                ('bDeviceSubClass', c_uint8),
                ('bDeviceProtocol', c_uint8),
                ('bMaxPacketSize0', c_uint8),
                ('idVendor', c_uint16),
                ('idProduct', c_uint16),
                ('bcdDevice', c_uint16),
                ('iManufacturer', c_uint8),
                ('iProduct', c_uint8),
                ('iSerialNumber', c_uint8),
                ('bNumConfigurations', c_uint8)]

class _usb_device(Structure):
    pass

class _usb_bus(Structure):
    pass

_usb_device._fields_ = [('next', POINTER(_usb_device)),
                        ('prev', POINTER(_usb_device)),
                        ('filename', c_int8 * (_PATH_MAX + 1)),
                        ('bus', POINTER(_usb_bus)),
                        ('descriptor', _usb_device_descriptor),
                        ('config', POINTER(_usb_config_descriptor)),
                        ('dev', c_void_p),
                        ('devnum', c_uint8),
                        ('num_children', c_ubyte),
                        ('children', POINTER(POINTER(_usb_device)))]

_usb_bus._fields_ = [('next', POINTER(_usb_bus)),
                    ('prev', POINTER(_usb_bus)),
                    ('dirname', c_char * (_PATH_MAX + 1)),
                    ('devices', POINTER(_usb_device)),
                    ('location', c_uint32),
                    ('root_dev', POINTER(_usb_device))]

_usb_dev_handle = c_void_p

_lib = None

def _load_library():
    if sys.platform == 'win32':
        libname = ctypes.util.find_library('libusb0')
    else:
        libname = ctypes.util.find_library('usb')
    if libname is None:
        raise OSError('USB library could not be found')
    return CDLL(libname)

def _setup_prototypes(lib):
    # usb_dev_handle *usb_open(struct usb_device *dev);
    lib.usb_open.argtypes = [POINTER(_usb_device)]
    lib.usb_open.restype = _usb_dev_handle

    # int usb_close(usb_dev_handle *dev);
    lib.usb_close.argtypes = [_usb_dev_handle]

    # int usb_get_string(usb_dev_handle *dev, int index, int langid, char *buf,size_t buflen);
    lib.usb_get_string.argtypes = [_usb_dev_handle, c_int, c_int, c_char_p, c_size_t]


    # int usb_get_string_simple(usb_dev_handle *dev, int index, char *buf, size_t buflen);
    lib.usb_get_string_simple.argtypes = [_usb_dev_handle, c_int, c_char_p, c_size_t]


    # int usb_get_descriptor_by_endpoint(usb_dev_handle *udev, int ep,
    #	unsigned char type, unsigned char index, void *buf, int size);
    lib.usb_get_descriptor_by_endpoint.argtypes = [_usb_dev_handle, c_int, c_ubyte,
                                                    c_ubyte, c_void_p, c_int]


    # int usb_get_descriptor(usb_dev_handle *udev, unsigned char type,
    #	unsigned char index, void *buf, int size);
    lib.usb_get_descriptor.argtypes = [_usb_dev_handle, c_ubyte, c_ubyte,
                                        c_void_p, c_int]


    # int usb_bulk_write(usb_dev_handle *dev, int ep, const char *bytes, int size,
    #	int timeout);
    lib.usb_bulk_write.argtypes = [_usb_dev_handle, c_int, c_char_p, c_int, c_int]


    # int usb_bulk_read(usb_dev_handle *dev, int ep, char *bytes, int size,
    #	int timeout);
    lib.usb_bulk_read.argtypes = [_usb_dev_handle, c_int, c_char_p, c_int, c_int]

    # int usb_interrupt_write(usb_dev_handle *dev, int ep, const char *bytes, int size,
    #         int timeout);
    lib.usb_interrupt_write.argtypes = [_usb_dev_handle, c_int, c_char_p,
                                        c_int, c_int]

    # int usb_interrupt_read(usb_dev_handle *dev, int ep, char *bytes, int size,
    #         int timeout);
    lib.usb_interrupt_read.argtypes = [_usb_dev_handle, c_int, c_char_p,
                                        c_int, c_int]

    # int usb_control_msg(usb_dev_handle *dev, int requesttype, int request,
    # 	int value, int index, char *bytes, int size, int timeout);
    lib.usb_control_msg.argtypes = [_usb_dev_handle, c_int, c_int, c_int,
                                    c_int, c_char_p, c_int, c_int]

    # int usb_set_configuration(usb_dev_handle *dev, int configuration);
    lib.usb_set_configuration.argtypes = [_usb_dev_handle, c_int]

    # int usb_claim_interface(usb_dev_handle *dev, int interface);
    lib.usb_claim_interface.argtypes = [_usb_dev_handle, c_int]

    # int usb_release_interface(usb_dev_handle *dev, int interface);
    lib.usb_release_interface.argtypes = [_usb_dev_handle, c_int]

    # int usb_set_altinterface(usb_dev_handle *dev, int alternate);
    lib.usb_set_altinterface.argtypes = [_usb_dev_handle, c_int]

    # int usb_resetep(usb_dev_handle *dev, unsigned int ep);
    lib.usb_resetep.argtypes = [_usb_dev_handle, c_int]

    # int usb_clear_halt(usb_dev_handle *dev, unsigned int ep);
    lib.usb_clear_halt.argtypes = [_usb_dev_handle, c_int]

    # int usb_reset(usb_dev_handle *dev);
    lib.usb_reset.argtypes = [_usb_dev_handle]

    # char *usb_strerror(void);
    lib.usb_strerror.argtypes = []
    lib.usb_strerror.restype = c_char_p

    # void usb_set_debug(int level);
    lib.usb_set_debug.argtypes = [c_int]

    # struct usb_device *usb_device(usb_dev_handle *dev);
    lib.usb_device.argtypes = [_usb_dev_handle]
    lib.usb_device.restype = POINTER(_usb_device)

    # struct usb_bus *usb_get_busses(void);
    lib.usb_get_busses.restype = POINTER(_usb_bus)

def _check(retval):
    if retval is None:
        errmsg = _lib.usb_strerror()
    else:
        ret = int(retval)
        if ret < 0:
            errmsg = _lib.usb_strerror()
            # No error means that we need to get the error
            # message from the return code
            # Thanks to Nicholas Wheeler to point out the problem...
            # Also see issue #2860940
            if errmsg.lower() == 'no error':
                errmsg = os.strerror(-ret)
        else:
            return ret
    raise USBError(errmsg)

# implementation of libusb 0.1.x backend
class _LibUSB(usb.backend.IBackend):
    def enumerate_devices(self):
        _check(_lib.usb_find_busses())
        _check(_lib.usb_find_devices())

        bus = _lib.usb_get_busses()

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
        return _check(_lib.usb_open(dev))

    def close_device(self, dev_handle):
        _check(_lib.usb_close(dev_handle))

    def set_configuration(self, dev_handle, config_value):
        _check(_lib.usb_set_configuration(dev_handle, config_value))

    def set_interface_altsetting(self, dev_handle, intf, altsetting):
        _check(_lib.usb_set_altinterface(dev_handle, altsetting))

    def claim_interface(self, dev_handle, intf):
        _check(_lib.usb_claim_interface(dev_handle, intf))

    def release_interface(self, dev_handle, intf):
        _check(_lib.usb_release_interface(dev_handle, intf))

    def bulk_write(self, dev_handle, ep, intf, data, timeout):
        return self.__write(_lib.usb_bulk_write, dev_handle, ep, intf, data, timeout)

    def bulk_read(self, dev_handle, ep, intf, size, timeout):
        return self.__read(_lib.usb_bulk_read, dev_handle, ep, intf, size, timeout)

    def intr_write(self, dev_handle, ep, intf, data, timeout):
        return self.__write(_lib.usb_interrupt_write, dev_handle, ep, intf, data, timeout)

    def intr_read(self, dev_handle, ep, intf, size, timeout):
        return self.__read(_lib.usb_interrupt_read, dev_handle, ep, intf, size, timeout)

    def ctrl_transfer(self, dev_handle, bmRequestType, bRequest, wValue, wIndex, data_or_wLength, timeout):
        if usb.util.ctrl_direction(bmRequestType) == usb.util.CTRL_OUT:
            address, length = data_or_wLength.buffer_info()
            length *= data_or_wLength.itemsize
            return _check(_lib.usb_control_msg(dev_handle, bmRequestType, bRequest, wValue,
                                               wIndex, cast(address, c_char_p), length, timeout))
        else:
            buffer = array.array('B', '\x00' * data_or_wLength)
            read = int(_check(_lib.usb_control_msg(dev_handle, bmRequestType, bRequest, wValue,
                                            wIndex, cast(buffer.buffer_info()[0], c_char_p),
                                            data_or_wLength, timeout)))
            return buffer[:read]

    def reset_device(self, dev_handle):
        _check(_lib.usb_reset(dev_handle))

    def detach_kernel_driver(self, dev_handle, intf):
        _check(_lib.usb_detach_kernel_driver_np(dev_handle, intf))

    def __write(self, fn, dev_handle, ep, intf, data, timeout):
        address, length = data.buffer_info()
        return int(_check(fn(dev_handle, ep, cast(address, c_char_p), length, timeout)))

    def __read(self, fn, dev_handle, ep, intf, size, timeout):
        buffer = array.array('B', '\x00' * size)
        address, length = buffer.buffer_info()
        ret = int(_check(fn(dev_handle, ep, cast(address, c_char_p), length, timeout)))
        return buffer[:ret]

def get_backend():
    global _lib
    try:
        if _lib is None:
            _lib = _load_library()
            _setup_prototypes(_lib)
            _lib.usb_init()
        return _LibUSB()
    except OSError:
        return None
