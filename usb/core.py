r"""usb.core - Core USB features.

This module exports:

Device - a class representing a USB device.
Configuration - a class representing a configuration descriptor.
Interface - a class representing an interface descriptor.
Endpoint - a class representing an endpoint descriptor.
find() - a function to find USB devices.
"""

import array
import util

_DEFAULT_TIMEOUT = 1000

def _set_attr(input, output, fields):
    for f in fields:
        setattr(output, f, int(getattr(input, f)))

# This class is responsible for managing device opens
# automatically. We don't want to bother the user with
# such low level details
class _DeviceManager(object):
    def __init__(self, dev, backend):
        self.handle = None
        self.dev = dev
        self.backend = backend
    def open(self):
        if self.handle is None:
            self.handle = self.backend.open_device(self.dev)
    def close(self):
        if self.handle is not None:
            self.backend.close_device(self.handle)
            self.handle = None
    def __del__(self):
        self.close()

# Interface claiming is something which does not exist in
# USB spec, but an implementation detail. This manages
# the interface claiming stuff
class _InterfaceClaimPolicy(object):
    def __init__(self, device_manager):
        self.interfaces_claimed = []
        self.device_manager = device_manager
    def claim(self, intf):
        self.device_manager.open()
        self.device_manager.backend.claim_interface(
            self.device_manager.handle, intf)
        if intf not in self.interfaces_claimed:
            self.interfaces_claimed.append(intf)
    def release_interfaces(self):
        self.device_manager.open()
        for intf in self.interfaces_claimed:
            self.device_manager.backend.release_interface(
                self.device_manager.handle, intf)
            # Although would be faster just associate an
            # empty list after the for loop, but removing
            # the interfaces from the list as they are
            # released is safer from exception point of view.
            self.interfaces_claimed.remove(intf)
    def __del__(self):
        self.release_interfaces()

# Map the bInterfaceNumber field with
# the active alternate setting
class _AlternateSettingMapper(object):
    def __init__(self):
        self.alt_map = {}
    def __setitem__(self, intf, alt):
        self.alt_map[intf] = alt
    def __getitem__(self, intf):
        try:
            return self.alt_map[intf]
        except KeyError:
            self.alt_map[intf] = 0
            return 0

# Map the endpoint address and the
# endpoint type
class _EndpointTypeMapper(object):
    def __init__(self, dev):
        self.dev = dev
        self.ep_map = {}
    def get(self, ep, cfg, intf, alt):
        try:
            return self.ep_map[ep]
        except KeyError:
            l = filter(lambda e: ep == e.bEndpointAddress, (e for e in Interface(self.dev, intf, alt, cfg)))
            if len(l) == 0:
                raise USBError('Invalid endpoint address %02X' % (ep))
            type = util.endpoint_type(l[0].bmAttributes)
            self.ep_map[ep] = type
            return type


class USBError(IOError):
    r"""Exception class for USB errors.
    
    Backends must raise this exception when USB related errors occur.
    """
    pass

class Endpoint(object):
    r"""Represent an endpoint descriptor."""

    def __init__(self, device, endpoint, interface = 0,
                    alternate_setting = 0, configuration = 0):
        r""" Initialize the Endpoint object.

        Parameters:
            device - the device object for which the Endpoint belongs to.
            endpoint - endpoint logical index.
            interface - the logical interface index for which the Endpoint belongs to.
            alternate_setting - the alternate setting (if any) for which the
                                Endpoint belongs to.
            configuration - the logical configuration index for which the Endpoint belongs to.
        """
        self.device = device
        intf = Interface(device, interface, alternate_setting, configuration)
        self.interface = intf.bInterfaceNumber
        self.index = endpoint

        desc = device.devmgr.backend.get_endpoint_descriptor(
                    device.devmgr.dev,
                    endpoint,
                    interface,
                    alternate_setting,
                    configuration)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'bEndpointAddress', 'bmAttributes',
            'wMaxPacketSize', 'bInterval', 'bRefresh', 'bSynchAddress'))

    def write(self, data, timeout = None):
        r"""Write data to the endpoint."""
        return self.device.write(self.bEndpointAddress, data, self.interface, timeout)

    def read(self, size, timeout = None):
        r"""Read data from the endpoint."""
        return self.device.read(self.bEndpointAddress, size, self.interface, timeout)

