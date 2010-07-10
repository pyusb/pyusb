//DOM-IGNORE-BEGIN
/*********************************************************************
 * The following lines are used by VDI.
 * GUID=E537A0C0-6FEE-4afd-89B9-0C35BF72A80B
 * GUIInterfaceVersion=1.00
 * LibraryVersion=2.4
 *********************************************************************/
//DOM-IGNORE-END
/*******************************************************************************

    USB Header File

Summary:
    This file aggregates all necessary header files for the Microchip USB Host,
    Device, and OTG libraries.  It provides a single-file can be included in
    application code.  The USB libraries simplify the implementation of USB
    applications by providing an abstraction of the USB module and its registers
    and bits such that the source code for the can be the same across various
    hardware platforms.

Description:
    This file aggregates all necessary header files for the Microchip USB Host,
    Device, and OTG libraries.  It provides a single-file can be included in
    application code.  The USB libraries simplify the implementation of USB
    applications by providing an abstraction of the USB module and its registers
    and bits such that the source code for the can be the same across various
    hardware platforms.
    
    Note that this file does not include the header files for any client or
    function drivers.
    
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

******************************************************************************/
/******************************************************************************
 FileName:     usb.h
 Dependencies: See INCLUDES section
 Processor:    PIC18, PIC24, & PIC32 USB Microcontrollers
 Hardware:
 Complier:     Microchip C18 (for PIC18), C30 (for PIC24), or C32 (for PIC32)
 Company:      Microchip Technology, Inc.

 Software License Agreement:

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
********************************************************************/
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
/********************************************************************
 File Description:

 Change History:
  Rev    Description
  ----   -----------
  2.6    Changed MCHPFSUSB stack revision number
********************************************************************/
//DOM-IGNORE-END

#ifndef _USB_H_
#define _USB_H_
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: All necessary USB Library headers
// *****************************************************************************
// *****************************************************************************

#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "usb_config.h"             // Must be defined by the application

#include "usb/usb_common.h"         // Common USB library definitions
#include "usb/usb_ch9.h"            // USB device framework definitions

#if defined( USB_SUPPORT_DEVICE )
    #include "usb/usb_device.h"     // USB Device abstraction layer interface
#endif

#if defined( USB_SUPPORT_HOST )
    #include "usb/usb_host.h"       // USB Host abstraction layer interface
#endif

#if defined ( USB_SUPPORT_OTG )
    #include "usb/usb_otg.h" 
#endif

#include "usb/usb_hal.h"            // Hardware Abstraction Layer interface

// *****************************************************************************
// *****************************************************************************
// Section: MCHPFSUSB Firmware Version
// *****************************************************************************
// *****************************************************************************

#define USB_MAJOR_VER   2       // Firmware version, major release number.
#define USB_MINOR_VER   6       // Firmware version, minor release number.
#define USB_DOT_VER     0       // Firmware version, dot release number.

#endif // _USB_H_
/*************************************************************************
 * EOF
 */

