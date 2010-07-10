/******************************************************************************

  Common USB Library Definitions (Header File)

Summary:
    This file defines data types, constants, and macros that are common to
    multiple layers of the Microchip USB Firmware Stack.

Description:
    This file defines data types, constants, and macros that are common to
    multiple layers of the Microchip USB Firmware Stack.

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
    ..\\..\\MicrochipInclude
        
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 
*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************

* FileName:        usb_common.h
* Dependencies:    See included files, below.
* Processor:       PIC18/PIC24/PIC32MX microcontrollers with USB module
* Compiler:        C18 v3.13+/C30 v2.01+/C32 v0.00.18+
* Company:         Microchip Technology, Inc.

Software License Agreement

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

*******************************************************************************/
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  2.6    Moved many of the USB events
********************************************************************/
//DOM-IGNORE-END


//DOM-IGNORE-BEGIN
#ifndef _USB_COMMON_H_
#define _USB_COMMON_H_
//DOM-IGNORE-END

#include <limits.h>

// *****************************************************************************
// *****************************************************************************
// Section: USB Constants
// *****************************************************************************
// *****************************************************************************

// Section: Error Code Values

#define USB_SUCCESS                             0x00    // USB operation successful.
#define USB_INVALID_STATE                       0x01    // Operation cannot be performed in current state.
#define USB_BUSY                                0x02    // A transaction is already in progress.
#define USB_ILLEGAL_REQUEST                     0x03    // Cannot perform requested operation.
#define USB_INVALID_CONFIGURATION               0x04    // Configuration descriptor not found.
#define USB_MEMORY_ALLOCATION_ERROR             0x05    // Out of dynamic memory.
#define USB_UNKNOWN_DEVICE                      0x06    // Device with specified address is not attached.
#define USB_CANNOT_ENUMERATE                    0x07    // Cannot enumerate the attached device.
#define USB_EVENT_QUEUE_FULL                    0x08    // Event queue was full when an event occured.
#define USB_ENDPOINT_BUSY                       0x10    // Endpoint is currently processing a transaction.
#define USB_ENDPOINT_STALLED                    0x11    // Endpoint is currently stalled. User must clear the condition.
#define USB_ENDPOINT_ERROR                      0x12    // Will need more than this eventually
#define USB_ENDPOINT_ERROR_ILLEGAL_PID          0x13    // Illegal PID received.
#define USB_ENDPOINT_NOT_FOUND                  0x14    // Requested endpoint does not exist on device.
#define USB_ENDPOINT_ILLEGAL_DIRECTION          0x15    // Reads must be performe on IN endpoints, writes on OUT endpoints.
//#define USB_ENDPOINT_TRANSACTION_IN_PROGRESS    0x16
#define USB_ENDPOINT_NAK_TIMEOUT                0x17    // Too many NAK's occurred while waiting for the current transaction.
#define USB_ENDPOINT_ILLEGAL_TYPE               0x18    // Transfer type must match endpoint description.
#define USB_ENDPOINT_UNRESOLVED_STATE           0x19    // Endpoint is in an unknown state after completing a transaction.
#define USB_ENDPOINT_ERROR_BIT_STUFF            0x20    // USB Module - Bit stuff error.
#define USB_ENDPOINT_ERROR_DMA                  0x21    // USB Module - DMA error.
#define USB_ENDPOINT_ERROR_TIMEOUT              0x22    // USB Module - Bus timeout.
#define USB_ENDPOINT_ERROR_DATA_FIELD           0x23    // USB Module - Data field size error.
#define USB_ENDPOINT_ERROR_CRC16                0x24    // USB Module - CRC16 failure.
#define USB_ENDPOINT_ERROR_END_OF_FRAME         0x25    // USB Module - End of Frame error.
#define USB_ENDPOINT_ERROR_PID_CHECK            0x26    // USB Module - Illegal PID received.
#define USB_ENDPOINT_ERROR_BMX                  0x27    // USB Module - Bus Matrix error.
#define USB_ERROR_INSUFFICIENT_POWER            0x28    // Too much power was requested

