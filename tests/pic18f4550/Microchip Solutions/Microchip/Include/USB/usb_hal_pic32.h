/******************************************************************************

    USB Hardware Abstraction Layer (HAL)  (Header File)

Summary:
    This file abstracts the hardware interface.  The USB stack firmware can be
    compiled to work on different USB microcontrollers, such as PIC18 and PIC24.
    The USB related special function registers and bit names are generally very
    similar between the device families, but small differences in naming exist.

Description:
    This file abstracts the hardware interface.  The USB stack firmware can be
    compiled to work on different USB microcontrollers, such as PIC18 and PIC24.
    The USB related special function registers and bit names are generally very
    similar between the device families, but small differences in naming exist.
    
    In order to make the same set of firmware work accross the device families,
    when modifying SFR contents, a slightly abstracted name is used, which is
    then "mapped" to the appropriate real name in the usb_hal_picxx.h header.
    
    Make sure to include the correct version of the usb_hal_picxx.h file for 
    the microcontroller family which will be used.

    This file is located in the "\<Install Directory\>\\Microchip\\Include\\USB"
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
    
    ..\\..\\Microchip\\Include
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 


*******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************

 File Description:

 This file defines the interface to the USB hardware abstraction layer.

 Filename:        usb_hal.h
 Dependancies:    none
 Processor:       PIC18, PIC24, or PIC32 USB Microcontrollers
 Hardware:        The code is natively intended to be used on the following
     				hardware platforms: PICDEM™ FS USB Demo Board, 
     				PIC18F87J50 FS USB Plug-In Module, or
     				Explorer 16 + PIC24 USB PIM.  The firmware may be
     				modified for use on other USB platforms by editing the
     				HardwareProfile.h file.
 Compiler:        Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:         Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PICmicro® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PICmicro Microcontroller products. The
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

 Change History:
  Rev    Description
  ----   -----------
  2.6    Changed the inplementation of the interrupt clearing macro
         to be more efficient.  

 *************************************************************************/

#if !defined(USB_HAL_PIC32_H)
#if defined(USB_SUPPORT_DEVICE) | defined(USB_SUPPORT_OTG)
#include "Compiler.h"

#define USB_HAL_PIC32_H

#define USBSetBDTAddress(addr)         U1BDTP1 = (((unsigned int)addr)/256);
#define USBPowerModule() U1PWRCbits.USBPWR = 1;
#define USBPingPongBufferReset U1CONbits.PPBRST

#define USBTransactionCompleteIE U1IEbits.TRNIE
#define USBTransactionCompleteIF U1IRbits.TRNIF
#define USBTransactionCompleteIFReg U1IR
#define USBTransactionCompleteIFBitNum 3

#define USBResetIE  U1IEbits.URSTIE
#define USBResetIF  U1IRbits.URSTIF
#define USBResetIFReg U1IR
#define USBResetIFBitNum 0

#define USBIdleIE U1IEbits.IDLEIE
#define USBIdleIF U1IRbits.IDLEIF
#define USBIdleIFReg U1IR
#define USBIdleIFBitNum 4

#define USBActivityIE U1OTGIEbits.ACTVIE
#define USBActivityIF U1OTGIRbits.ACTVIF
#define USBActivityIFReg U1OTGIR
#define USBActivityIFBitNum 4

#define USBSOFIE U1IEbits.SOFIE
#define USBSOFIF U1IRbits.SOFIF
#define USBSOFIFReg U1IR
#define USBSOFIFBitNum 2

#define USBStallIE U1IEbits.STALLIE
#define USBStallIF U1IRbits.STALLIF
#define USBStallIFReg U1IR
#define USBStallIFBitNum 7

#define USBErrorIE U1IEbits.UERRIE
#define USBErrorIF U1IRbits.UERRIF
#define USBErrorIFReg U1IR
#define USBErrorIFBitNum 1

#define USBSE0Event 0// U1IRbits.URSTIF//  U1CONbits.SE0
#define USBSuspendControl U1PWRCbits.USUSPEND
#define USBPacketDisable U1CONbits.PKTDIS
#define USBResumeControl U1CONbits.RESUME

#define USBT1MSECIE U1OTGIEbits.T1MSECIE
#define USBT1MSECIF U1OTGIRbits.T1MSECIF
#define USBT1MSECIFReg U1OTGIR
#define USBT1MSECIFBitNum   6

