# Copyright 2009-2017 Wander Lairson Costa
# Copyright 2009-2020 PyUSB contributors
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

import usb.util

ID_VENDOR = 0x04d8
ID_PRODUCT = 0xfa2e

# transfer interfaces
INTF_BULK = 0
INTF_INTR = 1
INTF_ISO = 2

# endpoints address
EP_BULK = 1
EP_INTR = 2
EP_ISO = 3

# test type
TEST_NONE = 0
TEST_PCREAD = 1
TEST_PCWRITE = 2
TEST_LOOP = 3

# Vendor requests
PICFW_SET_TEST = 0x0e
PICFW_SET_TEST = 0x0f
PICFW_SET_VENDOR_BUFFER = 0x10
PICFW_GET_VENDOR_BUFFER = 0x11

def set_test_type(t, dev = None):
    if dev is None:
        dev = usb.core.find(idVendor = ID_VENDOR, idProduct = ID_PRODUCT)

    bmRequestType = usb.util.build_request_type(
                        usb.util.CTRL_OUT,
                        usb.util.CTRL_TYPE_VENDOR,
                        usb.util.CTRL_RECIPIENT_INTERFACE
                    )

    dev.ctrl_transfer(
        bmRequestType = bmRequestType,
        bRequest = PICFW_SET_TEST,
        wValue = t,
        wIndex = 0
    )

