#!/usr/bin/env python
# Copyright 2009-2017 Wander Lairson Costa
# Copyright 2009-2021 PyUSB contributors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import sys
from setuptools import setup
from distutils.version import LooseVersion
from setuptools import __version__ as setuptools_version

setuptools_scm = 'setuptools_scm'
if LooseVersion(setuptools_version).version[0] < 12:
    setuptools_scm += '<2.0'


def pyusb_scm_version():
    """append '-editable' to version for editable installs"""
    from setuptools_scm.version import get_local_node_and_date

    def editable_local_scheme(version):
        local_scheme = get_local_node_and_date(version)
        # distutils scans sys.argv for matching command classes
        # (see `distutils.dist.Distribution._parse_command_opts`)
        if "develop" in sys.argv[1:]:
            return local_scheme + "-editable"
        return local_scheme

    return {
        "version_scheme": "post-release",
        "local_scheme": editable_local_scheme,
        "write_to": "usb/_version.py",
    }


# workaround: sdist installs with callables were broken between "setuptools_scm
# >=1.8, <=1.10.1" and Ubuntu 16.04 ships with 1.10.1; since we're not using
# "root" we can just noop (see pypa/setuptools_scm@ff948dcd99)
pyusb_scm_version.pop = lambda *_: None


setup(
    name='pyusb',
    use_scm_version=pyusb_scm_version,
    setup_requires=setuptools_scm,
    description='Python USB access module',
    author='Jonas Malaco',
    author_email='me@jonasmalaco.com',
    url='https://pyusb.github.io/pyusb',
    packages=['usb', 'usb.backend'],
    long_description=
"""
PyUSB offers easy USB devices communication in Python.
It should work without additional code in any environment with
Python >= 3.6, ctypes and a pre-built USB backend library
(currently: libusb 1.x, libusb 0.1.x or OpenUSB).
""",
    classifiers=[
        'Development Status :: 5 - Production/Stable',
        'Intended Audience :: Developers',
        'Intended Audience :: Information Technology',
        'Intended Audience :: Manufacturing', # USB automation, or mfg USB devs
        'Intended Audience :: Science/Research', # interface with instruments
        'Intended Audience :: System Administrators', # integrate strange devs
        'Intended Audience :: Telecommunications Industry', # telecomm devs
        'License :: OSI Approved :: BSD License',
        'Natural Language :: English',
        # try to union the OSes that can build any of the backend libraries...
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows :: Windows Vista',
        'Operating System :: Microsoft :: Windows :: Windows 7',
        'Operating System :: Microsoft :: Windows :: Windows 10',
        'Operating System :: POSIX :: BSD :: FreeBSD',
        'Operating System :: POSIX :: BSD :: NetBSD',
        'Operating System :: POSIX :: BSD :: OpenBSD',
        'Operating System :: POSIX :: Linux',
        'Operating System :: POSIX :: SunOS/Solaris',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
        'Programming Language :: Python :: 3.8',
        'Programming Language :: Python :: 3.9',
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
    ],
    python_requires='>=3.6.0'
)

