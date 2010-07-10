/********************************************************************
  File Information:
    FileName:     	usb_device.c
    Dependencies:	See INCLUDES section
    Processor:		PIC18 or PIC24 USB Microcontrollers
    Hardware:		The code is natively intended to be used on the following
    				hardware platforms: PICDEM™ FS USB Demo Board, 
    				PIC18F87J50 FS USB Plug-In Module, or
    				Explorer 16 + PIC24 USB PIM.  The firmware may be
    				modified for use on other USB platforms by editing the
    				HardwareProfile.h file.
    Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
    Company:		Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  Summary:
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the MCHPFSUSB device
    stack. This file should be included in projects that use the device stack. 
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB"
    directory.

  Description:
    USB Device Stack File
    
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the MCHPFSUSB device
    stack. This file should be included in projects that use the device stack.
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB"
    directory.
    
    When including this file in a new project, this file can either be
    referenced from the directory in which it was installed or copied
    directly into the user application folder. If the first method is
    chosen to keep the file located in the folder in which it is installed
    then include paths need to be added so that the library and the
    application both know where to reference each others files. If the
    application folder is located in the same folder as the Microchip
    folder (like the current demo folders), then the following include
    paths need to be added to the application's project:
    
    .
    ..\\..\\MicrochipInclude
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 

********************************************************************
 File Description:

 Change History:
  Rev    Description
  ----   -----------
  2.6    Added USBCancelIO() function.  Moved and some stack
         defintions to be more consistant with the host stack.
********************************************************************/

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "./USB/USB.h"
#include "HardwareProfile.h"


#if defined(USB_USE_MSD)
    #include "./USB/usb_function_msd.h"
#endif

#if defined(__C32__)
    #if (USB_PING_PONG_MODE != USB_PING_PONG__FULL_PING_PONG)
        #error "PIC32 only supports full ping pong mode.  A different mode other than full ping pong is selected in the usb_config.h file."
    #endif
#endif

//#define DEBUG_MODE

#ifdef DEBUG_MODE
    #include "uart2.h"
#endif

/** Definitions ****************************************************/
/* v2.1 fix - Short Packet States - Used by Control Transfer Read  - CTRL_TRF_TX */
#define SHORT_PKT_NOT_USED  0
#define SHORT_PKT_PENDING   1
#define SHORT_PKT_SENT      2

/* Control Transfer States */
#define WAIT_SETUP          0
#define CTRL_TRF_TX         1
#define CTRL_TRF_RX         2

#if (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
    #define USB_NEXT_EP0_OUT_PING_PONG 0x0000   // Used in USB Device Mode only
    #define USB_NEXT_EP0_IN_PING_PONG 0x0000    // Used in USB Device Mode only
    #define USB_NEXT_PING_PONG 0x0000           // Used in USB Device Mode only
    #define EP0_OUT_EVEN    0                   // Used in USB Device Mode only
    #define EP0_OUT_ODD     0                   // Used in USB Device Mode only
    #define EP0_IN_EVEN     1                   // Used in USB Device Mode only
    #define EP0_IN_ODD      1                   // Used in USB Device Mode only
    #define EP1_OUT_EVEN    2                   // Used in USB Device Mode only
    #define EP1_OUT_ODD     2                   // Used in USB Device Mode only
    #define EP1_IN_EVEN     3                   // Used in USB Device Mode only
    #define EP1_IN_ODD      3                   // Used in USB Device Mode only
    #define EP2_OUT_EVEN    4                   // Used in USB Device Mode only
    #define EP2_OUT_ODD     4                   // Used in USB Device Mode only
    #define EP2_IN_EVEN     5                   // Used in USB Device Mode only
    #define EP2_IN_ODD      5                   // Used in USB Device Mode only
    #define EP3_OUT_EVEN    6                   // Used in USB Device Mode only
    #define EP3_OUT_ODD     6                   // Used in USB Device Mode only
    #define EP3_IN_EVEN     7                   // Used in USB Device Mode only
    #define EP3_IN_ODD      7                   // Used in USB Device Mode only
    #define EP4_OUT_EVEN    8                   // Used in USB Device Mode only
    #define EP4_OUT_ODD     8                   // Used in USB Device Mode only
    #define EP4_IN_EVEN     9                   // Used in USB Device Mode only
    #define EP4_IN_ODD      9                   // Used in USB Device Mode only
    #define EP5_OUT_EVEN    10                  // Used in USB Device Mode only
    #define EP5_OUT_ODD     10                  // Used in USB Device Mode only
    #define EP5_IN_EVEN     11                  // Used in USB Device Mode only
    #define EP5_IN_ODD      11                  // Used in USB Device Mode only
    #define EP6_OUT_EVEN    12                  // Used in USB Device Mode only
    #define EP6_OUT_ODD     12                  // Used in USB Device Mode only
    #define EP6_IN_EVEN     13                  // Used in USB Device Mode only
    #define EP6_IN_ODD      13                  // Used in USB Device Mode only
    #define EP7_OUT_EVEN    14                  // Used in USB Device Mode only
    #define EP7_OUT_ODD     14                  // Used in USB Device Mode only
    #define EP7_IN_EVEN     15                  // Used in USB Device Mode only
    #define EP7_IN_ODD      15                  // Used in USB Device Mode only
    #define EP8_OUT_EVEN    16                  // Used in USB Device Mode only
    #define EP8_OUT_ODD     16                  // Used in USB Device Mode only
    #define EP8_IN_EVEN     17                  // Used in USB Device Mode only
    #define EP8_IN_ODD      17                  // Used in USB Device Mode only
    #define EP9_OUT_EVEN    18                  // Used in USB Device Mode only
    #define EP9_OUT_ODD     18                  // Used in USB Device Mode only
    #define EP9_IN_EVEN     19                  // Used in USB Device Mode only
    #define EP9_IN_ODD      19                  // Used in USB Device Mode only
    #define EP10_OUT_EVEN   20                  // Used in USB Device Mode only
    #define EP10_OUT_ODD    20                  // Used in USB Device Mode only
    #define EP10_IN_EVEN    21                  // Used in USB Device Mode only
    #define EP10_IN_ODD     21                  // Used in USB Device Mode only
    #define EP11_OUT_EVEN   22                  // Used in USB Device Mode only
    #define EP11_OUT_ODD    22                  // Used in USB Device Mode only
    #define EP11_IN_EVEN    23                  // Used in USB Device Mode only
    #define EP11_IN_ODD     23                  // Used in USB Device Mode only
    #define EP12_OUT_EVEN   24                  // Used in USB Device Mode only
    #define EP12_OUT_ODD    24                  // Used in USB Device Mode only
    #define EP12_IN_EVEN    25                  // Used in USB Device Mode only
    #define EP12_IN_ODD     25                  // Used in USB Device Mode only
    #define EP13_OUT_EVEN   26                  // Used in USB Device Mode only
    #define EP13_OUT_ODD    26                  // Used in USB Device Mode only
    #define EP13_IN_EVEN    27                  // Used in USB Device Mode only
    #define EP13_IN_ODD     27                  // Used in USB Device Mode only
    #define EP14_OUT_EVEN   28                  // Used in USB Device Mode only
    #define EP14_OUT_ODD    28                  // Used in USB Device Mode only
    #define EP14_IN_EVEN    29                  // Used in USB Device Mode only
    #define EP14_IN_ODD     29                  // Used in USB Device Mode only
    #define EP15_OUT_EVEN   30                  // Used in USB Device Mode only
    #define EP15_OUT_ODD    30                  // Used in USB Device Mode only
    #define EP15_IN_EVEN    31                  // Used in USB Device Mode only
    #define EP15_IN_ODD     31                  // Used in USB Device Mode only

    #define EP(ep,dir,pp) (2*ep+dir)            // Used in USB Device Mode only

    #define BD(ep,dir,pp)   ((8 * ep) + (4 * dir))      // Used in USB Device Mode only

#elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
    #define USB_NEXT_EP0_OUT_PING_PONG 0x0004
    #define USB_NEXT_EP0_IN_PING_PONG 0x0000
    #define USB_NEXT_PING_PONG 0x0000
    #define EP0_OUT_EVEN    0
    #define EP0_OUT_ODD     1
    #define EP0_IN_EVEN     2
    #define EP0_IN_ODD      2
    #define EP1_OUT_EVEN    3
    #define EP1_OUT_ODD     3
    #define EP1_IN_EVEN     4
    #define EP1_IN_ODD      4
    #define EP2_OUT_EVEN    5
    #define EP2_OUT_ODD     5
    #define EP2_IN_EVEN     6
    #define EP2_IN_ODD      6
    #define EP3_OUT_EVEN    7
    #define EP3_OUT_ODD     7
    #define EP3_IN_EVEN     8
    #define EP3_IN_ODD      8
    #define EP4_OUT_EVEN    9
    #define EP4_OUT_ODD     9
    #define EP4_IN_EVEN     10
    #define EP4_IN_ODD      10
    #define EP5_OUT_EVEN    11
    #define EP5_OUT_ODD     11
    #define EP5_IN_EVEN     12
    #define EP5_IN_ODD      12
    #define EP6_OUT_EVEN    13
    #define EP6_OUT_ODD     13
    #define EP6_IN_EVEN     14
    #define EP6_IN_ODD      14
    #define EP7_OUT_EVEN    15
    #define EP7_OUT_ODD     15
    #define EP7_IN_EVEN     16
    #define EP7_IN_ODD      16
    #define EP8_OUT_EVEN    17
    #define EP8_OUT_ODD     17
    #define EP8_IN_EVEN     18
    #define EP8_IN_ODD      18
    #define EP9_OUT_EVEN    19
    #define EP9_OUT_ODD     19
    #define EP9_IN_EVEN     20
    #define EP9_IN_ODD      20
    #define EP10_OUT_EVEN   21
    #define EP10_OUT_ODD    21
    #define EP10_IN_EVEN    22
    #define EP10_IN_ODD     22
    #define EP11_OUT_EVEN   23
    #define EP11_OUT_ODD    23
    #define EP11_IN_EVEN    24
    #define EP11_IN_ODD     24
    #define EP12_OUT_EVEN   25
    #define EP12_OUT_ODD    25
    #define EP12_IN_EVEN    26
    #define EP12_IN_ODD     26
    #define EP13_OUT_EVEN   27
    #define EP13_OUT_ODD    27
    #define EP13_IN_EVEN    28
    #define EP13_IN_ODD     28
    #define EP14_OUT_EVEN   29
    #define EP14_OUT_ODD    29
    #define EP14_IN_EVEN    30
    #define EP14_IN_ODD     30
    #define EP15_OUT_EVEN   31
    #define EP15_OUT_ODD    31
    #define EP15_IN_EVEN    32
    #define EP15_IN_ODD     32

    #define EP(ep,dir,pp) (2*ep+dir+(((ep==0)&&(dir==0))?pp:2))
    #define BD(ep,dir,pp) (4*(ep+dir+(((ep==0)&&(dir==0))?pp:2)))