// Section: Return values for USBHostDeviceStatus()

#define USB_DEVICE_STATUS                       0x30                        // Offset for USBHostDeviceStatus() return codes
#define USB_DEVICE_ATTACHED                     (USB_DEVICE_STATUS | 0x30)  // Device is attached and running
#define USB_DEVICE_DETACHED                     (USB_DEVICE_STATUS | 0x01)  // No device is attached
#define USB_DEVICE_ENUMERATING                  (USB_DEVICE_STATUS | 0x02)  // Device is enumerating
#define USB_HOLDING_OUT_OF_MEMORY               (USB_DEVICE_STATUS | 0x03)  // Not enough heap space available
#define USB_HOLDING_UNSUPPORTED_DEVICE          (USB_DEVICE_STATUS | 0x04)  // Invalid configuration or unsupported class
#define USB_HOLDING_UNSUPPORTED_HUB             (USB_DEVICE_STATUS | 0x05)  // Hubs are not supported
#define USB_HOLDING_INVALID_CONFIGURATION       (USB_DEVICE_STATUS | 0x06)  // Invalid configuration requested
#define USB_HOLDING_PROCESSING_CAPACITY         (USB_DEVICE_STATUS | 0x07)  // Processing requirement excessive
#define USB_HOLDING_POWER_REQUIREMENT           (USB_DEVICE_STATUS | 0x08)  // Power requirement excessive
#define USB_HOLDING_CLIENT_INIT_ERROR           (USB_DEVICE_STATUS | 0x09)  // Client driver failed to initialize
#define USB_DEVICE_SUSPENDED                    (USB_DEVICE_STATUS | 0x0A)  // Device is suspended

#define USB_ERROR_CLASS_DEFINED                 0x50    // Offset for application defined errors

#define USB_SINGLE_DEVICE_ADDRESS               0x01    // Default USB device address (single device support)


// *****************************************************************************
// *****************************************************************************
// Section: USB Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Data Transfer Flags

The following flags are used in the flags parameter of the "USBDEVTransferData"
and "USBHALTransferData" routines.  They can be accessed by the bitfield
definitions or the macros can be OR'd together to identify the endpoint number
and properties of the data transfer.

<code>
 7 6 5 4 3 2 1 0 - Field name
 | | | | \_____/
 | | | |    +----- ep_num    - Endpoint number
 | | | +---------- zero_pkt  - End transfer with short or zero-sized packet
 | | +------------ dts       - 0=DATA0 packet, 1=DATA1 packet
 | +-------------- force_dts - Force data toggle sync to match dts field
 +---------------- direction - Transfer direction: 0=Receive, 1=Transmit
</code>
*/

typedef union
{
    BYTE    bitmap;
    struct
    {
        BYTE ep_num:    4;
        BYTE zero_pkt:  1;
        BYTE dts:       1;
        BYTE force_dts: 1;
        BYTE direction: 1;
    }field;

} TRANSFER_FLAGS;

// *****************************************************************************
/* Data Transfer Flags, Endpoint Number Constants

These macros can be used as values for the "ep_num" field of the TRANSFER_FLAGS
data type.
*/
#define USB_EP0        0        //
#define USB_EP1        1        //
#define USB_EP2        2        //
#define USB_EP3        3        //
#define USB_EP4        4        //
#define USB_EP5        5        //
#define USB_EP6        6        //
#define USB_EP7        7        //
#define USB_EP8        8        //
#define USB_EP9        9        //
#define USB_EP10       10       //
#define USB_EP11       11       //
#define USB_EP12       12       //
#define USB_EP13       13       //
#define USB_EP14       14       //
#define USB_EP15       15       //

