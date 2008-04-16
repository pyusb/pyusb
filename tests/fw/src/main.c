/*
 * PyUSB Test Firmware
 *
 * Main File
 *
 * Author: Wander Lairson Costa
 */

#include <91x_lib.h>
#include "USB_lib.h"
#include "USB_conf.h"
#include "USB_prop.h"
#include "USB_pwr.h"
#include "USB_mem.h"

static void setup_hardware(void)
{
    /* FMI clock configuration */
   // SCU_FMICLKDivisorConfig(SCU_FMICLK_Div2);

    /* Baudrate clock configuration */
   // SCU_BRCLKDivisorConfig(SCU_BRCLK_Div1);

    /* USB clock configuration */
    SCU_USBCLKConfig(SCU_USBCLK_MCLK2);
    SCU_AHBPeriphClockConfig(__USB48M, ENABLE);
    SCU_AHBPeriphClockConfig(__USB, ENABLE);
    SCU_AHBPeriphReset(__USB, DISABLE);
    
    /* Interrupt controller setup */
    SCU_AHBPeriphClockConfig(__VIC, ENABLE);
    VIC_DeInit();
}

int main(void)
{
    setup_hardware();
    wInterrupt_Mask = IMR_MSK;
    USB_Init();
    for(;;);
}
