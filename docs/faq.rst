FAQ
===

How do I fix "No backend available" errors?
-------------------------------------------

Generally, there are four possible causes for this problem:

1. You didn't install libusb library.
2. Your libusb library isn't in the standard shared library paths.
3. Your libusb version is too old.
4. Your PyUSB version is too old.
5. You're using an Alpine-based container (see separate FAQ entry bellow).

To debug what's wrong, run the following script in your environment::

    import os
    os.environ['PYUSB_DEBUG'] = 'debug'
    import usb.core
    usb.core.find()

This will print debug messages to the console. If you still have problems
to figure out what's going on, please ask for help in the mailing list,
providing the debug output.


How do I run PyUSB in an Alpine-based container?
------------------------------------------------

In musl-based containers and as of Python 3.13, ``ctypes.find_library`` depends
on the GCC linker being available, as well as the development files for the
desired library.

This means that, in practice and particularly in Alpine-based containers, the
following is suggested as a basis for your ``Dockerfile`` (or equivalent)::

    FROM python:alpine

    ENV LD_LIBRARY_PATH=/lib:/usr/lib

    WORKDIR /work
    RUN apk update && \
        apk add --no-cache gcc libusb-dev
    RUN python -m pip install pyusb

    CMD python -c "import usb; print(usb.core.find())"

Note that in addition to GCC and ``libusb-dev``, ``LD_LIBRARY_PATH`` is also set
with the location of the LibUSB files.


How do I install libusb on Windows?
-----------------------------------

On Windows, `pyocd/libusb-package`_ is a convenient [1]_ [2]_ way to provide the
necessary libusb 1.0 DLL, as well as a suitable PyUSB backend and a easy to use
wrapper over PyUSB's ``find()`` API::

    # with pure PyUSB
    for dev in usb.core.find(find_all=True):
        print(dev)

    # with pyocd/libusb-package
    for dev in libusb_package.find(find_all=True):
        print(dev)


Alternatively, the libusb 1.0 DLL can be manually copied from an official
release archive into the ``C:\Windows\System32`` system folder, or packaged
together with the complete application.

Do I need special kernel drivers?
---------------------------------

Occasionally, on Windows: check out the documentation of the backend in use
(either libusb 1.0 or libusb 0.1/libusb-win32).

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

How to practically deal with permission issues on Linux?
--------------------------------------------------------

Linux and BSD are typically set up in a way in which root permissions are
needed for low level/generic access to USB devices.

Using sudo or similar tools is one way to temporarily give a program access to
USB devices, but there are several reasons why that may not be ideal in all
situations.

A better way on Linux is to use udev_ rules to allow unprivileged access to
specific USB devices by certain users::

    # these examples assume a hypothetical device with 1111:aaaa vendor:product IDs

    # allow r/w access by all local/physical sessions (seats)
    # https://github.com/systemd/systemd/issues/4288
    SUBSYSTEMS=="usb", ATTRS{idVendor}=="1111", ATTRS{idProduct}=="aaaa", TAG+="uaccess"

    # allow r/w access by users of the plugdev group
    SUBSYSTEMS=="usb", ATTRS{idVendor}=="1111", ATTRS{idProduct}=="aaaa", GROUP="plugdev", MODE="0660"

    # allow r/w access by all users
    SUBSYSTEMS=="usb", ATTRS{idVendor}=="1111", ATTRS{idProduct}=="aaaa", MODE="0660"

How (not) to call set_configuration() on a device already configured with the selected configuration?
-----------------------------------------------------------------------------------------------------

Typically ``set_configuration()`` is called during device initialization. The
`libusb documentation`_ on ``libusb_set_configuration()`` states:

    If you call this function on a device already configured with the selected
    configuration, then this function will act as a lightweight device reset:
    it will issue a SET_CONFIGURATION request using the current configuration,
    causing most USB-related device state to be reset (altsetting reset to
    zero, endpoint halts cleared, toggles reset).

Calling ``write()`` subsequently will therefore result in a timeout error.

One solution to this behaviour is to consider the currently active
configuration, as described in the `configuration selection and handling`_. "If
the configuration we want is already active, then we don't have to select any
configuration"::

    try:
        cfg = dev.get_active_configuration()
    except usb.core.USBError:
        cfg = None
    if cfg is None or cfg.bConfigurationValue != cfg_desired:
        dev.set_configuration(cfg_desired)

Footnotes
---------

.. [1] Unline PyUSB, pyocd/libusb-package uses the more restrictive Apache 2.0
   license.

.. [2] While pyocd/libusb-package supports platforms other than Windows,
   there are advantages to sticking to a system-provided libusb, if it is
   available and the platform has a robust package manager (e.g. Linux, BSD,
   macOS with Homebrew).

.. _configuration selection and handling: http://libusb.sourceforge.net/api-1.0/libusb_caveats.html#configsel
.. _libusb documentation: http://libusb.sourceforge.net/api-1.0/group__libusb__dev.html#ga785ddea63a2b9bcb879a614ca4867bed
.. _pyocd/libusb-package: https://github.com/pyocd/libusb-package
.. _tutorial: https://github.com/pyusb/pyusb/blob/master/docs/tutorial.rst
.. _udev: https://www.man7.org/linux/man-pages/man7/udev.7.html
