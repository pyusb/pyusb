from ctypes import *
import usb.util
import array

# libusb.h

# return codes

_LIBUSB_SUCCESS = 0
_LIBUSB_ERROR_IO = -1
_LIBUSB_ERROR_INVALID_PARAM = -2
_LIBUSB_ERROR_ACCESS = -3
_LIBUSB_ERROR_NO_DEVICE = -4
_LIBUSB_ERROR_NOT_FOUND = -5
_LIBUSB_ERROR_BUSY = -6
_LIBUSB_ERROR_TIMEOUT = -7
_LIBUSB_ERROR_OVERFLOW = -8
_LIBUSB_ERROR_PIPE = -9
_LIBUSB_ERROR_INTERRUPTED = -10
_LIBUSB_ERROR_NO_MEM = -11
_LIBUSB_ERROR_NOT_SUPPORTED = -12
_LIBUSB_ERROR_OTHER = -99

# map return codes to strings
_str_error = {_LIBUSB_SUCCESS:'LIBUSB_SUCCESS',
            _LIBUSB_ERROR_IO:'LIBUSB_ERROR_IO',
            _LIBUSB_ERROR_INVALID_PARAM:'LIBUSB_ERROR_INVALID_PARAM',
            _LIBUSB_ERROR_ACCESS:'LIBUSB_ERROR_ACCESS',
            _LIBUSB_ERROR_NO_DEVICE:'LIBUSB_ERROR_NO_DEVICE',
            _LIBUSB_ERROR_NOT_FOUND:'LIBUSB_ERROR_NOT_FOUND',
            _LIBUSB_ERROR_BUSY:'LIBUSB_ERROR_BUSY',
            _LIBUSB_ERROR_TIMEOUT:'LIBUSB_ERROR_TIMEOUT',
            _LIBUSB_ERROR_OVERFLOW:'LIBUSB_ERROR_OVERFLOW',
            _LIBUSB_ERROR_PIPE:'LIBUSB_ERROR_PIPE',
            _LIBUSB_ERROR_INTERRUPTED:'LIBUSB_ERROR_INTERRUPTED',
            _LIBUSB_ERROR_NO_MEM:'LIBUSB_ERROR_NO_MEM',
            _LIBUSB_ERROR_NOT_SUPPORTED:'LIBUSB_ERROR_NOT_SUPPORTED',
            _LIBUSB_ERROR_OTHER:'LIBUSB_ERROR_OTHER'}

# Data structures

class _libusb_endpoint_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('bEndpointAddress', c_uint8),
                ('bmAttributes', c_uint8),
                ('wMaxPacketSize', c_uint16),
                ('bInterval', c_uint8),
                ('bRefresh', c_uint8),
                ('bSynchAddress', c_uint8),
                ('extra', POINTER(c_ubyte)),
                ('extra_length', c_int)]

class _libusb_interface_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('bInterfaceNumber', c_uint8),
                ('bAlternateSetting', c_uint8),
                ('bNumEndpoints', c_uint8),
                ('bInterfaceClass', c_uint8),
                ('bInterfaceSubClass', c_uint8),
                ('bInterfaceProtocol', c_uint8),
                ('iInterface', c_uint8),
                ('endpoint', POINTER(_libusb_endpoint_descriptor)),
                ('extra', POINTER(c_ubyte)),
                ('extra_length', c_int)]

class _libusb_interface(Structure):
    _fields_ = [('altsetting', POINTER(_libusb_interface_descriptor)),
                ('num_altsetting', c_int)]

class _libusb_config_descriptor(Structure):
    _fields_ = [('bLength', c_uint8),
                ('bDescriptorType', c_uint8),
                ('wTotalLength', c_uint16),
                ('bNumInterfaces', c_uint8),
                ('bConfigurationValue', c_uint8),
                ('iConfiguration', c_uint8),
                ('bmAttributes', c_uint8),
                ('bMaxPower', c_uint8),
                ('interface', POINTER(_libusb_interface)),
                ('extra', POINTER(c_ubyte)),
                ('extra_length', c_int)]