#elif (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
    #if defined (__18CXX) || defined(__C30__)
        #define USB_NEXT_EP0_OUT_PING_PONG 0x0004
        #define USB_NEXT_EP0_IN_PING_PONG 0x0004
        #define USB_NEXT_PING_PONG 0x0004
    #elif defined(__C32__)
        #define USB_NEXT_EP0_OUT_PING_PONG 0x0008
        #define USB_NEXT_EP0_IN_PING_PONG 0x0008
        #define USB_NEXT_PING_PONG 0x0008
    #else
        #error "Not defined for this compiler"
    #endif
    #define EP0_OUT_EVEN    0
    #define EP0_OUT_ODD     1
    #define EP0_IN_EVEN     2
    #define EP0_IN_ODD      3
    #define EP1_OUT_EVEN    4
    #define EP1_OUT_ODD     5
    #define EP1_IN_EVEN     6
    #define EP1_IN_ODD      7
    #define EP2_OUT_EVEN    8
    #define EP2_OUT_ODD     9
    #define EP2_IN_EVEN     10
    #define EP2_IN_ODD      11
    #define EP3_OUT_EVEN    12
    #define EP3_OUT_ODD     13
    #define EP3_IN_EVEN     14
    #define EP3_IN_ODD      15
    #define EP4_OUT_EVEN    16
    #define EP4_OUT_ODD     17
    #define EP4_IN_EVEN     18
    #define EP4_IN_ODD      19
    #define EP5_OUT_EVEN    20
    #define EP5_OUT_ODD     21
    #define EP5_IN_EVEN     22
    #define EP5_IN_ODD      23
    #define EP6_OUT_EVEN    24
    #define EP6_OUT_ODD     25
    #define EP6_IN_EVEN     26
    #define EP6_IN_ODD      27
    #define EP7_OUT_EVEN    28
    #define EP7_OUT_ODD     29
    #define EP7_IN_EVEN     30
    #define EP7_IN_ODD      31
    #define EP8_OUT_EVEN    32
    #define EP8_OUT_ODD     33
    #define EP8_IN_EVEN     34
    #define EP8_IN_ODD      35
    #define EP9_OUT_EVEN    36
    #define EP9_OUT_ODD     37
    #define EP9_IN_EVEN     38
    #define EP9_IN_ODD      39
    #define EP10_OUT_EVEN   40
    #define EP10_OUT_ODD    41
    #define EP10_IN_EVEN    42
    #define EP10_IN_ODD     43
    #define EP11_OUT_EVEN   44
    #define EP11_OUT_ODD    45
    #define EP11_IN_EVEN    46
    #define EP11_IN_ODD     47
    #define EP12_OUT_EVEN   48
    #define EP12_OUT_ODD    49
    #define EP12_IN_EVEN    50
    #define EP12_IN_ODD     51
    #define EP13_OUT_EVEN   52
    #define EP13_OUT_ODD    53
    #define EP13_IN_EVEN    54
    #define EP13_IN_ODD     55
    #define EP14_OUT_EVEN   56
    #define EP14_OUT_ODD    57
    #define EP14_IN_EVEN    58
    #define EP14_IN_ODD     59
    #define EP15_OUT_EVEN   60
    #define EP15_OUT_ODD    61
    #define EP15_IN_EVEN    62
    #define EP15_IN_ODD     63

    #define EP(ep,dir,pp) (4*ep+2*dir+pp)

    #if defined (__18CXX) || defined(__C30__)
        #define BD(ep,dir,pp) (4*(4*ep+2*dir+pp))
    #elif defined(__C32__)
        #define BD(ep,dir,pp) (8*(4*ep+2*dir+pp))
    #else
        #error "Not defined for this compiler"
    #endif

#elif (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)
    #define USB_NEXT_EP0_OUT_PING_PONG 0x0000
    #define USB_NEXT_EP0_IN_PING_PONG 0x0000
    #define USB_NEXT_PING_PONG 0x0004
    #define EP0_OUT_EVEN    0
    #define EP0_OUT_ODD     0
    #define EP0_IN_EVEN     1
    #define EP0_IN_ODD      1
    #define EP1_OUT_EVEN    2
    #define EP1_OUT_ODD     3
    #define EP1_IN_EVEN     4
    #define EP1_IN_ODD      5
    #define EP2_OUT_EVEN    6
    #define EP2_OUT_ODD     7
    #define EP2_IN_EVEN     8
    #define EP2_IN_ODD      9
    #define EP3_OUT_EVEN    10
    #define EP3_OUT_ODD     11
    #define EP3_IN_EVEN     12
    #define EP3_IN_ODD      13
    #define EP4_OUT_EVEN    14
    #define EP4_OUT_ODD     15
    #define EP4_IN_EVEN     16
    #define EP4_IN_ODD      17
    #define EP5_OUT_EVEN    18
    #define EP5_OUT_ODD     19
    #define EP5_IN_EVEN     20
    #define EP5_IN_ODD      21
    #define EP6_OUT_EVEN    22
    #define EP6_OUT_ODD     23
    #define EP6_IN_EVEN     24
    #define EP6_IN_ODD      25
    #define EP7_OUT_EVEN    26
    #define EP7_OUT_ODD     27
    #define EP7_IN_EVEN     28
    #define EP7_IN_ODD      29
    #define EP8_OUT_EVEN    30
    #define EP8_OUT_ODD     31
    #define EP8_IN_EVEN     32
    #define EP8_IN_ODD      33
    #define EP9_OUT_EVEN    34
    #define EP9_OUT_ODD     35
    #define EP9_IN_EVEN     36
    #define EP9_IN_ODD      37
    #define EP10_OUT_EVEN   38
    #define EP10_OUT_ODD    39
    #define EP10_IN_EVEN    40
    #define EP10_IN_ODD     41
    #define EP11_OUT_EVEN   42
    #define EP11_OUT_ODD    43
    #define EP11_IN_EVEN    44
    #define EP11_IN_ODD     45
    #define EP12_OUT_EVEN   46
    #define EP12_OUT_ODD    47
    #define EP12_IN_EVEN    48
    #define EP12_IN_ODD     49
    #define EP13_OUT_EVEN   50
    #define EP13_OUT_ODD    51
    #define EP13_IN_EVEN    52
    #define EP13_IN_ODD     53
    #define EP14_OUT_EVEN   54
    #define EP14_OUT_ODD    55
    #define EP14_IN_EVEN    56
    #define EP14_IN_ODD     57
    #define EP15_OUT_EVEN   58
    #define EP15_OUT_ODD    59
    #define EP15_IN_EVEN    60
    #define EP15_IN_ODD     61

    #define EP(ep,dir,pp) (4*ep+2*dir+((ep==0)?0:(pp-2)))
    #define BD(ep,dir,pp) (4*(4*ep+2*dir+((ep==0)?0:(pp-2))))

#else
    #error "No ping pong mode defined."
#endif

/** VARIABLES ******************************************************/
#pragma udata

USB_VOLATILE USB_DEVICE_STATE USBDeviceState;
USB_VOLATILE BYTE USBActiveConfiguration;
USB_VOLATILE BYTE USBAlternateInterface[USB_MAX_NUM_INT];
volatile BDT_ENTRY *pBDTEntryEP0OutCurrent;
volatile BDT_ENTRY *pBDTEntryEP0OutNext;
volatile BDT_ENTRY *pBDTEntryOut[USB_MAX_EP_NUMBER+1];
volatile BDT_ENTRY *pBDTEntryIn[USB_MAX_EP_NUMBER+1];
USB_VOLATILE BYTE shortPacketStatus;
USB_VOLATILE BYTE controlTransferState;
USB_VOLATILE IN_PIPE inPipes[1];
USB_VOLATILE OUT_PIPE outPipes[1];
USB_VOLATILE BYTE *pDst;
USB_VOLATILE BOOL RemoteWakeup;
USB_VOLATILE BYTE USTATcopy;
USB_VOLATILE BOOL BothEP0OutUOWNsSet;
USB_VOLATILE WORD USBInMaxPacketSize[USB_MAX_EP_NUMBER]; 
USB_VOLATILE BYTE *USBInData[USB_MAX_EP_NUMBER];

/** USB FIXED LOCATION VARIABLES ***********************************/
#if defined(__18CXX)
    #if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50)
        #pragma udata USB_BDT=0x200     //See Linker Script,usb2:0x200-0x2FF(256-byte)
    #else
        #pragma udata USB_BDT=0x400     //See Linker Script,usb4:0x400-0x4FF(256-byte)
    #endif
#endif

/********************************************************************
 * Section A: Buffer Descriptor Table
 * - 0x400 - 0x4FF(max)
 * - USB_MAX_EP_NUMBER is defined in usb_config.h
 *******************************************************************/
#if (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
    volatile BDT_ENTRY BDT[(USB_MAX_EP_NUMBER + 1) * 2] __attribute__ ((aligned (512)));
#elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
    volatile BDT_ENTRY BDT[((USB_MAX_EP_NUMBER + 1) * 2)+1] __attribute__ ((aligned (512)));
#elif (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
    volatile BDT_ENTRY BDT[(USB_MAX_EP_NUMBER + 1) * 4] __attribute__ ((aligned (512)));
#elif (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)
    volatile BDT_ENTRY BDT[((USB_MAX_EP_NUMBER + 1) * 4)-2] __attribute__ ((aligned (512)));
#else
    #error "No ping pong mode defined."
#endif

//#if defined(__18CXX)
//#pragma udata usbram5=0x400     //See Linker Script,usb5:0x500-0x5FF(256-byte)
//#endif

/********************************************************************
 * Section B: EP0 Buffer Space
 *******************************************************************/
volatile CTRL_TRF_SETUP SetupPkt;           // 8-byte only
volatile BYTE CtrlTrfData[USB_EP0_BUFF_SIZE];

/********************************************************************
 * Section C: non-EP0 Buffer Space
 *******************************************************************/
// Can provide compile time option to do software pingpong
#if defined(USB_USE_HID)
    volatile unsigned char hid_report_out[HID_INT_OUT_EP_SIZE];
    volatile unsigned char hid_report_in[HID_INT_IN_EP_SIZE];
#endif

#if defined(USB_USE_MSD)
	//volatile far USB_MSD_CBW_CSW msd_cbw_csw;
	volatile USB_MSD_CBW msd_cbw;
	volatile USB_MSD_CSW msd_csw;
	//#pragma udata

	#if defined(__18CXX)
		#pragma udata myMSD=MSD_BUFFER_ADDRESS
	#endif
	volatile char msd_buffer[512];
#endif

#if defined(__18CXX)
#pragma udata
#endif