class Interface(object):
    r"""Represent an interface descriptor."""

    def __init__(self, device, interface = 0,
            alternate_setting = 0, configuration = 0):
        r"""Initialize the interface object.

        Parameters:
            device - device for which the interface belongs to.
            interface - the logical index of the interface.
            alternate_settting - the logical alternate setting index (if any)
                                 of the interface.
            configuration - the configuration for which the interface
                            belongs to.
        """
        self.device = device
        self.alternate_index = alternate_setting
        self.index = interface
        self.configuration = configuration

        desc = device.devmgr.backend.get_interface_descriptor(
                    self.device.devmgr.dev,
                    interface,
                    alternate_setting,
                    configuration)

        _set_attr(desc, self, 
            ('bLength', 'bDescriptorType', 'bInterfaceNumber', 'bAlternateSetting',
             'bNumEndpoints', 'bInterfaceClass', 'bInterfaceSubClass', 
             'bInterfaceProtocol', 'iInterface'))

    def set_altsetting(self):
        r"""Set the interface alternate setting."""
        self.device.set_interface_altsetting(
            self.bInterfaceNumber, self.bAlternateSetting)

    def __iter__(self):
        for i in range(self.bNumEndpoints):
            yield Endpoint(self.device, i, self.index,
                        self.alternate_index, self.configuration)

class Configuration(object):
    r"""Represent a configuration descriptor."""

    def __init__(self, device, configuration = 0):
        r"""Initialize the configuration object.
        
        Parameters:
            device - The device for which the configuration belongs to.
            configuration - the configuration logical index.
        """
        self.device = device
        self.index = configuration

        desc = device.devmgr.backend.get_configuration_descriptor(
                self.device.devmgr.dev,
                configuration)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'wTotalLength', 'bNumInterfaces',
             'bConfigurationValue', 'iConfiguration', 'bmAttributes', 'bMaxPower'))

    def set(self):
        r"""Set this configuration as the active one."""
        self.device.set_configuration(self.bConfigurationValue)

    def __iter__(self):
        r"""Iterate on all interfaces of the configuration"""
        for i in range(self.bNumInterfaces):
            alt = 0
            try:
                while True:
                    yield Interface(self.device, i, alt, self.index)
                    alt += 1
            except (USBError, IndexError):
                pass