#define USBIDIE U1OTGIEbits.IDIE
#define USBIDIF U1OTGIRbits.IDIF
#define USBIDIFReg U1OTGIR
#define USBIDIFBitNum   7

#define USB_OTG_ENABLE                0x04
#define USB_OTG_DPLUS_ENABLE    0x80

/* Buffer Descriptor Status Register Initialization Parameters */

//The _BSTALL definition is changed from 0x04 to 0x00 to
// fix a difference in the PIC18 and PIC24 definitions of this
// bit.  This should be changed back once the definitions are
// synced.
#define _BSTALL     0x04        //Buffer Stall enable
#define _DTSEN      0x08        //Data Toggle Synch enable
#define _DAT0       0x00        //DATA0 packet expected next
#define _DAT1       0x40        //DATA1 packet expected next
#define _DTSMASK    0x40        //DTS Mask
#define _USIE       0x80        //SIE owns buffer
#define _UCPU       0x00        //CPU owns buffer

#define _STAT_MASK  0xFC



#define USTAT_EP0_PP_MASK   ~0x04
#define USTAT_EP_MASK       0xFC
#define USTAT_EP0_OUT       0x00
#define USTAT_EP0_OUT_EVEN  0x00
#define USTAT_EP0_OUT_ODD   0x04

#define USTAT_EP0_IN        0x08
#define USTAT_EP0_IN_EVEN   0x08
#define USTAT_EP0_IN_ODD    0x0C
#define ENDPOINT_MASK       0xF0

typedef union
{
    WORD UEP[16];
} _UEP;

#define UEP_STALL 0x0002

typedef union _POINTER
{
    struct
    {
        BYTE bLow;
        BYTE bHigh;
        //byte bUpper;
    };
    WORD _word;                         // bLow & bHigh
    
    //pFunc _pFunc;                       // Usage: ptr.pFunc(); Init: ptr.pFunc = &<Function>;

    BYTE* bRam;                         // Ram byte pointer: 2 bytes pointer pointing
                                        // to 1 byte of data
    WORD* wRam;                         // Ram word poitner: 2 bytes poitner pointing
                                        // to 2 bytes of data

    ROM BYTE* bRom;                     // Size depends on compiler setting
    ROM WORD* wRom;
    //rom near byte* nbRom;               // Near = 2 bytes pointer
    //rom near word* nwRom;
    //rom far byte* fbRom;                // Far = 3 bytes pointer
    //rom far word* fwRom;
} POINTER;

 //******** Depricated: v2.2 - will be removed at some point of time ***
#define _LS         0x00            // Use Low-Speed USB Mode
#define _FS         0x00            // Use Full-Speed USB Mode
#define _TRINT      0x00            // Use internal transceiver
#define _TREXT      0x00            // Use external transceiver
#define _PUEN       0x00            // Use internal pull-up resistor
#define _OEMON      0x00            // Use SIE output indicator
//**********************************************************************

#define USB_PULLUP_ENABLE 0x10
#define USB_PULLUP_DISABLE 0x00

#define USB_INTERNAL_TRANSCEIVER 0x00
#define USB_EXTERNAL_TRANSCEIVER 0x01

#define USB_FULL_SPEED 0x00
//USB_LOW_SPEED not currently supported in PIC24F USB products

#if(USB_PING_PONG_MODE != USB_PING_PONG__FULL_PING_PONG)
    #error "Unsupported ping pong mode for this device"
#endif

#define ConvertToPhysicalAddress(a) KVA_TO_PA(a)

/****************************************************************
    Function:
        void USBModuleDisable(void)
        
    Description:
        This macro is used to disable the USB module
        
    Parameters:
        None
        
    Return Values:
        None
        
    Remarks:
        None
        
  ****************************************************************/
#define USBModuleDisable() {\
    U1CON = 0;\
    U1IE = 0;\
    U1OTGIE = 0;\
    U1PWRCbits.USBPWR = 1;\
    USBDeviceState = DETACHED_STATE;\
}    

/********************************************************************
 * Function (macro): void USBClearInterruptFlag(register, BYTE if_flag_offset)
 *
 * PreCondition:    None
 *
 * Input:           
 *   register - the register mnemonic for the register holding the interrupt 
 *				flag to be "kleared"
 *   BYTE if_flag_offset - the bit position offset (for the interrupt flag to 
 *							"klear") from the "right of the register"
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Klears the specified USB interrupt flag.
 *
 * Note:    		Individual USB interrupt flag bits are "Kleared" by writing
 *					'1' to the bit
 *******************************************************************/
