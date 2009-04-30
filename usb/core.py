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
            # released is safer from exception pointer of view.
            self.interfaces_claimed.remove(intf)
    def __del__(self):
        self.release_interfaces()

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
        self.interface = interface

        desc = device._Device__devmgr.backend.get_endpoint_descriptor(
                    device._Device__devmgr.dev,
                    endpoint,
                    interface,
                    alternate_setting,
                    configuration)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'bEndpointAddress', 'bmAttributes',
            'wMaxPacketSize', 'bInterval', 'bRefresh', 'bSynchAddress'))

    def transfer(self, buffer_or_length, timeout = _DEFAULT_TIMEOUT):
        r"""Do a data transfer through the Endpoint.

        The transfer type and direction are automatically
        inferred from the endpoint type.

        Parameters:
            buffer_or_length - on out transfers, it is the buffer containing
                            the data to be trasferered. For in transfers,
                            is the number of bytes to read.
            timeout - timeout of the operation.

        Return:
            On write calls, the number of bytes transfered. On read calls,
            the data read.
        """
        transfer_map = {util.ENDPOINT_TYPE_BULK:self.self.device.bulk_transfer,
                        uitl.ENDPOINT_TYPE_INTERRUPT:self.device.interrupt_transfer,
                        uitl.ENDPOINT_TYPE_ISOCHRONOUS:self.device.isochronous_transfer}

        return transfer_map[util.endpoint_transfer_type(self.bmAttributes)](
                self.bEndpointAddress,
                buffer_or_length,
                self.interface)

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

        desc = device._Device__devmgr.backend.get_interface_descriptor(
                    self.device._Device__devmgr.dev,
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

        desc = device._Device__devmgr.backend.get_configuration_descriptor(
                self.device._Device__devmgr.dev,
                configuration)

        _set_attr(desc, self,
            ('bLength', 'bDescriptorType', 'wTotalLength', 'bNumInterfaces',
             'bConfigurationValue', 'iConfiguration', 'bmAttributes', 'MaxPower'))

    def set(self):
        r"""Set this configuration as the active one."""
        self.device.set_configuration(self.bConfigurationValue)

    def __iter__(self):
        r"""Iterate on the all interfaces of the configuration"""
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
        self.__devmgr = _DeviceManager(dev, backend)
        self.__intf_claim = _InterfaceClaimPolicy(self.__devmgr)
        desc = backend.get_device_descriptor(self.__devmgr.dev)

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
        self.__devmgr.open()
        self.__devmgr.backend.set_configuration(self.__devmgr.handle, configuration_value)

    def set_interface_altsetting(self, interface = 0, alternate_setting = 0):
        r"""Set the alternate setting for an interface.
        
        Parameters:
            interface - the bInterfaceNumber field of the interface.
            alternate_setting - the bAlternateSetting field of the interface.
        """
        self.__intf_claim.claim(interface)
        self.__devmgr.backend.set_interface_altsetting(interface, alternate_setting)

    def reset(self):
        r"""Reset the device."""
        self.__devmgr.open()
        self.__devmgr.backend.reset_device(self.__devmgr.handle)

    def bulk_transfer(self, endpoint, data_or_length, interface = 0, timeout = _DEFAULT_TIMEOUT):
        r"""Do an USB bulk transfer.

        The bulk_transfer() function writes or reads data to/from the USB device. The
        direction of the transfer is inferred from the endpoint address.

        Parameters:
            endpoint - the endpoint address.
            data_or_length - For out transfers, the data buffer to be transmitted.
                             For in transfers, the number of bytes to read.
            interface - the bInterfaceNumber field of the interface which the endpoint belongs to.
            timeout - the timeout of the operation.

        Return the number of bytes written (for out transfers) or the data
        read (for in transfers).
        """
        if util.endpoint_address(endpoint) == util.ENDPOINT_OUT:
            a = array.array('B', data_or_length)
        else:
            a = data_or_length

        self.__devmgr.open()

        return self.__devmgr.backend.bulk_transfer(
                    self.__devmgr.handle,
                    endpoint,
                    a,
                    interface,
                    timeout)

    def interrupt_transfer(self, endpoint, data_or_length, interface = 0, timeout = _DEFAULT_TIMEOUT):
        r"""Do an USB interrupt transfer.

        The interrupt_transfer() function writes or reads data to/from the USB device. The
        direction of the transfer is inferred from the endpoint address.

        Parameters:
            endpoint - the endpoint address.
            data_or_length - For out transfers, the data buffer to be transmitted.
                             For in transfers, the number of bytes to read.
            interface - the bInterfaceNumber field of the interface which the endpoint belongs to.
            timeout - the timeout of the operation.

        Return the number of bytes written (for out transfers) or the data
        read (for in transfers).
        """
        if util.endpoint_address(endpoint) == util.ENDPOINT_OUT:
            a = array.array('B', data_or_length)
        else:
            a = data_or_length

        self.__devmgr.open()

        return self.__devmgr.backend.bulk_transfer(
                    self.__devmgr.handle,
                    endpoint,
                    a,
                    interface,
                    timeout)

    def interrupt_transfer(self, endpoint, data_or_length, interface = 0, timeout = _DEFAULT_TIMEOUT):
        r"""Do an USB isochronous transfer.

        The isochronous_transfer() function writes or reads data to/from the USB device. The
        direction of the transfer is inferred from the endpoint address.

        Parameters:
            endpoint - the endpoint address.
            data_or_length - For out transfers, the data buffer to be transmitted.
                             For in transfers, the number of bytes to read.
            interface - the bInterfaceNumber field of the interface which the endpoint belongs to.
            timeout - the timeout of the operation.

        Return the number of bytes written (for out transfers) or the data
        read (for in transfers).
        """
        if util.endpoint_address(endpoint) == util.ENDPOINT_OUT:
            a = array.array('B', data_or_length)
        else:
            a = data_or_length

        self.__devmgr.open()

        return self.__devmgr.backend.bulk_transfer(
                    self.__devmgr.handle,
                    endpoint,
                    a,
                    interface,
                    timeout)

    def ctrl_transfer(bmRequestType, bRequest, wValue, wIndex,
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
        if util.endpoint_address(endpoint) == util.ENDPOINT_OUT:
            a = array.array('B', data_or_wLength)
        else:
            a = data_or_wLength

        self.__devmgr.open()

        return self.__devmgr.backend.ctrl_transfer(
                    self.__devmgr.handle,
                    bmRequest,
                    bRequest,
                    wValue,
                    wIndex,
                    a,
                    timeout)

    def is_kernel_driver_active(self, interface):
        r"""Determine if there is kernel driver associated with the interface."""
        return self.__devmgr.backend.is_kernel_driver_active(self.__devmgr.handle, interface)
    
    def detach_kernel_driver(self, interface):
        r"""Detach a kernel driver."""
        self.__devmgr.backend.detach_kernel_driver(self.__devmgr.handle, interface)

    def attach_kernel_driver(self, interface):
        r"""Attach a kernel driver."""
        self.__devmgr.backend.attach_kernel_driver(self.__devmgr.handle, interface)

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
    the first one found will be returned. If you want to get all
    devices, you can set the parameter find_all to True, then find
    will return an iterator like object which contains the devices. Example:

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

    def device_iter(pred, k, v):
        for dev in backend.enumerate_devices():
            if (predicate is None or predicate(dev)) and \
                reduce(lambda a, b: a and b, map(operator.eq, v,
                                map(lambda i: getattr(dev, i), k)), True):
                yield dev

    if backend is None:
        # TODO: implement automatic backend management
        import usb.backend.libusb01
        backend = usb.backend.libusb01.LibUSB()

    k, v = args.keys(), args.values()
    
    if find_all:
        return (Device(dev, backend) for dev in device_iter(predicate, k, v))
    else:
        return Device(device_iter(predicate, k, v).next(), backend)
