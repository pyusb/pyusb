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

r"""usb.util - Utility functions.

This module exports:

endpoint_address - return the endpoint absolute address.
endpoint_direction - return the endpoint transfer direction.
endpoint_type - return the endpoint type
ctrl_direction - return the direction of a control transfer
build_request_type - build a bmRequestType field of a control transfer.
find_descriptor - find an inner descriptor.
claim_interface - explicitly claim an interface.
release_interface - explicitly release an interface.
dispose_resources - release internal resources allocated by the object.
get_langids - retrieve the list of supported string languages from the device.
get_string - retrieve a string descriptor from the device.
"""

__author__ = 'Wander Lairson Costa'

import operator
import array
from typing import Union, Generator, Any, Callable, Tuple

import usb.core

# descriptor type
DESC_TYPE_DEVICE = 0x01
DESC_TYPE_CONFIG = 0x02
DESC_TYPE_STRING = 0x03
DESC_TYPE_INTERFACE = 0x04
DESC_TYPE_ENDPOINT = 0x05

# endpoint direction
ENDPOINT_IN = 0x80
ENDPOINT_OUT = 0x00

# endpoint type
ENDPOINT_TYPE_CTRL = 0x00
ENDPOINT_TYPE_ISO = 0x01
ENDPOINT_TYPE_BULK = 0x02
ENDPOINT_TYPE_INTR = 0x03

# control request type
CTRL_TYPE_STANDARD = (0 << 5)
CTRL_TYPE_CLASS = (1 << 5)
CTRL_TYPE_VENDOR = (2 << 5)
CTRL_TYPE_RESERVED = (3 << 5)

# control request recipient
CTRL_RECIPIENT_DEVICE = 0
CTRL_RECIPIENT_INTERFACE = 1
CTRL_RECIPIENT_ENDPOINT = 2
CTRL_RECIPIENT_OTHER = 3

# control request direction
CTRL_OUT = 0x00
CTRL_IN = 0x80

_ENDPOINT_ADDR_MASK = 0x0f
_ENDPOINT_DIR_MASK = 0x80
_ENDPOINT_TRANSFER_TYPE_MASK = 0x03
_CTRL_DIR_MASK = 0x80

# speed type
SPEED_LOW = 1
SPEED_FULL = 2
SPEED_HIGH = 3
SPEED_SUPER = 4
SPEED_UNKNOWN = 0


def endpoint_address(address: int) -> int:
    r"""Return the endpoint absolute address.

    The address parameter is the bEndpointAddress field
    of the endpoint descriptor.
    """


def endpoint_direction(address: int) -> int:
    r"""Return the endpoint direction.

    The address parameter is the bEndpointAddress field
    of the endpoint descriptor.
    The possible return values are ENDPOINT_OUT or ENDPOINT_IN.
    """


def endpoint_type(bmAttributes: int) -> int:
    r"""Return the transfer type of the endpoint.

    The bmAttributes parameter is the bmAttributes field
    of the endpoint descriptor.
    The possible return values are: ENDPOINT_TYPE_CTRL,
    ENDPOINT_TYPE_ISO, ENDPOINT_TYPE_BULK or ENDPOINT_TYPE_INTR.
    """


def ctrl_direction(bmRequestType: int) -> int:
    r"""Return the direction of a control request.

    The bmRequestType parameter is the value of the
    bmRequestType field of a control transfer.
    The possible return values are CTRL_OUT or CTRL_IN.
    """


def build_request_type(direction: int, type: int, recipient: int):
    r"""Build a bmRequestType field for control requests.

    These is a conventional function to build a bmRequestType
    for a control request.

    The direction parameter can be CTRL_OUT or CTRL_IN.
    The type parameter can be CTRL_TYPE_STANDARD, CTRL_TYPE_CLASS,
    CTRL_TYPE_VENDOR or CTRL_TYPE_RESERVED values.
    The recipient can be CTRL_RECIPIENT_DEVICE, CTRL_RECIPIENT_INTERFACE,
    CTRL_RECIPIENT_ENDPOINT or CTRL_RECIPIENT_OTHER.

    Return the bmRequestType value.
    """


