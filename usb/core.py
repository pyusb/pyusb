# Copyright (C) 2009-2011 Wander Lairson Costa 
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

r"""usb.core - Core USB features.

This module exports:

Device - a class representing a USB device.
Configuration - a class representing a configuration descriptor.
Interface - a class representing an interface descriptor.
Endpoint - a class representing an endpoint descriptor.
find() - a function to find USB devices.
"""

__author__ = 'Wander Lairson Costa'

__all__ = ['Device', 'Configuration', 'Interface', 'Endpoint', 'find']

import usb.util as util
import copy
import operator
import usb._interop as _interop
import logging

_logger = logging.getLogger('usb.core')

_DEFAULT_TIMEOUT = 1000

def _set_attr(input, output, fields):
    for f in fields:
        setattr(output, f, int(getattr(input, f)))

class _ResourceManager(object):
    def __init__(self, dev, backend):
        self.backend = backend
        self._active_cfg_index = None
        self.dev = dev
        self.handle = None
        self._claimed_intf = _interop._set()
        self._alt_set = {}
        self._ep_type_map = {}

    def managed_open(self):
        if self.handle is None:
            self.handle = self.backend.open_device(self.dev)
        return self.handle

    def managed_close(self):
        if self.handle is not None:
            self.backend.close_device(self.handle)
            self.handle = None

    def managed_set_configuration(self, device, config):
        if config is None:
            cfg = device[0]
        elif isinstance(config, Configuration):
            cfg = config
        elif config == 0: # unconfigured state
            class FakeConfiguration(object):
                def __init__(self):
                    self.index = None
                    self.bConfigurationValue = 0
            cfg = FakeConfiguration()
        else:
            cfg = util.find_descriptor(device, bConfigurationValue=config)
        self.managed_open()
        self.backend.set_configuration(self.handle, cfg.bConfigurationValue)
        # cache the index instead of the object to avoid cyclic references
        # of the device and Configuration (Device tracks the _ResourceManager,
        # which tracks the Configuration, which tracks the Device)
        self._active_cfg_index = cfg.index
        # after changing configuration, our alternate setting and endpoint type caches
        # are not valid anymore
        self._ep_type_map.clear()
        self._alt_set.clear()

    def managed_claim_interface(self, device, intf):
        self.managed_open()
        if intf is None:
            cfg = self.get_active_configuration(device)
            i = cfg[(0,0)].bInterfaceNumber
        elif isinstance(intf, Interface):
            i = intf.bInterfaceNumber
        else:
            i = intf
        if i not in self._claimed_intf:
            self.backend.claim_interface(self.handle, i)
            self._claimed_intf.add(i)

    def managed_release_interface(self, device, intf):
        if intf is None:
            cfg = self.get_active_configuration(device)
            i = cfg[(0,0)].bInterfaceNumber
        elif isinstance(intf, Interface):
            i = intf.bInterfaceNumber
        else:
            i = intf
        if i in self._claimed_intf:
            self.backend.release_interface(self.handle, i)
            self._claimed_intf.remove(i)

    def managed_set_interface(self, device, intf, alt):
        if intf is None:
            i = self.get_interface(device, intf)
        elif isinstance(intf, Interface):
            i = intf
        else:
            cfg = self.get_active_configuration(device)
            if alt is not None:
                i = util.find_descriptor(cfg, bInterfaceNumber=intf, bAlternateSetting=alt)
            else:
                i = util.find_descriptor(cfg, bInterfaceNumber=intf)
        self.managed_claim_interface(device, i)
        if alt is None:
            alt = i.bAlternateSetting
        self.backend.set_interface_altsetting(self.handle, i.bInterfaceNumber, alt)
        self._alt_set[i.bInterfaceNumber] = alt

    def get_interface(self, device, intf):
        # TODO: check the viability of issuing a GET_INTERFACE
        # request when we don't have a alternate setting cached
        if intf is None:
            cfg = self.get_active_configuration(device)
            return cfg[(0,0)]
        elif isinstance(intf, Interface):
            return intf
        else:
            cfg = self.get_active_configuration(device)
            if intf in self._alt_set:
                return util.find_descriptor(cfg,
                                            bInterfaceNumber=intf,
                                            bAlternateSetting=self._alt_set[intf])
            else:
                return util.find_descriptor(cfg, bInterfaceNumber=intf)

    def get_active_configuration(self, device):
        if self._active_cfg_index is None:
            self.managed_open()
            cfg = util.find_descriptor(
                    device,
                    bConfigurationValue=self.backend.get_configuration(self.handle)
                )
            if cfg is None:
                raise USBError('Configuration not set')
            self._active_cfg_index = cfg.index
            return cfg
        return device[self._active_cfg_index]

    def get_endpoint_type(self, device, address, intf):
        intf = self.get_interface(device, intf)
        key = (address, intf.bInterfaceNumber, intf.bAlternateSetting)
        try:
            return self._ep_type_map[key]
        except KeyError:
            e = util.find_descriptor(intf, bEndpointAddress=address)
            etype = util.endpoint_type(e.bmAttributes)
            self._ep_type_map[key] = etype
            return etype

    def release_all_interfaces(self, device):
        claimed = copy.copy(self._claimed_intf)
        for i in claimed:
            self.managed_release_interface(device, i)

    def dispose(self, device, close_handle = True):
        self.release_all_interfaces(device)
        if close_handle:
            self.managed_close()
        self._ep_type_map.clear()
        self._alt_set.clear()
        self._active_cfg_index = None

