/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.3
 *
 *********************************************************************
 * FileName:        usb9.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.11+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original.
 * Rawin Rojvanit       01/04/08    Bug fixes.
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "io_cfg.h"                     // Required for self_power status

/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void USBStdGetDscHandler(void);
void USBStdSetCfgHandler(void);
void USBStdGetStatusHandler(void);
void USBStdFeatureReqHandler(void);

/** D E C L A R A T I O N S **************************************************/
#pragma code
/******************************************************************************
 * Function:        void USBCheckStdRequest(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine checks the setup data packet to see if it
 *                  knows how to handle it
 *
 * Note:            None
 *****************************************************************************/
void USBCheckStdRequest(void)
{   
    if(SetupPkt.RequestType != STANDARD) return;
    
    switch(SetupPkt.bRequest)
    {
        case SET_ADR:
            ctrl_trf_session_owner = MUID_USB9;
            usb_device_state = ADR_PENDING_STATE;       // Update state only
            /* See USBCtrlTrfInHandler() in usbctrltrf.c for the next step */
            break;
        case GET_DSC:
            USBStdGetDscHandler();
            break;
        case SET_CFG:
            USBStdSetCfgHandler();
            break;
        case GET_CFG:
            ctrl_trf_session_owner = MUID_USB9;
            pSrc.bRam = (byte*)&usb_active_cfg;         // Set Source
            usb_stat.ctrl_trf_mem = _RAM;               // Set memory type
            LSB(wCount) = 1;                            // Set data count
            break;
        case GET_STATUS:
            USBStdGetStatusHandler();
            break;
        case CLR_FEATURE:
        case SET_FEATURE:
            USBStdFeatureReqHandler();
            break;
        case GET_INTF:
            ctrl_trf_session_owner = MUID_USB9;
            pSrc.bRam = (byte*)&usb_alt_intf+SetupPkt.bIntfID;  // Set source
            usb_stat.ctrl_trf_mem = _RAM;               // Set memory type
            LSB(wCount) = 1;                            // Set data count
            break;
        case SET_INTF:
            ctrl_trf_session_owner = MUID_USB9;
            usb_alt_intf[SetupPkt.bIntfID] = SetupPkt.bAltID;
            break;
        case SET_DSC:
        case SYNCH_FRAME:
        default:
            break;
    }//end switch
    
}//end USBCheckStdRequest

/******************************************************************************
 * Function:        void USBStdGetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles the standard GET_DESCRIPTOR request.
 *                  It utilizes tables dynamically looks up descriptor size.
 *                  This routine should never have to be modified if the tables
 *                  in usbdsc.c are declared correctly.
 *
 * Note:            None
 *****************************************************************************/
void USBStdGetDscHandler(void)
{
    if(SetupPkt.bmRequestType == 0x80)
    {
        switch(SetupPkt.bDscType)
        {
            case DSC_DEV:
                ctrl_trf_session_owner = MUID_USB9;
                pSrc.bRom = (rom byte*)&device_dsc;
                wCount._word = sizeof(device_dsc);          // Set data count
                break;
            case DSC_CFG:
                ctrl_trf_session_owner = MUID_USB9;
                pSrc.bRom = *(USB_CD_Ptr+SetupPkt.bDscIndex);
                wCount._word = *(pSrc.wRom+1);              // Set data count
                break;
            case DSC_STR:
                ctrl_trf_session_owner = MUID_USB9;
                pSrc.bRom = *(USB_SD_Ptr+SetupPkt.bDscIndex);
                wCount._word = *pSrc.bRom;                  // Set data count
                break;
        }//end switch
        
        usb_stat.ctrl_trf_mem = _ROM;                       // Set memory type
    }//end if
}//end USBStdGetDscHandler

/******************************************************************************
 * Function:        void USBStdSetCfgHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine first disables all endpoints by clearing
 *                  UEP registers. It then configures (initializes) endpoints
 *                  specified in the modifiable section.
 *
 * Note:            None
 *****************************************************************************/
