DESC_TYPE_DEVICE = 0x01
DESC_TYPE_CONFIG = 0x02
DESC_TYPE_STRING = 0x03
DESC_TYPE_INTERFACE = 0x04
DESC_TYPE_ENDPOINT = 0x05

ENDPOINT_IN = 0x80
ENDPOINT_OUT = 0x00

ENDPOINT_TYPE_CONTROL = 0x00
ENDPOINT_TYPE_ISOCHRONOUS = 0x01
ENDPOINT_TYPE_BULK = 0x02
ENDPOINT_TYPE_INTERRUPT = 0x03

CTRL_OUT = 0x00
CTRL_IN = 0x80

_ENDPOINT_ADDR_MASK = 0x0f
_ENDPOINT_DIR_MASK = 0x7f
_ENDPOINT_TRANSFER_TYPE_MASK = 0x03
_CTRL_DIR_MASK = 0x7f

def endpoint_address(address):
    return address & _ENDPOINT_ADDR_MASK

def endpoint_direction(address):
    return address & _ENDPOINT_DIR_MASK

def endpoint_transfer_type(bmAttributes):
    return bmAttributes & _ENDPOINT_TRANSFER_TYPE_MASK

def ctrl_direction(bmRequestType):
    return bmRequestType & _CTRL_DIR_MASK
