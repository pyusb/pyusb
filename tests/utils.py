# Copyright (C) 2009-2014 Wander Lairson Costa
#
# The following terms apply to all files associated
# with the software unless explicitly disclaimed in individual files.
#
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.
#
# IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
# DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
# IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
# NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
# MODIFICATIONS.

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
    runner.run(suite)

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
