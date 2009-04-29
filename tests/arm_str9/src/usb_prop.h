/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : All processings related to PyUSB device
********************************************************************************
* History:
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


void PYUSB_init(void);
void PYUSB_Reset(void);
#define PYUSB_Status_In	NOP_Process
#define PYUSB_Status_Out	NOP_Process
RESULT PYUSB_Data_Setup(BYTE);
RESULT PYUSB_NoData_Setup(BYTE);
RESULT PYUSB_Get_Interface_Setting(BYTE Interface,BYTE AlternateSetting);
BYTE *PYUSB_GetDeviceDescriptor(WORD );
BYTE *PYUSB_GetConfigDescriptor(WORD);
BYTE *PYUSB_GetStringDescriptor(WORD);

