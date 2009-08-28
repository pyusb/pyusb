r"""usb.backend - Backend interface.

This module exports:

IBackend - backend interface.

Backends are implemented by modules which provide a get_backend()
function which returns an IBackend like object, i.e, the object
returned must obay the IBackend interface. The easiest way to do so
is inherinting from IBackend.

PyUSB provides by default backends for libusb versions 0.1 and 1.0,
and OpenUSB library. You can provide your own customized backend if you
want to. This is a skeleton of a backend implementation module:

import usb.backend

class MyBackend(usb.backend.IBackend):
    pass

def get_backend():
    return MyBackend()

You can use your customized backend using the backend parameter of the
usb.core.find() function. For example:

import custom_backend
import usb.core

myidVendor = 0xfffe
myidProduct = 0x0001

mybackend = custom_backend.get_backend()

dev = usb.core.find(backend = mybackend, idProduct=myidProduct, idVendor=myidVendor)

If you do not provide a backend to find(), it will use one of the defaults backend according
to its internal rules. For details, consult the find() documentation.
"""

__all__ = []

def _not_implemented(func):
    raise NotImplementedError(func.__name__)

class IBackend(object):
    r"""Backend interface.

    IBackend is the basic interface for backend implementations. By default,
    the methods of the interface raise a NotImplementedError exception. A
    backend implementation should replace the methods to provide the funcionality
    necessary.

    As Python is a dynamic typed language, you are not obligated to derive from
    IBackend, everything that bahaves like a IBackend is a IBackend. But you
    are strongly recommended to do so, inheriting from IBackend provides consistent
    default behavior.
    """
    def enumerate_devices(self):
        r"""Return an iterable object which yields a device identifier
            for each USB device present in the system.
        """
        _not_implemented(self.enumerate_devices)

    def get_device_descriptor(self, dev):
        r"""Return the device descriptor of the given device id.
        
        Parameters:
            dev - device id.
        """
        _not_implemented(self.get_device_descriptor)

    def get_configuration_descriptor(self, dev, config):
        r"""Return the descriptor of the given configuration.

        Parameters:
            dev - device id.
            config - configuration logical index.
        """
        _not_implemented(self.get_configuration_descriptor)

    def get_interface_descriptor(self, dev, intf, alt, config):
        r"""Return the descriptor of the given interface.

        Parameters:
            dev - device id.
            intf - interface logical index.
            alt - alternate setting logical index.
            config - configuration index.
        """
        _not_implemented(self.get_interface_descriptor)

    def get_endpoint_descriptor(self, dev, ep, intf, alt, config):
        r"""Return the descriptor of the given endpoint.

        Parameters:
            dev - device id.
            ep - endpoint logical index.
            intf - interface logical index.
            alt - alternate setting logical index.
            config - configuration logical index.
        """
        _not_implemented(self.get_endpoint_descriptor)

    def open_device(self, dev):
        r"""Return a handle used to communicate with the device.

        Parameters:
            dev - device id.
        """
        _not_implemented(self.open_device)

    def close_device(self, dev_handle):
        r"""Close the device handle.

        Parameters:
            dev_handle - device handle.
        """
        _not_implemented(self.close_device)

    def set_configuration(self, dev_handle, config_value):
        r"""Set the current configuration.

        Parameters:
            dev_handle - device handle.
            config_value - bConfigurationValue field value.
        """
        _not_implemented(self.set_configuration)

    def set_interface_altsetting(self, dev_handle, intf, altsetting):
        r"""Set the interface alternate setting.

        Parameters:
            dev_handle - device handle.
            intf - bInterfaceNumber field value.
            altsetting - bAlternateSetting field value.
        """
        _not_implemented(self.set_interface_altsetting)

    def claim_interface(self, dev_handle, intf):
        r"""Claim the interface.

        Parameters:
            dev_handle - device handle.
            intf - bInterfaceNumber field value.
        """
        _not_implemented(self.claim_interface)

    def release_interface(self, dev_handle, intf):
        r"""Release the claimed interface.

        Parameters:
            dev_handle - device handle.
            intf - bInterfaceNumber field value.
        """
        _not_implemented(self.release_interface)

    def bulk_write(self, dev_handle, ep, intf, data, timeout):
        r"""Do a bulk write.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data - Data to be written. Must be a instance of array object.
            timeout - timeout in miliseconds.

        Return the number of bytes written.
        """
        _not_implemented(self.bulk_write)

    def bulk_read(self, dev_handle, ep, intf, size, timeout):
        r"""Do a bulk read.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            size - Number of data to read.
            timeout - timeout in miliseconds.

        Return a array object with the data read.
        """
        _not_implemented(self.bulk_read)

    def intr_write(self, dev_handle, ep, intf, data, timeout):
        r"""Do a interrupt write.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data - Data to be written. Must be a instance of array object.
            timeout - timeout in miliseconds.

        Return the number of bytes written.
        """
        _not_implemented(self.intr_write)

    def intr_read(self, dev_handle, ep, intf, size, timeout):
        r"""Do a interrut read.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            size - Number of data to read.
            timeout - timeout in miliseconds.

        Return a array object with the data read.
        """
        _not_implemented(self.intr_read)

    def iso_write(self, dev_handle, ep, intf, data, timeout):
        r"""Do a isochronous write.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data - Data to be written. Must be a instance of array object.
            timeout - timeout in miliseconds.

        Return the number of bytes written.
        """
        _not_implemented(self.iso_write)

    def iso_read(self, dev_handle, ep, intf, size, timeout):
        r"""Do a isochronous read.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            size - Number of data to read.
            timeout - timeout in miliseconds.

        Return a array object with the data read.
        """
        _not_implemented(self.iso_read)


    def ctrl_transfer(self, dev_handle, bmRequestType, bRequest, wValue, wIndex, data_or_wLength, timeout):
        r"""Do a control transfer on endpoint 0.

        Parameters:
            dev_handle - device handle.
            bmRequestType - the request type field for the setup packet.
            bRequest - the request field for the setup packet.
            wValue - the value field for the setup packet.
            wIndex - the index field for the setup packet.
            data_or_wLength - for an in transfer, it constains the number
                              of bytes to read. For out transfers, the
                              data to be written. The data is excepted to
                              be an instance of the array.array class.
            timeout - timeout in miliseconds.

        Return the number of bytes written (for out transfers) or the data
        read (for in transfers).
        """
        _not_implemented(self.ctrl_transfer)

    def reset_device(self, dev_handle):
        r"""Reset the device.

        Parameters:
            dev_handle - device handle.
        """
        _not_implemented(self.reset_device)

    def is_kernel_driver_active(self, dev_handle, intf):
        r"""Detect if a kernel mode driver is attached to the device.

        Parameters:
            dev_handle - device handle.
            intf - interface index.

        Note: optional.
        """
        _not_implemented(self.is_kernel_driver_active)

    def detach_kernel_driver(self, dev_handle, intf):
        r"""Detach the kernel driver from the device.

        Parameters:
            dev_handle - device handle.
            intf - interface index.

        Note: optional.
        """
        _not_implemented(self.detach_kernel_driver)

    def attach_kernel_driver(self, dev_handle, intf):
        r"""Attach the kernel driver to the device.

        Parameters:
            dev_handle - device handle.
            intf - interface index.

        Note: optional.
        """
        _not_implemented(self.attach_kernel_driver)