//Depricated in v2.2 - will be removed in a future revision
#if !defined(USB_USER_DEVICE_DESCRIPTOR)
    //Device descriptor
    extern ROM USB_DEVICE_DESCRIPTOR device_dsc;
#else
    USB_USER_DEVICE_DESCRIPTOR_INCLUDE;
#endif

#if !defined(USB_USER_CONFIG_DESCRIPTOR)
    //Array of configuration descriptors
    extern ROM BYTE *ROM USB_CD_Ptr[];
#else
    USB_USER_CONFIG_DESCRIPTOR_INCLUDE;
#endif

extern ROM BYTE *ROM USB_SD_Ptr[];

/** DECLARATIONS ***************************************************/
#pragma code

/** Macros *********************************************************/

/****** Event callback enabling/disabling macros ********************
    This section of code is used to disable specific USB events that may not be
    desired by the user.  This can save code size and increase throughput and
    decrease CPU utiliazation.
********************************************************************/
#if defined USB_DISABLE_SUSPEND_HANDLER
    #define USB_SUSPEND_HANDLER(event,pointer,size) 
    
    #warning "Disabling the suspend handler is not recommended.  Proper suspend handling is required to create a compilant USB device."                
#else
    #define USB_SUSPEND_HANDLER(event,pointer,size)             USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

#if defined USB_DISABLE_WAKEUP_FROM_SUSPEND_HANDLER
    #define USB_WAKEUP_FROM_SUSPEND_HANDLER(event,pointer,size) 

    #warning "Disabling the wake from suspend handler is not recommended.  Proper suspend handling is required to create a compilant USB device."                
#else
    #define USB_WAKEUP_FROM_SUSPEND_HANDLER(event,pointer,size) USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)   
#endif

#if defined USB_DISABLE_SOF_HANDLER
    #define USB_SOF_HANDLER(event,pointer,size)                
#else
    #define USB_SOF_HANDLER(event,pointer,size)                 USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

#if defined USB_DISABLE_ERROR_HANDLER 
    #define USB_ERROR_HANDLER(event,pointer,size)             
#else
    #define USB_ERROR_HANDLER(event,pointer,size)               USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

#if defined USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER 
    #define USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER(event,pointer,size)                 
#else
    #define USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER(event,pointer,size)       USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

#if defined USB_DISABLE_SET_DESCRIPTOR_HANDLER 
    #define USB_SET_DESCRIPTOR_HANDLER(event,pointer,size)                
#else
    #define USB_SET_DESCRIPTOR_HANDLER(event,pointer,size)      USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size) 
#endif

#if defined USB_DISABLE_SET_CONFIGURATION_HANDLER
    #define USB_SET_CONFIGURATION_HANDLER(event,pointer,size)                
#else
    #define USB_SET_CONFIGURATION_HANDLER(event,pointer,size)             USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

#if defined USB_DISABLE_TRANSFER_COMPLETE_HANDLER 
    #define USB_TRASFER_COMPLETE_HANDLER(event,pointer,size)               
#else
    #define USB_TRASFER_COMPLETE_HANDLER(event,pointer,size)    USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#endif

/** Function Prototypes ********************************************/
//External
//This is the prototype for the required user event handler
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size);

//Internal
void USBCtrlEPService(void);
void USBCtrlTrfSetupHandler(void);
void USBCtrlTrfInHandler(void);
void USBCheckStdRequest(void);
void USBStdGetDscHandler(void);
void USBCtrlEPServiceComplete(void);
void USBCtrlTrfTxService(void);
void USBPrepareForNextSetupTrf(void);
void USBCtrlTrfRxService(void);
void USBStdSetCfgHandler(void);
void USBStdGetStatusHandler(void);
void USBStdFeatureReqHandler(void);
void USBCtrlTrfOutHandler(void);
BOOL USBIsTxBusy(BYTE EPNumber);
void USBPut(BYTE EPNum, BYTE Data);
void USBEPService(void);
void USBConfigureEndpoint(BYTE EPNum, BYTE direction);

void USBProtocolResetHandler(void);
void USBWakeFromSuspend(void);
void USBSuspend(void);
void USBStallHandler(void);
USB_HANDLE USBTransferOnePacket(BYTE ep, BYTE dir, BYTE* data, BYTE len);
void USBEnableEndpoint(BYTE ep,BYTE options);

//DOM-IGNORE-BEGIN
/****************************************************************************
  Function:
    void USBDeviceInit(void)

  Description:
    This function initializes the device stack
    it in the default state

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    The USB module will be completely reset including
    all of the internal variables, registers, and
    interrupt flags.
  ***************************************************************************/
//DOM-IGNORE-END
void USBDeviceInit(void)
{
    BYTE i;

    // Clear all USB error flags
    USBClearInterruptRegister(U1EIR);  
       
    // Clears all USB interrupts          
    USBClearInterruptRegister(U1IR); 

    SetConfigurationOptions();

    //power up the module
    USBPowerModule();

    //set the address of the BDT (if applicable)
    USBSetBDTAddress(BDT);

    // Assert reset request to all of the Ping Pong buffer pointers
    USBPingPongBufferReset = 1;                    

    // Reset to default address
    U1ADDR = 0x00;                   

    //Clear all of the endpoint control registers
    memset((void*)&U1EP1,0x00,(USB_MAX_EP_NUMBER-1));

    //Stop trying to reset ping pong buffer pointers
    USBPingPongBufferReset = 0;

    //Clear all of the BDT entries
    for(i=0;i<(sizeof(BDT)/sizeof(BDT_ENTRY));i++)
    {
        BDT[i].Val = 0x00;
    }

    // Initialize EP0 as a Ctrl EP
    U1EP0 = EP_CTRL|USB_HANDSHAKE_ENABLED;        

    // Flush any pending transactions
    while(USBTransactionCompleteIF == 1)      
    {
        USBClearInterruptFlag(USBTransactionCompleteIFReg,USBTransactionCompleteIFBitNum);
    }

    //clear all of the internal pipe information
    inPipes[0].info.Val = 0;
    outPipes[0].info.Val = 0;
    outPipes[0].wCount.Val = 0;

    // Make sure packet processing is enabled
    USBPacketDisable = 0;           

    //Get ready for the first packet
    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&BDT[EP0_IN_EVEN];

    // Clear active configuration
    USBActiveConfiguration = 0;     

    //Indicate that we are now in the detached state        
    USBDeviceState = DETACHED_STATE;
}

//DOM-IGNORE-BEGIN
/****************************************************************************
  Function:
    void USBDeviceTasks(void)

  Description:
    This function is the main state machine of the 
    USB device side stack.  This function should be
    called periodically to receive and transmit
    packets through the stack.  This function should
    be called  preferably once every 100us 
    during the enumeration process.  After the
    enumeration process this function still needs to
    be called periodically to respond to various
    situations on the bus but is more relaxed in its
    time requirements.  This function should also
    be called at least as fast as the OUT data
    expected from the PC.

  Precondition:
    None

  Parameters:
    None

  Return Values:
    None

  Remarks:
    None
  ***************************************************************************/
//DOM-IGNORE-END
#if defined(USB_INTERRUPT) 
  #if defined(__18CXX)
    void USBDeviceTasks(void)
  #elif defined(__C30__)
    //void __attribute__((interrupt,auto_psv,address(0xA800))) _USB1Interrupt()
    void __attribute__((interrupt,auto_psv)) _USB1Interrupt()
  #elif defined(__PIC32MX__)
    #pragma interrupt _USB1Interrupt ipl4 vector 45
    void _USB1Interrupt( void ) 
  #endif
