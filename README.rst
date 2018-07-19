=======================================
PyUSB 1.0 - Easy USB access from Python
=======================================

Introduction
============

The PyUSB module provides for Python easy access to the host
machine's Universal Serial Bus (USB) system.

Until 0.4 version, PyUSB used to be a thin wrapper over libusb.
With 1.0 version, things changed considerably. Now PyUSB is an
API rich, backend neutral Python USB module easy to use.

As with most Python modules, PyUSB's documentation is based on Python
doc strings and can therefore be manipulated by tools such as pydoc.

You can also find a tutorial at:
https://github.com/pyusb/pyusb/blob/master/docs/tutorial.rst.

PyUSB is being developed and tested on Linux and Windows, but it should work
fine on any platform running Python >= 2.4, ctypes and at least one of the
builtin backends.

PyUSB supports libusb 0.1, libusb 1.0 and OpenUSB, but the user does not need
to worry about that, unless in some corner cases.

If you have any question about PyUSB, you can use the PyUSB mailing list
hosted in the SourceForge. In the PyUSB website (http://pyusb.github.io/pyusb/)
you can find instructions on how to subscribe to the mailing list.

Installing
==========

PyUSB is installed through `pip <https://pypi.python.org/pypi/pyusb>`:

    pip install pyusb

Remember that you need libusb (1.0 or 0.1) or OpenUSB running on your
system. For Windows users, libusb 0.1 is provided through
`libusb-win32 <http://libusb-win32.sourceforge.net>`_
package. Check the libusb website for updates
(http://www.libusb.info).

Reporting bugs/Submitting patches
=================================

Some people have been sending patches and reporting bugs directly
at my email. Please, do it through
`github <https://github.com/pyusb/pyusb>`_, I had a hard time tracking
their names to put them in the acknowledgments file.

PS: this README file was based on the great Josh Lifton's one.
