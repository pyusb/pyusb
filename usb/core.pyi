# Copyright 2009-2017 Wander Lairson Costa
# Copyright 2009-2021 PyUSB contributors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

r"""usb.core - Core USB features.

This module exports:

Device - a class representing a USB device.
Configuration - a class representing a configuration descriptor.
Interface - a class representing an interface descriptor.
Endpoint - a class representing an endpoint descriptor.
find() - a function to find USB devices.
show_devices() - a function to show the devices present.
"""

__author__ = 'Wander Lairson Costa'

__all__ = ['Device', 'Configuration', 'Interface', 'Endpoint', 'USBError',
           'USBTimeoutError', 'NoBackendError', 'find', 'show_devices']

import usb.util as util
import copy
import operator
import usb._interop as _interop
import usb._objfinalizer as _objfinalizer
import usb._lookup as _lu
import logging
import array
import threading
import functools
from typing import Union, Any, Tuple, Generator, Callable

from usb.backend import IBackend

_logger = logging.getLogger('usb.core')

_DEFAULT_TIMEOUT: int = 1000
_sentinel: object


def _set_attr(input: Any, output: Any, fields: Tuple[str]) -> None:
    ...


def _try_getattr(object: object, name: str) -> Any:
    ...


def _try_get_string(dev: Device, index: int, langid: int = None, default_str_i0: str = "",
                    default_access_error: str = "Error Accessing String") -> str:
    """ try to get a string, but return a string no matter what
    """


def _try_lookup(table: dict, value: Any, default: str = "") -> str:
    """ try to get a string from the lookup table, return "" instead of key
    error
    """


class _DescriptorInfo(str):
    """ this class is used so that when a descriptor is shown on the
    terminal it is propely formatted """

    def __repr__(self) -> str:
        ...


def synchronized(f: Callable) -> Callable:
    """decorator"""


class _ResourceManager:
    backend: usb.backend.IBackend
    _active_cfg_index: int
    dev: Device
    handle: Any
    _claimed_intf: set
    _intf_setting: dict
    _ep_info: dict
    lock: threading.RLock

    def __init__(self, dev: Device, backend: usb.backend.IBackend) -> None:
        ...

    @synchronized
    def managed_open(self) -> None:
        ...

    @synchronized
    def managed_close(self) -> None:
        ...

    @synchronized
    def managed_set_configuration(self, device: Device, config: Configuration) -> None:
        ...

    @synchronized
    def managed_claim_interface(self, device: Device, intf: Union[Interface, int]) -> None:
        ...

    @synchronized
    def managed_release_interface(self, device: Device, intf: Union[Interface, int]) -> None:
        ...

    @synchronized
    def managed_set_interface(self, device: Device, intf: Union[Interface, int], alt: int) -> None:
        ...

    @synchronized
    def setup_request(self, device: Device, endpoint: Union[Endpoint, int]) -> None:
        # we need the endpoint address, but the "endpoint" parameter
        # can be either the a Endpoint object or the endpoint address itself
        ...

    # Find the interface and endpoint objects which endpoint address belongs to
    @synchronized
    def get_interface_and_endpoint(self, device: Device, endpoint_address: int) -> (Interface, Endpoint):
        ...

    @synchronized
    def get_active_configuration(self, device: Device) -> Configuration:
        ...

    @synchronized
    def release_all_interfaces(self, device: Device) -> None:
        ...

    @synchronized
    def dispose(self, device: Device, close_handle: bool = True) -> None:
        ...


class USBError(IOError):
    r"""Exception class for USB errors.

    Backends must raise this exception when USB related errors occur.  The
    backend specific error code is available through the backend_error_code
    member variable.
    """

    def __init__(self, strerror: str, error_code: Any = None, errno: Any = None) -> None:
        r"""Initialize the object.

        This initializes the USBError object. The strerror and errno are passed
        to the parent object. The error_code parameter is attributed to the
        backend_error_code member variable.
        """


class USBTimeoutError(USBError):
    r"""Exception class for connection timeout errors.

    Backends must raise this exception when a call on a USB connection returns
    a timeout error code.
    """


class NoBackendError(ValueError):
    r"""Exception class when a valid backend is not found."""


