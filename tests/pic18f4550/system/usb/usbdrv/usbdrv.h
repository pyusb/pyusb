/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.2
 *
 *********************************************************************
 * FileName:        usbdrv.h
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
 * Rawin Rojvanit		05/14/07    Fixed endpoint definitions
 ********************************************************************/

#ifndef USBDRV_H
#define USBDRV_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"
#include "system\usb\usb.h"

/** D E F I N I T I O N S ****************************************************/

/* UCFG Initialization Parameters */
#define _PPBM0      0x00            // Pingpong Buffer Mode 0
#define _PPBM1      0x01            // Pingpong Buffer Mode 1
#define _PPBM2      0x02            // Pingpong Buffer Mode 2
#define _LS         0x00            // Use Low-Speed USB Mode
#define _FS         0x04            // Use Full-Speed USB Mode
#define _TRINT      0x00            // Use internal transceiver
#define _TREXT      0x08            // Use external transceiver
#define _PUEN       0x10            // Use internal pull-up resistor
#define _OEMON      0x40            // Use SIE output indicator
#define _UTEYE      0x80            // Use Eye-Pattern test

/* UEPn Initialization Parameters */
#define EP_CTRL     0x06            // Cfg Control pipe for this ep
#define EP_OUT      0x0C            // Cfg OUT only pipe for this ep
#define EP_IN       0x0A            // Cfg IN only pipe for this ep
#define EP_OUT_IN   0x0E            // Cfg both OUT & IN pipes for this ep
#define HSHK_EN     0x10            // Enable handshake packet
                                    // Handshake should be disable for isoch

/******************************************************************************
 * USB - PICmicro Endpoint Definitions
 * PICmicro EP Address Format: X:EP3:EP2:EP1:EP0:DIR:PPBI:X
 * This is used when checking the value read from USTAT
 *
 * NOTE: These definitions are not used in the descriptors.
 * EP addresses used in the descriptors have different format and
 * are defined in: "system\usb\usbdefs\usbdefs_std_dsc.h"
 *****************************************************************************/
#define OUT         0
#define IN          1

#define PIC_EP_NUM_MASK 0b01111000
#define PIC_EP_DIR_MASK 0b00000100

#define EP00_OUT    ((0x00<<3)|(OUT<<2))
#define EP00_IN     ((0x00<<3)|(IN<<2))
#define EP01_OUT    ((0x01<<3)|(OUT<<2))
#define EP01_IN     ((0x01<<3)|(IN<<2))
#define EP02_OUT    ((0x02<<3)|(OUT<<2))
#define EP02_IN     ((0x02<<3)|(IN<<2))
#define EP03_OUT    ((0x03<<3)|(OUT<<2))
#define EP03_IN     ((0x03<<3)|(IN<<2))
#define EP04_OUT    ((0x04<<3)|(OUT<<2))
#define EP04_IN     ((0x04<<3)|(IN<<2))
#define EP05_OUT    ((0x05<<3)|(OUT<<2))
#define EP05_IN     ((0x05<<3)|(IN<<2))
#define EP06_OUT    ((0x06<<3)|(OUT<<2))
#define EP06_IN     ((0x06<<3)|(IN<<2))
#define EP07_OUT    ((0x07<<3)|(OUT<<2))
#define EP07_IN     ((0x07<<3)|(IN<<2))
#define EP08_OUT    ((0x08<<3)|(OUT<<2))
#define EP08_IN     ((0x08<<3)|(IN<<2))
#define EP09_OUT    ((0x09<<3)|(OUT<<2))
#define EP09_IN     ((0x09<<3)|(IN<<2))
#define EP10_OUT    ((0x0A<<3)|(OUT<<2))
#define EP10_IN     ((0x0A<<3)|(IN<<2))
#define EP11_OUT    ((0x0B<<3)|(OUT<<2))
#define EP11_IN     ((0x0B<<3)|(IN<<2))
#define EP12_OUT    ((0x0C<<3)|(OUT<<2))
#define EP12_IN     ((0x0C<<3)|(IN<<2))
#define EP13_OUT    ((0x0D<<3)|(OUT<<2))
#define EP13_IN     ((0x0D<<3)|(IN<<2))
#define EP14_OUT    ((0x0E<<3)|(OUT<<2))
#define EP14_IN     ((0x0E<<3)|(IN<<2))
#define EP15_OUT    ((0x0F<<3)|(OUT<<2))
#define EP15_IN     ((0x0F<<3)|(IN<<2))

/******************************************************************************
 * Macro:           void mInitializeUSBDriver(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Configures the USB module, definition of UCFG_VAL can be
 *                  found in autofiles\usbcfg.h
 *
 *                  This register determines: USB Speed, On-chip pull-up
 *                  resistor selection, On-chip tranceiver selection, bus
 *                  eye pattern generation mode, Ping-pong buffering mode
 *                  selection.
 *
 * Note:            None
 *****************************************************************************/
#define mInitializeUSBDriver()      {UCFG = UCFG_VAL;                       \
                                     usb_device_state = DETACHED_STATE;     \
                                     usb_stat._byte = 0x00;                 \
                                     usb_active_cfg = 0x00;}

/******************************************************************************
 * Macro:           void mDisableEP1to15(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This macro disables all endpoints except EP0.
 *                  This macro should be called when the host sends a RESET
 *                  signal or a SET_CONFIGURATION request.
 *
 * Note:            None
 *****************************************************************************/
#define mDisableEP1to15()       ClearArray((byte*)&UEP1,15);
/*
#define mDisableEP1to15()       UEP1=0x00;UEP2=0x00;UEP3=0x00;\
                                UEP4=0x00;UEP5=0x00;UEP6=0x00;UEP7=0x00;\
                                UEP8=0x00;UEP9=0x00;UEP10=0x00;UEP11=0x00;\
                                UEP12=0x00;UEP13=0x00;UEP14=0x00;UEP15=0x00;
*/

/******************************************************************************
 * Macro:           void mUSBBufferReady(buffer_dsc)
 *
 * PreCondition:    IN Endpoint: Buffer is loaded and ready to be sent.
 *                  OUT Endpoint: Buffer is free to be written to by SIE.
 *
 * Input:           byte buffer_dsc: Root name of the buffer descriptor group.
 *                  i.e. ep0Bo, ep1Bi, ... Declared in usbmmap.c
 *                  Names can be remapped for readability, see examples in
 *                  usbcfg.h (#define HID_BD_OUT      ep1Bo)
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This macro should be called each time after:
 *                  1. A non-EP0 IN endpoint buffer is populated with data.
 *                  2. A non-EP0 OUT endpoint buffer is read.
 *                  This macro turns the buffer ownership to SIE for servicing.
 *                  It also toggles the DTS bit for synchronization.
 *
 * Note:            None
 *****************************************************************************/
#define mUSBBufferReady(buffer_dsc)                                         \
{                                                                           \
    buffer_dsc.Stat._byte &= _DTSMASK;          /* Save only DTS bit */     \
    buffer_dsc.Stat.DTS = !buffer_dsc.Stat.DTS; /* Toggle DTS bit    */     \
    buffer_dsc.Stat._byte |= _USIE|_DTSEN;      /* Turn ownership to SIE */ \
}

/** T Y P E S ****************************************************************/

/** E X T E R N S ************************************************************/

/** P U B L I C  P R O T O T Y P E S *****************************************/
void USBCheckBusStatus(void);
void USBDriverService(void);
void USBRemoteWakeup(void);
void USBSoftDetach(void);

void ClearArray(byte* startAdr,byte count);
#endif //USBDRV_H
