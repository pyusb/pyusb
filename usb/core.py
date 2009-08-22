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
    r"""Exception class for USB errors."""
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

        desc = device.devmgr.backend.get_endpoint_descriptor(
                    device.devmgr.dev,
                    endpoint,
                    interface,
                    alternate_setting,
                    configuration)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'bEndpointAddress', 'bmAttributes',
            'wMaxPacketSize', 'bInterval', 'bRefresh', 'bSynchAddress'))

    def write(self, data, timeout = _DEFAULT_TIMEOUT):
        r"""Write data to the endpoint."""
        return self.device.write(self.bEndpointAddress, data, self.interface, timeout)

    def read(self, size, timeout = _DEFAULT_TIMEOUT):
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
    r"""Represent a device descriptor."""

    def __init__(self, dev, backend):
        r"""Initialize the Device object.

        Parameters:
            dev - the device representation returned by backend.enumerate_devices()
            backend - the backend object.
        """
        self.devmgr = _DeviceManager(dev, backend)
        self.intf_claimed = _InterfaceClaimPolicy(self.devmgr)
        desc = backend.get_device_descriptor(self.devmgr.dev)
        self.current_configuration = -1
        self.alt_map = _AlternateSettingMapper()
        self.ep_map = _EndpointTypeMapper(self)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'bcdUSB', 'bDeviceClass',
             'bDeviceSubClass', 'bDeviceProtocol', 'bMaxPacketSize0',
             'idVendor', 'idProduct', 'bcdDevice', 'iManufacturer',
             'iProduct', 'iSerialNumber', 'bNumConfigurations'))

    def set_configuration(self, configuration = 1):
        r"""Set the current active configuration.
        
        Parameters:
            configuration - the bConfigurationValue field of the desired configuration.
        """
        self.devmgr.open()
        self.devmgr.backend.set_configuration(self.devmgr.handle, configuration)
        self.current_configuration = 0
        # discover the configuration index
        for c in self:
            if c.bConfigurationValue == configuration:
                break
            self.current_configuration += 1
        else:
            assert not "Configuration not found????"

    def set_interface_altsetting(self, interface = 0, alternate_setting = 0):
        r"""Set the alternate setting for an interface.
        
        Parameters:
            interface - the bInterfaceNumber field of the interface.
            alternate_setting - the bAlternateSetting field of the interface.
        """
        self.intf_claimed.claim(interface)
        self.devmgr.backend.set_interface_altsetting(self.devmgr.handle, interface, alternate_setting)
        self.alt_map[interface] = alternate_setting

    def reset(self):
        r"""Reset the device."""
        self.devmgr.open()
        self.devmgr.backend.reset_device(self.devmgr.handle)

    def write(self, endpoint, data, interface = 0, timeout = _DEFAULT_TIMEOUT):
        r"""Write data to the endpoint.

        Parameters:
            endpoint - endpoint address.
            data - data to transfer.
            interface - bInterfaceNumber.
            timeout - operation timeout.

        Return the number of data written.
        """
        def get_write_fn():
            fn_map = {util.ENDPOINT_TYPE_BULK:self.devmgr.backend.bulk_write,
                      util.ENDPOINT_TYPE_INTR:self.devmgr.backend.intr_write,
                      util.ENDPOINT_TYPE_ISO:self.devmgr.backend.iso_write}
            alt = self.alt_map[interface]
            return fn_map[self.ep_map.get(endpoint, self.current_configuration, interface, alt)]
        self.devmgr.backend.claim_interface(self.devmgr.handle, interface)
        return get_write_fn()(self.devmgr.handle, endpoint, interface, array.array('B', data), timeout)

    def read(self, endpoint, size, interface = 0, timeout = _DEFAULT_TIMEOUT):
        r"""Read data from the endpoint.

        Parameters:
            endpoint - endpoint address.
            size - number of data to read.
            interface - bInterfaceNumber.
            timeout - operation timeout.

        Return the data read as a array object.
        """
        def get_read_fn():
            fn_map = {util.ENDPOINT_TYPE_BULK:self.devmgr.backend.bulk_read,
                      util.ENDPOINT_TYPE_INTR:self.devmgr.backend.intr_read,
                      util.ENDPOINT_TYPE_ISO:self.devmgr.backend.iso_read}
            alt = self.alt_map[interface]
            return fn_map[self.ep_map.get(endpoint, self.current_configuration, interface, alt)]
        self.devmgr.backend.claim_interface(self.devmgr.handle, interface)
        return get_read_fn()(self.devmgr.handle, endpoint, interface, size, timeout)


    def ctrl_transfer(self, bmRequestType, bRequest, wValue, wIndex,
            data_or_wLength = None, timeout = _DEFAULT_TIMEOUT):
        r"""Do a control transfer on endpoint 0.

        Parameters:
            bmRequestType - the request type field for the setup packet.
            bRequest - the request field for the setup packet.
            wValue - the value field for the setup packet.
            wIndex - the index field for the setup packet.
            data_or_wLength - for an in transfer, it constains the number
                              of bytes to read. For out transfers, the
                              data to be written.
            timeout - timeout of the operation.

        Return the number of bytes written (for out transfers) or the data
        read (for in transfers).
        """
        if util.ctrl_direction(bmRequestType) == util.CTRL_IN:
            a = array.array('B', data_or_wLength)
        else:
            a = (data_or_wLength is None) and 0 or data_or_wLength

        self.devmgr.open()

        return self.devmgr.backend.ctrl_transfer(
                    self.devmgr.handle,
                    bmRequestType,
                    bRequest,
                    wValue,
                    wIndex,
                    a,
                    timeout)

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
        r"""Iterate on the all configurations of the device"""
        for i in range(self.bNumConfigurations):
            yield Configuration(self, i)

def find(find_all=False, backend = None, predicate = None, **args):
    r"""Find an USB device and return it.

    find() is the function used to discover USB devices.
    As arguments, you can pass any combination of the
    USB Device descriptor fields to match a device. For example:

    find(idVendor=0x3f4, idProduct=0x2009)

    Will return the Device object for the device with
    idVendor Device descriptor field equals to 0x3f4 and
    idProduct equals to 0x2009.

    If there is more than one device which match the criteria,
    the first one found will be returned. If a matching device cannot
    be found the function returns None. If you want to get all
    devices, you can set the parameter find_all to True, then find
    will return an iterator like object which contains the devices. If
    no matching device is found, it will return an empty iterator. Example:

    printers = [p for p in find(find_all=True, bDeviceClass=7)]

    This call will get all the USB printers connected to the system.
    (actually may be not, because some devices put their class
     information in the Interface).

    You can also use a custom predicate as a match criteria:

    dev = find(predicate = lambda d: d.idProduct=0x3f4 and d.idvendor=2009)

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

    Finally, you can pass a custom backend to find:

    find(backend = MyBackend())

    Backends are explained in the usb.backend module.
    """
    import operator

    def device_iter(k, v):
        for dev in backend.enumerate_devices():
            d = Device(dev, backend)
            if (predicate is None or predicate(dev)) and \
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
