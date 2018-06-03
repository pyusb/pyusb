#!/usr/bin/env python
# Copyright (C) 2009-2017 Wander Lairson Costa
# Copyright (C) 2017-2018 Robert Wlodarczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from setuptools import setup

import usb


setup(
    name='pyusb',
    version=usb.__version__,
    description='Python USB access module',
    author='Robert Wlodarczyk',
    author_email='robert@simplicityguy.com',
    license='Apache',
    url='http://pyusb.github.io',
    packages=['usb', 'usb.backend'],
    long_description=
"""
PyUSB offers easy USB devices communication in Python.
It should work without additional code in any environment with
Python >= 2.4, ctypes and an pre-built usb backend library
(currently, libusb 0.1.x, libusb 1.x, and OpenUSB).
""",
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'Intended Audience :: Manufacturing', # USB automation, or mfg USB devs
        'Intended Audience :: Science/Research', # interface with instruments
        'Intended Audience :: System Administrators', # integrate strange devs
        'Intended Audience :: Telecommunications Industry', # telecomm devs
        'License :: OSI Approved :: Apache Software License',
        'Natural Language :: English',
        # try to union the OSes that can build any of the backend libraries...
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows :: Windows Vista',
        'Operating System :: Microsoft :: Windows :: Windows 7',
        'Operating System :: POSIX :: BSD :: FreeBSD',
        'Operating System :: POSIX :: BSD :: NetBSD',
        'Operating System :: POSIX :: BSD :: OpenBSD',
        'Operating System :: POSIX :: Linux',
        'Operating System :: POSIX :: SunOS/Solaris',
        'Programming Language :: Python :: 2.4',
        'Programming Language :: Python :: 2.5',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        # source(CPython,Jython,IronPython,PyPy): "The Long Term" section of
        # http://ojs.pythonpapers.org/index.php/tpp/article/viewFile/23/23
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: IronPython',
        'Programming Language :: Python :: Implementation :: Jython',
        'Programming Language :: Python :: Implementation :: PyPy',
        'Topic :: Scientific/Engineering :' \
            ': Interface Engine/Protocol Translator',
        'Topic :: Software Development :: Libraries',
        'Topic :: Software Development :: Libraries :: Python Modules',
        'Topic :: System :: Hardware :: Hardware Drivers'
    ]
)

