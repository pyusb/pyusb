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

