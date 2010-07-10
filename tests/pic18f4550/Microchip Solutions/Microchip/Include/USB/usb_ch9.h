/*******************************************************************************

    USB Chapter 9 Protocol (Header File)

Summary:
    This file defines data structures, constants, and macros that are used to
    to support the USB Device Framework protocol described in Chapter 9 of the
    USB 2.0 specification.

Description:
    This file defines data structures, constants, and macros that are used to
    to support the USB Device Framework protocol described in Chapter 9 of the
    USB 2.0 specification.

    This file is located in the "\<Install Directory\>\\Microchip\\Include\\USB"
    directory.
    
    When including this file in a new project, this file can either be
    referenced from the directory in which it was installed or copied
    directly into the user application folder. If the first method is
    chosen to keep the file located in the folder in which it is installed
    then include paths need to be added so that the library and the
    application both know where to reference each others files. If the
    application folder is located in the same folder as the Microchip
    folder (like the current demo folders), then the following include
    paths need to be added to the application's project:
    
    .
    
    ..\\..\\Microchip\\Include
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************

* FileName:        usb_ch9.h
* Dependencies:    None
* Processor:       PIC18/PIC24/PIC32MX microcontrollers with USB module
* Compiler:        C18 v3.13+/C30 v2.01+/C32 v0.00.18+
* Company:         Microchip Technology, Inc.
* File Description:
* This file contains the definitions and prototypes used for
* specification chapter 9 compliance.

Software License Agreement

The software supplied herewith by Microchip Technology Incorporated
(the “Company”) for its PICmicro® Microcontroller is intended and
supplied to you, the Company’s customer, for use solely and
exclusively on Microchip PICmicro Microcontroller products. The
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

*******************************************************************************/
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  2.6    Moved many of the CH9 defintions from the device stack files
         into this file.
********************************************************************/
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
#ifndef _USB_CH9_H_
#define _USB_CH9_H_
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: USB Descriptors
// *****************************************************************************
// *****************************************************************************

#define USB_DESCRIPTOR_DEVICE           0x01    // bDescriptorType for a Device Descriptor.
#define USB_DESCRIPTOR_CONFIGURATION    0x02    // bDescriptorType for a Configuration Descriptor.
#define USB_DESCRIPTOR_STRING           0x03    // bDescriptorType for a String Descriptor.
#define USB_DESCRIPTOR_INTERFACE        0x04    // bDescriptorType for an Interface Descriptor.
#define USB_DESCRIPTOR_ENDPOINT         0x05    // bDescriptorType for an Endpoint Descriptor.
#define USB_DESCRIPTOR_DEVICE_QUALIFIER 0x06    // bDescriptorType for a Device Qualifier.
#define USB_DESCRIPTOR_OTHER_SPEED      0x07    // bDescriptorType for a Other Speed Configuration.
#define USB_DESCRIPTOR_INTERFACE_POWER  0x08    // bDescriptorType for Interface Power.
#define USB_DESCRIPTOR_OTG              0x09    // bDescriptorType for an OTG Descriptor.