#else
void USBDeviceTasks(void)
#endif
{
    BYTE i;

#ifdef USB_SUPPORT_OTG
    //SRP Time Out Check
    if (USBOTGSRPIsReady())
    {
        if (USBT1MSECIF && USBT1MSECIE)
        {
            if (USBOTGGetSRPTimeOutFlag())
            {
                if (USBOTGIsSRPTimeOutExpired())
                {
                    USB_OTGEventHandler(0,OTG_EVENT_SRP_FAILED,0,0);
                }       
            }

            //Clear Interrupt Flag
            USBClearInterruptFlag(USBT1MSECIFReg,USBT1MSECIFBitNum);
        }
    }
#endif

    #if defined(USB_POLLING)
    //If the interrupt option is selected then the customer is required
    //  to notify the stack when the device is attached or removed from the
    //  bus by calling the USBDeviceAttach() and USBDeviceDetach() functions.
    if (USB_BUS_SENSE != 1)
    {
         // Disable module & detach from bus
         U1CON = 0;             

         // Mask all USB interrupts              
         U1IE = 0;          

         //Move to the detached state                  
         USBDeviceState = DETACHED_STATE;

         #ifdef  USB_SUPPORT_OTG    
             //Disable D+ Pullup
             U1OTGCONbits.DPPULUP = 0;

             //Disable HNP
             USBOTGDisableHnp();

             //Deactivate HNP
             USBOTGDeactivateHnp();
             
             //If ID Pin Changed State
             if (USBIDIF && USBIDIE)
             {  
                 //Re-detect & Initialize
                  USBOTGInitialize();

                  //Clear ID Interrupt Flag
                  USBClearInterruptFlag(USBIDIFReg,USBIDIFBitNum);
             }
         #endif

         #ifdef __C30__
             //USBClearInterruptFlag(U1OTGIR, 3); 
         #endif
            //return so that we don't go through the rest of 
            //the state machine
         USBClearUSBInterrupt();
         return;
    }

	#ifdef USB_SUPPORT_OTG
    //If Session Is Started Then
    else
	{
        //If SRP Is Ready
        if (USBOTGSRPIsReady())
        {   
            //Clear SRPReady
            USBOTGClearSRPReady();

            //Clear SRP Timeout Flag
            USBOTGClearSRPTimeOutFlag();

            //Indicate Session Started
            UART2PrintString( "\r\n***** USB OTG B Event - Session Started  *****\r\n" );
        }
    }
	#endif	//#ifdef USB_SUPPORT_OTG

    //if we are in the detached state
    if(USBDeviceState == DETACHED_STATE)
    {
	    //Initialize register to known value
        U1CON = 0;                          

        // Mask all USB interrupts
        U1IE = 0;                                

        //Enable/set things like: pull ups, full/low-speed mode, 
        //set the ping pong mode, and set internal transceiver
        SetConfigurationOptions();

        // Enable module & attach to bus
        while(!U1CONbits.USBEN){U1CONbits.USBEN = 1;}

        //moved to the attached state
        USBDeviceState = ATTACHED_STATE;

        #ifdef  USB_SUPPORT_OTG
            U1OTGCON |= USB_OTG_DPLUS_ENABLE | USB_OTG_ENABLE;  
        #endif
    }
	#endif  //#if defined(USB_POLLING)

    if(USBDeviceState == ATTACHED_STATE)
    {
        /*
         * After enabling the USB module, it takes some time for the
         * voltage on the D+ or D- line to rise high enough to get out
         * of the SE0 condition. The USB Reset interrupt should not be
         * unmasked until the SE0 condition is cleared. This helps
         * prevent the firmware from misinterpreting this unique event
         * as a USB bus reset from the USB host.
         */

        if(!USBSE0Event)
        {
            USBClearInterruptRegister(U1IR);// Clear all USB interrupts
            #if defined(USB_POLLING)
                U1IE=0;                        // Mask all USB interrupts
            #endif
            USBResetIE = 1;             // Unmask RESET interrupt
            USBIdleIE = 1;             // Unmask IDLE interrupt
            USBDeviceState = POWERED_STATE;
        }
    }

    #ifdef  USB_SUPPORT_OTG
        //If ID Pin Changed State
        if (USBIDIF && USBIDIE)
        {  
            //Re-detect & Initialize
            USBOTGInitialize();

            USBClearInterruptFlag(USBIDIFReg,USBIDIFBitNum);
        }
    #endif

    /*
     * Task A: Service USB Activity Interrupt
     */
    if(USBActivityIF && USBActivityIE)
    {
        USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
        #if defined(USB_SUPPORT_OTG)
            U1OTGIR = 0x10;        
        #else
            USBWakeFromSuspend();
        #endif
    }

    /*
     * Pointless to continue servicing if the device is in suspend mode.
     */
    if(USBSuspendControl==1)
    {
        USBClearUSBInterrupt();
        return;
    }

    /*
     * Task B: Service USB Bus Reset Interrupt.
     * When bus reset is received during suspend, ACTVIF will be set first,
     * once the UCONbits.SUSPND is clear, then the URSTIF bit will be asserted.
     * This is why URSTIF is checked after ACTVIF.
     *
     * The USB reset flag is masked when the USB state is in
     * DETACHED_STATE or ATTACHED_STATE, and therefore cannot
     * cause a USB reset event during these two states.
     */
    if(USBResetIF && USBResetIE)
    {
        USBDeviceInit();
        USBDeviceState = DEFAULT_STATE;

        /********************************************************************
        Bug Fix: Feb 26, 2007 v2.1 (#F1)
        *********************************************************************
        In the original firmware, if an OUT token is sent by the host
        before a SETUP token is sent, the firmware would respond with an ACK.
        This is not a correct response, the firmware should have sent a STALL.
        This is a minor non-compliance since a compliant host should not
        send an OUT before sending a SETUP token. The fix allows a SETUP
        transaction to be accepted while stalling OUT transactions.
        ********************************************************************/
        BDT[EP0_OUT_EVEN].ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
        BDT[EP0_OUT_EVEN].CNT = USB_EP0_BUFF_SIZE;
        BDT[EP0_OUT_EVEN].STAT.Val &= ~_STAT_MASK;
        BDT[EP0_OUT_EVEN].STAT.Val |= _USIE|_DAT0|_DTSEN|_BSTALL;

        #ifdef USB_SUPPORT_OTG
             //Disable HNP
             USBOTGDisableHnp();

             //Deactivate HNP
             USBOTGDeactivateHnp();
        #endif

        USBClearInterruptFlag(USBResetIFReg,USBResetIFBitNum);
    }

    /*
     * Task C: Service other USB interrupts
     */
    if(USBIdleIF && USBIdleIE)
    { 
        #ifdef  USB_SUPPORT_OTG 
            //If Suspended, Try to switch to Host
            USBOTGSelectRole(ROLE_HOST);
        #else
            USBSuspend();
        #endif
        
        USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
    }

    if(USBSOFIF && USBSOFIE)
    {
        USB_SOF_HANDLER(EVENT_SOF,0,1);
        USBClearInterruptFlag(USBSOFIFReg,USBSOFIFBitNum);
    }

    if(USBStallIF && USBStallIE)
    {
        USBStallHandler();
    }

    if(USBErrorIF && USBErrorIE)
    {
        USB_ERROR_HANDLER(EVENT_BUS_ERROR,0,1);
        USBClearInterruptRegister(U1EIR);               // This clears UERRIF
    }

    /*
     * Pointless to continue servicing if the host has not sent a bus reset.
     * Once bus reset is received, the device transitions into the DEFAULT
     * state and is ready for communication.
     */
    if(USBDeviceState < DEFAULT_STATE)
    {
	    USBClearUSBInterrupt();
	    return; 
	}  

    /*
     * Task D: Servicing USB Transaction Complete Interrupt
     */
    if(USBTransactionCompleteIE)
    {
	    for(i = 0; i < 4; i++)	//Drain or deplete the USAT FIFO entries.  If the USB FIFO ever gets full, USB bandwidth 
		{						//utilization can be compromised, and the device won't be able to receive SETUP packets.
		    if(USBTransactionCompleteIF)
		    {
		        USTATcopy = U1STAT;

		        USBClearInterruptFlag(USBTransactionCompleteIFReg,USBTransactionCompleteIFBitNum);
		
		        /*
		         * USBCtrlEPService only services transactions over EP0.
		         * It ignores all other EP transactions.
		         */

                if((USTATcopy & ENDPOINT_MASK) == 0)
                {
		            USBCtrlEPService();
                }
                else
                {
                    USB_TRASFER_COMPLETE_HANDLER(
                        EVENT_TRANSFER, 
                        (BYTE*)&USTATcopy, 
                        0);
                }
		    }//end if(USBTransactionCompleteIF)
		    else
		    	break;	//USTAT FIFO must be empty.
		}//end for()
	}//end if(USBTransactionCompleteIE)   

    USBClearUSBInterrupt();
}//end of USBDeviceTasks()

/********************************************************************
 * Function:        void USBStallHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    
 *
 * Overview:        This function handles the event of a STALL 
 *                  occuring on the bus
 *
 * Note:            None
 *******************************************************************/
void USBStallHandler(void)
{
    /*
     * Does not really have to do anything here,
     * even for the control endpoint.
     * All BDs of Endpoint 0 are owned by SIE right now,
     * but once a Setup Transaction is received, the ownership
     * for EP0_OUT will be returned to CPU.
     * When the Setup Transaction is serviced, the ownership
     * for EP0_IN will then be forced back to CPU by firmware.
     */

    /* v2b fix */
    if(U1EP0bits.EPSTALL == 1)
    {
        // UOWN - if 0, owned by CPU, if 1, owned by SIE
        if((pBDTEntryEP0OutCurrent->STAT.Val == _USIE) && (pBDTEntryIn[0]->STAT.Val == (_USIE|_BSTALL)))
        {
            // Set ep0Bo to stall also
            pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
        }//end if
        U1EP0bits.EPSTALL = 0;               // Clear stall status
    }//end if

    USBClearInterruptFlag(USBStallIFReg,USBStallIFBitNum);
}

/********************************************************************
 * Function:        void USBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    
 *
 * Overview:        This function handles if the host tries to 
 *                  suspend the device
 *
 * Note:            None
 *******************************************************************/
void USBSuspend(void)
{
    /*
     * NOTE: Do not clear UIRbits.ACTVIF here!
     * Reason:
     * ACTVIF is only generated once an IDLEIF has been generated.
     * This is a 1:1 ratio interrupt generation.
     * For every IDLEIF, there will be only one ACTVIF regardless of
     * the number of subsequent bus transitions.
     *
     * If the ACTIF is cleared here, a problem could occur when:
     * [       IDLE       ][bus activity ->
     * <--- 3 ms ----->     ^
     *                ^     ACTVIF=1
     *                IDLEIF=1
     *  #           #           #           #   (#=Program polling flags)
     *                          ^
     *                          This polling loop will see both
     *                          IDLEIF=1 and ACTVIF=1.
     *                          However, the program services IDLEIF first
     *                          because ACTIVIE=0.
     *                          If this routine clears the only ACTIVIF,
     *                          then it can never get out of the suspend
     *                          mode.
     */
    USBActivityIE = 1;                     // Enable bus activity interrupt
    USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);

#if defined(__18CXX)
    U1CONbits.SUSPND = 1;                   // Put USB module in power conserve
                                            // mode, SIE clock inactive
#endif
 
 
    /*
     * At this point the PIC can go into sleep,idle, or
     * switch to a slower clock, etc.  This should be done in the
     * USBCBSuspend() if necessary.
     */
    USB_SUSPEND_HANDLER(EVENT_SUSPEND,0,0);
}

/********************************************************************
 * Function:        void USBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:
 *
 * Note:            None
 *******************************************************************/
void USBWakeFromSuspend(void)
{
    /*
     * If using clock switching, the place to restore the original
     * microcontroller core clock frequency is in the USBCBWakeFromSuspend() callback
     */
    USB_WAKEUP_FROM_SUSPEND_HANDLER(EVENT_RESUME,0,0);

    #if defined(__18CXX)
    //To avoid improperly clocking the USB module, make sure the oscillator
    //settings are consistant with USB operation before clearing the SUSPND bit.
    //Make sure the correct oscillator settings are selected in the 
    //"USB_WAKEUP_FROM_SUSPEND_HANDLER(EVENT_RESUME,0,0)" handler.
    U1CONbits.SUSPND = 0;   // Bring USB module out of power conserve
                            // mode.
    #endif


    USBActivityIE = 0;

    /********************************************************************
    Bug Fix: Feb 26, 2007 v2.1
    *********************************************************************
    The ACTVIF bit cannot be cleared immediately after the USB module wakes
    up from Suspend or while the USB module is suspended. A few clock cycles
    are required to synchronize the internal hardware state machine before
    the ACTIVIF bit can be cleared by firmware. Clearing the ACTVIF bit
    before the internal hardware is synchronized may not have an effect on
    the value of ACTVIF. Additonally, if the USB module uses the clock from
    the 96 MHz PLL source, then after clearing the SUSPND bit, the USB
    module may not be immediately operational while waiting for the 96 MHz
    PLL to lock.
    ********************************************************************/

    // UIRbits.ACTVIF = 0;                      // Removed
    #if defined(__18CXX)
    while(USBActivityIF)
    #endif
    {
        USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
    }  // Added

}//end USBWakeFromSuspend

/********************************************************************
 * Function:        void USBCtrlEPService(void)
 *
 * PreCondition:    USTAT is loaded with a valid endpoint address.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        USBCtrlEPService checks for three transaction
 *                  types that it knows how to service and services
 *                  them:
 *                  1. EP0 SETUP
 *                  2. EP0 OUT
 *                  3. EP0 IN
 *                  It ignores all other types (i.e. EP1, EP2, etc.)
 *
 * Note:            None
 *******************************************************************/
