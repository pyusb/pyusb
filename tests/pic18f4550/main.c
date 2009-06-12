/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.2
 *
 *********************************************************************
 * FileName:        main.c
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
 * Rawin Rojvanit       08/14/07    A few updates; added #if defined
 *									sections for PIC18F87J50
 *									family devices.
 ********************************************************************/

/*********************************************************************
IMPORTANT NOTE: This code is written to demonstrate the firmware
implementation of a custom device class USB application using the
General Purpose (Custom/Vendor Class) USB Driver which Microchip
distributes.  After programming a device with this code, during the
USB enumeration process, Windows will request a driver.  When
receiving this prompt, point Windows to the mchpusb.inf file which
should be located in the \MCHPFSUSB\Pc\MCHPUSB Driver\Release
directory.

This code is written to work with the PICDEM FS USB Demo Board, or the
PIC18F87J50 FS USB Plug-In Module (when used with the HPC Explorer
board), in conjunction with the PC side "PICDEM FS USB Demo Tool".
This Windows based GUI application is launched with the PDFSUSB.exe
file which should be located in the \MCHPFSUSB\Pc\Pdfsusb directory.

The demo aspects of this code/the PDFSUSB.exe program make use of
4 LEDs, a potentiometer on AN0, and a temperature sensor.  All of the
necessary hardware is built on the PICDEM FS USB Demo Board, but
the PIC18F87J50 FS USB Plug-In Module by itself does not have many
of these items.  Therefore, in order to demonstrate all of the
features when using the PIC18F87J50 FS USB Plug-In Module, it must
be plugged into the HPC Explorer demo board.

By default, the code is configured to work with the PICDEM
FS USB Demo board.  When using this board, nothing needs to be
changed.  If using the PIC18F87J50 FS USB Plug-In Module, make the
following changes:

1.  In MPLAB IDE, click "Configure --> Select Device" and then
	select the PIC18F87J50
2.  From the project window, change the linker script to the 
	18f87j50.lkr file
3.  Open usbcfg.h and uncomment the line that reads,
	"//#define PIC18F87J50_FS_USB_PIM" and then comment the other
	choices.

If using this code for other hardware platforms, follow the above
steps (while selecting the appropriate values), but in step #3,
select the "//#define YOUR_BOARD" section instead.  Then attempt to
build the project.  A number of build errors will deliberately occur,
due to the use of the "#error" compiler directive.  Double click on the
error messages and fill in the missing sections with values appropriate
for your hardware platform.  Some parts of the demo code (such as the
code found in temperature.c, which isn't USB related) may need to
be disabled, if your hardware platform doesn't support these type of
features.
**********************************************************************/


/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "system\typedefs.h"                        // Required
#include "system\usb\usb.h"                         // Required
#include "io_cfg.h"                                 // Required

#include "system\usb\usb_compile_time_validation.h" // Optional

/** C O N F I G U R A T I O N ************************************************/
// Note: For a complete list of the available config pragmas and their values, 
// see the compiler documentation, and/or click "Help --> Topics..." and then 
// select "PIC18 Config Settings" in the Language Tools section.

#if defined(PIC18F4550_PICDEM_FS_USB)		// Configuration bits for PICDEM FS USB Demo Board
        #pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
        #pragma config CPUDIV   = OSC1_PLL2	
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON		//USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
	    #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
	    #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
    	#pragma config CP2      = OFF
	    #pragma config CP3      = OFF
        #pragma config CPB      = OFF
	    #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
	    #pragma config WRT2     = OFF
	    #pragma config WRT3     = OFF
        #pragma config WRTB     = ON       // Boot Block Write Protection
        #pragma config WRTC     = OFF
	    #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
	    #pragma config EBTR2    = OFF
	    #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF


#elif defined(PIC18F87J50_FS_USB_PIM)		// Configuration bits for PIC18F87J50 FS USB Plug-In Module board
        #pragma config XINST    = OFF		// Extended instruction set
        #pragma config STVREN   = ON		// Stack overflow reset
        #pragma config PLLDIV   = 3			// (12 MHz crystal used on this board)
        #pragma config WDTEN    = OFF		// Watch Dog Timer (WDT)
        #pragma config CP0      = OFF		// Code protect
        #pragma config CPUDIV   = OSC1		// OSC1 = divide by 1 mode
        #pragma config IESO     = OFF		// Internal External (clock) Switchover
        #pragma config FCMEN    = OFF		// Fail Safe Clock Monitor
        #pragma config FOSC     = HSPLL		// Firmware must also set OSCTUNE<PLLEN> to start PLL!
        #pragma config WDTPS    = 32768