class _libusb_device_descriptor(Structure):
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

_dll = CDLL('libusb-1.0.so')

_libusb_device_handle = c_void_p

# Function prototypes

# void libusb_set_debug (libusb_context *ctx, int level)
_dll.libusb_set_debug.argtypes = [c_void_p, c_int]

# int libusb_init (libusb_context **context)
_dll.libusb_init.argtypes = [POINTER(c_void_p)]

# void libusb_exit (struct libusb_context *ctx)
_dll.libusb_exit.argtypes = [c_void_p]

# ssize_t libusb_get_device_list (libusb_context *ctx, libusb_device ***list)
_dll.libusb_get_device_list.argtypes = [c_void_p, POINTER(POINTER(c_void_p))]

# void libusb_free_device_list (libusb_device **list, int unref_devices)
_dll.libusb_free_device_list.argtypes = [POINTER(c_void_p), c_int]

# libusb_device *libusb_ref_device (libusb_device *dev)
_dll.libusb_ref_device.argtypes = [c_void_p]
_dll.libusb_ref_device.restype = c_void_p

# void libusb_unref_device (libusb_device *dev)
_dll.libusb_unref_device.argtypes = [c_void_p]

# int libusb_open (libusb_device *dev, libusb_device_handle **handle)
_dll.libusb_open.argtypes = [c_void_p, POINTER(_libusb_device_handle)]

# void libusb_close (libusb_device_handle *dev_handle)
_dll.libusb_close.argtypes = [_libusb_device_handle]

# int libusb_set_configuration (libusb_device_handle *dev, int configuration)
_dll.libusb_set_configuration.argtypes = [_libusb_device_handle, c_int]

# int libusb_claim_interface (libusb_device_handle *dev, int interface_number)
_dll.libusb_claim_interface.argtypes = [_libusb_device_handle, c_int]

# int libusb_release_interface (libusb_device_handle *dev, int interface_number)
_dll.libusb_release_interface.argtypes = [_libusb_device_handle, c_int]

# int libusb_set_interface_alt_setting (libusb_device_handle *dev, int interface_number, int alternate_setting)
_dll.libusb_set_interface_alt_setting.argtypes = [_libusb_device_handle, c_int, c_int]

# int libusb_reset_device (libusb_device_handle *dev)
_dll.libusb_reset_device.argtypes = [_libusb_device_handle]

# int libusb_kernel_driver_active (libusb_device_handle *dev, int interface)
_dll.libusb_kernel_driver_active.argtypes = [_libusb_device_handle, c_int]

# int libusb_detach_kernel_driver (libusb_device_handle *dev, int interface)
_dll.libusb_detach_kernel_driver.argtypes = [_libusb_device_handle, c_int]

# int libusb_attach_kernel_driver (libusb_device_handle *dev, int interface)
_dll.libusb_attach_kernel_driver.argtypes = [_libusb_device_handle, c_int]

# int libusb_get_device_descriptor (libusb_device *dev, struct libusb_device_descriptor *desc)
_dll.libusb_get_device_descriptor.argtypes = [c_void_p, POINTER(_libusb_device_descriptor)]

# int libusb_get_config_descriptor (libusb_device *dev, uint8_t config_index,
#                                struct libusb_config_descriptor **config)
_dll.libusb_get_config_descriptor.argtypes = [c_void_p, c_uint8,
        POINTER(POINTER(_libusb_config_descriptor))]

# void  libusb_free_config_descriptor (struct libusb_config_descriptor *config)
_dll.libusb_free_config_descriptor.argtypes = [POINTER(_libusb_config_descriptor)]