class Endpoint:
    r"""Represent an endpoint object.

    This class contains all fields of the Endpoint Descriptor according to the
    USB Specification. You can access them as class properties. For example, to
    access the field bEndpointAddress of the endpoint descriptor, you can do so:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     for i in cfg:
    >>>         for e in i:
    >>>             print e.bEndpointAddress
    """
    device: Device
    index: int

    bLength: int
    bDescriptorType: int
    bEndpointAddress: int
    bmAttributes: int
    wMaxPacketSize: int
    bInterval: int
    bRefresh: int
    bSynchAddress: int
    extra_descriptors: int

    def __init__(self, device: Device, endpoint: int, interface: int = 0,
                 alternate_setting: int = 0, configuration: int = 0):
        r"""Initialize the Endpoint object.

        The device parameter is the device object returned by the find()
        function. endpoint is the endpoint logical index (not the endpoint
        address). The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). The interface
        parameter is the interface logical index (not the bInterfaceNumber
        field) and alternate_setting is the alternate setting logical index
        (not the bAlternateSetting value). An interface may have only one
        alternate setting. In this case, the alternate_setting parameter
        should be zero. By "logical index" we mean the relative order of the
        configurations returned by the peripheral as a result of GET_DESCRIPTOR
        request.
        """

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        ...

    def write(self, data: Union[int, bytes, bytearray], timeout: int = None) -> int:
        r"""Write data to the endpoint.

        The parameter data contains the data to be sent to the endpoint and
        timeout is the time limit of the operation. The transfer type and
        endpoint address are automatically inferred.

        The method returns the number of bytes written.

        For details, see the Device.write() method.
        """
        return self.device.write(self, data, timeout)

    def read(self, size_or_buffer: Union[int, bytearray], timeout=None) -> array.array:
        r"""Read data from the endpoint.

        The parameter size_or_buffer is either the number of bytes to
        read or an array object where the data will be put in and timeout is the
        time limit of the operation. The transfer type and endpoint address
        are automatically inferred.

        The method returns either an array object or the number of bytes
        actually read.

        For details, see the Device.read() method.
        """

    def clear_halt(self) -> None:
        r"""Clear the halt/status condition of the endpoint."""

    def _str(self) -> str:
        ...


class Interface:
    r"""Represent an interface object.

    This class contains all fields of the Interface Descriptor
    according to the USB Specification. You may access them as class
    properties. For example, to access the field bInterfaceNumber
    of the interface descriptor, you can do so:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     for i in cfg:
    >>>         print i.bInterfaceNumber
    """
    device: Device
    alternate_index: int
    index: int
    configuration: int

    bLength: int
    bDescriptorType: int
    bInterfaceNumber: int
    bAlternateSetting: int
    bNumEndpoints: int
    bInterfaceClass: int
    bInterfaceSubClass: int
    bInterfaceProtocol: int
    iInterface: int
    extra_descriptors: list[int]

    def __init__(self, device: Device, interface: int = 0,
                 alternate_setting: int = 0, configuration: int = 0):
        r"""Initialize the interface object.

        The device parameter is the device object returned by the find()
        function. The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). The interface
        parameter is the interface logical index (not the bInterfaceNumber
        field) and alternate_setting is the alternate setting logical index
        (not the bAlternateSetting value). An interface may have only one
        alternate setting. In this case, the alternate_setting parameter
        should be zero.  By "logical index" we mean the relative order of
        the configurations returned by the peripheral as a result of
        GET_DESCRIPTOR request.
        """

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        """Show all information for the interface."""

    def endpoints(self) -> Tuple[Endpoint]:
        r"""Return a tuple of the interface endpoints."""

    def set_altsetting(self) -> None:
        r"""Set the interface alternate setting."""

    def __iter__(self) -> Generator[Endpoint, Any, None]:
        r"""Iterate over all endpoints of the interface."""

    def __getitem__(self, index: int) -> Endpoint:
        r"""Return the Endpoint object in the given position."""

    def _str(self) -> str:
        ...

    def _get_full_descriptor_str(self) -> str:
        ...