//      #pragma config WAIT     = OFF		// Commented choices are
//      #pragma config BW       = 16		// only available on the
//      #pragma config MODE     = MM		// 80 pin devices in the 
//      #pragma config EASHFT   = OFF		// family.
        #pragma config MSSPMSK  = MSK5
//      #pragma config PMPMX    = DEFAULT
//      #pragma config ECCPMX   = DEFAULT
        #pragma config CCP2MX   = DEFAULT


//If using the YOUR_BOARD hardware platform (see usbcfg.h), uncomment below and add pragmas
//#elif defined(YOUR_BOARD)
		//Add the configuration pragmas here for your hardware platform
		//#pragma config ... 		= ...

#else
	#error Not a supported board (yet), make sure the proper board is selected in usbcfg.h, and if so, set configuration bits in __FILE__, line __LINE__
#endif

/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
static void InitializeSystem(void);
void USBTasks(void);

/** V E C T O R  R E M A P P I N G *******************************************/

extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code

#pragma code _HIGH_INTERRUPT_VECTOR = 0x000808
void _high_ISR (void)
{
    ;
}

#pragma code _LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
    ;
}
#pragma code

/** D E C L A R A T I O N S **************************************************/
#pragma code
/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *****************************************************************************/
void main(void)
{
    InitializeSystem();
    while(1)
    {
        USBTasks();         // USB Tasks
        //ProcessIO();        // See user\user.c & .h

		USBDriverService();
		BulkRead();
		USBDriverService();
		BulkWrite();
		USBDriverService();
		IntrRead();
		USBDriverService();
		IntrWrite();
		USBDriverService();
    }//end while
}//end main

/******************************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization routine.
 *                  All required USB initialization routines are called from
 *                  here.
 *
 *                  User application initialization routine should also be
 *                  called from here.                  
 *
 * Note:            None
 *****************************************************************************/
static void InitializeSystem(void)
{
	//On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
	//by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
	//This allows the device to power up at a lower initial operating frequency, which can be
	//advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
	//operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
	//power up the PLL.

	#if defined(__18F87J50)||defined(__18F86J55)|| \
    	defined(__18F86J50)||defined(__18F85J50)|| \
    	defined(__18F67J50)||defined(__18F66J55)|| \
    	defined(__18F66J50)||defined(__18F65J50)

    unsigned int pll_startup_counter = 600;
    OSCTUNEbits.PLLEN = 1;  //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
    while(pll_startup_counter--);
    //Device switches over automatically to PLL output after PLL is locked and ready.

	//Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
	//use the ANCONx registers to control this, which is different from other devices which
	//use the ADCON1 register for this purpose.
    WDTCONbits.ADSHR = 1;			// Select alternate SFR location to access ANCONx registers
    ANCON0 = 0xFF;                  // Default all pins to digital
    ANCON1 = 0xFF;                  // Default all pins to digital
    WDTCONbits.ADSHR = 0;			// Select normal SFR locations

    #elif defined(PIC18F4550_PICDEM_FS_USB)
    ADCON1 |= 0x0F;                 // Default all pins to digital

    #else
        #error Double Click this message.  Please make sure the InitializeSystem() function correctly configures your hardware platform.
		//Also make sure the correct board is selected in usbcfg.h.  If 
		//everything is correct, comment out the above "#error ..." line
		//to suppress the error message.
    #endif


//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	usbcfg.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See io_cfg.h
    #endif

//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in usbcfg.h, and that an appropriate I/O pin has been mapped
//	to it in io_cfg.h.    
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;
    #endif
    
    mInitializeUSBDriver();         // See usbdrv.h
//    UserInit();                     // See user.c & .h

}//end InitializeSystem

/******************************************************************************
 * Function:        void USBTasks(void)
 *
 * PreCondition:    InitializeSystem has been called.
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Service loop for USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void USBTasks(void)
{
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

}// end USBTasks

/** EOF main.c ***************************************************************/
