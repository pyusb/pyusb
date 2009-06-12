/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbmmap.c
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

/******************************************************************************
 * -usbmmap.c-
 * USB Memory Map
 * This file is the USB memory manager; it serves as a compile-time memory
 * allocator for the USB endpoints. It uses the compile time options passed
 * from usbcfg.h to instantiate endpoints and endpoint buffer.
 *
 * Each endpoint requires to have a set of Buffer Descriptor registers(BDT).
 * A BDT is 4-byte long and has a specific RAM location for each endpoint.
 * The BDT for endpoint 0 out is located at address 0x400 to 0x403.
 * The BDT for endpoint 0 in is located at address 0x404 to 0x407.
 * The BDT for endpoint 1 out is located at address 0x408 to 0x40B.
 * and so on... The above allocation assumes the Ping-Pong Buffer Mode 0 is
 * used. These locations are already hard-wired in the silicon. The point
 * of doing instantiation, i.e. volatile far BDT ep0Bo;, is to provide the
 * C compiler a way to address each variable directly. This is very important
 * because when a register can be accessed directly, it saves execution time
 * and reduces program size.
 * 
 * Endpoints are defined using the endpoint number and the direction
 * of transfer. For simplicity, usbmmap.c only uses the endpoint number
 * in the BDT register allocation scheme. This means if the usbcfg.h states
 * that the MAX_EP_NUMBER is number 1, then four BDTs will be
 * instantiated: one each for endpoint0 in and endpoint0 out, which must
 * always be instantiated for control transfer by default, and one each sets
 * for endpoint1 in and endpoint1 out. The naming convention for instantiating
 * BDT is
 * 
 * ep<#>B<d>
 *
 * where # is the endpoint number, and d is the direction of
 * transfer, which could be either <i> or <o>.
 *
 * The USB memory manager uses MAX_EP_NUMBER, as defined in usbcfg.h, to define
 * the endpoints to be instantiated. This represents the highest endpoint
 * number to be allocated, not how many endpoints are used. Since the BDTs for
 * endpoints have hardware-assigned addresses in Bank 4, setting this value too
 * high may lead to inefficient use of data RAM. For example, if an application
 * uses only endpoints EP0 and EP4, then the MAX_EP_NUMBER is 4, and not 2.
 * The in-between endpoint BDTs in this example (EP1, EP2, and EP3) go unused,
 * and the 24 bytes of memory associated with them are wasted. It does not make
 * much sense to skip endpoints, but the final decision lies with the user.
 *
 * The next step is to assign the instantiated BDTs to different
 * USB functions. The firmware framework fundamentally assumes that every USB
 * function should know which endpoint it is using, i.e., the default control
 * transfer should know that it is using endpoint 0 in and endpoint 0 out.
 * A HID class can choose which endpoint it wants to use, but once chosen, it
 * should always know the number of the endpoint.
 *
 * The assignment of endpoints to USB functions is managed centrally
 * in usbcfg.h. This helps prevent the mistake of having more
 * than one USB function using the same endpoint. The "Endpoint Allocation"
 * section in usbcfg.h provides examples for how to map USB endpoints to USB
 * functions.
 * Quite a few things can be mapped in that section. There is no
 * one correct way to do the mapping and the user has the choice to
 * choose a method that is most suitable to the application.
 *
 * Typically, however, a user will want to map the following for a given
 * USB interface function:
 * 1. The USB interface ID
 * 2. The endpoint control registers (UEPn)
 * 3. The BDT registers (ep<#>B<d>)
 * 4. The endpoint size
 *
 * Example: Assume a USB device class "foo", which uses one out endpoint
 *          of size 64-byte and one in endpoint of size 64-byte, then:
 *
 * #define FOO_INTF_ID          0x00
 * #define FOO_UEP              UEP1
 * #define FOO_BD_OUT           ep1Bo
 * #define FOO_BD_IN            ep1Bi
 * #define FOO_EP_SIZE          64
 *
 * The mapping above has chosen class "foo" to use endpoint 1.
 * The names are arbitrary and can be anything other than FOO_??????.
 * For abstraction, the code for class "foo" should use the abstract
 * definitions of FOO_BD_OUT,FOO_BD_IN, and not ep1Bo or ep1Bi.
 *
 * Note that the endpoint size defined in the usbcfg.h file is again
 * used in the usbmmap.c file. This shows that the relationship between
 * the two files are tightly related.
 * 
 * The endpoint buffer for each USB function must be located in the
 * dual-port RAM area and has to come after all the BDTs have been
 * instantiated. An example declaration is:
 * volatile far unsigned char[FOO_EP_SIZE] data;
 *
 * The 'volatile' keyword tells the compiler not to perform any code
 * optimization on this variable because its content could be modified
 * by the hardware. The 'far' keyword tells the compiler that this variable
 * is not located in the Access RAM area (0x000 - 0x05F).
 *
 * For the variable to be globally accessible by other files, it should be
 * declared in the header file usbmmap.h as an extern definition, such as
 * extern volatile far unsigned char[FOO_EP_SIZE] data;
 *
 * Conclusion:
 * In a short summary, the dependencies between usbcfg and usbmmap can
 * be shown as:
 *
 * usbcfg[MAX_EP_NUMBER] -> usbmmap
 * usbmmap[ep<#>B<d>] -> usbcfg
 * usbcfg[EP size] -> usbmmap
 * usbcfg[abstract ep definitions] -> usb9/hid/cdc/etc class code
 * usbmmap[endpoint buffer variable] -> usb9/hid/cdc/etc class code
 *
 * Data mapping provides a means for direct addressing of BDT and endpoint
 * buffer. This means less usage of pointers, which equates to a faster and
 * smaller program code.
 *
 *****************************************************************************/
 
