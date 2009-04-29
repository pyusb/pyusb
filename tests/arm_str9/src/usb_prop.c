/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_prop.c
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
#include "91x_lib.h"
#include "USB_lib.h"
#include "USB_conf.h"
#include "USB_prop.h"
#include "USB_desc.h"
#include "USB_pwr.h"
#include "util.h"
#include "ring_buffer.h"

extern void USB_Istr(void);
extern void CTR_ISR(void);

extern ring_buffer_t rb_bulk;
extern ring_buffer_t rb_interrupt;
extern ring_buffer_t rb_isochronous;

extern rb_data_t bulk_buffer[RB_BUFFER_SIZE];
extern rb_data_t interrupt_buffer[RB_BUFFER_SIZE];
extern rb_data_t isochronous_buffer[RB_BUFFER_SIZE];

static u8 control_buffer[0x40];
static u16 control_buffer_len;

static u8 *control_write(u16 length);
static u8 *control_read(u16 length);

/* a device specific control request */
#define LOOPBACK_REQUEST    0x01

/*  Structures initializations */
DEVICE Device_Table = {
/*	ENDP0, */
	EP_NUM,
	1
};

DEVICE_PROP Device_Property = {
	PYUSB_init,
	PYUSB_Reset,
	PYUSB_Status_In,
	PYUSB_Status_Out,
	PYUSB_Data_Setup,
	PYUSB_NoData_Setup,
	PYUSB_Get_Interface_Setting,
	PYUSB_GetDeviceDescriptor,
	PYUSB_GetConfigDescriptor,
	PYUSB_GetStringDescriptor,
	/*PYUSB_EP0Buffer*/0,
	0x40
};

static void init_endpoints(void)
{
	/* Initialize Endpoint 1 */
	SetEPType(ENDP1, EP_BULK);
	SetEPTxStatus(ENDP1, EP_TX_VALID);
	SetEPRxStatus(ENDP1, EP_RX_VALID);
    SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPRxAddr(ENDP1, ENDP1_RXADDR);
    SetEPTxCount(ENDP1, 0);
	SetEPRxCount(ENDP1, 0x40);
    ClearDTOG_RX(ENDP1);
    ClearDTOG_TX(ENDP1);

	/* Initialize Endpoint 2 */
	SetEPType(ENDP2, EP_INTERRUPT);
	SetEPTxStatus(ENDP2, EP_TX_VALID);
	SetEPRxStatus(ENDP2, EP_RX_VALID);
    SetEPTxAddr(ENDP2, ENDP2_TXADDR);
	SetEPRxAddr(ENDP2, ENDP2_RXADDR);
    SetEPTxCount(ENDP2, 0);
	SetEPRxCount(ENDP2, 0x40);
    ClearDTOG_RX(ENDP2);
    ClearDTOG_TX(ENDP2);

	/* Initialize Endpoint 3 */
    SetEPType(ENDP3, EP_ISOCHRONOUS);
    SetEPDblBuffAddr(ENDP3,ENDP3_BUFF0ADDR,ENDP3_BUFF1ADDR);
    SetEPDblBuffCount(ENDP3, EP_DBUF_OUT, 0x40);
    ClearDTOG_RX(ENDP3);
    ClearDTOG_TX(ENDP3);
    ToggleDTOG_TX(ENDP3);
    SetEPRxStatus(ENDP3, EP_RX_VALID);
    SetEPTxStatus(ENDP3, EP_TX_DIS);

    /* Initialize Endpoint 4 */
    SetEPType(ENDP4, EP_ISOCHRONOUS);
    SetEPDblBuffAddr(ENDP4,ENDP4_BUFF0ADDR,ENDP4_BUFF1ADDR);
    SetEPDblBuffCount(ENDP4, EP_DBUF_IN,0);
    ClearDTOG_RX(ENDP4);
    ToggleDTOG_RX(ENDP4); 
    ClearDTOG_TX(ENDP4);  
    SetEPTxStatus(ENDP4, EP_TX_VALID);
    SetEPRxStatus(ENDP4, EP_RX_DIS);


    /* initialize endpoints circular buffers */
    rb_init(&rb_bulk, bulk_buffer, RB_BUFFER_SIZE);
    rb_init(&rb_interrupt, interrupt_buffer, RB_BUFFER_SIZE);
    rb_init(&rb_isochronous, isochronous_buffer, RB_BUFFER_SIZE);
}


