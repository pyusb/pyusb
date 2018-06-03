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

r"""usb._lookups - Lookup tables for USB
"""

descriptors = {
        0x1 : "Device",
        0x2 : "Configuration",
        0x3 : "String",
        0x4 : "Interface",
        0x5 : "Endpoint",
        0x6 : "Device qualifier",
        0x7 : "Other speed configuration",
        0x8 : "Interface power",
        0x9 : "OTG",
        0xA : "Debug",
        0xB : "Interface association",
        0xC : "Security",
        0xD : "Key",
        0xE : "Encryption type",
        0xF : "Binary device object store (BOS)",
        0x10 : "Device capability",
        0x11 : "Wireless endpoint companion",
        0x30 : "SuperSpeed endpoint companion",
        }

device_classes = {
        0x0 : "Specified at interface",
        0x2 : "Communications Device",
        0x9 : "Hub",
        0xF : "Personal Healthcare Device",
        0xDC : "Diagnostic Device",
        0xE0 : "Wireless Controller",
        0xEF : "Miscellaneous",
        0xFF : "Vendor-specific",
        }

interface_classes = {
        0x0 : "Reserved",
        0x1 : "Audio",
        0x2 : "CDC Communication",
        0x3 : "Human Interface Device",
        0x5 : "Physical",
        0x6 : "Image",
        0x7 : "Printer",
        0x8 : "Mass Storage",
        0x9 : "Hub",
        0xA : "CDC Data",
        0xB : "Smart Card",
        0xD : "Content Security",
        0xE : "Video",
        0xF : "Personal Healthcare",
        0xDC : "Diagnostic Device",
        0xE0 : "Wireless Controller",
        0xEF : "Miscellaneous",
        0xFE : "Application Specific",
        0xFF : "Vendor Specific",
        }

ep_attributes = {
        0x0 : "Control",
        0x1 : "Isochronous",
        0x2 : "Bulk",
        0x3 : "Interrupt",
        }

MAX_POWER_UNITS_USB2p0 = 2             # mA
MAX_POWER_UNITS_USB_SUPERSPEED = 8     # mA