class USBError(IOError):
    r"""Exception class for USB errors.
    
    Backends must raise this exception when USB related errors occur.
    The backend specific error code is available through the
    'backend_error_code' member variable.
    """

    def __init__(self, strerror, error_code = None, errno = None):
        r"""Initialize the object.

        This initializes the USBError object. The strerror and errno are passed
        to the parent object. The error_code parameter is attributed to the
        backend_error_code member variable.
        """
        IOError.__init__(self, errno, strerror)
        self.backend_error_code = error_code

class Endpoint(object):
    r"""Represent an endpoint object.

    This class contains all fields of the Endpoint Descriptor
    according to the USB Specification. You may access them as class
    properties.  For example, to access the field bEndpointAddress
    of the endpoint descriptor:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     for i in cfg:
    >>>         for e in i:
    >>>             print e.bEndpointAddress
    """

    def __init__(self, device, endpoint, interface = 0,
                    alternate_setting = 0, configuration = 0):
        r"""Initialize the Endpoint object.

        The device parameter is the device object returned by the find()
        function. endpoint is the endpoint logical index (not the endpoint address).
        The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). The interface
        parameter is the interface logical index (not the bInterfaceNumber field)
        and alternate_setting is the alternate setting logical index (not the
        bAlternateSetting value).  Not every interface has more than one alternate
        setting.  In this case, the alternate_setting parameter should be zero.
        By "logical index" we mean the relative order of the configurations returned by the
        peripheral as a result of GET_DESCRIPTOR request.
        """
        self.device = device
        intf = Interface(device, interface, alternate_setting, configuration)
        self.interface = intf.bInterfaceNumber
        self.index = endpoint

        backend = device._ctx.backend

        desc = backend.get_endpoint_descriptor(
                    device._ctx.dev,
                    endpoint,
                    interface,
                    alternate_setting,
                    configuration
                )

        _set_attr(
                desc,
                self,
                (
                    'bLength',
                    'bDescriptorType',
                    'bEndpointAddress',
                    'bmAttributes',
                    'wMaxPacketSize',
                    'bInterval',
                    'bRefresh',
                    'bSynchAddress'
                )
            )

    def write(self, data, timeout = None):
        r"""Write data to the endpoint.
        
        The parameter data contains the data to be sent to the endpoint and
        timeout is the time limit of the operation. The transfer type and
        endpoint address are automatically inferred.

        The method returns the number of bytes written.

        For details, see the Device.write() method.
        """
        return self.device.write(self.bEndpointAddress, data, self.interface, timeout)

    def read(self, size, timeout = None):
        r"""Read data from the endpoint.
        
        The parameter size is the number of bytes to read and timeout is the
        time limit of the operation.The transfer type and endpoint address
        are automatically inferred.

        The method returns an array.array object with the data read.

        For details, see the Device.read() method.
        """
        return self.device.read(self.bEndpointAddress, size, self.interface, timeout)

