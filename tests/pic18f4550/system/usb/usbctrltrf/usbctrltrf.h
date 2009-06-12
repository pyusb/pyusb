/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.2
 *
 *********************************************************************
 * FileName:        usbctrltrf.h
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
 * Rawin Rojvanit       08/14/07    Bug fixes.
 ********************************************************************/
#ifndef USBCTRLTRF_H
#define USBCTRLTRF_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/

/* Control Transfer States */
#define WAIT_SETUP          0
#define CTRL_TRF_TX         1
#define CTRL_TRF_RX         2

/********************************************************************
Bug Fix: May 14, 2007 (#F7)
*********************************************************************
For a control transfer read, if the host tries to read more data
than what it has requested, the peripheral device should stall the
extra IN transactions and the status stage. Typically, a host does
not try to read more data than what it has requested. The original
firmware did not handle this situation. Instead of stalling extra
IN transactions, the device kept sending out zero length packets.

The new definitions introduced is used to keep track if a short IN
packet has been sent or not. From this the state machine can
decide if it should stall future extra IN transactions or not.
********************************************************************/
/* Short Packet States - Used by Control Transfer Read  - CTRL_TRF_TX */
#define SHORT_PKT_NOT_USED  0
#define SHORT_PKT_PENDING   1
#define SHORT_PKT_SENT      2

/* USB PID: Token Types - See chapter 8 in the USB specification */
#define SETUP_TOKEN         0b00001101
#define OUT_TOKEN           0b00000001
#define IN_TOKEN            0b00001001

/* bmRequestType Definitions */
#define HOST_TO_DEV         0
#define DEV_TO_HOST         1

#define STANDARD            0x00
#define CLASS               0x01
#define VENDOR              0x02

#define RCPT_DEV            0
#define RCPT_INTF           1
#define RCPT_EP             2
#define RCPT_OTH            3

/** E X T E R N S ************************************************************/
extern byte ctrl_trf_session_owner;

extern POINTER pSrc;
extern POINTER pDst;
extern WORD wCount;

/** P U B L I C  P R O T O T Y P E S *****************************************/
byte USBCtrlEPService(void);			// Bug Fix - Work around, void->byte
void USBCtrlTrfTxService(void);
void USBCtrlTrfRxService(void);
void USBCtrlEPServiceComplete(void);
void USBPrepareForNextSetupTrf(void);


#endif //USBCTRLTRF_H