void PYUSB_init()
{
	DEVICE_INFO *pInfo = &Device_Info;
	pInfo->Current_Configuration = 0;

    /* connect interrupt line */
	vic_connect(USBLP_ITLine, USB_Istr);
	vic_connect(USBHP_ITLine, CTR_ISR);

	/* Connect the device */
	PowerOn();
	/* USB interrupts initialization */
	_SetISTR(0);               /* clear pending interrupts */
	wInterrupt_Mask = IMR_MSK;
	_SetCNTR(wInterrupt_Mask); /* set interrupts mask */
  pInfo->Current_Feature = 0x00; /* */
  /* Wait until device is configured */
  while (pInfo->Current_Configuration == 0) NOP_Process();
  bDeviceState = CONFIGURED;
} /* PYUSB_init() */


void PYUSB_Reset()
{
	/* Set PYUSB_DEVICE as not configured */
	Device_Info.Current_Configuration = 0;
	/*correction AS default interface*/
	Device_Info.Current_Interface = 0;/*the default Interface*/
	SetBTABLE(BTABLE_ADDRESS);
	/* Initialize Endpoint 0 */
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_STALL/*EP_TX_NAK*/);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPRxCount(ENDP0,0x40/*STD_MAXPACKETSIZE*/);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	SetEPTxCount(ENDP0,0x40 /* STD_MAXPACKETSIZE*/);
	SetEPAddress(ENDP0,0);
	Clear_Status_Out(ENDP0);
	SetEPRxValid(ENDP0);

    control_buffer_len = 0;

    init_endpoints();

	/* Set this device to response on default address */
	SetDeviceAddress(0);

} /* PYUSB_Reset() */


RESULT PYUSB_Data_Setup(BYTE RequestNo)
{
    u8 *(*CopyRoutine) (u16);

    /* ST USB Library swaps the bytes of wIndex and wValue. Don't ask me why... */
    if (LOOPBACK_REQUEST == RequestNo && 0x00FF == pInformation->USBwIndex && 0xFF00 == pInformation->USBwValue) {

        if (pInformation->USBbmRequestType & 0x80) {

            CopyRoutine = control_read;

        } else {

            CopyRoutine = control_write;
        }

        pInformation->Ctrl_Info.CopyData = CopyRoutine;
        (*CopyRoutine)(0);

        return USB_SUCCESS;
    }

	return UNSUPPORT;
} /* PYUSB_Data_Setup */

RESULT PYUSB_NoData_Setup(BYTE RequestNo)
{
	return UNSUPPORT;
} /* PYUSB_NoData_Setup */


ONE_DESCRIPTOR Device_Descriptor = {
	(BYTE*)PYUSB_DeviceDescriptor,
	PYUSB_SIZ_DEVICE_DESC
};

BYTE *PYUSB_GetDeviceDescriptor(WORD Length)
{
	return Standard_GetDescriptorData( Length, &Device_Descriptor );
}

ONE_DESCRIPTOR Config_Descriptor = {
	(BYTE*)PYUSB_ConfigDescriptor,
	PYUSB_SIZ_CONFIG_DESC
};

BYTE *PYUSB_GetConfigDescriptor(WORD Length)
{
	return Standard_GetDescriptorData( Length, &Config_Descriptor );
}

ONE_DESCRIPTOR String_Descriptor = {
	(BYTE*)PYUSB_StringDescriptor,
	PYUSB_SIZ_STRING_DESC
};

BYTE *PYUSB_GetStringDescriptor(WORD Length)
{
	return Standard_GetStringDescriptor( Length, &String_Descriptor );
}


RESULT PYUSB_Get_Interface_Setting(BYTE Interface,BYTE AlternateSetting)
{
	if(AlternateSetting >1) return UNSUPPORT;
	else if(Interface > 0) return UNSUPPORT;  /*in this application we have only 1 interfaces*/
    /* actually, just reseting the data toggle of each endpoint
       shoud be enough, but we initialize them again just to... be sure... ;-) */
    init_endpoints();
	return USB_SUCCESS;
}

/*
 * Support for loopback in the endpoint 0
 * Pay attention to the fact that this loopback endpoint is
 * more limited than the loopback of others endpoints.
 * If you write two packets of data, only the second will be
 * returned in a read.
 */

static u8 *control_write(u16 length)
{
    if (0 == length) {

        pInformation->Ctrl_Info.Usb_wLength = sizeof(control_buffer);
        return NULL;
    }

    control_buffer_len = length;

    return control_buffer;
}

static u8 *control_read(u16 length)
{
    if (0 == length) {

        pInformation->Ctrl_Info.Usb_wLength = control_buffer_len;
        return NULL;
    }

    control_buffer_len = 0;

    return control_buffer;
}
