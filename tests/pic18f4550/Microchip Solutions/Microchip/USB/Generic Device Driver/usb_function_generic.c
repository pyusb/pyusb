/******************************************************************************
  File Information:
          FileName:        usb_function_generic.c
          Dependencies:    See INCLUDES section below
          Processor:       PIC18, PIC24, or PIC32
          Compiler:        C18, C30, or C32
          Company:         Microchip Technology, Inc.
    
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
    
  Summary:
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for use of vendor class function
    drivers. This file should be included in projects that use vendor class
    \function drivers. Vendor class function drivers include MCHPUSB
    (Microchip's custom class driver), WinUSB, and LibUSB.
    
    
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\Generic
    Device Driver" directory.
  Description:
    USB Vender Class Custom Driver Header File
    
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for use of vendor class function
    drivers. This file should be included in projects that use vendor class
    \function drivers.
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB\\Generic
    Device Driver" directory.
    
    When including this file in a new project, this file can either be
    referenced from the directory in which it was installed or copied
    directly into the user application folder. If the first method is
    chosen to keep the file located in the folder in which it is installed
    then include paths need to be added so that the library and the
    application both know where to reference each others files. If the
    application folder is located in the same folder as the Microchip
    folder (like the current demo folders), then the following include
    paths need to be added to the application's project:
    
    ..\\..\\Microchip\\Include
    .
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application                               

//DOM-IGNORE-BEGIN
********************************************************************
 File Description:

 Change History:
  Rev    Description
  ----   -----------
  2.6    Minor changes in include file structure.
********************************************************************
//DOM-IGNORE-END

******************************************************************************/

/** I N C L U D E S **********************************************************/
#include "usb/usb.h"

#if defined(USB_USE_GEN)

/** V A R I A B L E S ********************************************************/

/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** D E C L A R A T I O N S **************************************************/

/** U S E R  A P I ***********************************************************/
//All API are defined in the usb_function_generic.h file

#endif //def USB_USE_GEN

/** EOF usbgen.c *************************************************************/