class Configuration:
    r"""Represent a configuration object.

    This class contains all fields of the Configuration Descriptor according to
    the USB Specification. You may access them as class properties.  For
    example, to access the field bConfigurationValue of the configuration
    descriptor, you can do so:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> for cfg in dev:
    >>>     print cfg.bConfigurationValue
    """
    device: Device
    index: int

    bLength: int
    bDescriptorType: int
    wTotalLength: int
    bNumInterfaces: int
    bConfigurationValue: int
    iConfiguration: int
    bmAttributes: int
    bMaxPower: int
    extra_descriptors: list[int]

    def __init__(self, device: Device, configuration: int = 0) -> None:
        r"""Initialize the configuration object.

        The device parameter is the device object returned by the find()
        function. The configuration parameter is the logical index of the
        configuration (not the bConfigurationValue field). By "logical index"
        we mean the relative order of the configurations returned by the
        peripheral as a result of GET_DESCRIPTOR request.
        """

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        ...

    def interfaces(self) -> Tuple[Interface]:
        r"""Return a tuple of the configuration interfaces."""

    def set(self) -> Any:
        r"""Set this configuration as the active one."""

    def __iter__(self) -> Generator[Interface, Any, None]:
        r"""Iterate over all interfaces of the configuration."""

    def __getitem__(self, index: Tuple[int, int]) -> Interface:
        r"""Return the Interface object in the given position.

        index is a tuple of two values with interface index and
        alternate setting index, respectivally. Example:

        >>> interface = config[(0, 0)]
        """

    def _get_power_multiplier(self) -> int:
        ...

    def _str(self) -> str:
        ...

    def _get_full_descriptor_str(self) -> str:
        ...


