# Copyright (C) 2016 Robert Hartung
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

r"""usb.hotplig - Hotplug functionality.

This module exports:

register_callback - register a callback.
deregister_callback - deregister a callback.
"""

__author__ = 'Robert Hartung'

__all__ = [ 'register_callback', 'deregister_callback', 'loop', 'LIBUSB_HOTPLUG_MATCH_ANY', 'LIBUSB_HOTPLUG_NO_FLAGS', 'LIBUSB_HOTPLUG_ENUMERATE', 'LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED', 'LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT' ]

import usb.backend.libusb1 as libusb1
import usb.core as core

# we need this because the callback() is defined on the fly and does not store the cb_fn variable
from functools import partial

# Wildcard matching for hotplug events
LIBUSB_HOTPLUG_MATCH_ANY = -1
# No flags
LIBUSB_HOTPLUG_NO_FLAGS = 0
# Call hotplug for already enumerated devices
LIBUSB_HOTPLUG_ENUMERATE = 1
# Event for when a device arrived (plugged-in)
LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED = 0x01
# Event for when a device left (unplugged)
LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT = 0x02

# General error class
class HotplugError(Exception):
    """Base class for exceptions in this module."""
    pass

# For now, we only support libusb1 as a backend!
backend = libusb1.get_backend()
if backend == None:
    raise HotplugError("Hotplug support requires libusb1 to be available!")

def register_callback(events, flags, vendor_id, product_id, dev_class, cb, user_data):
    return backend.hotplug_register_callback(events, flags, vendor_id, product_id, dev_class, cb, user_data)

def deregister_callback(handle):
    backend.hotplug_deregister_callback(handle)

def loop():
    return backend.loop()