class Device(object):
    r"""Device object.
    
    This class contains all fields of the Device Descriptor according
    to USB Specification. You may access them as class properties.
    For example, to access the field bDescriptorType of the Device
    Descriptor:

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
    >>> dev.write(1, 'teste')

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
        self.devmgr = _DeviceManager(dev, backend)
        self.intf_claimed = _InterfaceClaimPolicy(self.devmgr)
        desc = backend.get_device_descriptor(self.devmgr.dev)
        self.current_configuration = None
        self.alt_map = _AlternateSettingMapper()
        self.ep_map = _EndpointTypeMapper(self)
        self.__default_timeout = _DEFAULT_TIMEOUT

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'bcdUSB', 'bDeviceClass',
             'bDeviceSubClass', 'bDeviceProtocol', 'bMaxPacketSize0',
             'idVendor', 'idProduct', 'bcdDevice', 'iManufacturer',
             'iProduct', 'iSerialNumber', 'bNumConfigurations'))

    def set_configuration(self, configuration = None):
        r"""Set the active configuration.
        
        The configuration parameter is the bConfigurationValue field of the
        configuration you want to set as active. If you call this method
        without parameter, it will use the first configuration found.
        As a device hardly ever has more than one configuration, calling
        the method without parameter is enough to get the device ready.
        """
        self.devmgr.open()

        if configuration is None:
            configuration = Configuration(self).bConfigurationValue

        self.devmgr.backend.set_configuration(self.devmgr.handle, configuration)

        # discover the configuration index
        self.current_configuration = 0
        for c in self:
            if c.bConfigurationValue == configuration:
                break
            self.current_configuration += 1
        else:
            assert not "Configuration not found????"

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
        intf = None

        if interface is None:
            intf = Interface(self)
            interface = intf.bInterfaceNumber

        if alternate_setting is None:
            if intf is None:
                intf = Interface(self)
            alternate_setting = intf.bAlternateSetting

        self.intf_claimed.claim(interface)
        self.devmgr.backend.set_interface_altsetting(self.devmgr.handle, interface, alternate_setting)
        self.alt_map[interface] = alternate_setting

    def reset(self):
        r"""Reset the device."""
        self.devmgr.open()
        self.devmgr.backend.reset_device(self.devmgr.handle)

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
        def get_write_fn():
            fn_map = {util.ENDPOINT_TYPE_BULK:self.devmgr.backend.bulk_write,
                      util.ENDPOINT_TYPE_INTR:self.devmgr.backend.intr_write,
                      util.ENDPOINT_TYPE_ISO:self.devmgr.backend.iso_write}
            alt = self.alt_map[interface]
            return fn_map[self.ep_map.get(endpoint, self.current_configuration, interface, alt)]

        interface = self.__get_interface(interface)
        self.intf_claimed.claim(interface)

        return get_write_fn()(self.devmgr.handle,
                              endpoint,
                              interface,
                              array.array('B', data),
                              self.__get_timeout(timeout))

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
        def get_read_fn():
            fn_map = {util.ENDPOINT_TYPE_BULK:self.devmgr.backend.bulk_read,
                      util.ENDPOINT_TYPE_INTR:self.devmgr.backend.intr_read,
                      util.ENDPOINT_TYPE_ISO:self.devmgr.backend.iso_read}
            alt = self.alt_map[interface]
            return fn_map[self.ep_map.get(endpoint, self.current_configuration, interface, alt)]

        interface = self.__get_interface(interface)
        self.intf_claimed.claim(interface)

        return get_read_fn()(self.devmgr.handle,
                             endpoint,
                             interface,
                             size,
                             self.__get_timeout(timeout))


    def ctrl_transfer(self, bmRequestType, bRequest, wValue, wIndex,
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
            a = array.array('B', data_or_wLength)
        else:
            a = (data_or_wLength is None) and 0 or data_or_wLength

        self.devmgr.open()

        return self.devmgr.backend.ctrl_transfer(self.devmgr.handle,
                                                 bmRequestType,
                                                 bRequest,
                                                 wValue,
                                                 wIndex,
                                                 a,
                                                 self.__get_timeout(timeout))

    def is_kernel_driver_active(self, interface):
        r"""Determine if there is kernel driver associated with the interface."""
        self.devmgr.open()
        return self.devmgr.backend.is_kernel_driver_active(self.devmgr.handle, interface)

    def detach_kernel_driver(self, interface):
        r"""Detach a kernel driver."""
        self.devmgr.open()
        self.devmgr.backend.detach_kernel_driver(self.devmgr.handle, interface)

    def attach_kernel_driver(self, interface):
        r"""Attach a kernel driver."""
        self.devmgr.open()
        self.devmgr.backend.attach_kernel_driver(self.devmgr.handle, interface)

    def __iter__(self):
        r"""Iterate over all configurations of the device."""
        for i in range(self.bNumConfigurations):
            yield Configuration(self, i)

    def __get_interface(self, interface):
        if interface is not None:
            return interface
        return Interface(self, configuration = self.current_configuration).bInterfaceNumber

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

    default_timeout = property(__get_def_tmo, __set_def_tmo, doc = 'Default timeout for transfers')

def find(find_all=False, backend = None, predicate = None, **args):
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
    will return an iterator like object which contains the devices. If
    no matching device is found, it will return an empty iterator. Example:

    printers = [p for p in find(find_all=True, bDeviceClass=7)]

    This call will get all the USB printers connected to the system.
    (actually may be not, because some devices put their class
     information in the Interface Descriptor).

    You can also use a custom predicate as a match criteria:

    dev = find(predicate = lambda d: d.idProduct=0x3f4 and d.idvendor=0x2009)

    A more accurate printer finder using a custom predicate would be like
    so:

    def is_printer(dev):
        if dev.bDeviceClass == 7:
            return True

        for cfg in dev:
            for intf in cfg:
                if intf.bInterfaceClass == 7:
                    return True

    printers = [p for p in find(find_all=True, predicate = is_printer)]

    Now even if the device class code is in the interface descriptor the
    printer will be found.

    You can combine a custom predicate with device descriptor fields. In this
    case, the fields must match and the predicate must return True. In the our
    previous example, if we would like to get all printers belonging to the
    manufacturer 0x3f4, the code would be like so:

    printers = [p for p in find(find_all=True, idVendor=0x3f4, predicate=is_printer)]

    If you want to use find as a 'list all devices' function, just call
    it with find_all = True:

    devices = [dev for dev in find(find_all=True)]

    Finally, you may pass a custom backend to the find function:

    find(backend = MyBackend())

    Backends are explained in the usb.backend module.
    """
    import operator

    def device_iter(k, v):
        for dev in backend.enumerate_devices():
            d = Device(dev, backend)
            if (predicate is None or predicate(d)) and \
                reduce(lambda a, b: a and b, map(operator.eq, v,
                                map(lambda i: getattr(d, i), k)), True):
                yield d

    if backend is None:
        # TODO: implement automatic backend management
        import usb.backend.libusb01
        backend = usb.backend.libusb01.get_backend()

    k, v = args.keys(), args.values()
    
    if find_all:
        return (d for d in device_iter(k, v))
    else:
        try:
            return device_iter(k, v).next()
        except StopIteration:
            return None
