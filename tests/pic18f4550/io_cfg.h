/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.2
 *
 *********************************************************************
 * FileName:        io_cfg.h
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
 * Rawin Rojvanit       05/14/07    Added pin mapping for PIC18F87J50
 *									FS USB Plug In Module board.
 ********************************************************************/

#ifndef IO_CFG_H
#define IO_CFG_H

/** I N C L U D E S *************************************************/
#include "autofiles\usbcfg.h"

/** T R I S *********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

#if defined(PIC18F4550_PICDEM_FS_USB)
/** U S B ***********************************************************/
#define tris_usb_bus_sense  TRISAbits.TRISA1    // Input

#if defined(USE_USB_BUS_SENSE_IO)
#define usb_bus_sense       PORTAbits.RA1
#else
#define usb_bus_sense       1
#endif

#define tris_self_power     TRISAbits.TRISA2    // Input

#if defined(USE_SELF_POWER_SENSE_IO)
#define self_power          PORTAbits.RA2
#else
#define self_power          1
#endif

/** L E D ***********************************************************/
#define mInitAllLEDs()      LATD &= 0xF0; TRISD &= 0xF0;

#define mLED_1              LATDbits.LATD0
#define mLED_2              LATDbits.LATD1
#define mLED_3              LATDbits.LATD2
#define mLED_4              LATDbits.LATD3

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
#define mLED_4_Off()        mLED_4 = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;
#define mLED_4_Toggle()     mLED_4 = !mLED_4;

/** S W I T C H *****************************************************/
#define mInitAllSwitches()  TRISBbits.TRISB4=1;TRISBbits.TRISB5=1;
#define mInitSwitch2()      TRISBbits.TRISB4=1;
#define mInitSwitch3()      TRISBbits.TRISB5=1;
#define sw2                 PORTBbits.RB4
#define sw3                 PORTBbits.RB5

/** P O T ***********************************************************/
#define mInitPOT()          TRISAbits.TRISA0=1;ADCON0=0x01;ADCON2=0xBC;

/** S P I : Chip Select Lines ***************************************/
#define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
#define cs_temp_sensor      LATBbits.LATB2

#define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
#define cs_sdmmc            LATBbits.LATB3

/** S D M M C *******************************************************/
#define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
#define CARD_DETECT         PORTBbits.RB4

#define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
#define WRITE_DETECT        PORTAbits.RA4

/********************************************************************/
/********************************************************************/
/********************************************************************/

#elif defined(PIC18F87J50_FS_USB_PIM)
/** U S B ***********************************************************/
// Bus sense pin is RB5 on PIC18F87J50 FS USB Plug-In Module.
// Must put jumper JP1 in R-U position to use bus sense feature.
#define tris_usb_bus_sense  TRISBbits.TRISB5    // Input

#if defined(USE_USB_BUS_SENSE_IO)
#define usb_bus_sense       PORTBbits.RB5
#else
#define usb_bus_sense       1
#endif

#define self_power          0	

/** L E D ***********************************************************/
#define mInitAllLEDs()      {LATE &= 0xFC; TRISE &= 0xFC; 	\
							 LATD &= 0x00; TRISD &= 0x00;}	// LATD LEDs are on 
															// the HPC Explorer board

#define mLED_1              LATEbits.LATE1 	// Red RE1 LED on PIM board
#define mLED_2              LATEbits.LATE0	// Red RE0 LED on PIM board
#define mLED_3              LATDbits.LATD2	// Located on HPC Explorer board!
#define mLED_4              LATDbits.LATD3	// Located on HPC Explorer board!

#define mLED_1_On()         mLED_1 = 1;
#define mLED_2_On()         mLED_2 = 1;
#define mLED_3_On()         mLED_3 = 1;
#define mLED_4_On()         mLED_4 = 1;

#define mLED_1_Off()        mLED_1 = 0;
#define mLED_2_Off()        mLED_2 = 0;
#define mLED_3_Off()        mLED_3 = 0;
#define mLED_4_Off()        mLED_4 = 0;

#define mLED_1_Toggle()     mLED_1 = !mLED_1;
#define mLED_2_Toggle()     mLED_2 = !mLED_2;
#define mLED_3_Toggle()     mLED_3 = !mLED_3;
#define mLED_4_Toggle()     mLED_4 = !mLED_4;

/** S W I T C H *****************************************************/
#define mInitAllSwitches()  {TRISBbits.TRISB4=1; TRISBbits.TRISB0=1;}
#define mInitSwitch2()      TRISBbits.TRISB4=1;	// On PIC18F87J50 PIM
#define mInitSwitch3()      TRISBbits.TRISB0=1;	// On HPC Explorer

#define sw2                 PORTBbits.RB4		// On PIC18F87J50 PIM
#define sw3					PORTBbits.RB0		// On HPC Explorer

/** P O T ***********************************************************/
#define mInitPOT()          {TRISAbits.TRISA0=1;			\
							 WDTCONbits.ADSHR = 1;			\
							 ANCON0bits.PCFG0 = 1;			\
							 WDTCONbits.ADSHR = 0;			\
							 ADCON0=0x01;					\
							 ADCON1=0xBE;}		// POT on HPC Explorer				

/** I 2 C   T E M P   S E N S E *************************************/
#define	mInitI2CPins()		TRISC |= 0x18;		// RC3 and RC4 are I2C

/********************************************************************/
/********************************************************************/
/********************************************************************/

//Uncomment below if using the YOUR_BOARD hardware platform
//#elif defined(YOUR_BOARD)
//Add your hardware specific I/O pin mapping here

#else
    #error Not a supported board (yet), add I/O pin mapping in __FILE__, line __LINE__
#endif

#endif //IO_CFG_H
