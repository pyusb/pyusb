# Copyright (C) 2009-2010 Wander Lairson Costa 
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

r"""PyUSB - Easy USB access in Python

This package exports the following modules and subpackages:

    core - the main USB implementation
    legacy - the compatibility layer with 0.x version
    backend - the support for backend implementations.

Since version 1.0, main PyUSB implementation lives in the 'usb.core'
module. New applications are encouraged to use it.
"""

__author__ = 'Wander Lairson Costa'

__all__ = ['legacy', 'core', 'backend', 'util']

# We import all 'legacy' module symbols to provide compatility
# with applications that use 0.x versions.
# We avoid the from .module import * sentence because of a bug
# in Python 2.5 that makes it not work.
# See http://bugs.python.org/issue2400
from .legacy import (CLASS_AUDIO,
                    CLASS_COMM,
                    CLASS_DATA,
                    CLASS_HID,
                    CLASS_HUB,
                    CLASS_MASS_STORAGE,
                    CLASS_PER_INTERFACE,
                    CLASS_PRINTER,
                    CLASS_VENDOR_SPEC,
                    DT_CONFIG,
                    DT_CONFIG_SIZE,
                    DT_DEVICE,
                    DT_DEVICE_SIZE,
                    DT_ENDPOINT,
                    DT_ENDPOINT_AUDIO_SIZE,
                    DT_ENDPOINT_SIZE,
                    DT_HID,
                    DT_HUB,
                    DT_HUB_NONVAR_SIZE,
                    DT_INTERFACE,
                    DT_INTERFACE_SIZE,
                    DT_PHYSICAL,
                    DT_REPORT,
                    DT_STRING,
                    ENDPOINT_ADDRESS_MASK,
                    ENDPOINT_DIR_MASK,
                    ENDPOINT_IN,
                    ENDPOINT_OUT,
                    ENDPOINT_TYPE_BULK,
                    ENDPOINT_TYPE_CONTROL,
                    ENDPOINT_TYPE_INTERRUPT,
                    ENDPOINT_TYPE_ISOCHRONOUS,
                    ENDPOINT_TYPE_MASK,
                    ERROR_BEGIN,
                    MAXALTSETTING,
                    MAXCONFIG,
                    MAXENDPOINTS,
                    MAXINTERFACES,
                    RECIP_DEVICE,
                    RECIP_ENDPOINT,
                    RECIP_INTERFACE,
                    RECIP_OTHER,
                    REQ_CLEAR_FEATURE,
                    REQ_GET_CONFIGURATION,
                    REQ_GET_DESCRIPTOR,
                    REQ_GET_INTERFACE,
                    REQ_GET_STATUS,
                    REQ_SET_ADDRESS,
                    REQ_SET_CONFIGURATION,
                    REQ_SET_DESCRIPTOR,
                    REQ_SET_FEATURE,
                    REQ_SET_INTERFACE,
                    REQ_SYNCH_FRAME,
                    TYPE_CLASS,
                    TYPE_RESERVED,
                    TYPE_STANDARD,
                    TYPE_VENDOR,
                    Endpoint,
                    Interface,
                    Configuration,
                    DeviceHandle,
                    Device,
                    Bus,
                    busses)
