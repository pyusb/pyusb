/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbmmap.h
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

#ifndef USBMMAP_H
#define USBMMAP_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/

/* Buffer Descriptor Status Register Initialization Parameters */
#define _BSTALL     0x04                //Buffer Stall enable
#define _DTSEN      0x08                //Data Toggle Synch enable
#define _INCDIS     0x10                //Address increment disable
#define _KEN        0x20                //SIE keeps buff descriptors enable
#define _DAT0       0x00                //DATA0 packet expected next
#define _DAT1       0x40                //DATA1 packet expected next
#define _DTSMASK    0x40                //DTS Mask
#define _USIE       0x80                //SIE owns buffer
#define _UCPU       0x00                //CPU owns buffer

/* USB Device States - To be used with [byte usb_device_state] */
#define DETACHED_STATE          0
#define ATTACHED_STATE          1
#define POWERED_STATE           2
#define DEFAULT_STATE           3
#define ADR_PENDING_STATE       4
#define ADDRESS_STATE           5
#define CONFIGURED_STATE        6

/* Memory Types for Control Transfer - used in USB_DEVICE_STATUS */
#define _RAM 0
#define _ROM 1

/** T Y P E S ****************************************************************/
typedef union _USB_DEVICE_STATUS
{
    byte _byte;
    struct
    {
        unsigned RemoteWakeup:1;// [0]Disabled [1]Enabled: See usbdrv.c,usb9.c
        unsigned ctrl_trf_mem:1;// [0]RAM      [1]ROM
    };
} USB_DEVICE_STATUS;

typedef union _BD_STAT
{
    byte _byte;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1;              //Buffer Stall Enable
        unsigned DTSEN:1;               //Data Toggle Synch Enable
        unsigned INCDIS:1;              //Address Increment Disable
        unsigned KEN:1;                 //BD Keep Enable
        unsigned DTS:1;                 //Data Toggle Synch Value
        unsigned UOWN:1;                //USB Ownership
    };
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned PID0:1;
        unsigned PID1:1;
        unsigned PID2:1;
        unsigned PID3:1;
        unsigned :1;
        unsigned UOWN:1;
    };
    struct{
        unsigned :2;
        unsigned PID:4;                 //Packet Identifier
        unsigned :2;
    };
} BD_STAT;                              //Buffer Descriptor Status Register

typedef union _BDT
{
    struct
    {
        BD_STAT Stat;
        byte Cnt;
        byte ADRL;                      //Buffer Address Low
        byte ADRH;                      //Buffer Address High
    };
    struct
    {
        unsigned :8;
        unsigned :8;
        byte* ADR;                      //Buffer Address
    };
} BDT;                                  //Buffer Descriptor Table

/** E X T E R N S ************************************************************/
extern byte usb_device_state;
extern USB_DEVICE_STATUS usb_stat;
extern byte usb_active_cfg;
extern byte usb_alt_intf[MAX_NUM_INT];

extern volatile far BDT ep0Bo;          //Endpoint #0 BD Out
extern volatile far BDT ep0Bi;          //Endpoint #0 BD In
extern volatile far BDT ep1Bo;          //Endpoint #1 BD Out
extern volatile far BDT ep1Bi;          //Endpoint #1 BD In
extern volatile far BDT ep2Bo;          //Endpoint #2 BD Out
extern volatile far BDT ep2Bi;          //Endpoint #2 BD In
extern volatile far BDT ep3Bo;          //Endpoint #3 BD Out
extern volatile far BDT ep3Bi;          //Endpoint #3 BD In
extern volatile far BDT ep4Bo;          //Endpoint #4 BD Out
extern volatile far BDT ep4Bi;          //Endpoint #4 BD In
extern volatile far BDT ep5Bo;          //Endpoint #5 BD Out
extern volatile far BDT ep5Bi;          //Endpoint #5 BD In
extern volatile far BDT ep6Bo;          //Endpoint #6 BD Out
extern volatile far BDT ep6Bi;          //Endpoint #6 BD In
extern volatile far BDT ep7Bo;          //Endpoint #7 BD Out
extern volatile far BDT ep7Bi;          //Endpoint #7 BD In
extern volatile far BDT ep8Bo;          //Endpoint #8 BD Out
extern volatile far BDT ep8Bi;          //Endpoint #8 BD In
extern volatile far BDT ep9Bo;          //Endpoint #9 BD Out
extern volatile far BDT ep9Bi;          //Endpoint #9 BD In
extern volatile far BDT ep10Bo;         //Endpoint #10 BD Out
extern volatile far BDT ep10Bi;         //Endpoint #10 BD In
extern volatile far BDT ep11Bo;         //Endpoint #11 BD Out
extern volatile far BDT ep11Bi;         //Endpoint #11 BD In
extern volatile far BDT ep12Bo;         //Endpoint #12 BD Out
extern volatile far BDT ep12Bi;         //Endpoint #12 BD In
extern volatile far BDT ep13Bo;         //Endpoint #13 BD Out
extern volatile far BDT ep13Bi;         //Endpoint #13 BD In
extern volatile far BDT ep14Bo;         //Endpoint #14 BD Out
extern volatile far BDT ep14Bi;         //Endpoint #14 BD In
extern volatile far BDT ep15Bo;         //Endpoint #15 BD Out
extern volatile far BDT ep15Bi;         //Endpoint #15 BD In

extern volatile far CTRL_TRF_SETUP SetupPkt;
extern volatile far CTRL_TRF_DATA CtrlTrfData;

extern volatile far byte bulk_out[PYUSB_EP_SIZE];
extern volatile far byte bulk_in[PYUSB_EP_SIZE];

extern volatile far byte intr_out[PYUSB_EP_SIZE];
extern volatile far byte intr_in[PYUSB_EP_SIZE];

extern volatile far byte iso_out[PYUSB_EP_SIZE];
extern volatile far byte iso_in[PYUSB_EP_SIZE];

#endif //USBMMAP_H
