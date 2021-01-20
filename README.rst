=======================================
PyUSB 1.0 - Easy USB access from Python
=======================================

Introduction
============

The PyUSB module provides for Python easy access to the host
machine's Universal Serial Bus (USB) system.

Until 0.4 version, PyUSB used to be a thin wrapper over libusb.
With the 1.0 version, things changed considerably: now PyUSB is an
API rich, backend neutral Python USB module easy to use.

As with most Python modules, PyUSB's documentation is based on Python
doc strings and can therefore be manipulated by tools such as pydoc.

You can also find a tutorial at `docs/tutorial.rst
<https://github.com/pyusb/pyusb/blob/master/docs/tutorial.rst>`_.

PyUSB is being developed and tested on Linux and Windows, but it should work
fine on any platform running Python >= 3.6, ctypes and at least one of the
builtin backends.

PyUSB supports libusb 1.0, libusb 0.1 and OpenUSB, but the user does not need
to worry about that, unless in some corner cases.

If you have any question about PyUSB, consult the FAQ at `docs/faq.rst
<https://github.com/pyusb/pyusb/blob/master/docs/faq.rst>`_ or  the PyUSB mailing list
hosted in the SourceForge. In the `PyUSB website <https://pyusb.github.io/pyusb/>`_
you can find instructions on how to subscribe to the mailing list.

Installing
==========

PyUSB is installed through `pip <https://pypi.python.org/pypi/pyusb>`_:

    pip install pyusb

Remember that you need libusb (1.0 or 0.1) or OpenUSB running on your system.
For Windows users, libusb 1.0 DLLs are provided in the `releases
<https://github.com/libusb/libusb/releases>`_ (see 7z archives).  Check
the libusb website for updates (http://www.libusb.info). For MacOS users, 
``brew install libusb`` satisfies the requirement for running correctly.
