/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usb_compile_time_validation.h
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
 * Rawin Rojvanit       7/10/04     Original.
 ********************************************************************/

#ifndef USB_COMPILE_TIME_VALIDATION_H
#define USB_COMPILE_TIME_VALIDATION_H

/** I N C L U D E S *************************************************/
#include "system\typedefs.h"
#include "system\usb\usb.h"

/** U S B  V A L I D A T I O N **************************************/

#if (EP0_BUFF_SIZE != 8) && (EP0_BUFF_SIZE != 16) && \\
    (EP0_BUFF_SIZE != 32) && (EP0_BUFF_SIZE != 64)
#error(Invalid buffer size for endpoint 0,check "autofiles\usbcfg.h")
#endif

#if defined(HID_INT_OUT_EP_SIZE)
    #if (HID_INT_OUT_EP_SIZE > 64)
        #error(HID Out endpoint size cannot be bigger than 64, check "autofiles\usbcfg.h")
    #endif
#endif

#ifdef HID_INT_IN_EP_SIZE
    #if (HID_INT_IN_EP_SIZE > 64)
        #error(HID In endpoint size cannot be bigger than 64, check "autofiles\usbcfg.h")
    #endif
#endif

#endif //USB_COMPILE_TIME_VALIDATION_H
