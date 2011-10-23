=======================================
PyUSB 1.0 - Easy USB access from Python
=======================================

Introduction
============

The PyUSB module provides Python with easy access to the host
machine's Universal Serial Bus (USB) system.

Until 0.4 version, PyUSB used to be a thin wrapper aroung libusb.
With 1.0 version, things changed considerably. Now PyUSB is an
API rich, backend neutral Python USB module easy to use.

As with most Python modules, PyUSB's documentation is based on Python
doc strings and can therefore be manipulated by tools such as pydoc.

You can also find a tutorial at: http://pyusb.sourceforge.net/docs/1.0/tutorial.html.

PyUSB is being developed and tested in Linux and Windows, but it should work
fine in any platform running Python >= 2.4, ctypes and at least one of the
builtin backends.

PyUSB supports libusb 0.1, libusb 1.0 and OpenUSB, but the user does not need
to worry about that, unless in some corner cases.

If you have any question about PyUSB, you can use the PyUSB mailing list
hosted in the SourceForge. In the PyUSB website (http://pyusb.sourceforge.net)
you can find instructions on how to subscribe to the mailing list.

Installing PyUSB on GNU/Linux Systems
=====================================

These instructions are for Debian-based systems.  Instructions for
other flavors of GNU/Linux should be similar.  

You will first need to install the following packages:

1) python (PyUSB is useless without it), version >= 2.4
2) At least one of the supported libraries (libusb 1.0, libusb 0.1 or OpenUSB)
3) If your Python version is < 2.5, you have to install ctypes as a separate package,
   because these versions of Python does not ship it.

For example, the command

sudo apt-get install python libusb

should install all these packages on most Debian-based systems with
access to the proper package repositories.

Once the above packages are installed, you can install PyUSB
with the command

python setup.py install

run as root from within the same directory as this README file.

Installing PyUSB on Windows
===========================

Now that PyUSB is 100% written in Python, you install it on Windows
in the same way you do on Linux:

python setup.py install

Remember that you need libusb (1.0 or 0.1) or OpenUSB running on your
system. For Windows users, libusb 1.0 is still experimental, so it is
recommended libusb-win32 package. Check the libusb website for updates
(http://www.libusb.org).

Reporting bugs/Submitting patches
=================================

Some people have been sending me patches and reporting bugs directly
in my email. Please, do it through SourceForge tracker, I had
a hardtime tracking their names to put them in the acknowledgments file. ;-)

PS: this README file was based on the great Josh Lifton's one... ^_^