// *****************************************************************************
/* USB Device Descriptor Structure

This struct defines the structure of a USB Device Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_DEVICE_DESCRIPTOR
{
    BYTE bLength;               // Length of this descriptor.
    BYTE bDescriptorType;       // DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
    WORD bcdUSB;                // USB Spec Release Number (BCD).
    BYTE bDeviceClass;          // Class code (assigned by the USB-IF). 0xFF-Vendor specific.
    BYTE bDeviceSubClass;       // Subclass code (assigned by the USB-IF).
    BYTE bDeviceProtocol;       // Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
    BYTE bMaxPacketSize0;       // Maximum packet size for endpoint 0.
    WORD idVendor;              // Vendor ID (assigned by the USB-IF).
    WORD idProduct;             // Product ID (assigned by the manufacturer).
    WORD bcdDevice;             // Device release number (BCD).
    BYTE iManufacturer;         // Index of String Descriptor describing the manufacturer.
    BYTE iProduct;              // Index of String Descriptor describing the product.
    BYTE iSerialNumber;         // Index of String Descriptor with the device's serial number.
    BYTE bNumConfigurations;    // Number of possible configurations.
} USB_DEVICE_DESCRIPTOR;


// *****************************************************************************
/* USB Configuration Descriptor Structure

This struct defines the structure of a USB Configuration Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_CONFIGURATION_DESCRIPTOR
{
    BYTE bLength;               // Length of this descriptor.
    BYTE bDescriptorType;       // CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
    WORD wTotalLength;          // Total length of all descriptors for this configuration.
    BYTE bNumInterfaces;        // Number of interfaces in this configuration.
    BYTE bConfigurationValue;   // Value of this configuration (1 based).
    BYTE iConfiguration;        // Index of String Descriptor describing the configuration.
    BYTE bmAttributes;          // Configuration characteristics.
    BYTE bMaxPower;             // Maximum power consumed by this configuration.
} USB_CONFIGURATION_DESCRIPTOR;

// Attributes bits
#define USB_CFG_DSC_REQUIRED     0x80                       // Required attribute
#define USB_CFG_DSC_SELF_PWR    (0x40|USB_CFG_DSC_REQUIRED) // Device is self powered.
#define USB_CFG_DSC_REM_WAKE    (0x20|USB_CFG_DSC_REQUIRED) // Device can request remote wakup


// *****************************************************************************
/* USB Interface Descriptor Structure

This struct defines the structure of a USB Interface Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_INTERFACE_DESCRIPTOR
{
    BYTE bLength;               // Length of this descriptor.
    BYTE bDescriptorType;       // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
    BYTE bInterfaceNumber;      // Number of this interface (0 based).
    BYTE bAlternateSetting;     // Value of this alternate interface setting.
    BYTE bNumEndpoints;         // Number of endpoints in this interface.
    BYTE bInterfaceClass;       // Class code (assigned by the USB-IF).  0xFF-Vendor specific.
    BYTE bInterfaceSubClass;    // Subclass code (assigned by the USB-IF).
    BYTE bInterfaceProtocol;    // Protocol code (assigned by the USB-IF).  0xFF-Vendor specific.
    BYTE iInterface;            // Index of String Descriptor describing the interface.
} USB_INTERFACE_DESCRIPTOR;


// *****************************************************************************
/* USB Endpoint Descriptor Structure

This struct defines the structure of a USB Endpoint Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_ENDPOINT_DESCRIPTOR
{
    BYTE bLength;               // Length of this descriptor.
    BYTE bDescriptorType;       // ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
    BYTE bEndpointAddress;      // Endpoint address. Bit 7 indicates direction (0=OUT, 1=IN).
    BYTE bmAttributes;          // Endpoint transfer type.
    WORD wMaxPacketSize;        // Maximum packet size.
    BYTE bInterval;             // Polling interval in frames.
} USB_ENDPOINT_DESCRIPTOR;


// Endpoint Direction
#define EP_DIR_IN           0x80    // Data flows from device to host
#define EP_DIR_OUT          0x00    // Data flows from host to device


// ******************************************************************
// USB Endpoint Attributes
// ******************************************************************

// Section: Transfer Types
#define EP_ATTR_CONTROL     (0<<0)  // Endoint used for control transfers
#define EP_ATTR_ISOCH       (1<<0)  // Endpoint used for isochronous transfers
#define EP_ATTR_BULK        (2<<0)  // Endpoint used for bulk transfers
#define EP_ATTR_INTR        (3<<0)  // Endpoint used for interrupt transfers

// Section: Synchronization Types (for isochronous enpoints)
#define EP_ATTR_NO_SYNC     (0<<2)  // No Synchronization
#define EP_ATTR_ASYNC       (1<<2)  // Asynchronous
#define EP_ATTR_ADAPT       (2<<2)  // Adaptive synchronization
#define EP_ATTR_SYNC        (3<<2)  // Synchronous

// Section: Usage Types (for isochronous endpoints)
#define EP_ATTR_DATA        (0<<4)  // Data Endpoint
#define EP_ATTR_FEEDBACK    (1<<4)  // Feedback endpoint
#define EP_ATTR_IMP_FB      (2<<4)  // Implicit Feedback data EP

// Section: Max Packet Sizes
#define EP_MAX_PKT_INTR_LS  8       // Max low-speed interrupt packet
#define EP_MAX_PKT_INTR_FS  64      // Max full-speed interrupt packet
#define EP_MAX_PKT_ISOCH_FS 1023    // Max full-speed isochronous packet
#define EP_MAX_PKT_BULK_FS  64      // Max full-speed bulk packet
#define EP_LG_PKT_BULK_FS   32      // Large full-speed bulk packet
#define EP_MED_PKT_BULK_FS  16      // Medium full-speed bulk packet
#define EP_SM_PKT_BULK_FS   8       // Small full-speed bulk packet

/* Descriptor IDs
The descriptor ID type defines the information required by the HOST during a 
GET_DESCRIPTOR request
*/
typedef struct
{
    BYTE    index;
    BYTE    type;
    UINT16  language_id;

} DESCRIPTOR_ID;