void USBStdSetCfgHandler(void)
{
    ctrl_trf_session_owner = MUID_USB9;
    mDisableEP1to15();                          // See usbdrv.h
    ClearArray((byte*)&usb_alt_intf,MAX_NUM_INT);
    usb_active_cfg = SetupPkt.bCfgValue;
    if(SetupPkt.bCfgValue == 0)
        usb_device_state = ADDRESS_STATE;
    else
    {
        usb_device_state = CONFIGURED_STATE;

        /* Modifiable Section */
        
        PyUSBInitEP();
        
        /* End modifiable section */

    }//end if(SetupPkt.bcfgValue == 0)
}//end USBStdSetCfgHandler

/******************************************************************************
 * Function:        void USBStdGetStatusHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles the standard GET_STATUS request
 *
 * Note:            None
 *****************************************************************************/
void USBStdGetStatusHandler(void)
{
    CtrlTrfData._byte0 = 0;                         // Initialize content
    CtrlTrfData._byte1 = 0;
        
    switch(SetupPkt.Recipient)
    {
        case RCPT_DEV:
            ctrl_trf_session_owner = MUID_USB9;
            /*
             * _byte0: bit0: Self-Powered Status [0] Bus-Powered [1] Self-Powered
             *         bit1: RemoteWakeup        [0] Disabled    [1] Enabled
             */

/********************************************************************
Bug Fix: January 4, 2008
*********************************************************************
Removed an extra '0' from the code:
CtrlTrfData._byte0|=0b00000001;
********************************************************************/
            if(self_power == 1)                     // self_power defined in io_cfg.h
                CtrlTrfData._byte0|=0b00000001;     // Set bit0
/*******************************************************************/

            if(usb_stat.RemoteWakeup == 1)          // usb_stat defined in usbmmap.c
                CtrlTrfData._byte0|=0b00000010;     // Set bit1
            break;
        case RCPT_INTF:
            ctrl_trf_session_owner = MUID_USB9;     // No data to update
            break;
        case RCPT_EP:
            ctrl_trf_session_owner = MUID_USB9;
            /*
             * _byte0: bit0: Halt Status [0] Not Halted [1] Halted
             */
            pDst.bRam = (byte*)&ep0Bo+(SetupPkt.EPNum*8)+(SetupPkt.EPDir*4);
            if(*pDst.bRam & _BSTALL)    // Use _BSTALL as a bit mask
                CtrlTrfData._byte0=0x01;// Set bit0
            break;
    }//end switch
    
    if(ctrl_trf_session_owner == MUID_USB9)
    {
        pSrc.bRam = (byte*)&CtrlTrfData;            // Set Source
        usb_stat.ctrl_trf_mem = _RAM;               // Set memory type
        LSB(wCount) = 2;                            // Set data count
    }//end if(...)
}//end USBStdGetStatusHandler

/******************************************************************************
 * Function:        void USBStdFeatureReqHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles the standard SET & CLEAR FEATURES
 *                  requests
 *
 * Note:            None
 *****************************************************************************/
void USBStdFeatureReqHandler(void)
{
    if((SetupPkt.bFeature == DEVICE_REMOTE_WAKEUP)&&
       (SetupPkt.Recipient == RCPT_DEV))
    {
        ctrl_trf_session_owner = MUID_USB9;
        if(SetupPkt.bRequest == SET_FEATURE)
            usb_stat.RemoteWakeup = 1;
        else
            usb_stat.RemoteWakeup = 0;
    }//end if
    
    if((SetupPkt.bFeature == ENDPOINT_HALT)&&
       (SetupPkt.Recipient == RCPT_EP)&&
       (SetupPkt.EPNum != 0))
    {
        ctrl_trf_session_owner = MUID_USB9;
        /* Must do address calculation here */
        pDst.bRam = (byte*)&ep0Bo+(SetupPkt.EPNum*8)+(SetupPkt.EPDir*4);
        
        if(SetupPkt.bRequest == SET_FEATURE)
            *pDst.bRam = _USIE|_BSTALL;
        else
        {
/********************************************************************
Bug Fix: January 4, 2008
*********************************************************************
Added '|DAT1' to force a halted IN endpoint to reinitialize to send
out DAT0 on the next transaction.
********************************************************************/
            if(SetupPkt.EPDir == 1) // IN
                *pDst.bRam = _UCPU|_DAT1;
            else
                *pDst.bRam = _USIE|_DAT0|_DTSEN;
/*******************************************************************/
        }//end if
    }//end if
}//end USBStdFeatureReqHandler

/** EOF usb9.c ***************************************************************/