// *****************************************************************************
/* Data Transfer Flags, Bitmap Constants

These macros can be used as values for the "bitmap" field of the TRANSFER_FLAGS
data type.
*/
#define USB_TRANSMIT        0x80                            // Data will be transmitted to the USB
#define USB_RECEIVE         0x00                            // Data will be received from the USB
#define USB_FORCE_DTS       0x40                            // Forces data toggle sync as below:
#define USB_DTS_MASK        0x20                            // Mask for DTS bit (below)
#define USB_ZERO_PKT        0x10                            // End transfer w/a short or zero-length packet
#define USB_DATA0           0x00|USB_FORCE_DTS              // Force DATA0
#define USB_DATA1           0x20|USB_FORCE_DTS              // Force DATA1
#define USB_SETUP_PKT       USB_RECEIVE|USB_DATA0|USB_EP0   // Setup Packet
#define USB_SETUP_DATA      USB_DATA1|USB_ZERO_PKT|USB_EP0  // Setup-transfer Data Packet
#define USB_SETUP_STATUS    USB_DATA1|USB_EP0               // Setup-transfer Status Packet
#define USB_EP_NUM_MASK     0x0F                            // Endpoint number (ep_num) mask

// *****************************************************************************
/* Data Transfer Flags, Initialization Macro

This macro can be used with the above bitmap constants to initialize a
TRANSFER_FLAGS value.  It provides the correct data type to avoid compiler
warnings.
*/
#define XFLAGS(f) ((TRANSFER_FLAGS)((BYTE)(f)))             // Initialization Macro


// *****************************************************************************
/* USB Events

This enumeration identifies USB events that occur.  It is used to
inform USB drivers and applications of events on the bus.  It is passed
as a parameter to the event-handling routine, which must match the
prototype of the USB_CLIENT_EVENT_HANDLER data type, when an event occurs.
*/

typedef enum
{
    // No event occured (NULL event)
    EVENT_NONE = 0,

    EVENT_DEVICE_STACK_BASE = 1,

    EVENT_HOST_STACK_BASE = 100,

    // A USB hub has been attached.  Hub support is not currently available.
    EVENT_HUB_ATTACH,           
    
    // A stall has occured.  This event is not used by the Host stack.
    EVENT_STALL,                  
    
    // VBus SRP Pulse, (VBus > 2.0v),  Data: BYTE Port Number (For future support)
    EVENT_VBUS_SES_REQUEST,     
    
    // The voltage on Vbus has dropped below 4.4V/4.7V.  The application is 
    // responsible for monitoring Vbus and calling USBHostVbusEvent() with this
    // event.  This event is not generated by the stack.
    EVENT_VBUS_OVERCURRENT,     
    
    // An enumerating device is requesting power.  The data associated with this
    // event is of the data type USB_VBUS_POWER_EVENT_DATA.  Note that 
    // the requested current is specified in 2mA units, identical to the power
    // specification in a device's Configuration Descriptor.
    EVENT_VBUS_REQUEST_POWER,   
    
    // Release power from a detaching device. The data associated with this
    // event is of the data type USB_VBUS_POWER_EVENT_DATA.  The current value
    // specified in the data can be ignored.
    EVENT_VBUS_RELEASE_POWER,   
    
    // The voltage on Vbus is good, and the USB OTG module can be powered on.  
    // The application is responsible for monitoring Vbus and calling 
    // USBHostVbusEvent() with this event.  This event is not generated by the 
    // stack.  If the application issues an EVENT_VBUS_OVERCURRENT, then no
    // power will be applied to that port, and no device can attach to that
    // port, until the application issues the EVENT_VBUS_POWER_AVAILABLE for
    // the port.
    EVENT_VBUS_POWER_AVAILABLE, 
    
    // The attached device is not supported by the application.  The attached
    // device is not allowed to enumerate.
    EVENT_UNSUPPORTED_DEVICE,   
    
    // Cannot enumerate the attached device.  This is generated if communication
    // errors prevent the device from enumerating.
    EVENT_CANNOT_ENUMERATE,     
    
    // The client driver cannot initialize the the attached device.  The 
    // attached is not allowed to enumerate.
    EVENT_CLIENT_INIT_ERROR,    
    
    // The Host stack does not have enough heap space to enumerate the device.
    // Check the amount of heap space allocated to the application.  In MPLAB,
    // select Project> Build Options...> Project.  Select the appropriate
    // linker tab, and inspect the "Heap size" entry.
    EVENT_OUT_OF_MEMORY,        
    
    // Unspecified host error. (This error should not occur).
    EVENT_UNSPECIFIED_ERROR,     
             
    // USB cable has been detached.  The data associated with this event is the
    // address of detached device, a single BYTE.
    EVENT_DETACH, 
     
    // A USB transfer has completed.  The data associated with this event is of
    // the data type HOST_TRANSFER_DATA if the event is generated from the host
    // stack.
    EVENT_TRANSFER,
    
    // A USB Start of Frame token has been received.  This event is not
    // used by the Host stack.
    EVENT_SOF,                  
    
    // Device-mode resume received.  This event is not used by the Host stack.
    EVENT_RESUME,
    
    // Device-mode suspend/idle event received.  This event is not used by the
    // Host stack.
    EVENT_SUSPEND,
                  
    // Device-mode bus reset received.  This event is not used by the Host 
    // stack.                  
    EVENT_RESET,  

    // Class-defined event offsets start here:
    EVENT_GENERIC_BASE  = 400,      // Offset for Generic class events

    EVENT_MSD_BASE      = 500,      // Offset for Mass Storage Device class events

    EVENT_HID_BASE      = 600,      // Offset for Human Interface Device class events

    EVENT_PRINTER_BASE  = 700,      // Offset for Printer class events
    
    EVENT_CDC_BASE      = 800,      // Offset for CDC class events

    EVENT_CHARGER_BASE  = 900,      // Offset for Charger client driver events.

    EVENT_AUDIO_BASE    = 1000,      // Offset for Audio client driver events.
        
	EVENT_USER_BASE     = 10000,    // Add integral values to this event number
                                    // to create user-defined events.

    // There was a transfer error on the USB.  The data associated with this
    // event is of data type HOST_TRANSFER_DATA.
    EVENT_BUS_ERROR     = UINT_MAX  

} USB_EVENT;


