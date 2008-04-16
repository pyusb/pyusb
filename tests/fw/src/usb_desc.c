/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Descriptors for PyUSB
********************************************************************************
* History:
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "USB_lib.h"
#include "USB_desc.h"

const BYTE PYUSB_DeviceDescriptor[PYUSB_SIZ_DEVICE_DESC] = {
	0x12,	/* bLength */
	0x01,	/* bDescriptorType */
	0x00,	/* bcdUSB */
	0x02,
	0x00,	/* bDeviceClass */
	0x00,	/* bDeviceSubClass */
	0x00,	/* bDeviceProtocol */
	0x40,	/* bMaxPacketSize0 */
	0xfe,	/* idVendor	(FFFE) */
	0xff,
	0x01,	/* idProduct */
	0x00,
	0x00,	/* bcdDevice rel. 2.00 */
	0x01,
	4,		/* Index of string descriptor */
			  /*		describing manufacturer */
	22,		/* Index of string descriptor */
			  /*		describing product */
	0,    /*0,*/		/* Index of string descriptor */
			  /*		describing the device's */
			  /*		serial number */
	0x01	/* bNumConfigurations */
}; /* PYUSB_DeviceDescriptor */

const BYTE PYUSB_ConfigDescriptor[PYUSB_SIZ_CONFIG_DESC] = {
	0x09,	/* bLength: Configuation Descriptor size */
	0x02,	/* bDescriptorType: Configuration */
	PYUSB_SIZ_CONFIG_DESC,
			  /* wTotalLength: Bytes returned */
	0x00,
	0x01,	/* bNumInterfaces: 2 interfaces */
	0x01,	/* bConfigurationValue: */
			  /*	Configuration value */
	0x00,	/* iConfiguration: */
			  /*	Index of string descriptor */
			  /*	describing the configuration */
	0x80,	/* bmAttributes: */
			  /*	self powered */
	120,	/* MaxPower 120 mA: this current is used for detecting Vbus */

/******************** Descriptor of interface 1 ********************/

	0x09,	/* bLength: Interface Descriptor size */
	0x04,	/* bDescriptorType: */
			  /*	Interface descriptor type */
	0x00,	/* bInterfaceNumber: Number of Interface */
	0x00,	/* bAlternateSetting: Alternate setting */
	0x00,	/* bNumEndpoints*/
	0xFF,	/* bInterfaceClass:  */
	0xFF,	/* bInterfaceSubClass : */
	0xFF,	/* nInterfaceProtocol : bi-directional*/
	0,	  /* iInterface: */

/******************** Descriptor of interface 2 ********************/

	0x09,	/* bLength: Interface Descriptor size */
	0x04,	/* bDescriptorType: */
			  /*	Interface descriptor type */
	0x00,	/* bInterfaceNumber: Number of Interface */
	0x01,	/* bAlternateSetting: Alternate setting */
	0x06,	/* bNumEndpoints*/
	0xFF,	/* bInterfaceClass:  */
	0xFF,	/* bInterfaceSubClass : */
	0xFF,	/* nInterfaceProtocol : */
	0,	  /* iInterface: */
			  /*	Index of string descriptor */
/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x01,	/* bEndpointAddress: */
			  /*	Endpoint Address (OUT) */
	0x02,	/* bmAttributes: Bulk endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x00,	/* bInterval: Polling Interval */
/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x81,	/* bEndpointAddress: */
			  /*	Endpoint Address (IN) */
	0x02,	/* bmAttributes: Bulk endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x00,	/* bInterval: Polling Interval */

/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x02,	/* bEndpointAddress: */
			  /*	Endpoint Address (OUT) */
	0x03,	/* bmAttributes: Interrupt endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x0A,	/* bInterval: Polling Interval */

/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x82,	/* bEndpointAddress: */
			  /*	Endpoint Address (IN) */
	0x03,	/* bmAttributes: Interrupt endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x0A,	/* bInterval: Polling Interval */

/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x03,	/* bEndpointAddress: */
			  /*	Isochronous Address (OUT) */
	0x01,	/* bmAttributes: Bulk endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x04,	/* bInterval: Polling Interval */

/******************** Descriptor of endpoint ********************/

	0x07,	/* bLength: Endpoint Descriptor size */
	0x05,	/* bDescriptorType: */
			  /*	Endpoint descriptor type */
	0x84,	/* bEndpointAddress: */
			  /*	Endpoint Address (IN) */
	0x01,	/* bmAttributes: Isochronous endpoint */
	64,		/* wMaxPacketSize: 4 Byte max  */
	0x00,
	0x04	/* bInterval: Polling Interval */

}; /* PYUSB_ConfigDescriptor */

const BYTE PYUSB_StringDescriptor[PYUSB_SIZ_STRING_DESC] = {
	0x04,
	0x03,
	0x09,
	0x04,		/* LangID = 0x0409: U.S. English */
/* 4 */
	18,			/* Size of manufaturer string */
	0x03,		/* bDescriptorType = String descriptor */
	/* Manufacturer: "Mxyzp7lk" */
	'M',0, 'x',0, 'y',0, 'z',0, 'p',0, '7',0, 'l',0, 'k',0,
/* 22 */
	12,
	0x03,
	/* Product name: "PyUSB" */
	'P',0, 'y',0, 'U',0, 'S',0, 'B',0

}; /* PYUSB_StringDescriptor */