# int libusb_get_string_descriptor_ascii (libusb_device_handle *dev,
#                uint8_t desc_index, unsigned char *data, int length)
_dll.libusb_get_string_descriptor_ascii.argtypes = [_libusb_device_handle, c_uint8,
                                    POINTER(c_ubyte), c_int]

# int   libusb_control_transfer (libusb_device_handle *dev_handle, uint8_t bmRequestType,
#                               uint8_t bRequest, uint16_t wValue, uint16_t wIndex,
#                               unsigned char *data, uint16_t wLength, unsigned int timeout)
_dll.libusb_control_transfer.argtypes = [_libusb_device_handle, c_uint8, c_uint8, 
                                        c_uint16, c_uint16, POINTER(c_ubyte),
                                        c_uint16, c_uint]

#int libusb_bulk_transfer (struct libusb_device_handle *dev_handle, unsigned char endpoint,
#                           unsigned char *data, int length, int *transferred, unsigned int timeout)
_dll.libusb_bulk_transfer.argtypes = [_libusb_device_handle, c_ubyte, POINTER(c_ubyte),
                                        c_int, POINTER(c_int), c_uint]

# int libusb_interrupt_transfer(libusb_device_handle *dev_handle,
#	unsigned char endpoint, unsigned char *data, int length,
#	int *actual_length, unsigned int timeout);
_dll.libusb_interrupt_transfer.argtypes = [_libusb_device_handle, c_ubyte, POINTER(c_ubyte),
                                            c_int, POINTER(c_int), c_uint]

# check a libusb function call
def _check(retval):
    if isinstance(retval, int):
        retval = c_int(retval)
    if isinstance(retval, c_int):
        if retval.value < 0:
           from usb.core import USBError
           raise USBError(_str_error[retval.value])
    return retval

# wrap a device
class _Device(object):
    def __init__(self, devid):
        self.devid = _dll.libusb_ref_device(devid)
    def __del__(self):
        _dll.libusb_unref_device(self.devid)

# wrap a descriptor and keep a reference to another object
class _WrapDescriptor(object):
    def __init__(self, desc, obj = None):
        self.obj = obj
        self.desc = desc
    def __getattr__(self, name):
        return getattr(self.desc, name)

# wrap a configuration descriptor
class _ConfigDescriptor(object):
    def __init__(self, desc):
        self.desc = desc
    def __del__(self):
        _dll.libusb_free_config_descriptor(self.desc)
    def __getattr__(self, name):
        return getattr(self.desc.contents, name)

# initialize and finalize the library
class _Initializer(object):
    def __init__(self):
        _check(_dll.libusb_init(None))
    def __del__(self):
        _dll.libusb_exit(None)

_init = _Initializer()

# iterator for libusb devices
class _DevIterator(object):
    def __init__(self):
        self.dev_list = POINTER(c_void_p)()
        self.num_devs = _check(_dll.libusb_get_device_list(None, byref(self.dev_list))).value
    def __iter__(self):
        for i in range(self.num_devs):
            yield _Device(self.dev_list[i])
    def __del__(self):
        _dll.libusb_free_device_list(self.dev_list, 1)

