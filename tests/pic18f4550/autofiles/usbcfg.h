/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.2
 *
 *********************************************************************
 * FileName:        usbcfg.h
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
 ********************************************************************/

#ifndef USBCFG_H
#define USBCFG_H

/** D E F I N I T I O N S *******************************************/
#define MAX_NUM_INT             1   // For tracking Alternate Setting
#define EP0_BUFF_SIZE           64   // Valid Options: 8, 16, 32, or 64 bytes.
									// There is very little advantage in using 
									// more than 8 bytes on EP0 IN/OUT, so 8 is the
									// recommended value.

/* Parameter definitions are defined in usbdrv.h */
#define MODE_PP                 _PPBM0
#define UCFG_VAL                _PUEN|_TRINT|_FS|MODE_PP


/* Uncomment only the hardware platform that you are using*/
#define PIC18F4550_PICDEM_FS_USB
//#define PIC18F87J50_FS_USB_PIM
//#define YOUR_BOARD


#if defined(PIC18F4550_PICDEM_FS_USB)
    #define USE_SELF_POWER_SENSE_IO
    #define USE_USB_BUS_SENSE_IO

#elif defined(PIC18F87J50_FS_USB_PIM)
    //#define USE_USB_BUS_SENSE_IO		//JP1 must be in R-U position to use this feature on this board		

/*If using the YOUR_BOARD selection, uncomment below section as appropriate for your hardware*/
//#elif defined(YOUR_BOARD)
	//#define USE_SELF_POWER_SENSE_IO	//See MCHPFSUSB Firmware User's Guide
   	//#define USE_USB_BUS_SENSE_IO		//(DS51679) for more details about these features.

#else
    #error Not a supported board (yet), See __FILE__, line __LINE__, or double click on this text.
//See above commented section.  You need to select the features your hardware will be using.
#endif

/** D E V I C E  C L A S S  U S A G E *******************************/


/*
 * MUID = Microchip USB Class ID
 * Used to identify which of the USB classes owns the current
 * session of control transfer over EP0
 */
#define MUID_NULL               0
#define MUID_USB9               1
#define MUID_PYUSB				2

/** E N D P O I N T S  A L L O C A T I O N **************************/
/*
 * See usbmmap.c for an explanation of how the endpoint allocation works
 */

/* PICDEM FS USB Demo (using generic usb class template) */
#define UEP_BULK    	UEP1
#define UEP_INTR		UEP2
#define UEP_ISO			UEP3
#define BULK_BD_OUT		ep1Bo
#define BULK_BD_IN		ep1Bi
#define INTR_BD_OUT		ep2Bo
#define INTR_BD_IN		ep2Bi
#define ISO_BD_OUT		ep3Bo
#define ISO_BD_IN		ep3Bi
#define PYUSB_EP_SIZE   64

#define MAX_EP_NUMBER   3           // UEP3

#endif //USBCFG_H
