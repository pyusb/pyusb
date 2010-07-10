#ifndef _PICFWCOMMANDS_H
#define _PICFWCOMMANDS_H

// PicFW Vendor-Specific Requests
enum PICFW_COMMANDS
{
	CTRL_LOOPBACK_WRITE	= 0x00,
	CTRL_LOOPBACK_READ	= 0x01,
    PICFW_SET_TEST		= 0x0E,
    PICFW_GET_TEST		= 0x0F,
    PICFW_SET_EEDATA	= 0x10,
    PICFW_GET_EEDATA	= 0x11
};

#endif