#define USBClearInterruptFlag(reg_name, if_flag_offset)	(reg_name = (1 << if_flag_offset))	


#define USBClearUSBInterrupt() IFS1bits.USBIF = 0;
#if defined(USB_DISABLE_SOF_HANDLER)
    #define USB_SOF_INTERRUPT 0x00
#else
    #define USB_SOF_INTERRUPT 0x04
#endif
#if defined(USB_DISABLE_ERROR_HANDLER)
    #define USB_ERROR_INTERRUPT 0x02
#else
    #define USB_ERROR_INTERRUPT 0x02
#endif

//STALLIE, IDLEIE, TRNIE, and URSTIE are all enabled by default and are required
#define USBEnableInterrupts() {IEC1bits.USBIE = 1;IPC11CLR=0x0000FF00;IPC11SET=0x00001000;INTEnableSystemMultiVectoredInt(); INTEnableInterrupts();}
#if defined(USB_INTERRUPT)
    #define USBMaskInterrupts() {IEC1bits.USBIE = 0;}
    #define USBUnmaskInterrupts() {IEC1bits.USBIE = 1;}
#else
    #define USBMaskInterrupts() 
    #define USBUnmaskInterrupts() 
#endif

    #define EP_CTRL     0x0C            // Cfg Control pipe for this ep
    #define EP_OUT      0x18            // Cfg OUT only pipe for this ep
    #define EP_IN       0x14            // Cfg IN only pipe for this ep
    #define EP_OUT_IN   0x1C            // Cfg both OUT & IN pipes for this ep
    #define HSHK_EN     0x01            // Enable handshake packet
                                    // Handshake should be disable for isoch

    #define USB_HANDSHAKE_ENABLED   0x01
    #define USB_HANDSHAKE_DISABLED  0x00

    #define USB_OUT_ENABLED         0x08
    #define USB_OUT_DISABLED        0x00

    #define USB_IN_ENABLED          0x04
    #define USB_IN_DISABLED         0x00

    #define USB_ALLOW_SETUP         0x00
    #define USB_DISALLOW_SETUP      0x10

    #define USB_STALL_ENDPOINT      0x02

    #define SetConfigurationOptions()   {U1CNFG1 = 0;U1EIE = 0x9F;U1IE = 0x99 | USB_SOF_INTERRUPT | USB_ERROR_INTERRUPT;}

    #define USBClearInterruptRegister(reg) reg = 0xFF;
#endif  //Device or OTG

// Buffer Descriptor Status Register layout.
typedef union __attribute__ ((packed)) _BD_STAT 
{
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    BSTALL  :1;     //Buffer Stall Enable
        unsigned    DTSEN   :1;     //Data Toggle Synch Enable
        unsigned            :2;     //Reserved - write as 00
        unsigned    DTS     :1;     //Data Toggle Synch Value
        unsigned    UOWN    :1;     //USB Ownership
    };
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    PID0    :1;
        unsigned    PID1    :1;
        unsigned    PID2    :1;
        unsigned    PID3    :1;

    };
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    PID     :4;         //Packet Identifier
    };
    WORD           Val;
} BD_STAT;

// BDT Entry Layout
typedef union __attribute__ ((packed))__BDT
{
    struct __attribute__ ((packed))
    {
        BD_STAT     STAT;
        WORD       CNT:10;
        BYTE*       ADR;                      //Buffer Address
    };
    struct __attribute__ ((packed))
    {
        DWORD       res  :16;
        DWORD       count:10;
    };
    DWORD           w[2];
    WORD            v[4];
    QWORD           Val;
} BDT_ENTRY;

#if defined(USB_SUPPORT_DEVICE) | defined(USB_SUPPORT_OTG)
#if !defined(USBDEVICE_C)
    //extern USB_VOLATILE USB_DEVICE_STATE USBDeviceState;
    extern USB_VOLATILE BYTE USBActiveConfiguration;
    extern USB_VOLATILE IN_PIPE inPipes[1];
    extern USB_VOLATILE OUT_PIPE outPipes[1];
    extern volatile BDT_ENTRY *pBDTEntryIn[USB_MAX_EP_NUMBER+1];
#endif
#endif

#endif  //USB_HAL_PIC32_H
