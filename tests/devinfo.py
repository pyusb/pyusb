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