// *****************************************************************************
/* USB OTG Descriptor Structure

This struct defines the structure of a USB OTG Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_OTG_DESCRIPTOR
{
    BYTE bLength;               // Length of this descriptor.
    BYTE bDescriptorType;       // OTG descriptor type (USB_DESCRIPTOR_OTG).
    BYTE bmAttributes;          // OTG attributes.
} USB_OTG_DESCRIPTOR;


// ******************************************************************
// Section: USB String Descriptor Structure
// ******************************************************************
// This structure describes the USB string descriptor.  The string
// descriptor provides user-readable information about various aspects of
// the device.  The first string desriptor (string descriptor zero (0)),
// provides a list of the number of languages supported by the set of
// string descriptors for this device instead of an actual string.
//
// Note: The strings are in 2-byte-per-character unicode, not ASCII.
//
// Note: This structure only describes the "header" of the string
// descriptor.  The actual data (either the language ID array or the
// array of unicode characters making up the string, must be allocated
// immediately following this header with no padding between them.

typedef struct __attribute__ ((packed)) _USB_STRING_DSC
{
    BYTE   bLength;             // Size of this descriptor
    BYTE   bDescriptorType;     // Type, USB_DSC_STRING

} USB_STRING_DESCRIPTOR;


// ******************************************************************
// Section: USB Device Qualifier Descriptor Structure
// ******************************************************************
// This structure describes the device qualifier descriptor.  The device
// qualifier descriptor provides overall device information if the device
// supports "other" speeds.
//
// Note: A high-speed device may support "other" speeds (ie. full or low).
// If so, it may need to implement the the device qualifier and other
// speed descriptors.

typedef struct __attribute__ ((packed)) _USB_DEVICE_QUALIFIER_DESCRIPTOR
{
    BYTE bLength;               // Size of this descriptor
    BYTE bType;                 // Type, always USB_DESCRIPTOR_DEVICE_QUALIFIER
    WORD bcdUSB;                // USB spec version, in BCD
    BYTE bDeviceClass;          // Device class code
    BYTE bDeviceSubClass;       // Device sub-class code
    BYTE bDeviceProtocol;       // Device protocol
    BYTE bMaxPacketSize0;       // EP0, max packet size
    BYTE bNumConfigurations;    // Number of "other-speed" configurations
    BYTE bReserved;             // Always zero (0)

} USB_DEVICE_QUALIFIER_DESCRIPTOR;

// ******************************************************************
// Section: USB Setup Packet Structure
// ******************************************************************
// This structure describes the data contained in a USB standard device
// request's setup packet.  It is the data packet sent from the host to
// the device to control and configure the device.
//
// Note: Refer to the USB 2.0 specification for additional details on the
// usage of the setup packet and standard device requests.
typedef union __attribute__ ((packed))
{
    /** Standard Device Requests ***********************************/
    struct __attribute__ ((packed))
    {
        BYTE bmRequestType; //from table 9-2 of USB2.0 spec
        BYTE bRequest; //from table 9-2 of USB2.0 spec
        WORD wValue; //from table 9-2 of USB2.0 spec
        WORD wIndex; //from table 9-2 of USB2.0 spec
        WORD wLength; //from table 9-2 of USB2.0 spec
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        WORD_VAL W_Value; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        WORD_VAL W_Index; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        WORD_VAL W_Length; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
    };
    struct __attribute__ ((packed))
    {
        unsigned Recipient:5;   //Device,Interface,Endpoint,Other
        unsigned RequestType:2; //Standard,Class,Vendor,Reserved
        unsigned DataDir:1;     //Host-to-device,Device-to-host
        unsigned :8;
        BYTE bFeature;          //DEVICE_REMOTE_WAKEUP,ENDPOINT_HALT
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        union                           // offset   description
        {                               // ------   ------------------------
            BYTE bmRequestType;         //   0      Bit-map of request type
            struct
            {
                BYTE    recipient:  5;  //          Recipient of the request
                BYTE    type:       2;  //          Type of request
                BYTE    direction:  1;  //          Direction of data X-fer
            };
        }requestInfo;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        BYTE bDscIndex;         //For Configuration and String DSC Only
        BYTE bDescriptorType;          //Device,Configuration,String
        WORD wLangID;           //Language ID
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        BYTE_VAL bDevADR;       //Device Address 0-127
        BYTE bDevADRH;          //Must equal zero
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        BYTE bConfigurationValue;         //Configuration Value 0-255
        BYTE bCfgRSD;           //Must equal zero (Reserved)
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        BYTE bAltID;            //Alternate Setting Value 0-255
        BYTE bAltID_H;          //Must equal zero
        BYTE bIntfID;           //Interface Number Value 0-255
        BYTE bIntfID_H;         //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        BYTE bEPID;             //Endpoint ID (Number & Direction)
        BYTE bEPID_H;           //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned EPNum:4;       //Endpoint Number 0-15
        unsigned :3;
        unsigned EPDir:1;       //Endpoint Direction: 0-OUT, 1-IN
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };

    /** End: Standard Device Requests ******************************/

} CTRL_TRF_SETUP, SETUP_PKT, *PSETUP_PKT;