void USBCtrlEPService(void)
{
	//Check if the last transaction was on  EP0 OUT endpoint (of any kind, to either the even or odd buffer if ping pong buffers used)
    if((USTATcopy & USTAT_EP0_PP_MASK) == USTAT_EP0_OUT_EVEN)
    {
		//Point to the EP0 OUT buffer of the buffer that arrived
        #if defined(__18CXX)
            pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[(USTATcopy & USTAT_EP_MASK)>>1];
        #elif defined(__C30__) || defined(__C32__)
            pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[(USTATcopy & USTAT_EP_MASK)>>2];
        #else
            #error "unimplemented"
        #endif

		//Set the next out to the current out packet
        pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;
		//Toggle it to the next ping pong buffer (if applicable)
        ((BYTE_VAL*)&pBDTEntryEP0OutNext)->Val ^= USB_NEXT_EP0_OUT_PING_PONG;

		//If the current EP0 OUT buffer has a SETUP packet
        if(pBDTEntryEP0OutCurrent->STAT.PID == PID_SETUP)
        {
	        //Check if the SETUP transaction data went into the CtrlTrfData buffer.
	        //If so, need to copy it to the SetupPkt buffer so that it can be 
	        //processed correctly by USBCtrlTrfSetupHandler().
	        if(pBDTEntryEP0OutCurrent->ADR == (BYTE*)ConvertToPhysicalAddress(&CtrlTrfData))	
	        {
		        unsigned char setup_cnt;
		
		        pBDTEntryEP0OutCurrent->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
		        for(setup_cnt = 0; setup_cnt < sizeof(CTRL_TRF_SETUP); setup_cnt++)
		        {
		            *(((BYTE*)&SetupPkt)+setup_cnt) = *(((BYTE*)&CtrlTrfData)+setup_cnt);
		        }//end for
		    } 
	        
			//Handle the control transfer (parse the 8-byte SETUP command and figure out what to do)
            USBCtrlTrfSetupHandler();
        }
        else
        {
			//Handle the DATA transfer
            USBCtrlTrfOutHandler();
        }
    }
    else if((USTATcopy & USTAT_EP0_PP_MASK) == USTAT_EP0_IN)
    {
		//Otherwise the transmission was and EP0 IN
		//  so take care of the IN transfer
        USBCtrlTrfInHandler();
    }

}//end USBCtrlEPService

/********************************************************************
 * Function:        void USBCtrlTrfSetupHandler(void)
 *
 * PreCondition:    SetupPkt buffer is loaded with valid USB Setup Data
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is a task dispatcher and has 3 stages.
 *                  1. It initializes the control transfer state machine.
 *                  2. It calls on each of the module that may know how to
 *                     service the Setup Request from the host.
 *                     Module Example: USBD, HID, CDC, MSD, ...
 *                     A callback function, USBCBCheckOtherReq(),
 *                     is required to call other module handlers.
 *                  3. Once each of the modules has had a chance to check if
 *                     it is responsible for servicing the request, stage 3
 *                     then checks direction of the transfer to determine how
 *                     to prepare EP0 for the control transfer.
 *                     Refer to USBCtrlEPServiceComplete() for more details.
 *
 * Note:            Microchip USB Firmware has three different states for
 *                  the control transfer state machine:
 *                  1. WAIT_SETUP
 *                  2. CTRL_TRF_TX (device sends data to host through IN transactions)
 *                  3. CTRL_TRF_RX (device receives data from host through OUT transactions)
 *                  Refer to firmware manual to find out how one state
 *                  is transitioned to another.
 *
 *                  A Control Transfer is composed of many USB transactions.
 *                  When transferring data over multiple transactions,
 *                  it is important to keep track of data source, data
 *                  destination, and data count. These three parameters are
 *                  stored in pSrc,pDst, and wCount. A flag is used to
 *                  note if the data source is from ROM or RAM.
 *
 *******************************************************************/
void USBCtrlTrfSetupHandler(void)
{
	//if the SIE currently owns the buffer
    if(pBDTEntryIn[0]->STAT.UOWN != 0)
    {
		//give control back to the CPU
		//  Compensate for after a STALL
        pBDTEntryIn[0]->STAT.Val = _UCPU;           
    }

	//Keep track of if a short packet has been sent yet or not
    shortPacketStatus = SHORT_PKT_NOT_USED;

    /* Stage 1 */
    controlTransferState = WAIT_SETUP;

    inPipes[0].wCount.Val = 0;
    inPipes[0].info.Val = 0;

    /* Stage 2 */
    USBCheckStdRequest();
    USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER(EVENT_EP0_REQUEST,0,0);

    /* Stage 3 */
    USBCtrlEPServiceComplete();

}//end USBCtrlTrfSetupHandler
/******************************************************************************
 * Function:        void USBCtrlTrfOutHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles an OUT transaction according to
 *                  which control transfer state is currently active.
 *
 * Note:            Note that if the the control transfer was from
 *                  host to device, the session owner should be notified
 *                  at the end of each OUT transaction to service the
 *                  received data.
 *
 *****************************************************************************/
void USBCtrlTrfOutHandler(void)
{
    if(controlTransferState == CTRL_TRF_RX)
    {
        USBCtrlTrfRxService();	//Copies the newly received data into the appropriate buffer and configures EP0 OUT for next transaction.
    }
    else //In this case the last OUT transaction must have been a status stage of a CTRL_TRF_TX
    {
	    //Prepare EP0 OUT for the next SETUP transaction, however, it may have
	    //already been prepared if ping-pong buffering was enabled on EP0 OUT,
	    //and the last control transfer was of direction: device to host, see
	    //USBCtrlEPServiceComplete().  If it was already prepared, do not want
	    //to do anything to the BDT.
		USBPrepareForNextSetupTrf();
		if(BothEP0OutUOWNsSet == FALSE)
		{
	        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
	        pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
	        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;			
		}
		else
		{
			BothEP0OutUOWNsSet = FALSE;
		}
    }
}

/******************************************************************************
 * Function:        void USBCtrlTrfInHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine handles an IN transaction according to
 *                  which control transfer state is currently active.
 *
 * Note:            A Set Address Request must not change the acutal address
 *                  of the device until the completion of the control
 *                  transfer. The end of the control transfer for Set Address
 *                  Request is an IN transaction. Therefore it is necessary
 *                  to service this unique situation when the condition is
 *                  right. Macro mUSBCheckAdrPendingState is defined in
 *                  usb9.h and its function is to specifically service this
 *                  event.
 *****************************************************************************/
void USBCtrlTrfInHandler(void)
{
    BYTE lastDTS;

    lastDTS = pBDTEntryIn[0]->STAT.DTS;

    //switch to the next ping pong buffer
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;

    //mUSBCheckAdrPendingState();       // Must check if in ADR_PENDING_STATE
    if(USBDeviceState == ADR_PENDING_STATE)
    {
        U1ADDR = SetupPkt.bDevADR.Val;
        if(U1ADDR > 0)
        {
            USBDeviceState=ADDRESS_STATE;
        }
        else
        {
            USBDeviceState=DEFAULT_STATE;
        }
    }//end if


    if(controlTransferState == CTRL_TRF_TX)
    {
        pBDTEntryIn[0]->ADR = (BYTE *)ConvertToPhysicalAddress(CtrlTrfData);
        USBCtrlTrfTxService();

        /* v2b fix */
        if(shortPacketStatus == SHORT_PKT_SENT)
        {
            // If a short packet has been sent, don't want to send any more,
            // stall next time if host is still trying to read.
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
            if(lastDTS == 0)
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
            }
            else
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT0|_DTSEN;
            }
        }//end if(...)else
    }
	else // must have been a CTRL_TRF_RX status stage IN packet
	{
		USBPrepareForNextSetupTrf();
	}	

}

/********************************************************************
 * Function:        void USBPrepareForNextSetupTrf(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The routine forces EP0 OUT to be ready for a new
 *                  Setup transaction, and forces EP0 IN to be owned
 *                  by CPU.
 *
 * Note:            None
 *******************************************************************/
void USBPrepareForNextSetupTrf(void)
{
    controlTransferState = WAIT_SETUP;
	
	//Don't need to do anything to EP0 OUT BDT here, as EP0 OUT next is
	//already configured and ready to receive a SETUP transaction.  This is
	//done in the USBCtrlTrfOutHandler() or USBCtrlEPServiceComplete() function,
	//depending upon the type of the last control transfer.

    pBDTEntryIn[0]->STAT.Val = _UCPU;    
    {
        BDT_ENTRY* p;

        p = (BDT_ENTRY*)(((unsigned int)pBDTEntryIn[0])^USB_NEXT_EP0_IN_PING_PONG);
        p->STAT.Val = _UCPU;
    }

    //if someone is still expecting data from the control transfer
    //  then make sure to terminate that request and let them know that
    //  they are done
    if(outPipes[0].info.bits.busy == 1)
    {
        if(outPipes[0].pFunc != NULL)
        {
            outPipes[0].pFunc();
        }
        outPipes[0].info.bits.busy = 0;
    }
}//end USBPrepareForNextSetupTrf

/********************************************************************
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
 * Overview:        This routine checks the setup data packet to see
 *                  if it knows how to handle it
 *
 * Note:            None
 *******************************************************************/
void USBCheckStdRequest(void)
{
    if(SetupPkt.RequestType != USB_SETUP_TYPE_STANDARD_BITFIELD) return;

    switch(SetupPkt.bRequest)
    {
        case USB_REQUEST_SET_ADDRESS:
            inPipes[0].info.bits.busy = 1;            // This will generate a zero length packet
            USBDeviceState = ADR_PENDING_STATE;       // Update state only
            /* See USBCtrlTrfInHandler() for the next step */
            break;
        case USB_REQUEST_GET_DESCRIPTOR:
            USBStdGetDscHandler();
            break;
        case USB_REQUEST_SET_CONFIGURATION:
            USBStdSetCfgHandler();
            break;
        case USB_REQUEST_GET_CONFIGURATION:
            inPipes[0].pSrc.bRam = (BYTE*)&USBActiveConfiguration;         // Set Source
            inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
            inPipes[0].wCount.v[0] = 1;                         // Set data count
            inPipes[0].info.bits.busy = 1;
            break;
        case USB_REQUEST_GET_STATUS:
            USBStdGetStatusHandler();
            break;
        case USB_REQUEST_CLEAR_FEATURE:
        case USB_REQUEST_SET_FEATURE:
            USBStdFeatureReqHandler();
            break;
        case USB_REQUEST_GET_INTERFACE:
            inPipes[0].pSrc.bRam = (BYTE*)&USBAlternateInterface[SetupPkt.bIntfID];  // Set source
            inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
            inPipes[0].wCount.v[0] = 1;                         // Set data count
            inPipes[0].info.bits.busy = 1;
            break;
        case USB_REQUEST_SET_INTERFACE:
            inPipes[0].info.bits.busy = 1;
            USBAlternateInterface[SetupPkt.bIntfID] = SetupPkt.bAltID;
            break;
        case USB_REQUEST_SET_DESCRIPTOR:
            USB_SET_DESCRIPTOR_HANDLER(EVENT_SET_DESCRIPTOR,0,0);
            break;
        case USB_REQUEST_SYNCH_FRAME:
        default:
            break;
    }//end switch
}//end USBCheckStdRequest