class Device(_objfinalizer.AutoFinalizedObject):
    r"""Device object.

    This class contains all fields of the Device Descriptor according to the
    USB Specification. You may access them as class properties.  For example,
    to access the field bDescriptorType of the device descriptor, you can
    do so:

    >>> import usb.core
    >>> dev = usb.core.find()
    >>> dev.bDescriptorType

    Additionally, the class provides methods to communicate with the hardware.
    Typically, an application will first call the set_configuration() method to
    put the device in a known configured state, optionally call the
    set_interface_altsetting() to select the alternate setting (if there is
    more than one) of the interface used, and call the write() and read()
    methods to send and receive data, respectively.

    When working in a new hardware, the first try could be like this:

    >>> import usb.core
    >>> dev = usb.core.find(idVendor=myVendorId, idProduct=myProductId)
    >>> dev.set_configuration()
    >>> dev.write(1, test)

    This sample finds the device of interest (myVendorId and myProductId should
    be replaced by the corresponding values of your device), then configures
    the device (by default, the configuration value is 1, which is a typical
    value for most devices) and then writes some data to the endpoint 0x01.

    Timeout values for the write, read and ctrl_transfer methods are specified
    in milliseconds. If the parameter is omitted, Device.default_timeout value
    will be used instead. This property can be set by the user at anytime.
    """

    _ctx: _ResourceManager
    __default_timeout: int
    _serial_number: int
    _product: int
    _manufacturer: int
    _langids: Tuple[int]

    bLength: int
    bDescriptorType: int
    bcdUSB: int
    bDeviceClass: int
    bDeviceSubClass: int
    bDeviceProtocol: int
    bMaxPacketSize0: int
    idVendor: int
    idProduct: int
    bcdDevice: int
    iManufacturer: int
    iProduct: int
    iSerialNumber: int
    bNumConfigurations: int
    address: int
    bus: int
    port_number: int
    port_numbers: Tuple[int]
    speed: int

    _has_parent: bool
    _parent: Union[Device, None]

    def __eq__(self, other) -> bool:
        ...

    def __hash__(self) -> int:
        ...

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        ...

    def configurations(self) -> Tuple[Configuration]:
        r"""Return a tuple of the device configurations."""

    def __init__(self, dev, backend) -> None:
        r"""Initialize the Device object.

        Library users should normally get a Device instance through
        the find function. The dev parameter is the identification
        of a device to the backend and its meaning is opaque outside
        of it. The backend parameter is a instance of a backend
        object.
        """

    @property
    def langids(self) -> Tuple | Tuple[int, ...]:
        """ Return the USB devices supported language ID codes.

        These are 16-bit codes familiar to Windows developers, where for
        example instead of en-US you say 0x0409. USB_LANGIDS.pdf on the usb.org
        developer site for more info. String requests using a LANGID not
        in this array should not be sent to the device.

        This property will cause some USB traffic the first time it is accessed
        and cache the resulting value for future use.
        """

    @property
    def serial_number(self) -> str:
        """ Return the USB devices serial number string descriptor.

        This property will cause some USB traffic the first time it is accessed
        and cache the resulting value for future use.
        """

    @property
    def product(self) -> str:
        """ Return the USB devices product string descriptor.

        This property will cause some USB traffic the first time it is accessed
        and cache the resulting value for future use.
        """

    @property
    def parent(self) -> Union[Device, None]:
        """ Return the parent device. """

    @property
    def manufacturer(self) -> str:
        """ Return the USB devices manufacturer string descriptor.

        This property will cause some USB traffic the first time it is accessed
        and cache the resulting value for future use.
        """

    @property
    def backend(self) -> IBackend:
        """Return the backend being used by the device."""

    def set_configuration(self, configuration: Configuration = None) -> Any:
        r"""Set the active configuration.

        The configuration parameter is the bConfigurationValue field of the
        configuration you want to set as active. If you call this method
        without parameter, it will use the first configuration found.  As a
        device hardly ever has more than one configuration, calling the method
        without arguments is enough to get the device ready.
        """

    def get_active_configuration(self) -> Configuration:
        r"""Return a Configuration object representing the current
        configuration set.
        """

    def set_interface_altsetting(self, interface: Interface = None, alternate_setting: int = None) -> None:
        r"""Set the alternate setting for an interface.

        When you want to use an interface and it has more than one alternate
        setting, you should call this method to select the appropriate
        alternate setting. If you call the method without one or the two
        parameters, it will be selected the first one found in the Device in
        the same way of the set_configuration method.

        Commonly, an interface has only one alternate setting and this call is
        not necessary. For most devices, either it has more than one
        alternate setting or not, it is not harmful to make a call to this
        method with no arguments, as devices will silently ignore the request
        when there is only one alternate setting, though the USB Spec allows
        devices with no additional alternate setting return an error to the
        Host in response to a SET_INTERFACE request.

        If you are in doubt, you may want to call it with no arguments wrapped
        by a try/except clause:

        >>> try:
        >>>     dev.set_interface_altsetting()
        >>> except usb.core.USBError:
        >>>     pass
        """

    def clear_halt(self, ep: Endpoint) -> None:
        r""" Clear the halt/stall condition for the endpoint ep."""

    def reset(self) -> None:
        r"""Reset the device."""

    def write(self, endpoint: Endpoint, data: [bytes, bytearray], timeout: int = None) -> int:
        r"""Write data to the endpoint.

        This method is used to send data to the device. The endpoint parameter
        corresponds to the bEndpointAddress member whose endpoint you want to
        communicate with.

        The data parameter should be a sequence like type convertible to
        the array type (see array module).

        The timeout is specified in milliseconds.

        The method returns the number of bytes written.
        """

    def read(self, endpoint: Endpoint, size_or_buffer: [bytearray, int], timeout: int = None) -> array.array:
        r"""Read data from the endpoint.

        This method is used to receive data from the device. The endpoint
        parameter corresponds to the bEndpointAddress member whose endpoint
        you want to communicate with. The size_or_buffer parameter either
        tells how many bytes you want to read or supplies the buffer to
        receive the data (it *must* be an object of the type array).

        The timeout is specified in milliseconds.

        If the size_or_buffer parameter is the number of bytes to read, the
        method returns an array object with the data read. If the
        size_or_buffer parameter is an array object, it returns the number
        of bytes actually read.
        """

    def ctrl_transfer(self,
                      bmRequestType: int,
                      bRequest: int,
                      wValue: int = 0,
                      wIndex: int = 0,
                      data_or_wLength: Union[bytes, bytearray, int] = None,
                      timeout: int = None) -> Tuple[int, bytes, bytearray, None]:
        r"""Do a control transfer on the endpoint 0.

        This method is used to issue a control transfer over the endpoint 0
        (endpoint 0 is required to always be a control endpoint).

        The parameters bmRequestType, bRequest, wValue and wIndex are the same
        of the USB Standard Control Request format.

        Control requests may or may not have a data payload to write/read.
        In cases which it has, the direction bit of the bmRequestType
        field is used to infer the desired request direction. For
        host to device requests (OUT), data_or_wLength parameter is
        the data payload to send, and it must be a sequence type convertible
        to an array object. In this case, the return value is the number
        of bytes written in the data payload. For device to host requests
        (IN), data_or_wLength is either the wLength parameter of the control
        request specifying the number of bytes to read in data payload, and
        the return value is an array object with data read, or an array
        object which the data will be read to, and the return value is the
        number of bytes read.
        """

    def is_kernel_driver_active(self, interface: int) -> bool:
        r"""Determine if there is kernel driver associated with the interface.

        If a kernel driver is active, the object will be unable to perform
        I/O.

        The interface parameter is the device interface number to check.
        """

    def detach_kernel_driver(self, interface: int) -> None:
        r"""Detach a kernel driver.

        If successful, you will then be able to perform I/O.

        The interface parameter is the device interface number to detach the
        driver from.
        """

    def attach_kernel_driver(self, interface: int) -> None:
        r"""Re-attach an interfaces kernel driver, which was previously
        detached using detach_kernel_driver().

        The interface parameter is the device interface number to attach the
        driver to.
        """

    def __iter__(self) -> Generator[Configuration, Any, None]:
        r"""Iterate over all configurations of the device."""

    def __getitem__(self, index: int) -> Configuration:
        r"""Return the Configuration object in the given position."""

    def _finalize_object(self) -> None:
        ...

    def __get_timeout(self, timeout) -> int:
        ...

    def __set_def_tmo(self, tmo) -> int:
        ...

    def __get_def_tmo(self) -> int:
        ...

    def _str(self) -> str:
        ...

    def _get_full_descriptor_str(self) -> str:
        ...


