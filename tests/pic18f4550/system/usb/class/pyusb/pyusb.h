/*********************************************************************
 *
 *             Microchip USB C18 Firmware -  Generic
 *
 *********************************************************************
 * FileName:        usbgen.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
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
 ********************************************************************/
#ifndef USBGEN_H
#define USBGEN_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/

/******************************************************************************
 * Macro:           (bit) mUSBGenRxIsBusy(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This macro is used to check if the OUT endpoint is
 *                  busy (owned by SIE) or not.
 *                  Typical Usage: if(mUSBGenRxIsBusy())
 *
 * Note:            None
 *****************************************************************************/
#define mBulkRxIsBusy()          BULK_BD_OUT.Stat.UOWN
#define mIntrRxIsBusy()          INTR_BD_OUT.Stat.UOWN
#define mIsoRxIsBusy()           ISO_BD_OUT.Stat.UOWN

/******************************************************************************
 * Macro:           (bit) mUSBGenTxIsBusy(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This macro is used to check if the IN endpoint is
 *                  busy (owned by SIE) or not.
 *                  Typical Usage: if(mUSBGenTxIsBusy())
 *
 * Note:            None
 *****************************************************************************/
#define mBulkTxIsBusy()          BULK_BD_IN.Stat.UOWN
#define mIntrTxIsBusy()          INTR_BD_IN.Stat.UOWN
#define mIsoTxIsBusy()           ISO_BD_IN.Stat.UOWN

/******************************************************************************
 * Macro:           byte mUSBGenGetRxLength(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          mUSBGenGetRxLength returns usbgen_rx_len
 *
 * Side Effects:    None
 *
 * Overview:        mUSBGenGetRxLength is used to retrieve the number of bytes
 *                  copied to user's buffer by the most recent call to
 *                  USBGenRead function.
 *
 * Note:            None
 *****************************************************************************/
//#define mUSBGenGetRxLength()        usbgen_rx_len

/** S T R U C T U R E S ******************************************************/

/** E X T E R N S ************************************************************/
//extern byte usbgen_rx_len;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void BulkWrite(void);
void BulkRead(void);
void IntrWrite(void);
void IntrRead(void);
void PyUSBInitEP(void);
void PyUSBCheckRequest(void);

#endif //USBGEN_H