/********************************************************************
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
 * Overview:        This routine handles the standard SET & CLEAR
 *                  FEATURES requests
 *
 * Note:            None
 *******************************************************************/
void USBStdFeatureReqHandler(void)
{
    BDT_ENTRY *p;
    #if defined(__C32__)
        DWORD* pUEP;
    #else
        unsigned int* pUEP;             
    #endif
#ifdef	USB_SUPPORT_OTG
    if ((SetupPkt.bFeature == OTG_FEATURE_B_HNP_ENABLE)&&
        (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {  
        inPipes[0].info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            USBOTGEnableHnp();
        else
            USBOTGDisableHnp();
    }

    if ((SetupPkt.bFeature == OTG_FEATURE_A_HNP_SUPPORT)&&
        (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {
        inPipes[0].info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            USBOTGEnableSupportHnp();
        else
            USBOTGDisableSupportHnp();
    }


    if ((SetupPkt.bFeature == OTG_FEATURE_A_ALT_HNP_SUPPORT)&&
        (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {
        inPipes[0].info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            USBOTGEnableAltHnp();
        else
            USBOTGDisableAltHnp();
    }
#endif    
    if((SetupPkt.bFeature == USB_FEATURE_DEVICE_REMOTE_WAKEUP)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {
        inPipes[0].info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            RemoteWakeup = TRUE;
        else
            RemoteWakeup = FALSE;
    }//end if

    if((SetupPkt.bFeature == USB_FEATURE_ENDPOINT_HALT)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD)&&
       (SetupPkt.EPNum != 0))
    {
        inPipes[0].info.bits.busy = 1;
        /* Must do address calculation here */

        if(SetupPkt.EPDir == 0)
        {
            p = (BDT_ENTRY*)pBDTEntryOut[SetupPkt.EPNum];
        }
        else
        {
            p = (BDT_ENTRY*)pBDTEntryIn[SetupPkt.EPNum];
        }

		//if it was a SET_FEATURE request
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
        {
			//Then STALL the endpoint
            p->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
			//If it was not a SET_FEATURE
			//point to the appropriate UEP register
            #if defined(__C32__)
                pUEP = (DWORD*)(&U1EP0);
                pUEP += (SetupPkt.EPNum*4);
            #else
                pUEP = (unsigned int*)(&U1EP0+SetupPkt.EPNum);
            #endif

			//Clear the STALL bit in the UEP register
            *pUEP &= ~UEP_STALL;

            if(SetupPkt.EPDir == 1) // IN
            {
				//If the endpoint is an IN endpoint then we
				//  need to return it to the CPU and reset the
				//  DTS bit so that the next transfer is correct
                #if (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)   
                    p->STAT.Val = _UCPU|_DAT0;
                    //toggle over the to the next buffer
                    ((BYTE_VAL*)&p)->Val ^= USB_NEXT_PING_PONG;
                    p->STAT.Val = _UCPU|_DAT1;
                #else
                    p->STAT.Val = _UCPU|_DAT1;
                #endif
            }
            else
            {
				//If the endpoint was an OUT endpoint then we
				//  need to give control of the endpoint back to
				//  the SIE so that the function driver can 
				//  receive the data as they expected.  Also need
				//  to set the DTS bit so the next packet will be
				//  correct
                #if (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)   
                    p->STAT.Val = _USIE|_DAT0|_DTSEN;
                    //toggle over the to the next buffer
                    ((BYTE_VAL*)&p)->Val ^= USB_NEXT_PING_PONG;
                    p->STAT.Val = _USIE|_DAT1|_DTSEN;
                #else
                    p->STAT.Val = _USIE|_DAT1|_DTSEN;
                #endif

            }
        }//end if

    }//end if
}//end USBStdFeatureReqHandler

/********************************************************************
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
 * Overview:        This routine handles the standard GET_DESCRIPTOR
 *                  request.
 *
 * Note:            None
 *******************************************************************/
void USBStdGetDscHandler(void)
{
    if(SetupPkt.bmRequestType == 0x80)
    {
        inPipes[0].info.Val = USB_EP0_ROM | USB_EP0_BUSY | USB_EP0_INCLUDE_ZERO;

        switch(SetupPkt.bDescriptorType)
        {
            case USB_DESCRIPTOR_DEVICE:
                #if !defined(USB_USER_DEVICE_DESCRIPTOR)
                    inPipes[0].pSrc.bRom = (ROM BYTE*)&device_dsc;
                #else
                    inPipes[0].pSrc.bRom = (ROM BYTE*)USB_USER_DEVICE_DESCRIPTOR;
                #endif
                inPipes[0].wCount.Val = sizeof(device_dsc);
                break;
            case USB_DESCRIPTOR_CONFIGURATION:
                #if !defined(USB_USER_CONFIG_DESCRIPTOR)
                    inPipes[0].pSrc.bRom = *(USB_CD_Ptr+SetupPkt.bDscIndex);
                #else
                    inPipes[0].pSrc.bRom = *(USB_USER_CONFIG_DESCRIPTOR+SetupPkt.bDscIndex);
                #endif
                inPipes[0].wCount.Val = *(inPipes[0].pSrc.wRom+1);                // Set data count
                break;
            case USB_DESCRIPTOR_STRING:
                //USB_NUM_STRING_DESCRIPTORS was introduced as optional in release v2.3.  In v2.4 and
                //  later it is now manditory.  This should be defined in usb_config.h and should
                //  indicate the number of string descriptors.
                if(SetupPkt.bDscIndex<USB_NUM_STRING_DESCRIPTORS)
                {
                    //Get a pointer to the String descriptor requested
                    inPipes[0].pSrc.bRom = *(USB_SD_Ptr+SetupPkt.bDscIndex);
                    // Set data count
                    inPipes[0].wCount.Val = *inPipes[0].pSrc.bRom;                    
                }
                else
                {
                    inPipes[0].info.Val = 0;
                }
                break;
            default:
                inPipes[0].info.Val = 0;
                break;
        }//end switch
    }//end if
}//end USBStdGetDscHandler

/********************************************************************
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
 *******************************************************************/
void USBStdGetStatusHandler(void)
{
    CtrlTrfData[0] = 0;                 // Initialize content
    CtrlTrfData[1] = 0;

    switch(SetupPkt.Recipient)
    {
        case USB_SETUP_RECIPIENT_DEVICE_BITFIELD:
            inPipes[0].info.bits.busy = 1;
            /*
             * [0]: bit0: Self-Powered Status [0] Bus-Powered [1] Self-Powered
             *      bit1: RemoteWakeup        [0] Disabled    [1] Enabled
             */
            if(self_power == 1) // self_power is defined in HardwareProfile.h
            {
                CtrlTrfData[0]|=0x01;
            }

            if(RemoteWakeup == TRUE)
            {
                CtrlTrfData[0]|=0x02;
            }
            break;
        case USB_SETUP_RECIPIENT_INTERFACE_BITFIELD:
            inPipes[0].info.bits.busy = 1;     // No data to update
            break;
        case USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD:
            inPipes[0].info.bits.busy = 1;
            /*
             * [0]: bit0: Halt Status [0] Not Halted [1] Halted
             */
            {
                BDT_ENTRY *p;

                if(SetupPkt.EPDir == 0)
                {
                    p = (BDT_ENTRY*)pBDTEntryOut[SetupPkt.EPNum];
                }
                else
                {
                    p = (BDT_ENTRY*)pBDTEntryIn[SetupPkt.EPNum];
                }

                if(p->STAT.Val & _BSTALL)    // Use _BSTALL as a bit mask
                    CtrlTrfData[0]=0x01;    // Set bit0
                break;
            }
    }//end switch

    if(inPipes[0].info.bits.busy == 1)
    {
        inPipes[0].pSrc.bRam = (BYTE*)&CtrlTrfData;            // Set Source
        inPipes[0].info.bits.ctrl_trf_mem = USB_EP0_RAM;               // Set memory type
        inPipes[0].wCount.v[0] = 2;                         // Set data count
    }//end if(...)
}//end USBStdGetStatusHandler

/******************************************************************************
 * Function:        void USBCtrlEPServiceComplete(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine wrap up the remaining tasks in servicing
 *                  a Setup Request. Its main task is to set the endpoint
 *                  controls appropriately for a given situation. See code
 *                  below.
 *                  There are three main scenarios:
 *                  a) There was no handler for the Request, in this case
 *                     a STALL should be sent out.
 *                  b) The host has requested a read control transfer,
 *                     endpoints are required to be setup in a specific way.
 *                  c) The host has requested a write control transfer, or
 *                     a control data stage is not required, endpoints are
 *                     required to be setup in a specific way.
 *
 *                  Packet processing is resumed by clearing PKTDIS bit.
 *
 * Note:            None
 *****************************************************************************/
void USBCtrlEPServiceComplete(void)
{
    /*
     * PKTDIS bit is set when a Setup Transaction is received.
     * Clear to resume packet processing.
     */
    USBPacketDisable = 0;

    if(inPipes[0].info.bits.busy == 0)
    {
        if(outPipes[0].info.bits.busy == 1)
        {
            controlTransferState = CTRL_TRF_RX;
            /*
             * Control Write:
             * <SETUP[0]><OUT[1]><OUT[0]>...<IN[1]> | <SETUP[0]>
             *
             * 1. Prepare IN EP to respond to early termination
             *
             *    This is the same as a Zero Length Packet Response
             *    for control transfer without a data stage
             */
            pBDTEntryIn[0]->CNT = 0;
            pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;

            /*
             * 2. Prepare OUT EP to receive data.
             */
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&CtrlTrfData);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
        }
        else
        {
            /*
             * If no one knows how to service this request then stall.
             * Must also prepare EP0 to receive the next SETUP transaction.
             */
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL; 
        }
    }
    else    // A module has claimed ownership of the control transfer session.
    {
        if(outPipes[0].info.bits.busy == 0)
        {
			if(SetupPkt.DataDir == USB_SETUP_DEVICE_TO_HOST_BITFIELD)
			{
				if(SetupPkt.wLength < inPipes[0].wCount.Val)
				{
					inPipes[0].wCount.Val = SetupPkt.wLength;
				}
				USBCtrlTrfTxService();
				controlTransferState = CTRL_TRF_TX;
				/*
				 * Control Read:
				 * <SETUP[0]><IN[1]><IN[0]>...<OUT[1]> | <SETUP[0]>
				 * 1. Prepare OUT EP to respond to early termination
				 *
				 * NOTE:
				 * If something went wrong during the control transfer,
				 * the last status stage may not be sent by the host.
				 * When this happens, two different things could happen
				 * depending on the host.
				 * a) The host could send out a RESET.
				 * b) The host could send out a new SETUP transaction
				 *    without sending a RESET first.
				 * To properly handle case (b), the OUT EP must be setup
				 * to receive either a zero length OUT transaction, or a
				 * new SETUP transaction.
				 *
				 * Furthermore, the Cnt byte should be set to prepare for
				 * the SETUP data (8-byte or more), and the buffer address
				 * should be pointed to SetupPkt.
				 */
				pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
				pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
				pBDTEntryEP0OutNext->STAT.Val = _USIE;           // Note: DTSEN is 0
				BothEP0OutUOWNsSet = FALSE;	//Indicator flag used in USBCtrlTrfOutHandler()

				#if (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY) || (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG) 
				pBDTEntryEP0OutCurrent->CNT = USB_EP0_BUFF_SIZE;
				pBDTEntryEP0OutCurrent->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
				pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_BSTALL; //Prepare endpoint to accept a SETUP transaction
				BothEP0OutUOWNsSet = TRUE;	//Indicator flag used in USBCtrlTrfOutHandler()
				#endif

				/*
				 * 2. Prepare IN EP to transfer data, Cnt should have
				 *    been initialized by responsible request owner.
				 */
				pBDTEntryIn[0]->ADR = (BYTE*)ConvertToPhysicalAddress(&CtrlTrfData);
				pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
			}
			else   // (SetupPkt.DataDir == USB_SETUP_DIRECTION_HOST_TO_DEVICE)
			{
				controlTransferState = CTRL_TRF_RX;
				/*
				 * Control Write:
				 * <SETUP[0]><OUT[1]><OUT[0]>...<IN[1]> | <SETUP[0]>
				 *
				 * 1. Prepare IN EP to respond to early termination
				 *
				 *    This is the same as a Zero Length Packet Response
				 *    for control transfer without a data stage
				 */
				pBDTEntryIn[0]->CNT = 0;
				pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;

				/*
				 * 2. Prepare OUT EP to receive data.
				 */
				pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
				pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&CtrlTrfData);
				pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
			}
        }
    }//end if(ctrl_trf_session_owner == MUID_NULL)

}//end USBCtrlEPServiceComplete


/******************************************************************************
 * Function:        void USBCtrlTrfTxService(void)
 *
 * PreCondition:    pSrc, wCount, and usb_stat.ctrl_trf_mem are setup properly.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is used for device to host control transfers 
 *					(IN transactions).  This function takes care of managing a
 *                  transfer over multiple USB transactions.
 *					This routine should be called from only two places.
 *                  One from USBCtrlEPServiceComplete() and one from
 *                  USBCtrlTrfInHandler().
 *
 * Note:            This routine works with isochronous endpoint larger than
 *                  256 bytes and is shown here as an example of how to deal
 *                  with BC9 and BC8. In reality, a control endpoint can never
 *                  be larger than 64 bytes.
 *****************************************************************************/
void USBCtrlTrfTxService(void)
{
    WORD_VAL byteToSend;

    /*
     * First, have to figure out how many byte of data to send.
     */
    if(inPipes[0].wCount.Val < USB_EP0_BUFF_SIZE)
    {
        byteToSend.Val = inPipes[0].wCount.Val;

        /* v2b fix */
        if(shortPacketStatus == SHORT_PKT_NOT_USED)
        {
            shortPacketStatus = SHORT_PKT_PENDING;
        }
        else if(shortPacketStatus == SHORT_PKT_PENDING)
        {
            shortPacketStatus = SHORT_PKT_SENT;
        }//end if
        /* end v2b fix for this section */
    }
    else
    {
        byteToSend.Val = USB_EP0_BUFF_SIZE;
    }

    /*
     * Next, load the number of bytes to send to BC9..0 in buffer descriptor
     */
    #if defined(__18CXX)
        pBDTEntryIn[0]->STAT.BC9 = 0;
        pBDTEntryIn[0]->STAT.BC8 = 0;
    #endif
    
    #if defined(__18CXX) || defined(__C30__)
        pBDTEntryIn[0]->STAT.Val |= byteToSend.byte.HB;
        pBDTEntryIn[0]->CNT = byteToSend.byte.LB;
    #elif defined(__C32__)
        pBDTEntryIn[0]->CNT = byteToSend.Val;
    #else
        #error "Not defined for this compiler"
    #endif

    /*
     * Subtract the number of bytes just about to be sent from the total.
     */
    inPipes[0].wCount.Val = inPipes[0].wCount.Val - byteToSend.Val;

    pDst = (USB_VOLATILE BYTE*)CtrlTrfData;        // Set destination pointer

    if(inPipes[0].info.bits.ctrl_trf_mem == USB_EP0_ROM)       // Determine type of memory source
    {
        while(byteToSend.Val)
        {
            *pDst++ = *inPipes[0].pSrc.bRom++;
            byteToSend.Val--;
        }//end while(byte_to_send.Val)
    }
    else  // RAM
    {
        while(byteToSend.Val)
        {
            *pDst++ = *inPipes[0].pSrc.bRam++;
            byteToSend.Val--;
        }//end while(byte_to_send.Val)
    }//end if(usb_stat.ctrl_trf_mem == _ROM)

}//end USBCtrlTrfTxService

/******************************************************************************
 * Function:        void USBCtrlTrfRxService(void)
 *
 * PreCondition:    pDst and wCount are setup properly.
 *                  pSrc is always &CtrlTrfData
 *                  usb_stat.ctrl_trf_mem is always USB_EP0_RAM.
 *                  wCount should be set to 0 at the start of each control
 *                  transfer.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine is used for host to device control transfers
 *					(uses OUT transactions).  This function receives the data that arrives
 *					on EP0 OUT, and copies it into the appropriate outPipes[0].pDst.bRam
 *					buffer.  Once the host has sent all the data it was intending
 *					to send, this function will call the appropriate outPipes[0].pFunc()
 *					handler (unless it is NULL), so that it can be used by the
 *					intended target firmware.
 *
 * Note:            None
 *****************************************************************************/
void USBCtrlTrfRxService(void)
{
    BYTE byteToRead;
    BYTE i;

    byteToRead = pBDTEntryEP0OutCurrent->CNT;

    /*
     * Accumulate total number of bytes read
     */
    if(byteToRead > outPipes[0].wCount.Val)
    {
        byteToRead = outPipes[0].wCount.Val;
    }
    else
    {
        outPipes[0].wCount.Val = outPipes[0].wCount.Val - byteToRead;
    }

    for(i=0;i<byteToRead;i++)
    {
        *outPipes[0].pDst.bRam++ = CtrlTrfData[i];
    }//end while(byteToRead.Val)

    //If there is more data to read
    if(outPipes[0].wCount.Val > 0)
    {
        /*
         * Don't have to worry about overwriting _KEEP bit
         * because if _KEEP was set, TRNIF would not have been
         * generated in the first place.
         */
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&CtrlTrfData);
        if(pBDTEntryEP0OutCurrent->STAT.DTS == 0)
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
        }
        else
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN;
        }
    }
    else
    {
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);
        //Configure EP0 OUT to receive the next SETUP transaction for any future
        //control transfers.  However, set BSTALL in case the host tries to send
        //more data than it claims it was going to send.
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;

        if(outPipes[0].pFunc != NULL)
        {
            outPipes[0].pFunc();
        }
        outPipes[0].info.bits.busy = 0;
    }
}//end USBCtrlTrfRxService

