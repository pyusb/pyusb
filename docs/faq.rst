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

