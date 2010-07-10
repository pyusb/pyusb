/********************************************************************
 FileName:     	usb_config.h
 Dependencies: 	Always: GenericTypeDefs.h, usb_device.h
               	Situational: usb_function_hid.h, usb_function_cdc.h, usb_function_msd.h, etc.
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
 *******************************************************************/

/*********************************************************************
 * Descriptor specific type definitions are defined in: usbd.h
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H

// DESCRIPTOR CONFIGURATION /////////////////////////////////////////

// If defined, creates a dual interfaces device with two separate
// benchmark interfaces.
// 
//#define DUAL_INTERFACE
/////////////////////////////////////////////////////////////////////

// HARDWARE ID CONFIGURATION ////////////////////////////////////////
#define VENDOR_ID			0xFFFE
#define PRODUCT_ID			0x0001
#define BCD_RELEASE_NUMBER	0x0001
/////////////////////////////////////////////////////////////////////

// DESCRIPTOR STRING CONFIGURATION //////////////////////////////////
#define MANUFACTURER_STRING_LENGTH 8
#define MANUFACTURER_STRING 'M','x','y','z','p','7','l','k' // sorry Travis removing your credit, but it is
															// only to stay backward compatible with my old firmware

#if !defined(DUAL_INTERFACE)
	#define SERIAL_NUMBER_LENGTH 6
	#define SERIAL_NUMBER 'B','M','D','0','0','1'

	#define PRODUCT_STRING_LENGTH 5
	#define PRODUCT_STRING 'P','y','U','S','B'
#else
	#define SERIAL_NUMBER_LENGTH 6
	#define SERIAL_NUMBER 'B','M','D','0','0','2'

	#define PRODUCT_STRING_LENGTH 21
	#define PRODUCT_STRING 'D','u','a','l',' ','B','e','n','c','h','m','a','r','k',' ','D','e','v','i','c','e'

	#define INTF0_STRING_LENGTH 13
	#define INTF0_STRING 'B','e','n','c','h','m','a','r','k',' ','O','n','e'

	#define INTF1_STRING_LENGTH 13
	#define INTF1_STRING 'B','e','n','c','h','m','a','r','k',' ','T','w','o'
#endif

/////////////////////////////////////////////////////////////////////
#define EP_ISO        0x01            // Isochronous Transfer
#define EP_BULK       0x02            // Bulk Transfer
#define EP_INT        0x03			  // Interrupt Transfer


// INTERFACE & ENDPOINT CONFIGURATION ///////////////////////////////

/////////////////////////////////////////////////////////////////////
// ENDPOINT #1 (IN,OUT) Size & Type
#define USBGEN_EP_SIZE_INTF0	64
//#define INTF0					EP_ISO
#define INTF0_1					EP_BULK
#define INTF0_2					EP_INT
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ENDPOINT #2 (IN,OUT) Size & Type
#define USBGEN_EP_SIZE_INTF1	64
#define INTF1					EP_ISO
//#define INTF1					EP_BULK
// #define INTF1				EP_INT
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// USB SERVICE MODE
#define USB_POLLING
//#define USB_INTERRUPT
/////////////////////////////////////////////////////////////////////

// INTERFACE & ENDPOINT INTERNAL SETUP //////////////////////////////
#if (INTF0_1==EP_BULK)
	#define USBGEN_EP_ATTRIBUTES_INTF0_1	EP_BULK
	#define USBGEN_EP_HANDSHAKE_INTF0_1		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF0_1		0
#elif (INTF0_1==EP_INT)
	#define USBGEN_EP_ATTRIBUTES_INTF0_1	EP_INT
	#define USBGEN_EP_HANDSHAKE_INTF0_1		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF0_1		1
#elif (INTF0_1==EP_ISO)
	#define USBGEN_EP_ATTRIBUTES_INTF0_1	EP_ISO|_AS|_DE
	#define USBGEN_EP_HANDSHAKE_INTF0_1		0
	#define USBGEN_EP_INTERVAL_INTF0_1		1
#endif

#if (INTF0_2==EP_BULK)
	#define USBGEN_EP_ATTRIBUTES_INTF0_2	EP_BULK
	#define USBGEN_EP_HANDSHAKE_INTF0_2		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF0_2		0
#elif (INTF0_2==EP_INT)
	#define USBGEN_EP_ATTRIBUTES_INTF0_2	EP_INT
	#define USBGEN_EP_HANDSHAKE_INTF0_2		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF0_2		1
#elif (INTF0_2==EP_ISO)
	#define USBGEN_EP_ATTRIBUTES_INTF0_2	EP_ISO|_AS|_DE
	#define USBGEN_EP_HANDSHAKE_INTF0_2		0
	#define USBGEN_EP_INTERVAL_INTF0_2		1
#endif


#if (INTF1==EP_BULK)
	#define USBGEN_EP_ATTRIBUTES_INTF1		EP_BULK
	#define USBGEN_EP_HANDSHAKE_INTF1		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF1		0
#elif (INTF1==EP_INT)
	#define USBGEN_EP_ATTRIBUTES_INTF1		EP_INT
	#define USBGEN_EP_HANDSHAKE_INTF1		USB_HANDSHAKE_ENABLED
	#define USBGEN_EP_INTERVAL_INTF1		1
#elif (INTF1==EP_ISO)
	#define USBGEN_EP_ATTRIBUTES_INTF1		EP_ISO|_AS|_DE
	#define USBGEN_EP_HANDSHAKE_INTF1		0
	#define USBGEN_EP_INTERVAL_INTF1		1
#endif

/////////////////////////////////////////////////////////////////////


/** DEFINITIONS ****************************************************/
#define USBGEN_EP_NUM_INTF0_1	1
#define USBGEN_EP_NUM_INTF0_2	2
#define USBGEN_EP_NUM_INTF1		2