// *****************************************************************************
/* EVENT_TRANSFER Data

This data structure is passed to the appropriate layer's
USB_EVENT_HANDLER when an EVT_XFER event has occured, indicating
that a transfer has completed on the USB.  It provides the endpoint,
direction, and actual size of the transfer.
 */

typedef struct _transfer_event_data
{
    TRANSFER_FLAGS  flags;          // Transfer flags (see above)
    UINT32          size;           // Actual number of bytes transferred
    BYTE            pid;            // Packet ID

} USB_TRANSFER_EVENT_DATA;


// *****************************************************************************
/* EVENT_VBUS_REQUEST_POWER and EVENT_VBUS_RELEASE_POWER Data

This data structure is passed to the appropriate layer's
USB_EVENT_HANDLER when an EVENT_VBUS_REQUEST_POWER or EVENT_VBUS_RELEASE_POWER
event has occured, indicating that a change in Vbus power is being requested.
*/

typedef struct _vbus_power_data
{
    BYTE            port;           // Physical port number
    BYTE            current;        // Current in 2mA units
} USB_VBUS_POWER_EVENT_DATA;


// *****************************************************************************
/* EVT_STALL Data

The EVT_STALL event has a 16-bit data value associated with it where
a bit is set in the position for each endpoint that is currently
stalled (ie. bit 0 = EP0, bit 1 = EP1, etc.)
*/


