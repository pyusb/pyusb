/// Benchmark.h
/// Common header file.

/* USB Benchmark for libusb-win32

    Copyright © 2010 Travis Robinson. <libusbdotnet@gmail.com>
    website: http://sourceforge.net/projects/libusb-win32
 
    Software License Agreement:
    
    The software supplied herewith is intended for use solely and
    exclusively on Microchip PIC Microcontroller products. This 
    software is owned by Travis Robinson, and is protected under
	applicable copyright laws. All rights are reserved. Any use in 
	violation of the foregoing restrictions may subject the user to 
	criminal sanctions under applicable laws, as well as to civil 
	liability for the breach of the terms and conditions of this
    license.

	You may redistribute and/or modify this file under the terms
	described above.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE OWNER SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.	
*/

#ifndef _BENCHMARK_H
#define _BENCHMARK_H

/** INCLUDES *******************************************************/
#include "USB/usb.h"
#include "USB/usb_function_generic.h"
#include "HardwareProfile.h"

// These are vendor specific commands
// See the PICFW_COMMANDS for details
// on how this is implemented.
enum TestType
{
    TEST_NONE,
    TEST_PCREAD,
    TEST_PCWRITE,
    TEST_LOOP
};

/** BMARK CALLBACKS ************************************************/
void USBCBCheckOtherReq(void);
void USBCBInitEP(void);

/** USB FW EXTERNS DEFINES *****************************************/
extern volatile CTRL_TRF_SETUP SetupPkt;

/** BMARK DEFINES **************************************************/
void Benchmark_ProcessIO(void);
void Benchmark_Init(void);

#endif