// ******************************************************************
// ******************************************************************
// Section: USB Specification Constants
// ******************************************************************
// ******************************************************************

// Section: Valid PID Values
//DOM-IGNORE-BEGIN
#define PID_OUT                                 0x1     // PID for an OUT token
#define PID_ACK                                 0x2     // PID for an ACK handshake
#define PID_DATA0                               0x3     // PID for DATA0 data
#define PID_PING                                0x4     // Special PID PING
#define PID_SOF                                 0x5     // PID for a SOF token
#define PID_NYET                                0x6     // PID for a NYET handshake
#define PID_DATA2                               0x7     // PID for DATA2 data
#define PID_SPLIT                               0x8     // Special PID SPLIT
#define PID_IN                                  0x9     // PID for a IN token
#define PID_NAK                                 0xA     // PID for a NAK handshake
#define PID_DATA1                               0xB     // PID for DATA1 data
#define PID_PRE                                 0xC     // Special PID PRE (Same as PID_ERR)
#define PID_ERR                                 0xC     // Special PID ERR (Same as PID_PRE)
#define PID_SETUP                               0xD     // PID for a SETUP token
#define PID_STALL                               0xE     // PID for a STALL handshake
#define PID_MDATA                               0xF     // PID for MDATA data

#define PID_MASK_DATA                           0x03    // Data PID mask
#define PID_MASK_DATA_SHIFTED                  (PID_MASK_DATA << 2) // Data PID shift to proper position
//DOM-IGNORE-END

// Section: USB Token Types
//DOM-IGNORE-BEGIN
#define USB_TOKEN_OUT                           0x01    // U1TOK - OUT token
#define USB_TOKEN_IN                            0x09    // U1TOK - IN token
#define USB_TOKEN_SETUP                         0x0D    // U1TOK - SETUP token
//DOM-IGNORE-END

// Section: OTG Descriptor Constants

#define OTG_HNP_SUPPORT                         0x02    // OTG Descriptor bmAttributes - HNP support flag
#define OTG_SRP_SUPPORT                         0x01    // OTG Descriptor bmAttributes - SRP support flag

// Section: Endpoint Directions

#define USB_IN_EP                               0x80    // IN endpoint mask
#define USB_OUT_EP                              0x00    // OUT endpoint mask

// Section: Standard Device Requests

