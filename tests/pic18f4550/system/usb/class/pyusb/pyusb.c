/*********************************************************************
 *
 *             USB C18 Firmware -  PyUSB
 *
 *********************************************************************
 * FileName:        pyusb.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:
 *
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"


/** V A R I A B L E S ********************************************************/
#pragma udata
byte bulk_len = 0;
byte intr_len = 0;
byte iso_len = 0;
byte ctrl_len = 0;

byte bulk_buffer[PYUSB_EP_SIZE];
byte intr_buffer[PYUSB_EP_SIZE];
byte iso_buffer[PYUSB_EP_SIZE];
byte ctrl_buffer[EP0_BUFF_SIZE];

#define CTRL_LOOPBACK_WRITE 0
#define CTRL_LOOPBACK_READ 1

/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** D E C L A R A T I O N S **************************************************/
#pragma code

/** U S E R  A P I ***********************************************************/

/******************************************************************************
 * Function:        void PyUSBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        PyUSBInitEP initializes generic endpoints, buffer
 *                  descriptors, internal state-machine, and variables.
 *                  It should be called after the USB host has sent out a
 *                  SET_CONFIGURATION request.
 *                  See USBStdSetCfgHandler() in usb9.c for examples.
 *
 * Note:            None
 *****************************************************************************/
void PyUSBInitEP(void)
{   
    bulk_len = 0;
    intr_len = 0;
    iso_len = 0;
    
    // USBGEN_UEP = EP_OUT_IN|HSHK_EN;             // Enable 2 data pipes

	UEP_BULK = EP_OUT_IN|HSHK_EN;
	UEP_INTR = EP_OUT_IN|HSHK_EN;
	UEP_ISO = EP_OUT_IN;

    /*
     * Do not have to init Cnt of IN pipes here.
     * Reason:  Number of bytes to send to the host
     *          varies from one transaction to
     *          another. Cnt should equal the exact
     *          number of bytes to transmit for
     *          a given IN transaction.
     *          This number of bytes will only
     *          be known right before the data is
     *          sent.
     */

	BULK_BD_OUT.Cnt = sizeof(bulk_out);
	BULK_BD_OUT.ADR = (byte*)&bulk_out;
	BULK_BD_OUT.Stat._byte =  _USIE|_DAT0|_DTSEN;

	BULK_BD_IN.ADR = (byte*)&bulk_in;
	BULK_BD_IN.Stat._byte = _UCPU|_DAT1;
	BULK_BD_IN.Cnt = 0;

	INTR_BD_OUT.Cnt = sizeof(intr_out);
	INTR_BD_OUT.ADR = (byte*)&intr_out;
	INTR_BD_OUT.Stat._byte =  _USIE|_DAT0|_DTSEN;

	INTR_BD_IN.ADR = (byte*)&intr_in;
	INTR_BD_IN.Stat._byte = _UCPU|_DAT1;
	INTR_BD_IN.Cnt = 0;

	ISO_BD_OUT.Cnt = sizeof(iso_out);
	ISO_BD_OUT.ADR = (byte*)&iso_out;
	ISO_BD_OUT.Stat._byte =  _USIE|_DAT0|_DTSEN;

	ISO_BD_IN.ADR = (byte*)&iso_in;
	ISO_BD_IN.Stat._byte = _UCPU|_DAT1;
	ISO_BD_IN.Cnt = 0;

}

void BulkWrite(void)
{
	if(!mBulkTxIsBusy() && bulk_len)
	{
		byte i;
		
	    for (i = 0; i < bulk_len; i++)
	    	bulk_in[i] = bulk_buffer[i];
	
	    BULK_BD_IN.Cnt = bulk_len;
		bulk_len = 0;
	    mUSBBufferReady(BULK_BD_IN);
	}
}

void BulkRead(void)
{
	if(!mBulkRxIsBusy() && BULK_BD_OUT.Cnt)
	{
		byte i;

		bulk_len = BULK_BD_OUT.Cnt;

        for(i = 0; i < bulk_len; i++)
            bulk_buffer[i] = bulk_out[i];

        BULK_BD_OUT.Cnt = sizeof(bulk_out);
        mUSBBufferReady(BULK_BD_OUT);
	}
}

void IntrWrite(void)
{
	if(!mIntrTxIsBusy() && intr_len)
	{
		byte i;
		
	    for (i = 0; i < intr_len; i++)
	    	intr_in[i] = intr_buffer[i];
	
	    INTR_BD_IN.Cnt = intr_len;
		intr_len = 0;
	    mUSBBufferReady(INTR_BD_IN);
	}
}

void IntrRead(void)
{
	if(!mIntrRxIsBusy() && INTR_BD_OUT.Cnt)
	{
		byte i;

		intr_len = INTR_BD_OUT.Cnt;

        for(i = 0; i < intr_len; i++)
            intr_buffer[i] = intr_out[i];

        INTR_BD_OUT.Cnt = sizeof(intr_out);
        mUSBBufferReady(INTR_BD_OUT);
	}
}

void PyUSBCheckRequest(void)
{
	if(SetupPkt.RequestType != VENDOR) return;

	switch(SetupPkt.bRequest)
	{
		case CTRL_LOOPBACK_WRITE:
			ctrl_len = SetupPkt.wLength;
			ctrl_trf_session_owner = MUID_PYUSB;
			pDst.bRam = ctrl_buffer;
			break;
		case CTRL_LOOPBACK_READ:
			ctrl_trf_session_owner = MUID_PYUSB;
			pSrc.bRam = ctrl_buffer;
			usb_stat.ctrl_trf_mem = _RAM;
			wCount._word = ctrl_len;
			ctrl_len = 0;
			break;
	}
}

/** EOF pyusb.c *************************************************************/
