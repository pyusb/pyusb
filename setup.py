#!/usr/bin/env python

from distutils.core import setup

setup(
    name='PyUSB',
    version='1.0.0-a0',
    description='Python USB access module',
    author='Wander Lairson Costa',
    author_email='wander.lairson@gmail.com',
    license = 'BSD',
    url='http://pyusb.sourceforge.net',
    packages=['usb', 'usb.backend'],
    long_description =
"""
PyUSB offers easy USB devices communication on Python library.
It should work without additional code in any environment with
Python >= 2.3, ctypes and an pre-built usb backend library
(currently, libusb 0.1.x, libusb 1.x, and OpenUSB).
"""
)