class Interface(object):
    r"""Represent an interface object.

    This class contains all fields of the Interface Descriptor
    according to the USB Specification. You may access them as class
    properties.  For example, to access the field bInterfaceNumber
    of the interface descriptor:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     for i in cfg:
    >>>         print i.bInterfaceNumber
    """

    def __init__(self, device, interface = 0,
            alternate_setting = 0, configuration = 0):
        r"""Initialize the interface object.

        The device parameter is the device object returned by the find()
        function. The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). The interface
        parameter is the interface logical index (not the bInterfaceNumber field)
        and alternate_setting is the alternate setting logical index (not the
        bAlternateSetting value).  Not every interface has more than one alternate
        setting.  In this case, the alternate_setting parameter should be zero.
        By "logical index" we mean the relative order of the configurations returned by the
        peripheral as a result of GET_DESCRIPTOR request.
        """
        self.device = device
        self.alternate_index = alternate_setting
        self.index = interface
        self.configuration = configuration

        backend = device._ctx.backend

        desc = backend.get_interface_descriptor(
                    self.device._ctx.dev,
                    interface,
                    alternate_setting,
                    configuration
                )

        _set_attr(
                desc,
                self,
                (
                    'bLength',
                    'bDescriptorType',
                    'bInterfaceNumber',
                    'bAlternateSetting',
                    'bNumEndpoints',
                    'bInterfaceClass',
                    'bInterfaceSubClass',
                    'bInterfaceProtocol',
                    'iInterface',
                )
            )

    def set_altsetting(self):
        r"""Set the interface alternate setting."""
        self.device.set_interface_altsetting(
            self.bInterfaceNumber,
            self.bAlternateSetting
        )

    def __iter__(self):
        r"""Iterate over all endpoints of the interface."""
        for i in range(self.bNumEndpoints):
            yield Endpoint(
                    self.device,
                    i,
                    self.index,
                    self.alternate_index,
                    self.configuration
                )
    def __getitem__(self, index):
        r"""Return the Endpoint object in the given position."""
        return Endpoint(
                self.device,
                index,
                self.index,
                self.alternate_index,
                self.configuration
            )

class Configuration(object):
    r"""Represent a configuration object.
 
    This class contains all fields of the Configuration Descriptor
    according to the USB Specification. You may access them as class
    properties.  For example, to access the field bConfigurationValue
    of the configuration descriptor:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     print cfg.bConfigurationValue
    """

    def __init__(self, device, configuration = 0):
        r"""Initialize the configuration object.

        The device parameter is the device object returned by the find()
        function. The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). By "logical index"
        we mean the relative order of the configurations returned by the
        peripheral as a result of GET_DESCRIPTOR request.
        """
        self.device = device
        self.index = configuration

        backend = device._ctx.backend

        desc = backend.get_configuration_descriptor(
                self.device._ctx.dev,
                configuration
            )

        _set_attr(
                desc,
                self,
                (
                    'bLength',
                    'bDescriptorType',
                    'wTotalLength',
                    'bNumInterfaces',
                    'bConfigurationValue',
                    'iConfiguration',
                    'bmAttributes',
                    'bMaxPower'
                )
            )

    def set(self):
        r"""Set this configuration as the active one."""
        self.device.set_configuration(self.bConfigurationValue)

    def __iter__(self):
        r"""Iterate over all interfaces of the configuration."""
        for i in range(self.bNumInterfaces):
            alt = 0
            try:
                while True:
                    yield Interface(self.device, i, alt, self.index)
                    alt += 1
            except (USBError, IndexError):
                pass
    def __getitem__(self, index):
        r"""Return the Interface object in the given position.

        index is a tuple of two values with interface index and
        alternate setting index, respectivally. Example:

        >>> interface = config[(0, 0)]
        """
        return Interface(self.device, index[0], index[1], self.index)


