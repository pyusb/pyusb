__all__ = []

def _not_implemented(func):
    raise NotImplementedError(func.__name__)

class IBackend(object):
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

    def get_interface_altsetting(self, dev_handle, intf):
        r"""Return the current interface alternate setting.

        Parameters:
            dev_handle - device handle
            intf - bInterfaceNumber field value.
        """
        _not_implemented(self.get_interface_altsetting)

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

    def bulk_transfer(self, dev_handle, ep, intf, data_or_length, timeout):
        r"""Do a bulk transfer.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data_or_length - For out transfers, data to be transfered. It is expected
                             to an instance of the array.array class. For in transfers,
                             number of bytes to read.
            timeout - timeout in miliseconds.
        """
        _not_implemented(self.bulk_transfer)

    def interrupt_transfer(self, dev_handle, ep, data_or_length, intf, timeout):
        r"""Do an interrupt transfer.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data_or_length - For out transfers, data to be transfered. It is expected
                             to an instance of the array.array class. For in transfers,
                             number of bytes to read.
            timeout - timeout in miliseconds.
        """
        _not_implemented(self.interrupt_transfer)

    def isochronous_transfer(self, dev_handle, ep, data_or_length, intf, timeout):
        r"""Do an isochronous transfer.

        Parameters:
            dev_handle - device handle.
            ep - endpoint address.
            intf - bInterfaceNumber field value.
            data_or_length - For out transfers, data to be transfered. It is expected
                             to an instance of the array.array class. For in transfers,
                             number of bytes to read.
            timeout - timeout in miliseconds.
        """
        _not_implemented(self.isochronous_transfer)

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
