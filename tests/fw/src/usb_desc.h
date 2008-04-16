/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_desc.h
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


#define	PYUSB_SIZ_DEVICE_DESC	18
#define	PYUSB_SIZ_CONFIG_DESC	69
#define	PYUSB_SIZ_STRING_DESC	34

extern const BYTE PYUSB_DeviceDescriptor[PYUSB_SIZ_DEVICE_DESC];
extern const BYTE PYUSB_ConfigDescriptor[PYUSB_SIZ_CONFIG_DESC];
extern const BYTE PYUSB_StringDescriptor[PYUSB_SIZ_STRING_DESC];