# implementation of libusb 1.0 backend
class LibUSB(usb.backend.IBackend):
    def enumerate_devices(self):
        return _DevIterator()

    def get_device_descriptor(self, dev):
        dev_desc = _libusb_device_descriptor()
        _check(_dll.libusb_get_device_descriptor(dev.devid, byref(dev_desc)))
        return dev_desc

    def get_configuration_descriptor(self, dev, config):
        cfg = POINTER(_libusb_config_descriptor)()
        _check(_dll.libusb_get_config_descriptor(dev.devid, config, byref(cfg)))
        return _ConfigDescriptor(cfg)

    def get_interface_descriptor(self, dev, intf, alt, config):
        cfg = self.get_configuration_descriptor(dev, config)
        if intf >= cfg.bNumInterfaces:
            raise IndexError('Invalid interface index %d' % (intf))
        i = cfg.interface[intf]
        if alt >= i.num_altsetting:
            raise IndexError('Invalid alternate setting index %d' % (alt))
        return _WrapDescriptor(i.altsetting[alt], cfg)

    def get_endpoint_descriptor(self, dev, ep, intf, alt, config):
        i = self.get_interface_descriptor(dev, intf, alt, config)
        if ep > i.bNumEndpoints:
            raise IndexError('Invalid endpoint index %d' % (ep))
        return _WrapDescriptor(i.endpoint[ep], i)

    def open_device(self, dev):
        handle = _libusb_device_handle()
        _check(_dll.libusb_open(dev.devid, byref(handle)))
        return handle

    def close_device(self, dev_handle):
        _dll.libusb_close(dev_handle)

    def set_configuration(self, dev_handle, config_value):
        _check(_dll.libusb_set_configuration(dev_handle, config_value))

    def set_interface_altsetting(self, dev_handle, intf, altsetting):
        _check(_dll.libusb_set_interface_alt_setting(dev_handle, intf, altsetting))

    def claim_interface(self, dev_handle, intf):
        _check(_dll.libusb_claim_interface(dev_handle, intf))

    def release_interface(self, dev_handle, intf):
        _check(_dll.libusb_release_interface(dev_handle, intf))


# TODO: implement isochronous transfer
#    def isochronous_transfer(self, dev_handle, ep, data_or_length, intf, timeout):

    def bulk_write(self, dev_handle, ep, intf, data, timeout):
        return self.__write(_dll.libusb_bulk_transfer, dev_handle, ep, intf, data, timeout)

    def bulk_read(self, dev_handle, ep, intf, size, timeout):
        return self.__read(_dll.libusb_bulk_transfer, dev_handle, ep, intf, size, timeout)

    def intr_write(self, dev_handle, ep, intf, data, timeout):
        return self.__write(_dll.libusb_interrupt_transfer, dev_handle, ep, intf, data, timeout)

    def intr_read(self, dev_handle, ep, intf, size, timeout):
        return self.__read(_dll.libusb_interrupt_transfer, dev_handle, ep, intf, size, timeout)

    def ctrl_transfer(self, dev_handle, bmRequestType, bRequest, wValue,
                        wIndex, data_or_wLength, timeout):
        if usb.util.ctrl_direction(bmRequestType) == usb.util.CTRL_OUT:
            buff = data_or_wLength
        else:
            buff = array.array('B', data_or_wLength * '\x00')

        addr, length = buff.buffer_info()
        length *= buff.itemsize

        ret = _check(_dll.libusb_control_transfer(dev_handle, bmRequestType,
                        bRequest, wValue, wIndex, addr, length, timeout))

        if usb.util.ctrl_direction(bmRequestType) == usb.util.CTRL_OUT:
            return ret
        else:
            return buff[:ret]

    def reset_device(self, dev_handle):
        _check(_dll.libusb_reset_device(dev_handle))

    def is_kernel_driver_active(self, dev_handle, intf):
        return bool(_check(_dll.libusb_kernel_driver_active(dev_handle, intf)))

    def detach_kernel_driver(self, dev_handle, intf):
        _check(_dll.libusb_detach_kernel_driver(dev_handle, intf))

    def attach_kernel_driver(self, dev_handle, intf):
        _check(_dll.libusb_attach_kernel_driver(dev_handle, intf))

    def __write(self, fn, dev_handle, ep, intf, data, timeout):
        address, length = data.buffer_info()
        transferred = c_int()
        _check(fn(dev_handle, ep, address, length, byref(transferred), timeout))
        return transferred.value

    def __read(self, fn, dev_handle, ep, intf, size, timeout):
        buffer = array.array('B', '\x00' * size)
        address, length = buffer.buffer_info()
        transferred = c_int()
        _check(fn(dev_handle, ep, address, length, byref(transferred), timeout))
        return buffer[:transferred.value]