/********************************************************************
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
 * Overview:        This routine first disables all endpoints by
 *                  clearing UEP registers. It then configures
 *                  (initializes) endpoints by calling the callback
 *                  function USBCBInitEP().
 *
 * Note:            None
 *******************************************************************/
void USBStdSetCfgHandler(void)
{
    // This will generate a zero length packet
    inPipes[0].info.bits.busy = 1;            

    //disable all endpoints except endpoint 0
    memset((void*)&U1EP1,0x00,(USB_MAX_EP_NUMBER-1));

    //clear the alternate interface settings
    memset((void*)&USBAlternateInterface,0x00,USB_MAX_NUM_INT);

    //set the current configuration
    USBActiveConfiguration = SetupPkt.bConfigurationValue;

    //if the configuration value == 0
    if(USBActiveConfiguration == 0)
    {
        //Go back to the addressed state
        USBDeviceState = ADDRESS_STATE;
    }
    else
    {
        //Otherwise go to the configured state
        USBDeviceState = CONFIGURED_STATE;
        //initialize the required endpoints
        USB_SET_CONFIGURATION_HANDLER(EVENT_CONFIGURED,(void*)&USBActiveConfiguration,1);

    }//end if(SetupPkt.bConfigurationValue == 0)
}//end USBStdSetCfgHandler

/********************************************************************
 * Function:        void USBConfigureEndpoint(BYTE EPNum, BYTE direction)
 *
 * PreCondition:    None
 *
 * Input:           BYTE EPNum - the endpoint to be configured
 *                  BYTE direction - the direction to be configured
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function will configure the specified 
 *                  endpoint
 *
 * Note:            None
 *******************************************************************/
void USBConfigureEndpoint(BYTE EPNum, BYTE direction)
{
    volatile BDT_ENTRY* handle;

    handle = (volatile BDT_ENTRY*)&BDT[EP0_OUT_EVEN];
    handle += BD(EPNum,direction,0)/sizeof(BDT_ENTRY);

    handle->STAT.UOWN = 0;

    if(direction == 0)
    {
        pBDTEntryOut[EPNum] = handle;
    }
    else
    {
        pBDTEntryIn[EPNum] = handle;
    }

    #if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
        handle->STAT.DTS = 0;
        (handle+1)->STAT.DTS = 1;
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
        //Set DTS to one because the first thing we will do
        //when transmitting is toggle the bit
        handle->STAT.DTS = 1;
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
        if(EPNum != 0)
        {
            handle->STAT.DTS = 1;
        }
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)    
        if(EPNum != 0)
        {
            handle->STAT.DTS = 0;
            (handle+1)->STAT.DTS = 1;
        }
    #endif
}