class Device(object):
    r"""Device object.
    
    This class contains all fields of the Device Descriptor according
    to the USB Specification. You may access them as class properties.
    For example, to access the field bDescriptorType of the device
    descriptor:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> dev.bDescriptorType

    Additionally, the class provides methods to communicate with
    the hardware. Typically, an application will first call the
    set_configuration() method to put the device in a known configured
    state, optionally call the set_interface_altsetting() to select the
    alternate setting (if there is more than one) of the interface used,
    and call the write() and read() method to send and receive data.

    When working in a new hardware, one first try would be like this:

    >>> import usb.core
    >>> dev = usb.core.find(idVendor=myVendorId, idProduct=myProductId)
    >>> dev.set_configuration()
    >>> dev.write(1, 'test')

    This sample finds the device of interest (myVendorId and myProductId should be
    replaced by the corresponding values of your device), then configures the device
    (by default, the configuration value is 1, which is a typical value for most
    devices) and then writes some data to the endpoint 0x01.

    Timeout values for the write, read and ctrl_transfer methods are specified in
    miliseconds. If the parameter is omitted, Device.default_timeout value will
    be used instead. This property can be set by the user at anytime.
    """

    def __init__(self, dev, backend):
        r"""Initialize the Device object.

        Library users should normally get a Device instance through
        the find function. The dev parameter is the identification
        of a device to the backend and its meaning is opaque outside
        of it. The backend parameter is a instance of a backend
        object.
        """
        self._ctx = _ResourceManager(dev, backend)
        self.__default_timeout = _DEFAULT_TIMEOUT

        desc = backend.get_device_descriptor(dev)

        _set_attr(
                desc,
                self,
                (
                    'bLength',
                    'bDescriptorType',
                    'bcdUSB',
                    'bDeviceClass',
                    'bDeviceSubClass',
                    'bDeviceProtocol',
                    'bMaxPacketSize0',
                    'idVendor',
                    'idProduct',
                    'bcdDevice',
                    'iManufacturer',
                    'iProduct',
                    'iSerialNumber',
                    'bNumConfigurations',
                    'address',
                    'bus'
                )
            )

        self.bus = int(desc.bus) if desc.bus is not None else None
        self.address = int(desc.address) if desc.address is not None else None

    def set_configuration(self, configuration = None):
        r"""Set the active configuration.
        
        The configuration parameter is the bConfigurationValue field of the
        configuration you want to set as active. If you call this method
        without parameter, it will use the first configuration found.
        As a device hardly ever has more than one configuration, calling
        the method without parameter is enough to get the device ready.
        """
        self._ctx.managed_set_configuration(self, configuration)

    def get_active_configuration(self):
        r"""Return a Configuration object representing the current configuration set."""
        return self._ctx.get_active_configuration(self)

    def set_interface_altsetting(self, interface = None, alternate_setting = None):
        r"""Set the alternate setting for an interface.
 
        When you want to use an interface and it has more than one alternate setting,
        you should call this method to select the alternate setting you would like
        to use. If you call the method without one or the two parameters, it will
        be selected the first one found in the Device in the same way of set_configuration
        method.

        Commonly, an interface has only one alternate setting and this call is
        not necessary. For most of the devices, either it has more than one alternate
        setting or not, it is not harmful to make a call to this method with no arguments,
        as devices will silently ignore the request when there is only one alternate
        setting, though the USB Spec allows devices with no additional alternate setting
        return an error to the Host in response to a SET_INTERFACE request.

        If you are in doubt, you may want to call it with no arguments wrapped by
        a try/except clause:

        >>> try:
        >>>     dev.set_interface_altsetting()
        >>> except usb.core.USBError:
        >>>     pass
        """
        self._ctx.managed_set_interface(self, interface, alternate_setting)

    def reset(self):
        r"""Reset the device."""
        self._ctx.dispose(self, False)
        self._ctx.backend.reset_device(self._ctx.handle)
        self._ctx.dispose(self, True)

    def write(self, endpoint, data, interface = None, timeout = None):
        r"""Write data to the endpoint.

        This method is used to send data to the device. The endpoint parameter
        corresponds to the bEndpointAddress member whose endpoint you want to
        communicate with. The interface parameter is the bInterfaceNumber field
        of the interface descriptor which contains the endpoint. If you do not
        provide one, the first one found will be used, as explained in the
        set_interface_altsetting() method.

        The data parameter should be a sequence like type convertible to
        array type (see array module).

        The timeout is specified in miliseconds.

        The method returns the number of bytes written.
        """
        backend = self._ctx.backend

        fn_map = {
                    util.ENDPOINT_TYPE_BULK:backend.bulk_write,
                    util.ENDPOINT_TYPE_INTR:backend.intr_write,
                    util.ENDPOINT_TYPE_ISO:backend.iso_write
                }

        intf = self._ctx.get_interface(self, interface)
        fn = fn_map[self._ctx.get_endpoint_type(self, endpoint, intf)]
        self._ctx.managed_claim_interface(self, intf)

        return fn(
                self._ctx.handle,
                endpoint,
                intf.bInterfaceNumber,
                _interop.as_array(data),
                self.__get_timeout(timeout)
            )

    def read(self, endpoint, size, interface = None, timeout = None):
        r"""Read data from the endpoint.

        This method is used to receive data from the device. The endpoint parameter
        corresponds to the bEndpointAddress member whose endpoint you want to
        communicate with. The interface parameter is the bInterfaceNumber field
        of the interface descriptor which contains the endpoint. If you do not
        provide one, the first one found will be used, as explained in the
        set_interface_altsetting() method. The size parameters tells how many
        bytes you want to read.

        The timeout is specified in miliseconds.

        The method returns an array object with the data read.
        """
        backend = self._ctx.backend

        fn_map = {
                    util.ENDPOINT_TYPE_BULK:backend.bulk_read,
                    util.ENDPOINT_TYPE_INTR:backend.intr_read,
                    util.ENDPOINT_TYPE_ISO:backend.iso_read
                }

        intf = self._ctx.get_interface(self, interface)
        fn = fn_map[self._ctx.get_endpoint_type(self, endpoint, intf)]
        self._ctx.managed_claim_interface(self, intf)

        return fn(
                self._ctx.handle,
                endpoint,
                intf.bInterfaceNumber,
                size,
                self.__get_timeout(timeout)
            )


    def ctrl_transfer(self, bmRequestType, bRequest, wValue=0, wIndex=0,
            data_or_wLength = None, timeout = None):
        r"""Do a control transfer on the endpoint 0.

        This method is used to issue a control transfer over the
        endpoint 0(endpoint 0 is required to always be a control endpoint).

        The parameters bmRequestType, bRequest, wValue and wIndex are the
        same of the USB Standard Control Request format.

        Control requests may or may not have a data payload to write/read.
        In cases which it has, the direction bit of the bmRequestType
        field is used to infere the desired request direction. For
        host to device requests (OUT), data_or_wLength parameter is
        the data payload to send, and it must be a sequence type convertible
        to an array object. In this case, the return value is the number of data
        payload written. For device to host requests (IN), data_or_wLength
        is the wLength parameter of the control request specifying the
        number of bytes to read in data payload. In this case, the return
        value is the data payload read, as an array object.
        """
        if util.ctrl_direction(bmRequestType) == util.CTRL_OUT:
            a = _interop.as_array(data_or_wLength)
        elif data_or_wLength is None:
            a = 0
        else:
            a = data_or_wLength

        self._ctx.managed_open()

        return self._ctx.backend.ctrl_transfer(
                                    self._ctx.handle,
                                    bmRequestType,
                                    bRequest,
                                    wValue,
                                    wIndex,
                                    a,
                                    self.__get_timeout(timeout)
                                )

    def is_kernel_driver_active(self, interface):
        r"""Determine if there is kernel driver associated with the interface.

        If a kernel driver is active, and the object will be unable to perform I/O.
        """
        self._ctx.managed_open()
        return self._ctx.backend.is_kernel_driver_active(self._ctx.handle, interface)

    def detach_kernel_driver(self, interface):
        r"""Detach a kernel driver.

        If successful, you will then be able to perform I/O.
        """
        self._ctx.managed_open()
        self._ctx.backend.detach_kernel_driver(self._ctx.handle, interface)

    def attach_kernel_driver(self, interface):
        r"""Re-attach an interface's kernel driver, which was previously
        detached using detach_kernel_driver()."""
        self._ctx.managed_open()
        self._ctx.backend.attach_kernel_driver(self._ctx.handle, interface)

    def __iter__(self):
        r"""Iterate over all configurations of the device."""
        for i in range(self.bNumConfigurations):
            yield Configuration(self, i)

    def __getitem__(self, index):
        r"""Return the Configuration object in the given position."""
        return Configuration(self, index)

    def __del__(self):
        self._ctx.dispose(self)

    def __get_timeout(self, timeout):
        if timeout is not None:
            return timeout
        return self.__default_timeout

    def __set_def_tmo(self, tmo):
        if tmo < 0:
            raise ValueError('Timeout cannot be a negative value')
        self.__default_timeout = tmo

    def __get_def_tmo(self):
        return self.__default_timeout

    default_timeout = property(
                        __get_def_tmo,
                        __set_def_tmo,
                        doc = 'Default timeout for transfer I/O functions'
                    )

