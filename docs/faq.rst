FAQ
===

How do I fix "No backend available" errors?
-------------------------------------------

Generally, there are four possible causes for this problem:

1. You didn't install libusb library.
2. Your libusb library isn't in the standard shared library paths.
3. Your libusb version is too old.
4. Your PyUSB version is too old.

To debug what's wrong, run the following script in your environment::

    import os
    os.environ['PYUSB_DEBUG'] = 'debug'
    import usb.core
    usb.core.find()

This will print debug messages to the console. If you still have problems
to figure out what's going on, please ask for help in the mailing list,
providing the debug output.

How do I enforce a backend?
---------------------------

Here is an example for the *libusb1* backend::

    >>> import usb.core
    >>> from usb.backend import libusb1
    >>> be = libusb1.get_backend()
    >>> dev = usb.core.find(backend=be)

How can I pass the libusb library path to the backend?
------------------------------------------------------

Check the *Specify libraries by hand* section in the tutorial_.

.. _tutorial: https://github.com/walac/pyusb/docs/tutorial.rst

How (not) to call set_configuration() on a device already configured with the selected configuration?
-----------------------------------------------------------------------------------------------------

Typically ``set_configuration()`` is called during device initialization. The `libusb documentation`_ on ``libusb_set_configuration()`` states:

.. _libusb documentation: http://libusb.org/static/api-1.0/group__dev.html#ga186593ecae576dad6cd9679f45a2aa43

    If you call this function on a device already configured with the selected configuration, then this function will act as a lightweight device reset: it will issue a SET_CONFIGURATION request using the current configuration, causing most USB-related device state to be reset (altsetting reset to zero, endpoint halts cleared, toggles reset).

Calling ``write()`` subsequently will therefore result in a timeout error.

One solution to this behaviour is to consider the currently active configuration, as described in the `configuration selection and handling`_. "If the configuration we want is already active, then we don't have to select any configuration"::

    cfg = dev.get_active_configuration()
    if cfg is None or cfg.bConfigurationValue != cfg_desired:
        dev.set_configuration(cfg_desired)

.. _configuration selection and handling: http://libusb.org/static/api-1.0/caveats.html