/*****************************************************************************************************************
  Function:
        void USBEnableEndpoint(BYTE ep, BYTE options)
    
  Summary:
    This function will enable the specified endpoint with the specified
    options
  Description:
    This function will enable the specified endpoint with the specified
    options.
    
    Typical Usage:
    <code>
    void USBCBInitEP(void)
    {
        USBEnableEndpoint(MSD_DATA_IN_EP,USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
        USBMSDInit();
    }
    </code>
    
    In the above example endpoint number MSD_DATA_IN_EP is being configured
    for both IN and OUT traffic with handshaking enabled. Also since
    MSD_DATA_IN_EP is not endpoint 0 (MSD does not allow this), then we can
    explicitly disable SETUP packets on this endpoint.
  Conditions:
    None
  Input:
    BYTE ep -       the endpoint to be configured
    BYTE options -  optional settings for the endpoint. The options should
                    be ORed together to form a single options string. The
                    available optional settings for the endpoint. The
                    options should be ORed together to form a single options
                    string. The available options are the following\:
                    * USB_HANDSHAKE_ENABLED enables USB handshaking (ACK,
                      NAK)
                    * USB_HANDSHAKE_DISABLED disables USB handshaking (ACK,
                      NAK)
                    * USB_OUT_ENABLED enables the out direction
                    * USB_OUT_DISABLED disables the out direction
                    * USB_IN_ENABLED enables the in direction
                    * USB_IN_DISABLED disables the in direction
                    * USB_ALLOW_SETUP enables control transfers
                    * USB_DISALLOW_SETUP disables control transfers
                    * USB_STALL_ENDPOINT STALLs this endpoint
  Return:
    None
  Remarks:
    None                                                                                                          
  *****************************************************************************************************************/
void USBEnableEndpoint(BYTE ep, BYTE options)
{
    //Set the options to the appropriate endpoint control register
    //*((unsigned char*)(&U1EP0+ep)) = options;
    {
        unsigned char* p;

        #if defined(__C32__)
            p = (unsigned char*)(&U1EP0+(4*ep));
        #else
            p = (unsigned char*)(&U1EP0+ep);
        #endif
        *p = options;
    }

    if(options & USB_OUT_ENABLED)
    {
        USBConfigureEndpoint(ep,0);
    }
    if(options & USB_IN_ENABLED)
    {
        USBConfigureEndpoint(ep,1);
    }
}

/********************************************************************
 * Function:        void USBStallEndpoint(BYTE ep, BYTE dir)
 *
 * PreCondition:    None
 *
 * Input:
 *   BYTE ep - the endpoint the data will be transmitted on
 *   BYTE dir - the direction of the transfer
 *
 * Output:          None
 *
 * Side Effects:    Endpoint is STALLed
 *
 * Overview:        STALLs the specified endpoint
 *
 * Note:            None
 *******************************************************************/
void USBStallEndpoint(BYTE ep, BYTE dir)
{
    BDT_ENTRY *p;

    if(ep == 0)
    {
        /*
         * If no one knows how to service this request then stall.
         * Must also prepare EP0 to receive the next SETUP transaction.
         */
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = (BYTE*)ConvertToPhysicalAddress(&SetupPkt);

        /* v2b fix */
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
        pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL; 
    }
    else
    {
        p = (BDT_ENTRY*)(&BDT[EP(ep,dir,0)]);
        p->STAT.Val |= _BSTALL | _USIE;
    
        //If the device is in FULL or ALL_BUT_EP0 ping pong modes
        //then stall that entry as well
        #if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG) || \
            (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0)
    
        p = (BDT_ENTRY*)(&BDT[EP(ep,dir,1)]);
        p->STAT.Val |= _BSTALL | _USIE;
        #endif
    }
}

/********************************************************************
 * Function:        USB_HANDLE USBTransferOnePacket(
 *                      BYTE ep, 
 *                      BYTE dir, 
 *                      BYTE* data, 
 *                      BYTE len)
 *
 * PreCondition:    None
 *
 * Input:
 *   BYTE ep - the endpoint the data will be transmitted on
 *   BYTE dir - the direction of the transfer
                This value is either OUT_FROM_HOST or IN_TO_HOST
 *   BYTE* data - pointer to the data to be sent
 *   BYTE len - length of the data needing to be sent
 *
 * Output:          
 *   USB_HANDLE - handle to the transfer
 *
 * Side Effects:    None
 *
 * Overview:        Transfers one packet over the USB
 *
 * Note:            None
 *******************************************************************/
USB_HANDLE USBTransferOnePacket(BYTE ep,BYTE dir,BYTE* data,BYTE len)
{
    volatile BDT_ENTRY* handle;

    //If the direction is IN
    if(dir != 0)
    {
        //point to the IN BDT of the specified endpoint
        handle = pBDTEntryIn[ep];
    }
    else
    {
        //else point to the OUT BDT of the specified endpoint
        handle = pBDTEntryOut[ep];
    }

    //Toggle the DTS bit if required
    #if (USB_PING_PONG_MODE == USB_PING_PONG__NO_PING_PONG)
        handle->STAT.Val ^= _DTSMASK;
    #elif (USB_PING_PONG_MODE == USB_PING_PONG__EP0_OUT_ONLY)
        if(ep != 0)
        {
            handle->STAT.Val ^= _DTSMASK;
        }
    #endif

    //Set the data pointer, data length, and enable the endpoint
    handle->ADR = (BYTE*)ConvertToPhysicalAddress(data);
    handle->CNT = len;
    handle->STAT.Val &= _DTSMASK;
    handle->STAT.Val |= _USIE | _DTSEN;

    //Point to the next buffer for ping pong purposes.
    if(dir != 0)
    {
        //toggle over the to the next buffer for an IN endpoint
        ((BYTE_VAL*)&pBDTEntryIn[ep])->Val ^= USB_NEXT_PING_PONG;
    }
    else
    {
        //toggle over the to the next buffer for an OUT endpoint
        ((BYTE_VAL*)&pBDTEntryOut[ep])->Val ^= USB_NEXT_PING_PONG;
    }
    return (USB_HANDLE)handle;
}

/**************************************************************************
    Function:
        void USBCancelIO(BYTE endpoint)
    
    Description:
        This function cancels the transfers pending on the specified endpoint.
        This function can only be used after a SETUP packet is received and 
        before that setup packet is handled.  This is the time period in which
        the EVENT_EP0_REQUEST is thrown, before the event handler function
        returns to the stack.

    Precondition:
  
    Parameters:
        BYTE endpoint - the endpoint number you wish to cancel the transfers for
     
    Return Values:
        None
        
    Remarks:
        None
                                                          
  **************************************************************************/
void USBCancelIO(BYTE endpoint)
{
    if(USBPacketDisable == 1)
    {
    	//The PKTDIS bit is currently set right now.  It is therefore "safe"
    	//to mess with the BDT right now.
    	pBDTEntryIn[endpoint]->Val &= _DTSMASK;	//Makes UOWN = 0 (_UCPU mode).  Deactivates endpoint.  Only sends NAKs.
    	pBDTEntryIn[endpoint]->Val ^= _DTSMASK;	//Toggle the DTS bit.  This packet didn't get sent yet, and the next call to USBTransferOnePacket() will re-toggle the DTS bit back to the original (correct) value.
    	
    	//Need to do additional handling if ping-pong buffering is being used
        #if ((USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG) || (USB_PING_PONG_MODE == USB_PING_PONG__ALL_BUT_EP0))
        //Point to the next buffer for ping pong purposes.  UOWN getting cleared
        //(either due to SIE clearing it after a transaction, or the firmware
        //clearing it) makes hardware ping pong pointer advance.
        ((BYTE_VAL*)&pBDTEntryIn[endpoint])->Val ^= USB_NEXT_PING_PONG;
    
    	pBDTEntryIn[endpoint]->STAT.Val &= _DTSMASK;
    	pBDTEntryIn[endpoint]->STAT.Val ^= _DTSMASK;
        #endif
    }
}

/**************************************************************************
    Function:
        void USBDeviceDetach(void)
    
    Description:
                
    Precondition:
        
    Parameters:
        None
     
    Return Values:
        None
        
    Remarks:
        None
                                                          
  **************************************************************************/
#if defined(USB_INTERRUPT)
void USBDeviceDetach(void)
{
    //If the interrupt option is selected then the customer is required
    //  to notify the stack when the device is attached or removed from the
    //  bus by calling the USBDeviceAttach() and USBDeviceDetach() functions.
    if (USB_BUS_SENSE != 1)
    {
         // Disable module & detach from bus
         U1CON = 0;             

         // Mask all USB interrupts              
         U1IE = 0;          

         //Move to the detached state                  
         USBDeviceState = DETACHED_STATE;

         #ifdef  USB_SUPPORT_OTG    
             //Disable D+ Pullup
             U1OTGCONbits.DPPULUP = 0;

             //Disable HNP
             USBOTGDisableHnp();

             //Deactivate HNP
             USBOTGDeactivateHnp();
             
             //If ID Pin Changed State
             if (USBIDIF && USBIDIE)
             {  
                 //Re-detect & Initialize
                  USBOTGInitialize();

                  //Clear ID Interrupt Flag
                  USBClearInterruptFlag(USBIDIFReg,USBIDIFBitNum);
             }
         #endif

         #ifdef __C30__
             //USBClearInterruptFlag(U1OTGIR, 3); 
         #endif
            //return so that we don't go through the rest of 
            //the state machine
          return;
    }

#ifdef USB_SUPPORT_OTG
    //If Session Is Started Then
   else
   {
        //If SRP Is Ready
        if (USBOTGSRPIsReady())
        {   
            //Clear SRPReady
            USBOTGClearSRPReady();

            //Clear SRP Timeout Flag
            USBOTGClearSRPTimeOutFlag();

            //Indicate Session Started
            UART2PrintString( "\r\n***** USB OTG B Event - Session Started  *****\r\n" );
        }
    }
#endif
}
#endif  //#if defined(USB_INTERRUPT)
/**************************************************************************
    Function:
        void USBDeviceAttach(void)
    
    Description:
                
    Precondition:
  		For normal USB devices:
  		Make sure that if the module was previously on, that it has been turned off 
  		for a long time (ex: 100ms+) before calling this function to re-enable the module.
		If the device turns off the D+ (for full speed) or D- (for low speed) ~1.5k ohm
		pull up resistor, and then turns it back on very quickly, common hosts will sometimes 
		reject this event, since no human could ever unplug and reattach a USB device in a 
		microseconds (or nanoseconds) timescale.  The host could simply treat this as some kind 
		of glitch and ignore the event altogether.  
    Parameters:
        None
     
    Return Values:
        None
        
    Remarks:
        None
                                                          
  **************************************************************************/
#if defined(USB_INTERRUPT)
void USBDeviceAttach(void)
{
    //if we are in the detached state
    if(USBDeviceState == DETACHED_STATE)
    {
	    //Initialize registers to known states.
        U1CON = 0;          

        // Mask all USB interrupts
        U1IE = 0;                                

        //Configure things like: pull ups, full/low-speed mode, 
        //set the ping pong mode, and set internal transceiver
        SetConfigurationOptions();

        USBEnableInterrupts();

        // Enable module & attach to bus
        while(!U1CONbits.USBEN){U1CONbits.USBEN = 1;}

        //moved to the attached state
        USBDeviceState = ATTACHED_STATE;

        #ifdef  USB_SUPPORT_OTG
            U1OTGCON = USB_OTG_DPLUS_ENABLE | USB_OTG_ENABLE;  
        #endif
    }
}
#endif  //#if defined(USB_INTERRUPT)
/** EOF USBDevice.c *****************************************************/