// *****************************************************************************
// *****************************************************************************
// Section: Event  Handling Routines
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
    Function:
        BOOL <Event-handling Function Name> ( USB_EVENT event,
              void *data, unsigned int size )

    Description:
        This routine is a "call out" routine that must be implemented by
        any layer of the USB SW Stack (except the HAL which is at the
        root of the event-call tree that needs to receive events.  When
        an event occurs, the HAL calls the next higher layer in the
        stack to handle the event.  Each layer either handles the event
        or calls the layer above it to handle the event.  Events are
        identified by the "event" parameter and may have associated
        data.  If the higher layer was able to handle the event, it
        should return TRUE.  If not, it should return FALSE.
        
    Preconditions:
        USBInitialize must have been called to initialize the USB SW
        Stack.
        
    Paramters:
        USB_EVENT event   - Identifies the bus event that occured
        void *data        - Pointer to event-specific data
        unsigned int size - Size of the event-specific data
        
    Return Values:
        None
        
    Remarks:
        The function is name is defined by the layer that implements
        it.  A pointer to the function will be placed by into a table
        that the lower-layer will use to call it.  This requires the
        function to use a specific call "signature" (return data type
        and values and data parameter types and values).
 
*******************************************************************************/

typedef BOOL (*USB_EVENT_HANDLER) ( USB_EVENT event, void *data, unsigned int size );


// *****************************************************************************
// *****************************************************************************
// Section: USB Application Program Interface (API) Routines
// *****************************************************************************
// *****************************************************************************

/****************************************************************************
    Function:
        BOOL USBInitialize ( unsigned long flags )

    Summary:
        This interface initializes the variables of the USB host stack.

    Description:
        This interface initializes the USB stack.

    Precondition:
        None

    Parameters:
        flags - reserved

    Return Values:
        TRUE  - Initialization successful
        FALSE - Initialization failure

    Remarks:
        This interface is implemented as a macro that can be defined by the
        application or by default is defined correctly for the stack mode.
        
  ***************************************************************************/

#ifndef USBInitialize   
    #if defined( USB_SUPPORT_DEVICE )
        #if defined( USB_SUPPORT_HOST )
            #if defined( USB_SUPPORT_OTG )
                #error "USB OTG is not yet supported."
            #else
                #define USBInitialize(f) \
                        (USBDEVInitialize(f) && USBHostInit(f)) ? \
                        TRUE : FALSE
            #endif
        #else
            #define USBInitialize(f) USBDeviceInit()
        #endif
    #else
        #if defined( USB_SUPPORT_HOST )
            #define USBInitialize(f) USBHostInit(f)
        #else
            #error "Application must define support mode in usb_config.h"
        #endif
    #endif
#endif


/****************************************************************************
    Function:
        void USBTasks( void )

    Summary:
        This function executes the tasks for USB operation.

    Description:
        This function executes the tasks for USB host operation.  It must be
        executed on a regular basis to keep everything functioning.

    Precondition:
        USBInitialize() has been called.

    Parameters:
        None

    Returns:
        None

    Remarks:
        This interface is implemented as a macro that can be defined by the
        application or by default is defined correctly for the stack mode.
        
  ***************************************************************************/

#ifndef USBTasks    // Implemented as a macro that can be overridden.
    #if defined( USB_SUPPORT_DEVICE )
        #if defined( USB_SUPPORT_HOST )
            #if defined( USB_SUPPORT_OTG )
                #error "USB OTG is not yet supported."
            #else
                #define USBTasks() {USBHostTasks(); USBHALHandleBusEvent();}
            #endif
        #else
            #define USBTasks() USBDeviceTasks()
        #endif
    #else
        #if defined( USB_SUPPORT_HOST )
            #define USBTasks() USBHostTasks()
        #else
            #error "Application must define support mode in usb_config.h"
        #endif
    #endif
#endif

#define USB_PING_PONG__NO_PING_PONG         0x00    //0b00
#define USB_PING_PONG__EP0_OUT_ONLY         0x01    //0b01
#define USB_PING_PONG__FULL_PING_PONG       0x02    //0b10
#define USB_PING_PONG__ALL_BUT_EP0          0x03    //0b11

#endif // _USB_COMMON_H_
/*************************************************************************
 * EOF
 */

