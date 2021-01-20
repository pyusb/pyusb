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
import os
import os.path
import operator
import logging
from ctypes import c_ubyte, POINTER, cast

parent_dir = os.path.split(os.getcwd())[0]

# if we are at PyUSB source tree, add usb package to python path
if os.path.exists(os.path.join(parent_dir, 'usb')):
    sys.path.insert(0, parent_dir)

import usb.core
import logging
import devinfo
import time
import unittest
import usb._interop as _interop

logger = logging.getLogger('usb.test')

# data generation functions
def get_array_data1(length = 8):
    return _interop.as_array(range(length))

def get_array_data2(length = 8):
    data = list(range(length))
    data.reverse()
    return _interop.as_array(data)

def get_list_data1(length = 8):
    return list(range(length))

def get_list_data2(length = 8):
    data = list(range(length))
    data.reverse()
    return data

def get_str_data1(length = 8):
    if sys.version_info[0] >= 3:
        # On Python 3, string char is 4 bytes long
        length = int(length / 4)
    return ''.join([chr(x) for x in range(length)])

def get_str_data2(length = 8):
    if sys.version_info[0] >= 3:
        length = int(length / 4)
    data = list(range(length))
    data.reverse()
    return ''.join([chr(x) for x in data])

def to_array(data):
    return _interop.as_array(data)

def delay_after_reset():
    time.sleep(3) # necessary to wait device reenumeration

# check if our test hardware is present
def find_my_device(backend = None):
    try:
        return usb.core.find(backend=backend,
                             idVendor=devinfo.ID_VENDOR,
                             idProduct=devinfo.ID_PRODUCT)
    except Exception:
        return None

def run_tests(suite):
    runner = unittest.TextTestRunner()
    return runner.run(suite)

def data_len(data):
    a = _interop.as_array(data)
    return len(data) * a.itemsize

def array_equals(a1, a2):
    if a1.typecode != 'u' and a2.typecode != 'u':
        return a1 == a2
    else:
        # as python3 strings are unicode, loads of trouble,
        # because we read data from USB devices are byte arrays
        l1 = len(a1) * a1.itemsize
        l2 = len(a2) * a2.itemsize
        if l1 != l2:
            return False
        c_ubyte_p = POINTER(c_ubyte)
        p1 = cast(a1.buffer_info()[0], c_ubyte_p)
        p2 = cast(a2.buffer_info()[0], c_ubyte_p)
        # we do a item by item compare we unicode is involved
        return all(map(operator.eq, p1[:l1], p2[:l2]))

def is_windows():
    return 'getwindowsversion' in dir(sys)

def is_iso_test_allowed():
    return int(os.getenv('PYUSB_TEST_ISO_TRANSFER', 1))