def create_buffer(length: Union[bytes, bytearray]) -> array.array:
    r"""Create a buffer to be passed to a read function.

    A read function may receive an out buffer so the data
    is read inplace and the object can be reused, avoiding
    the overhead of creating a new object at each new read
    call. This function creates a compatible sequence buffer
    of the given length.
    """


def find_descriptor(desc: Union[
    usb.core.Device,
    usb.core.Interface,
    usb.core.Configuration,
    usb.core.Endpoint,
], find_all: bool = False,
                    custom_match: Callable[[Any], bool] = None,
                    **args
                    ) -> Generator[Any, None, None]:
    r"""Find an inner descriptor.

    find_descriptor works in the same way as the core.find() function does,
    but it acts on general descriptor objects. For example, suppose you
    have a Device object called dev and want a Configuration of this
    object with its bConfigurationValue equals to 1, the code would
    be like so:

    >>> cfg = util.find_descriptor(dev, bConfigurationValue=1)

    You can use any field of the Descriptor as a match criteria, and you
    can supply a customized match just like core.find() does. The
    find_descriptor function also accepts the find_all parameter to get
    an iterator instead of just one descriptor.
    """


def claim_interface(device: usb.core.Device, interface: Union[usb.core.Interface, int]) -> None:
    r"""Explicitly claim an interface.

    PyUSB users normally do not have to worry about interface claiming,
    as the library takes care of it automatically. But there are situations
    where you need deterministic interface claiming. For these uncommon
    cases, you can use claim_interface.

    If the interface is already claimed, either through a previously call
    to claim_interface or internally by the device object, nothing happens.
    """


def release_interface(device: usb.core.Device, interface: Union[usb.core.Interface, int]) -> None:
    r"""Explicitly release an interface.

    This function is used to release an interface previously claimed,
    either through a call to claim_interface or internally by the
    device object.

    Normally, you do not need to worry about claiming policies, as
    the device object takes care of it automatically.
    """


def dispose_resources(device: usb.core.Device) -> None:
    r"""Release internal resources allocated by the object.

    Sometimes you need to provide deterministic resources
    freeing, for example to allow another application to
    talk to the device. As Python does not provide deterministic
    destruction, this function releases all internal resources
    allocated by the device, like device handle and interface
    policy.

    After calling this function, you can continue using the device
    object normally. If the resources will be necessary again, it
    will be allocated automatically.
    """


def get_langids(dev: usb.core.Device) -> Tuple[int]:
    r"""Retrieve the list of supported Language IDs from the device.

    Most client code should not call this function directly, but instead use
    the langids property on the Device object, which will call this function as
    needed and cache the result.

    USB LANGIDs are 16-bit integers familiar to Windows developers, where
    for example instead of en-US you say 0x0409. See the file USB_LANGIDS.pdf
    somewhere on the usb.org site for a list, which does not claim to be
    complete. It requires "system software must allow the enumeration and
    selection of LANGIDs that are not currently on this list." It also requires
    "system software should never request a LANGID not defined in the LANGID
    code array (string index = 0) presented by a device." Client code can
    check this tuple before issuing string requests for a specific language ID.

    dev is the Device object whose supported language IDs will be retrieved.

    The return value is a tuple of integer LANGIDs, possibly empty if the
    device does not support strings at all (which USB 3.1 r1.0 section
    9.6.9 allows). In that case client code should not request strings at all.

    A USBError may be raised from this function for some devices that have no
    string support, instead of returning an empty tuple. The accessor for the
    langids property on Device catches that case and supplies an empty tuple,
    so client code can ignore this detail by using the langids property instead
    of directly calling this function.
    """


def get_string(dev: usb.core.Device, index: int, langid: int = None) -> str:
    r"""Retrieve a string descriptor from the device.

    dev is the Device object which the string will be read from.

    index is the string descriptor index and langid is the Language
    ID of the descriptor. If langid is omitted, the string descriptor
    of the first Language ID will be returned.

    Zero is never the index of a real string. The USB spec allows a device to
    use zero in a string index field to indicate that no string is provided.
    So the caller does not have to treat that case specially, this function
    returns None if passed an index of zero, and generates no traffic
    to the device.

    The return value is the unicode string present in the descriptor, or None
    if the requested index was zero.
    """