#define USB_REQUEST_GET_STATUS                  0       // Standard Device Request - GET STATUS
#define USB_REQUEST_CLEAR_FEATURE               1       // Standard Device Request - CLEAR FEATURE
#define USB_REQUEST_SET_FEATURE                 3       // Standard Device Request - SET FEATURE
#define USB_REQUEST_SET_ADDRESS                 5       // Standard Device Request - SET ADDRESS
#define USB_REQUEST_GET_DESCRIPTOR              6       // Standard Device Request - GET DESCRIPTOR
#define USB_REQUEST_SET_DESCRIPTOR              7       // Standard Device Request - SET DESCRIPTOR
#define USB_REQUEST_GET_CONFIGURATION           8       // Standard Device Request - GET CONFIGURATION
#define USB_REQUEST_SET_CONFIGURATION           9       // Standard Device Request - SET CONFIGURATION
#define USB_REQUEST_GET_INTERFACE               10      // Standard Device Request - GET INTERFACE
#define USB_REQUEST_SET_INTERFACE               11      // Standard Device Request - SET INTERFACE
#define USB_REQUEST_SYNCH_FRAME                 12      // Standard Device Request - SYNCH FRAME

#define USB_FEATURE_ENDPOINT_HALT               0       // CLEAR/SET FEATURE - Endpoint Halt
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP        1       // CLEAR/SET FEATURE - Device remote wake-up
#define USB_FEATURE_TEST_MODE                   2       // CLEAR/SET FEATURE - Test mode

// Section: Setup Data Constants

#define USB_SETUP_HOST_TO_DEVICE                0x00    // Device Request bmRequestType transfer direction - host to device transfer
#define USB_SETUP_DEVICE_TO_HOST                0x80    // Device Request bmRequestType transfer direction - device to host transfer
#define USB_SETUP_TYPE_STANDARD                 0x00    // Device Request bmRequestType type - standard
#define USB_SETUP_TYPE_CLASS                    0x20    // Device Request bmRequestType type - class
#define USB_SETUP_TYPE_VENDOR                   0x40    // Device Request bmRequestType type - vendor
#define USB_SETUP_RECIPIENT_DEVICE              0x00    // Device Request bmRequestType recipient - device
#define USB_SETUP_RECIPIENT_INTERFACE           0x01    // Device Request bmRequestType recipient - interface
#define USB_SETUP_RECIPIENT_ENDPOINT            0x02    // Device Request bmRequestType recipient - endpoint
#define USB_SETUP_RECIPIENT_OTHER               0x03    // Device Request bmRequestType recipient - other

#define USB_SETUP_HOST_TO_DEVICE_BITFIELD       (USB_SETUP_HOST_TO_DEVICE>>7)       // Device Request bmRequestType transfer direction - host to device transfer - bit definition
#define USB_SETUP_DEVICE_TO_HOST_BITFIELD       (USB_SETUP_DEVICE_TO_HOST>>7)       // Device Request bmRequestType transfer direction - device to host transfer - bit definition
#define USB_SETUP_TYPE_STANDARD_BITFIELD        (USB_SETUP_TYPE_STANDARD>>5)        // Device Request bmRequestType type - standard
#define USB_SETUP_TYPE_CLASS_BITFIELD           (USB_SETUP_TYPE_CLASS>>5)           // Device Request bmRequestType type - class
#define USB_SETUP_TYPE_VENDOR_BITFIELD          (USB_SETUP_TYPE_VENDOR>>5)          // Device Request bmRequestType type - vendor
#define USB_SETUP_RECIPIENT_DEVICE_BITFIELD     (USB_SETUP_RECIPIENT_DEVICE)        // Device Request bmRequestType recipient - device
#define USB_SETUP_RECIPIENT_INTERFACE_BITFIELD  (USB_SETUP_RECIPIENT_INTERFACE)     // Device Request bmRequestType recipient - interface
#define USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD   (USB_SETUP_RECIPIENT_ENDPOINT)      // Device Request bmRequestType recipient - endpoint
#define USB_SETUP_RECIPIENT_OTHER_BITFIELD      (USB_SETUP_RECIPIENT_OTHER)         // Device Request bmRequestType recipient - other

// Section: OTG SET FEATURE Constants