def find(find_all: bool = False,
         backend: IBackend = None,
         custom_match: Callable[[Any], bool] = None, **args) -> [Generator[Device, Any, None], Device, None]:
    r"""Find an USB device and return it.

    find() is the function used to discover USB devices.  You can pass as
    arguments any combination of the USB Device Descriptor fields to match a
    device. For example:

    find(idVendor=0x3f4, idProduct=0x2009)

    will return the Device object for the device with idVendor field equals
    to 0x3f4 and idProduct equals to 0x2009.

    If there is more than one device which matchs the criteria, the first one
    found will be returned. If a matching device cannot be found the function
    returns None. If you want to get all devices, you can set the parameter
    find_all to True, then find will return an iterator with all matched devices.
    If no matching device is found, it will return an empty iterator. Example:

    for printer in find(find_all=True, bDeviceClass=7):
        print (printer)

    This call will get all the USB printers connected to the system.  (actually
    may be not, because some devices put their class information in the
    Interface Descriptor).

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

    for printer in find(find_all=True, custom_match = is_printer):
        print (printer)

    Now even if the device class code is in the interface descriptor the
    printer will be found.

    You can combine a customized match with device descriptor fields. In this
    case, the fields must match and the custom_match must return True. In the
    our previous example, if we would like to get all printers belonging to the
    manufacturer 0x3f4, the code would be like so:

    printers = list(find(find_all=True, idVendor=0x3f4, custom_match=is_printer))

    If you want to use find as a list all devices function, just call
    it with find_all = True:

    devices = list(find(find_all=True))

    Finally, you can pass a custom backend to the find function:

    find(backend = MyBackend())

    PyUSB has builtin backends for libusb 0.1, libusb 1.0 and OpenUSB.  If you
    do not supply a backend explicitly, find() function will select one of the
    predefineds backends according to system availability.

    Backends are explained in the usb.backend module.
    """

    def device_iter(**kwargs) -> Union[Generator[Device, Any, None], Device, None]:
        ...


def show_devices(verbose: bool = False, **kwargs) -> _DescriptorInfo:
    """Show information about connected devices.

    The verbose flag sets to verbose or not.
    **kwargs are passed directly to the find() function.
    """