/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"
#include "system\usb\usb.h"

/** U S B  G L O B A L  V A R I A B L E S ************************************/
#pragma udata
byte usb_device_state;          // Device States: DETACHED, ATTACHED, ...
USB_DEVICE_STATUS usb_stat;     // Global USB flags
byte usb_active_cfg;            // Value of current configuration
byte usb_alt_intf[MAX_NUM_INT]; // Array to keep track of the current alternate
                                // setting for each interface ID

/** U S B  F I X E D  L O C A T I O N  V A R I A B L E S *********************/
#pragma udata usbram4=0x400     //See Linker Script,usb4:0x400-0x4FF(256-byte)

/******************************************************************************
 * Section A: Buffer Descriptor Table
 * - 0x400 - 0x4FF(max)
 * - MAX_EP_NUMBER is defined in autofiles\usbcfg.h
 * - BDT data type is defined in system\usb\usbmmap.h
 *****************************************************************************/

#if(0 <= MAX_EP_NUMBER)
volatile far BDT ep0Bo;         //Endpoint #0 BD Out
volatile far BDT ep0Bi;         //Endpoint #0 BD In
#endif

#if(1 <= MAX_EP_NUMBER)
volatile far BDT ep1Bo;         //Endpoint #1 BD Out
volatile far BDT ep1Bi;         //Endpoint #1 BD In
#endif

#if(2 <= MAX_EP_NUMBER)
volatile far BDT ep2Bo;         //Endpoint #2 BD Out
volatile far BDT ep2Bi;         //Endpoint #2 BD In
#endif

#if(3 <= MAX_EP_NUMBER)
volatile far BDT ep3Bo;         //Endpoint #3 BD Out
volatile far BDT ep3Bi;         //Endpoint #3 BD In
#endif

#if(4 <= MAX_EP_NUMBER)
volatile far BDT ep4Bo;         //Endpoint #4 BD Out
volatile far BDT ep4Bi;         //Endpoint #4 BD In
#endif

#if(5 <= MAX_EP_NUMBER)
volatile far BDT ep5Bo;         //Endpoint #5 BD Out
volatile far BDT ep5Bi;         //Endpoint #5 BD In
#endif

#if(6 <= MAX_EP_NUMBER)
volatile far BDT ep6Bo;         //Endpoint #6 BD Out
volatile far BDT ep6Bi;         //Endpoint #6 BD In
#endif

#if(7 <= MAX_EP_NUMBER)
volatile far BDT ep7Bo;         //Endpoint #7 BD Out
volatile far BDT ep7Bi;         //Endpoint #7 BD In
#endif

#if(8 <= MAX_EP_NUMBER)
volatile far BDT ep8Bo;         //Endpoint #8 BD Out
volatile far BDT ep8Bi;         //Endpoint #8 BD In
#endif

#if(9 <= MAX_EP_NUMBER)
volatile far BDT ep9Bo;         //Endpoint #9 BD Out
volatile far BDT ep9Bi;         //Endpoint #9 BD In
#endif

#if(10 <= MAX_EP_NUMBER)
volatile far BDT ep10Bo;        //Endpoint #10 BD Out
volatile far BDT ep10Bi;        //Endpoint #10 BD In
#endif

#if(11 <= MAX_EP_NUMBER)
volatile far BDT ep11Bo;        //Endpoint #11 BD Out
volatile far BDT ep11Bi;        //Endpoint #11 BD In
#endif

#if(12 <= MAX_EP_NUMBER)
volatile far BDT ep12Bo;        //Endpoint #12 BD Out
volatile far BDT ep12Bi;        //Endpoint #12 BD In
#endif

#if(13 <= MAX_EP_NUMBER)
volatile far BDT ep13Bo;        //Endpoint #13 BD Out
volatile far BDT ep13Bi;        //Endpoint #13 BD In
#endif

#if(14 <= MAX_EP_NUMBER)
volatile far BDT ep14Bo;        //Endpoint #14 BD Out
volatile far BDT ep14Bi;        //Endpoint #14 BD In
#endif

#if(15 <= MAX_EP_NUMBER)
volatile far BDT ep15Bo;        //Endpoint #15 BD Out
volatile far BDT ep15Bi;        //Endpoint #15 BD In
#endif

/******************************************************************************
 * Section B: EP0 Buffer Space
 ******************************************************************************
 * - Two buffer areas are defined:
 *
 *   A. CTRL_TRF_SETUP
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Detailed data structure allows direct adddressing of bits and bytes.
 *
 *   B. CTRL_TRF_DATA
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Data structure allows direct adddressing of the first 8 bytes.
 *
 * - Both data types are defined in system\usb\usbdefs\usbdefs_ep0_buff.h
 *****************************************************************************/
volatile far CTRL_TRF_SETUP SetupPkt;
volatile far CTRL_TRF_DATA CtrlTrfData;

/******************************************************************************
 * Section C: Buffer
 ******************************************************************************
 *
 *****************************************************************************/

#pragma udata usbram5=0x500

volatile far byte bulk_out[PYUSB_EP_SIZE];
volatile far byte bulk_in[PYUSB_EP_SIZE];

volatile far byte intr_out[PYUSB_EP_SIZE];
volatile far byte intr_in[PYUSB_EP_SIZE];

#pragma udata usbram6=0x600

volatile far byte iso_out[PYUSB_EP_SIZE];
volatile far byte iso_in[PYUSB_EP_SIZE];

#pragma udata

/** EOF usbmmap.c ************************************************************/
