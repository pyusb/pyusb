r"""usb.util - Utility functions."""

__author__ = 'Wander Lairson Costa'

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
CTRL_TYPE_STANDARD = 0
CTRL_TYPE_CLASS = 1
CTRL_TYPE_VENDOR = 2
CTRL_TYPE_RESERVED = 3

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

def endpoint_address(address):
    r"""Return the endpoint absolute address.
    
    The address parameter is the bEndpointAddress field
    of the endpoint descriptor.
    """
    return address & _ENDPOINT_ADDR_MASK

def endpoint_direction(address):
    r"""Return the endpoint direction.

    The address parameter is the bEndpointAddress field
    of the endpoint descriptor.
    The possible return values are ENDPOINT_OUT or ENDPOINT_IN.
    """
    return address & _ENDPOINT_DIR_MASK

def endpoint_type(bmAttributes):
    r"""Return the transfer type of the endpoint.
    
    The bmAttributes parameter is the bmAttributes field
    of the endpoint descriptor.
    The possible return values are: ENDPOINT_TYPE_CTRL,
    ENDPOINT_TYPE_ISO, ENDPOINT_TYPE_BULK or ENDPOINT_TYPE_INTR.
    """
    return bmAttributes & _ENDPOINT_TRANSFER_TYPE_MASK

def ctrl_direction(bmRequestType):
    r"""Return the direction of a control request.
    
    The bmRequestType parameter is the value of the
    bmRequestType field of a control transfer.
    The possible return values are CTRL_OUT or CTRL_IN.
    """
    return bmRequestType & _CTRL_DIR_MASK

def build_request_type(direction, type, recipient):
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
    return recipient | (type << 5) | direction