#define OTG_FEATURE_B_HNP_ENABLE                3       // SET FEATURE OTG - Enable B device to perform HNP
#define OTG_FEATURE_A_HNP_SUPPORT               4       // SET FEATURE OTG - A device supports HNP
#define OTG_FEATURE_A_ALT_HNP_SUPPORT           5       // SET FEATURE OTG - Another port on the A device supports HNP

// Section: USB Endpoint Transfer Types

#define USB_TRANSFER_TYPE_CONTROL               0x00    // Endpoint is a control endpoint.
#define USB_TRANSFER_TYPE_ISOCHRONOUS           0x01    // Endpoint is an isochronous endpoint.
#define USB_TRANSFER_TYPE_BULK                  0x02    // Endpoint is a bulk endpoint.
#define USB_TRANSFER_TYPE_INTERRUPT             0x03    // Endpoint is an interrupt endpoint.

// Section: Standard Feature Selectors for CLEAR_FEATURE Requests
#define USB_FEATURE_ENDPOINT_STALL              0       // Endpoint recipient
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP        1       // Device recipient
#define USB_FEATURE_TEST_MODE                   2       // Device recipient


// Section: USB Class Code Definitions
#define USB_HUB_CLASSCODE                       0x09    //  Class code for a hub.

/********************************************************************
USB Endpoint Definitions
USB Standard EP Address Format: DIR:X:X:X:EP3:EP2:EP1:EP0
This is used in the descriptors. 
********************************************************************/
#define _EP_IN      0x80
#define _EP_OUT     0x00
#define _EP01_OUT   0x01
#define _EP01_IN    0x81
#define _EP02_OUT   0x02
#define _EP02_IN    0x82
#define _EP03_OUT   0x03
#define _EP03_IN    0x83
#define _EP04_OUT   0x04
#define _EP04_IN    0x84
#define _EP05_OUT   0x05
#define _EP05_IN    0x85
#define _EP06_OUT   0x06
#define _EP06_IN    0x86
#define _EP07_OUT   0x07
#define _EP07_IN    0x87
#define _EP08_OUT   0x08
#define _EP08_IN    0x88
#define _EP09_OUT   0x09
#define _EP09_IN    0x89
#define _EP10_OUT   0x0A
#define _EP10_IN    0x8A
#define _EP11_OUT   0x0B
#define _EP11_IN    0x8B
#define _EP12_OUT   0x0C
#define _EP12_IN    0x8C
#define _EP13_OUT   0x0D
#define _EP13_IN    0x8D
#define _EP14_OUT   0x0E
#define _EP14_IN    0x8E
#define _EP15_OUT   0x0F
#define _EP15_IN    0x8F

/* Configuration Attributes */
#define _DEFAULT    (0x01<<7)       //Default Value (Bit 7 is set)
#define _SELF       (0x01<<6)       //Self-powered (Supports if set)
#define _RWU        (0x01<<5)       //Remote Wakeup (Supports if set)
#define _HNP	    (0x01 << 1)     //HNP (Supports if set)
#define _SRP	  	(0x01)		    //SRP (Supports if set)

/* Endpoint Transfer Type */
#define _CTRL       0x00            //Control Transfer
#define _ISO        0x01            //Isochronous Transfer
#define _BULK       0x02            //Bulk Transfer

#define _INTERRUPT        0x03            //Interrupt Transfer
#if defined(__18CXX) || defined(__C30__)
    #define _INT        0x03            //Interrupt Transfer
#endif

/* Isochronous Endpoint Synchronization Type */
#define _NS         (0x00<<2)       //No Synchronization
#define _AS         (0x01<<2)       //Asynchronous
#define _AD         (0x02<<2)       //Adaptive
#define _SY         (0x03<<2)       //Synchronous

/* Isochronous Endpoint Usage Type */
#define _DE         (0x00<<4)       //Data endpoint
#define _FE         (0x01<<4)       //Feedback endpoint
#define _IE         (0x02<<4)       //Implicit feedback Data endpoint

//These are the directional indicators used for the USBTransferOnePacket()
//  function.
#define OUT_FROM_HOST 0
#define IN_TO_HOST 1

#endif  // _USB_CH9_H_
/*************************************************************************
 * EOF
 */