#define USB_EP0_BUFF_SIZE		16	// Valid Options: 8, 16, 32, or 64 bytes.
								// Using larger options take more SRAM, but
								// does not provide much advantage in most types
								// of applications.  Exceptions to this, are applications
								// that use EP0 IN or OUT for sending large amounts of
								// application related data.
#if defined(DUAL_INTERFACE)
	#define USB_MAX_NUM_INT     		2   // For tracking Alternate Setting
	#define USB_MAX_EP_NUMBER			2
	#define USB_NUM_STRING_DESCRIPTORS	6
#else
	#define USB_MAX_NUM_INT     		1   // For tracking Alternate Setting
	#define USB_MAX_EP_NUMBER			2
	#define USB_NUM_STRING_DESCRIPTORS	4
#endif

//Device descriptor - if these two definitions are not defined then
//  a ROM USB_DEVICE_DESCRIPTOR variable by the exact name of device_dsc
//  must exist.
#define USB_USER_DEVICE_DESCRIPTOR &device_dsc
#define USB_USER_DEVICE_DESCRIPTOR_INCLUDE extern ROM USB_DEVICE_DESCRIPTOR device_dsc

//Configuration descriptors - if these two definitions do not exist then
//  a ROM BYTE *ROM variable named exactly USB_CD_Ptr[] must exist.
#define USB_USER_CONFIG_DESCRIPTOR USB_CD_Ptr
#define USB_USER_CONFIG_DESCRIPTOR_INCLUDE extern ROM BYTE *ROM USB_CD_Ptr[]

//Make sure only one of the below "#define USB_PING_PONG_MODE"
//is uncommented.
//#define USB_PING_PONG_MODE USB_PING_PONG__NO_PING_PONG
#define USB_PING_PONG_MODE USB_PING_PONG__FULL_PING_PONG
//#define USB_PING_PONG_MODE USB_PING_PONG__EP0_OUT_ONLY
//#define USB_PING_PONG_MODE USB_PING_PONG__ALL_BUT_EP0		//NOTE: This mode is not supported in PIC18F4550 family rev A3 devices

/* Parameter definitions are defined in usb_device.h */
#define USB_PULLUP_OPTION USB_PULLUP_ENABLE
//#define USB_PULLUP_OPTION USB_PULLUP_DISABLED

#define USB_TRANSCEIVER_OPTION USB_INTERNAL_TRANSCEIVER
//External Transceiver support is not available on all product families.  Please
//  refer to the product family datasheet for more information if this feature
//  is available on the target processor.
//#define USB_TRANSCEIVER_OPTION USB_EXTERNAL_TRANSCEIVER

#define USB_SPEED_OPTION USB_FULL_SPEED
//#define USB_SPEED_OPTION USB_LOW_SPEED //(not valid option for PIC24F devices)

#define USB_SUPPORT_DEVICE

// The USB_ENABLE_xx are not implemented in the MCP 2.7 stack.
#define USB_ENABLE_ALL_HANDLERS


/** DEVICE CLASS USAGE *********************************************/
#define USB_USE_GEN
#define	EVN	0
#define	ODD	1

#endif //USBCFG_H