def find(find_all=False, backend = None, custom_match = None, **args):
    r"""Find an USB device and return it.

    find() is the function used to discover USB devices.
    You can pass as arguments any combination of the
    USB Device Descriptor fields to match a device. For example:

    find(idVendor=0x3f4, idProduct=0x2009)

    will return the Device object for the device with
    idVendor Device descriptor field equals to 0x3f4 and
    idProduct equals to 0x2009.

    If there is more than one device which matchs the criteria,
    the first one found will be returned. If a matching device cannot
    be found the function returns None. If you want to get all
    devices, you can set the parameter find_all to True, then find
    will return an list with all matched devices. If no matching device
    is found, it will return an empty list. Example:

    printers = find(find_all=True, bDeviceClass=7)

    This call will get all the USB printers connected to the system.
    (actually may be not, because some devices put their class
     information in the Interface Descriptor).

    You can also use a customized match criteria:

    dev = find(custom_match = lambda d: d.idProduct=0x3f4 and d.idvendor=0x2009)

    A more accurate printer finder using a customized match would be like
    so:

    def is_printer(dev):
        import usb.util
        if dev.bDeviceClass == 7:
            return True
        for cfg in dev:
            if usb.util.find_descriptor(cfg, bInterfaceClass=7) is not None:
                return True

    printers = find(find_all=True, custom_match = is_printer)

    Now even if the device class code is in the interface descriptor the
    printer will be found.

    You can combine a customized match with device descriptor fields. In this
    case, the fields must match and the custom_match must return True. In the our
    previous example, if we would like to get all printers belonging to the
    manufacturer 0x3f4, the code would be like so:

    printers = find(find_all=True, idVendor=0x3f4, custom_match=is_printer)

    If you want to use find as a 'list all devices' function, just call
    it with find_all = True:

    devices = find(find_all=True)

    Finally, you may pass a custom backend to the find function:

    find(backend = MyBackend())

    PyUSB has builtin backends for libusb 0.1, libusb 1.0 and OpenUSB.
    If you do not supply a backend explicitly, find() function will select
    one of the predefineds backends according to system availability.

    Backends are explained in the usb.backend module.
    """

    def device_iter(k, v):
        for dev in backend.enumerate_devices():
            d = Device(dev, backend)
            if (custom_match is None or custom_match(d)) and \
                _interop._reduce(
                        lambda a, b: a and b,
                        map(
                            operator.eq,
                            v,
                            map(lambda i: getattr(d, i), k)
                        ),
                        True
                    ):
                yield d

    if backend is None:
        import usb.backend.libusb10 as libusb10
        import usb.backend.libusb01 as libusb01
        import usb.backend.openusb as openusb

        for m in (libusb10, openusb, libusb01):
            backend = m.get_backend()
            if backend is not None:
                _logger.info('find(): using backend "%s"', m.__name__)
                break
        else:
            raise ValueError('No backend available')

    k, v = args.keys(), args.values()
    
    if find_all:
        return [d for d in device_iter(k, v)]
    else:
        try:
            return _interop._next(device_iter(k, v))
        except StopIteration:
            return None
