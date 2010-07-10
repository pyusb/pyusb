/******************************************************************************
  File Information:
      FileName:       usb_function_generic.h
      Dependencies:   See INCLUDES section
      Processor:      PIC18 or PIC24 USB Microcontrollers
      Hardware:       The code is natively intended to be used on the following
                      hardware platforms: PICDEM™ FS USB Demo Board,
                      PIC18F87J50 FS USB Plug-In Module, or
                      Explorer 16 + PIC24 USB PIM.  The firmware may be
                      modified for use on other USB platforms by editing the
                      HardwareProfile.h file.
      Complier:       Microchip C18 (for PIC18) or C30 (for PIC24)
      Company:        Microchip Technology, Inc.
    
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
    
       Change History:
       Rev         Description
    
  Summary:
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with vendor class function
    drivers. This file should be included in projects that use vendor class
    \function drivers. This file should also be included into the
    usb_descriptors.c file and any other user file that requires access to
    vendor class interfaces.
    
    
    
    This file is located in the "\<Install
    Directory\>\\Microchip\\Include\\USB" directory.
  Description:
    USB Vender Class Custom Driver File
    
    This file contains all of functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with vendor class function
    drivers. This file should be included in projects that use vendor class
    \function drivers. This file should also be included into the
    usb_descriptors.c file and any other user file that requires access to
    vendor class interfaces.
    
    This file is located in the "\<Install
    Directory\>\\Microchip\\Include\\USB" directory.
    
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

//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  2.6    No Change
********************************************************************/
//DOM-IGNORE-END

#ifndef USBGEN_H
#define USBGEN_H

#include "GenericTypeDefs.h"
#include "usb_config.h"

/** I N C L U D E S **********************************************************/

/** D E F I N I T I O N S ****************************************************/

/******************************************************************************
    Macro:
        (bit) mUSBGenRxIsBusy(void)
        
    Description:
        This macro is used to check if the OUT endpoint is
        busy (owned by SIE) or not.
        Typical Usage: if(mUSBGenRxIsBusy())
        
    PreCondition:
        None
        
    Parameters:
        None
        
    Return Values:
        None
        
    Remarks:
        None
        
 *****************************************************************************/

/******************************************************************************
    Macro:
        (bit) mUSBGenTxIsBusy(void)
        
    Description:
        This macro is used to check if the IN endpoint is
        busy (owned by SIE) or not.
        Typical Usage: if(mUSBGenTxIsBusy())
        
    PreCondition:
        None
        
    Parameters:
        None
        
    Return Values:
        None
        
    Remarks:
        None
        
 *****************************************************************************/

/******************************************************************************
    Macro:
        byte mUSBGenGetRxLength(void)
        
    Description:
        mUSBGenGetRxLength is used to retrieve the number of bytes
        copied to user's buffer by the most recent call to
        USBGenRead function.
        
    PreCondition:
        None
        
    Parameters:
        None
        
    Return Values:
        mUSBGenGetRxLength returns usbgen_rx_len
        
    Remarks:
        None
        
 *****************************************************************************/

/** S T R U C T U R E S ******************************************************/

/** E X T E R N S ************************************************************/

/** P U B L I C  P R O T O T Y P E S *****************************************/

/********************************************************************
    Function:
        USB_HANDLE USBGenWrite(BYTE ep, BYTE* data, WORD len)
        
    Summary:
        Sends the specified data out the specified endpoint

    Description:
        This function sends the specified data out the specified 
        endpoint and returns a handle to the transfer information.

        Typical Usage:
        <code>
        //make sure that the last transfer isn't busy by checking the handle
        if(!USBHandleBusy(USBGenericInHandle))
        {
            //Send the data contained in the INPacket[] array out on
            //  endpoint USBGEN_EP_NUM
            USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM,(BYTE*)&INPacket[0],sizeof(INPacket));
        }
        </code>
        
    PreCondition:
        None
        
    Parameters:
        ep - the endpoint you want to send the data out of
        data - pointer to the data that you wish to send
        len - the length of the data that you wish to send
        
    Return Values:
        USB_HANDLE - a handle for the transfer.  This information
        should be kept to track the status of the transfer
        
    Remarks:
        None
  
 *******************************************************************/
#define USBGenWrite(ep,data,len) USBTxOnePacket(ep,data,len)

/********************************************************************
    Function:
        USB_HANDLE USBGenRead(BYTE ep, BYTE* data, WORD len)
        
    Summary:
        Receives the specified data out the specified endpoint
        
    Description:
        Receives the specified data out the specified endpoint.

        Typical Usage:
        <code>
        //Read 64-bytes from endpoint USBGEN_EP_NUM, into the OUTPacket array.
        //  Make sure to save the return handle so that we can check it later
        //  to determine when the transfer is complete.
        if(!USBHandleBusy(USBOutHandle))
        {
            USBOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,64);
        }
        </code>

    PreCondition:
        None
        
    Parameters:
        ep - the endpoint you want to receive the data into
        data - pointer to where the data will go when it arrives
        len - the length of the data that you wish to receive
        
    Return Values:
        USB_HANDLE - a handle for the transfer.  This information
        should be kept to track the status of the transfer
        
    Remarks:
        None
  
 *******************************************************************/
#define USBGenRead(ep,data,len) USBRxOnePacket(ep,data,len)

#endif //USBGEN_H
