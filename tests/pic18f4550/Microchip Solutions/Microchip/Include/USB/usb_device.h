/*******************************************************************************

    USB Device header file

Summary:
    This file, with its associated C source file, provides the main substance of
    the USB device side stack.  These files will receive, transmit, and process
    various USB commands as well as take action when required for various events
    that occur on the bus.

Description:
    This file, with its associated C source file, provides the main substance of
    the USB device side stack.  These files will receive, transmit, and process
    various USB commands as well as take action when required for various events
    that occur on the bus.

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
    
    ..\\..\\Microchip\\Include

    .
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 

******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************
 FileName:     	usb_device.h
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
*******************************************************************/

/********************************************************************
 Change History:
  Rev    Description
  ----   -----------
  2.1    Added "(" & ")" to EP definitions
         updated for simplicity and to use common
         coding style
  2.6    Removed many of the device specific information to the
         HAL layer files.  Moved many of the CH9 defintions to the
         CH9 file.

********************************************************************/

#ifndef USBDEVICE_H
#define USBDEVICE_H
//DOM-IGNORE-END

/** DEFINITIONS ****************************************************/

//USB_HANDLE is a pointer to an entry in the BDT.  This pointer can be used
//  to read the length of the last transfer, the status of the last transfer,
//  and various other information.  Insure to initialize USB_HANDLE objects
//  to NULL so that they are in a known state during their first usage.
#define USB_HANDLE void*

#define USB_EP0_ROM            0x00     //Data comes from RAM
#define USB_EP0_RAM            0x01     //Data comes from ROM
#define USB_EP0_BUSY           0x80     //The PIPE is busy
#define USB_EP0_INCLUDE_ZERO   0x40     //include a trailing zero packet
#define USB_EP0_NO_DATA        0x00     //no data to send
#define USB_EP0_NO_OPTIONS     0x00     //no options set

/********************************************************************
 * Standard Request Codes
 * USB 2.0 Spec Ref Table 9-4
 *******************************************************************/

/* USB Device States as returned by USBGetDeviceState().  Only the defintions
   for these states should be used.  The actual value for each state should
   not be relied upon as constant and may change based on the implementation. */
typedef enum
{
    /* Detached is the state in which the device is not attached to the bus.  When
    in the detached state a device should not have any pull-ups attached to either
    the D+ or D- line.  */
    DETACHED_STATE 
    /*DOM-IGNORE-BEGIN*/    = 0x00                         /*DOM-IGNORE-END*/,
    /* Attached is the state in which the device is attached ot the bus but the
    hub/port that it is attached to is not yet configured. */
    ATTACHED_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x01                         /*DOM-IGNORE-END*/,
    /* Powered is the state in which the device is attached to the bus and the 
    hub/port that it is attached to is configured. */
    POWERED_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x02                         /*DOM-IGNORE-END*/,
    /* Default state is the state after the device receives a RESET command from
    the host. */
    DEFAULT_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x04                         /*DOM-IGNORE-END*/,
    /* Address pending state is not an official state of the USB defined states.
    This state is internally used to indicate that the device has received a 
    SET_ADDRESS command but has not received the STATUS stage of the transfer yet.
    The device is should not switch addresses until after the STATUS stage is
    complete.  */
    ADR_PENDING_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x08                         /*DOM-IGNORE-END*/,
    /* Address is the state in which the device has its own specific address on the
    bus. */
    ADDRESS_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x10                         /*DOM-IGNORE-END*/,
    /* Configured is the state where the device has been fully enumerated and is
    operating on the bus.  The device is now allowed to excute its application
    specific tasks.  It is also allowed to increase its current consumption to the
    value specified in the configuration descriptor of the current configuration.
    */
    CONFIGURED_STATE
    /*DOM-IGNORE-BEGIN*/    = 0x20                        /*DOM-IGNORE-END*/
} USB_DEVICE_STATE;


/* USB device stack events description here - DWF */
typedef enum
{
    // Notification that a SET_CONFIGURATION() command was received (device)
    EVENT_CONFIGURED
    /*DOM-IGNORE-BEGIN*/    = EVENT_DEVICE_STACK_BASE        /*DOM-IGNORE-END*/,

    // A SET_DESCRIPTOR request was received (device)
    EVENT_SET_DESCRIPTOR,

    // An endpoint 0 request was received that the stack did not know how to
    // handle.  This is most often a request for one of the class drivers.  
    // Please refer to the class driver documenation for information related
    // to what to do if this request is received. (device)
    EVENT_EP0_REQUEST,

//    // A USB transfer has completed.  The data associated with this event is of
//    // the data type HOST_TRANSFER_DATA if the event is generated from the host
//    // stack.
//    EVENT_TRANSFER,
//    
//    // A USB Start of Frame token has been received.  This event is not
//    // used by the Host stack.
//    EVENT_SOF,                  
//    
//    // Device-mode resume received.  This event is not used by the Host stack.
//    EVENT_RESUME,
//    
//    // Device-mode suspend/idle event received.  This event is not used by the
//    // Host stack.
//    EVENT_SUSPEND,
//                  
//    // Device-mode bus reset received.  This event is not used by the Host 
//    // stack.                  
//    EVENT_RESET,                
    
//    // Device Mode: A setup packet received (data: SETUP_PKT).  This event is
//    // not used by the Host stack.
//    EVENT_SETUP,

    // Device-mode USB cable has been attached.  This event is not used by the
    // Host stack.  The client driver may provide an application event when a
    // device attaches.
    EVENT_ATTACH                 

} USB_DEVICE_STACK_EVENTS;

/** Function Prototypes **********************************************/

/**************************************************************************
  Function:
        void USBDeviceTasks(void)
    
  Summary:
    This function is the main state machine of the USB device side stack.
    This function should be called periodically to receive and transmit
    packets through the stack. This function should be called preferably
    once every 100us during the enumeration process. After the enumeration
    process this function still needs to be called periodically to respond
    to various situations on the bus but is more relaxed in its time
    requirements. This function should also be called at least as fast as
    the OUT data expected from the PC.

  Description:
    This function is the main state machine of the USB device side stack.
    This function should be called periodically to receive and transmit
    packets through the stack. This function should be called preferably
    once every 100us during the enumeration process. After the enumeration
    process this function still needs to be called periodically to respond
    to various situations on the bus but is more relaxed in its time
    requirements. This function should also be called at least as fast as
    the OUT data expected from the PC.

    Typical usage:
    <code>
    void main(void)
    {
        USBDeviceInit()
        while(1)
        {
            USBDeviceTasks();
            if((USBGetDeviceState() \< CONFIGURED_STATE) ||
               (USBIsDeviceSuspended() == TRUE))
            {
                //Either the device is not configured or we are suspended
                //  so we don't want to do execute any application code
                continue;   //go back to the top of the while loop
            }
            else
            {
                //Otherwise we are free to run user application code.
                UserApplication();
            }
        }
    }
    </code>

  Conditions:
    None
  Remarks:
    This function should be called preferably once every 100us during the
    enumeration process. After the enumeration process this function still
    needs to be called periodically to respond to various situations on the
    bus but is more relaxed in its time requirements.                      
  **************************************************************************/
void USBDeviceTasks(void);

/**************************************************************************
    Function:
        void USBDeviceInit(void)
    
    Description:
        This function initializes the device stack it in the default state. The
        USB module will be completely reset including all of the internal
        variables, registers, and interrupt flags.
                
    Precondition:
        This function must be called before any of the other USB Device
        functions can be called, including USBDeviceTasks().
        
    Parameters:
        None
     
    Return Values:
        None
        
    Remarks:
        None
                                                          
  **************************************************************************/
void USBDeviceInit(void);

/********************************************************************
  Function:
        BOOL USBGetRemoteWakeupStatus(void)
    
  Summary:
    This function indicates if remote wakeup has been enabled by the host.
    Devices that support remote wakeup should use this function to
    determine if it should send a remote wakeup.

  Description:
    This function indicates if remote wakeup has been enabled by the host.
    Devices that support remote wakeup should use this function to
    determine if it should send a remote wakeup.
    
    If a device does not support remote wakeup (the Remote wakeup bit, bit
    5, of the bmAttributes field of the Configuration descriptor is set to
    1), then it should not send a remote wakeup command to the PC and this
    function is not of any use to the device. If a device does support
    remote wakeup then it should use this function as described below.
    
    If this function returns FALSE and the device is suspended, it should
    not issue a remote wakeup (resume).
    
    If this function returns TRUE and the device is suspended, it should
    issue a remote wakeup (resume).
    
    A device can add remote wakeup support by having the _RWU symbol added
    in the configuration descriptor (located in the usb_descriptors.c file
    in the project). This done in the 8th byte of the configuration
    descriptor. For example:

    <code lang="c">
    ROM BYTE configDescriptor1[]={
        0x09,                           // Size 
        USB_DESCRIPTOR_CONFIGURATION,   // descriptor type 
        DESC_CONFIG_WORD(0x0022),       // Total length 
        1,                              // Number of interfaces 
        1,                              // Index value of this cfg 
        0,                              // Configuration string index 
        _DEFAULT | _SELF | _RWU,        // Attributes, see usb_device.h 
        50,                             // Max power consumption in 2X mA(100mA)
        
        //The rest of the configuration descriptor should follow
    </code>

    For more information about remote wakeup, see the following section of
    the USB v2.0 specification available at www.usb.org: 
        * Section 9.2.5.2
        * Table 9-10 
        * Section 7.1.7.7 
        * Section 9.4.5

  Conditions:
    None

  Return Values:
    TRUE -   Remote Wakeup has been enabled by the host
    FALSE -  Remote Wakeup is not currently enabled

  Remarks:
    None
                                                                                                                                                                                                                                                                                                                       
  *******************************************************************/
BOOL USBGetRemoteWakeupStatus(void);
/*DOM-IGNORE-BEGIN*/
#define USBGetRemoteWakeupStatus() RemoteWakeup
/*DOM-IGNORE-END*/

/***************************************************************************
  Function:
        USB_DEVICE_STATE USBGetDeviceState(void)
    
  Summary:
    This function will return the current state of the device on the USB.
    This function should return CONFIGURED_STATE before an application
    tries to send information on the bus.
  Description:
    This function returns the current state of the device on the USB. This
    \function is used to determine when the device is ready to communicate
    on the bus. Applications should not try to send or receive data until
    this function returns CONFIGURED_STATE.
    
    It is also important that applications yield as much time as possible
    to the USBDeviceTasks() function as possible while the this function
    \returns any value between ATTACHED_STATE through CONFIGURED_STATE.
    
    For more information about the various device states, please refer to
    the USB specification section 9.1 available from www.usb.org.
    
    Typical usage:
    <code>
    void main(void)
    {
        USBDeviceInit()
        while(1)
        {
            USBDeviceTasks();
            if((USBGetDeviceState() \< CONFIGURED_STATE) ||
               (USBIsDeviceSuspended() == TRUE))
            {
                //Either the device is not configured or we are suspended
                //  so we don't want to do execute any application code
                continue;   //go back to the top of the while loop
            }
            else
            {
                //Otherwise we are free to run user application code.
                UserApplication();
            }
        }
    }
    </code>
  Conditions:
    None
  Return Values:
    USB_DEVICE_STATE - the current state of the device on the bus

  Remarks:
    None                                                                    
  ***************************************************************************/
USB_DEVICE_STATE USBGetDeviceState(void);
/*DOM-IGNORE-BEGIN*/
#define USBGetDeviceState() USBDeviceState
/*DOM-IGNORE-END*/

/***************************************************************************
  Function:
        BOOL USBGetSuspendState(void)
    
  Summary:
    This function indicates if this device is currently suspended. When a
    device is suspended it will not be able to transfer data over the bus.
  Description:
    This function indicates if this device is currently suspended. When a
    device is suspended it will not be able to transfer data over the bus.
    This function can be used by the application to skip over section of
    code that do not need to exectute if the device is unable to send data
    over the bus.
    
    Typical usage:
    <code>
       void main(void)
       {
           USBDeviceInit()
           while(1)
           {
               USBDeviceTasks();
               if((USBGetDeviceState() \< CONFIGURED_STATE) ||
                  (USBIsDeviceSuspended() == TRUE))
               {
                   //Either the device is not configured or we are suspended
                   //  so we don't want to do execute any application code
                   continue;   //go back to the top of the while loop
               }
               else
               {
                   //Otherwise we are free to run user application code.
                   UserApplication();
               }
           }
       }
    </code>
  Conditions:
    None
  Return Values:
    TRUE -   this device is suspended.
    FALSE -  this device is not suspended.
  Remarks:
    None                                                                    
  ***************************************************************************/
BOOL USBGetSuspendState(void);

/*******************************************************************************
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
  *****************************************************************************/
void USBEnableEndpoint(BYTE ep, BYTE options);

/*******************************************************************************
  Function:
        BOOL USBIsDeviceSuspended(void)
    
  Summary:
    This function indicates if the USB module is in suspend mode.

  Description:
    This function indicates if the USB module is in suspend mode.  This function
    does NOT indicate that a suspend request has been received.  It only
    reflects the state of the USB module.
   
    Typical Usage:
    <code>
    if(USBIsDeviceSuspended() == TRUE)
    {
        return;
    }
    // otherwise do some application specific tasks
    </code>
    
  Conditions:
    None
  Input:
    None
  Return:
    None
  Remarks:
    None                                                                                                          
  *****************************************************************************/
BOOL USBIsDeviceSuspended(void);
/*DOM-IGNORE-BEGIN*/
#define USBIsDeviceSuspended() USBSuspendControl 
/*DOM-IGNORE-END*/

/*******************************************************************************
  Function:
        void USBSoftDetach(void);
    
  Summary:
    This function performs a detach from the USB bus via software.

  Description:
    This function performs a detach from the USB bus via software.
    
  Conditions:
    None
  Input:
    None
  Return:
    None
  Remarks:
    Caution should be used when detaching from the bus.  Some PC drivers and 
    programs may require additional time after a detach before a device can be 
    reattached to the bus.                                                                                                          
  *****************************************************************************/
void USBSoftDetach(void);
/*DOM-IGNORE-BEGIN*/
#define USBSoftDetach()  U1CON = 0; U1IE = 0; USBDeviceState = DETACHED_STATE;
/*DOM-IGNORE-END*/

/*************************************************************************
  Function:
    USB_HANDLE USBTransferOnePacket(BYTE ep, BYTE dir, BYTE* data, BYTE len)
    
  Summary:
    Transfers a single packed on the USB bus.

  Description:
    Transfers a single packed on the USB bus.

    Typical Usage
    <code>
    //make sure that the last transfer isn't busy by checking the handle
    if(!USBHandleBusy(USBInHandle))
    {
        //Send the data contained in the INPacket[] array out on
        //  endpoint EP_NUM
        USBInHandle = USBTransferOnePacket(EP_NUM,IN_TO_HOST,(BYTE*)&INPacket[0],sizeof(INPacket));
    }
    </code>

  Conditions:
    The user must insure that there isn't currently a transfer pending on the 
    requested endpoint.  This is done by checking the previous request using the
    USBHandleBusy() function (see the typical usage example).

  Input:
    BYTE ep - the endpoint the data will be transmitted on
    BYTE dir - the direction of the transfer
                This value is either OUT_FROM_HOST or IN_TO_HOST
    BYTE* data - pointer to the data to be sent
    BYTE len - length of the data needing to be sent

  Return Values:
    USB_HANDLE - handle to the transfer.

  Remarks:
    None                                                                  
  *************************************************************************/
USB_HANDLE USBTransferOnePacket(BYTE ep,BYTE dir,BYTE* data,BYTE len);

/*************************************************************************
  Function:
    BOOL USBHandleBusy(USB_HANDLE handle)
    
  Summary:
    Checks to see if the input handle is busy

  Description:
    Checks to see if the input handle is busy

    Typical Usage
    <code>
    //make sure that the last transfer isn't busy by checking the handle
    if(!USBHandleBusy(USBGenericInHandle))
    {
        //Send the data contained in the INPacket[] array out on
        //  endpoint USBGEN_EP_NUM
        USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM,(BYTE*)&INPacket[0],sizeof(INPacket));
    }
    </code>

  Conditions:
    None
  Input:
    USB_HANDLE handle -  handle of the transfer that you want to check the
                         status of
  Return Values:
    TRUE -   The specified handle is busy
    FALSE -  The specified handle is free and available for a transfer
  Remarks:
    None                                                                  
  *************************************************************************/
BOOL USBHandleBusy(USB_HANDLE handle);
/*DOM-IGNORE-BEGIN*/
#define USBHandleBusy(handle) (handle==0?0:((volatile BDT_ENTRY*)handle)->STAT.UOWN)
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        WORD USBHandleGetLength(USB_HANDLE handle)
        
    Summary:
        Retrieves the length of the destination buffer of the input
        handle
        
    Description:
        Retrieves the length of the destination buffer of the input
        handle

    PreCondition:
        None
        
    Parameters:
        USB_HANDLE handle - the handle to the transfer you want the
        address for.
        
    Return Values:
        WORD - length of the current buffer that the input handle
        points to.  If the transfer is complete then this is the 
        length of the data transmitted or the length of data
        actually received.
        
    Remarks:
        None
 
 *******************************************************************/
WORD USBHandleGetLength(USB_HANDLE handle);
/*DOM-IGNORE-BEGIN*/
#define USBHandleGetLength(handle) (((volatile BDT_ENTRY*)handle)->CNT)
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        WORD USBHandleGetAddr(USB_HANDLE)
        
    Summary:
        Retrieves the address of the destination buffer of the input
        handle
        
    Description:
        Retrieves the address of the destination buffer of the input
        handle

    PreCondition:
        None
        
    Parameters:
        USB_HANDLE handle - the handle to the transfer you want the
        address for.
        
    Return Values:
        WORD - address of the current buffer that the input handle
        points to.
       
    Remarks:
        None
 
 *******************************************************************/
WORD USBHandleGetAddr(USB_HANDLE);
/*DOM-IGNORE-BEGIN*/
#define USBHandleGetAddr(handle) (((volatile BDT_ENTRY*)handle)->ADR)
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        void USBEP0Transmit(BYTE options)
        
    Summary:
        Sets the address of the data to send over the
        control endpoint
        
    PreCondition:
        None
        
    Paramters:
        options - the various options that you want
                  when sending the control data. Options are:
                       USB_EP0_ROM
                       USB_EP0_RAM
                       USB_EP0_BUSY
                       USB_EP0_INCLUDE_ZERO
                       USB_EP0_NO_DATA
                       USB_EP0_NO_OPTIONS
                       
    Return Values:
        None
    
    Remarks:
        None
 
 *******************************************************************/
void USBEP0Transmit(BYTE options);
/*DOM-IGNORE-BEGIN*/
#define USBEP0Transmit(options) inPipes[0].info.Val = options | USB_EP0_BUSY
/*DOM-IGNORE-END*/

/*************************************************************************
  Function:
        void USBEP0SendRAMPtr(BYTE* src, WORD size, BYTE Options)
    
  Summary:
    Sets the source, size, and options of the data you wish to send from a
    RAM source
  Conditions:
    None
  Input:
    src -      address of the data to send
    size -     the size of the data needing to be transmitted
    options -  the various options that you want when sending the control
               data. Options are\:
               * USB_EP0_ROM
               * USB_EP0_RAM
               * USB_EP0_BUSY
               * USB_EP0_INCLUDE_ZERO
               * USB_EP0_NO_DATA
               * USB_EP0_NO_OPTIONS
  Remarks:
    None                                                                  
  *************************************************************************/
void USBEP0SendRAMPtr(BYTE* src, WORD size, BYTE Options);
/*DOM-IGNORE-BEGIN*/
#define USBEP0SendRAMPtr(src,size,options)  {\
            inPipes[0].pSrc.bRam = src;\
            inPipes[0].wCount.Val = size;\
            inPipes[0].info.Val = options | USB_EP0_BUSY | USB_EP0_RAM;\
            }
/*DOM-IGNORE-END*/

/**************************************************************************
  Function:
        void USBEP0SendROMPtr(BYTE* src, WORD size, BYTE Options)
    
  Summary:
    Sets the source, size, and options of the data you wish to send from a
    ROM source
  Conditions:
    None
  Input:
    src -      address of the data to send
    size -     the size of the data needing to be transmitted
    options -  the various options that you want when sending the control
               data. Options are\:
               * USB_EP0_ROM
               * USB_EP0_RAM
               * USB_EP0_BUSY
               * USB_EP0_INCLUDE_ZERO
               * USB_EP0_NO_DATA
               * USB_EP0_NO_OPTIONS
  Remarks:
    None                                                                   
  **************************************************************************/
void USBEP0SendROMPtr(BYTE* src, WORD size, BYTE Options);
/*DOM-IGNORE-BEGIN*/
#define USBEP0SendROMPtr(src,size,options)  {\
            inPipes[0].pSrc.bRom = src;\
            inPipes[0].wCount.Val = size;\
            inPipes[0].info.Val = options | USB_EP0_BUSY | USB_EP0_ROM;\
            }
/*DOM-IGNORE-END*/

/***************************************************************************
  Function:
    void USBEP0Receive(BYTE* dest, WORD size, void (*function))
  Summary:
    Sets the destination, size, and a function to call on the completion of
    the next control write.
  Conditions:
    None
  Input:
    dest -        address of where the incoming data will go (make sure that this
                  address is directly accessable by the USB module for parts with
                  dedicated USB RAM this address must be in that space)
    size -        the size of the data being received (is almost always going tobe
                  presented by the preceeding setup packet SetupPkt.wLength)
    (*function) - a function that you want called once the data is received. If
                  this is specificed as NULL then no function is called.
  Remarks:
    None                                                                    
  ***************************************************************************/
void USBEP0Receive(BYTE* dest, WORD size, void (*function));
/*DOM-IGNORE-BEGIN*/
#define USBEP0Receive(dest,size,function)  {outPipes[0].pDst.bRam = dest;outPipes[0].wCount.Val = size;outPipes[0].pFunc = function;outPipes[0].info.bits.busy = 1; }
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        USB_HANDLE USBTxOnePacket(BYTE ep, BYTE* data, WORD len)
        
    Summary:
        Sends the specified data out the specified endpoint
        
    PreCondition:
        None
        
    Parameters:
        ep - the endpoint you want to send the data out of
        data - the data that you wish to send
        len - the length of the data that you wish to send
        
    Return Values:
        USB_HANDLE - a handle for the transfer.  This information
        should be kept to track the status of the transfer
        
    Remarks:
        None
  
 *******************************************************************/
USB_HANDLE USBTxOnePacket(BYTE ep, BYTE* data, WORD len);
/*DOM-IGNORE-BEGIN*/
#define USBTxOnePacket(ep,data,len)     USBTransferOnePacket(ep,IN_TO_HOST,data,len)
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        USB_HANDLE USBRxOnePacket(BYTE ep, BYTE* data, WORD len)
        
    Summary:
        Receives the specified data out the specified endpoint
        
    PreCondition:
        None
        
    Parameters:
        ep - the endpoint you want to receive the data into
        data - where the data will go when it arrives
        len - the length of the data that you wish to receive
        
    Return Values:
        None
        
    Remarks:
        None
  
 *******************************************************************/
USB_HANDLE USBRxOnePacket(BYTE ep, BYTE* data, WORD len);
/*DOM-IGNORE-BEGIN*/
#define USBRxOnePacket(ep,data,len)      USBTransferOnePacket(ep,OUT_FROM_HOST,data,len)
/*DOM-IGNORE-END*/

/********************************************************************
    Function:
        void USBStallEndpoint(BYTE ep, BYTE dir)
        
    Summary:
         STALLs the specified endpoint
    
    PreCondition:
        None
        
    Parameters:
        BYTE ep - the endpoint the data will be transmitted on
        BYTE dir - the direction of the transfer
        
    Return Values:
        None
        
    Remarks:
        None

 *******************************************************************/
void USBStallEndpoint(BYTE ep, BYTE dir);

/**************************************************************************
    Function:
        void USBDeviceDetach(void)
   
    Summary:
        This function indicates to the USB module that the USB device has been
        detached from the bus.

    Description:
        This function indicates to the USB module that the USB device has been
        detached from the bus.  This function needs to be called in order for the
        device to start to properly prepare for the next attachment.
   
    Precondition:
        Should only be called when USB_INTERRUPT is defined.

    Parameters:
        None
     
    Return Values:
        None
        
    Remarks:
        None
                                                          
  **************************************************************************/
void USBDeviceDetach(void);

/**************************************************************************
    Function:
        void USBDeviceAttach(void)
    
    Summary:
        This function indicates to the USB module that the USB device has been
        attached to the bus.

    Description:
        This function indicates to the USB module that the USB device has been
        attached to the bus.  This function needs to be called in order for the
        device to start to enumerate on the bus.
                
    Precondition:
        Should only be called when USB_INTERRUPT is defined.

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
****************************************************************************/
void USBDeviceAttach(void);

/*******************************************************************************
  Function:
    BOOL USB_APPLICATION_EVENT_HANDLER(BYTE address, USB_EVENT event, void *pdata, WORD size);
    
  Summary:
    This function is called whenever the USB stack wants to notify the user of
    an event.

  Description:
    This function is called whenever the USB stack wants to notify the user of
    an event.  This function should be implemented by the user.
    
    Example Usage:
  Conditions:
    None

  Input:
    BYTE address -  the address of the device when the event happened
    BYTE event   -  The event input specifies which event happened.  The
                    possible options are listed in the USB_DEVICE_STACK_EVENTS
                    enumeration.

  Return:
    None
  Remarks:
    None                                                                                                          
  *****************************************************************************/
BOOL USB_APPLICATION_EVENT_HANDLER(BYTE address, USB_EVENT event, void *pdata, WORD size);

/*******************************************************************************
  Function:
    ROM void *USBDeviceCBGetDescriptor (UINT16 *length, DESCRIPTOR_ID *id);
    
  Summary:
    This function is called whenever the USB stack gets a USB GET_DESCRIPTOR
    request.

  Description:
    This function is called whenever the USB stack gets a USB GET_DESCRIPTOR
    request.  This function is responsible for returning a pointer to the 
    requested descriptor and setting that the length for the that descriptor.

    This function should be implemented by the user.  This function might be 
    generated automatically by the USB configuration tool.
    
  Conditions:
    None

  Input:
    BYTE *length -  pointer to a variable that should be set to the length of 
                    the requested descriptor.  
    BYTE *id     -  This structure contains information about the requested
                    descriptor

  Return:
    ROM void* - pointer to the requested descriptor.
  Remarks:
    None                                                                                                          
  *****************************************************************************/
ROM void *USBDeviceCBGetDescriptor (UINT16 *length, DESCRIPTOR_ID *id);

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
void USBCancelIO(BYTE endpoint);


/** Section: MACROS ******************************************************/

/* The DESC_CONFIG_WORD() macro is implemented for convinence.  Since the 
    configuration descriptor array is a BYTE array, each entry needs to be a
    BYTE in LSB format.  The DESC_CONFIG_WORD() macro breaks up a WORD into 
    the appropriate BYTE entries in LSB.
    Typical Usage:
    <code>
        ROM BYTE configDescriptor1[]={
            0x09,                           // Size of this descriptor in bytes
            USB_DESCRIPTOR_CONFIGURATION,   // CONFIGURATION descriptor type
            DESC_CONFIG_WORD(0x0022),       // Total length of data for this cfg
    </code>
*/
#define DESC_CONFIG_WORD(a) (a&0xFF),((a>>8)&0xFF)

/* The DESC_CONFIG_DWORD() macro is implemented for convinence.  Since the 
    configuration descriptor array is a BYTE array, each entry needs to be a
    BYTE in LSB format.  The DESC_CONFIG_DWORD() macro breaks up a DWORD into 
    the appropriate BYTE entries in LSB.
*/
#define DESC_CONFIG_DWORD(a) (a&0xFF),((a>>8)&0xFF),((a>>16)&0xFF),((a>>24)&0xFF)

/* The DESC_CONFIG_BYTE() macro is implemented for convinence.  The 
    DESC_CONFIG_BYTE() macro provides a consistant macro for use with a byte
    when generating a configuratin descriptor when using either the 
    DESC_CONFIG_WORD() or DESC_CONFIG_DWORD() macros.
*/
#define DESC_CONFIG_BYTE(a) (a)

















/* DOM-IGNORE-BEGIN */
/*******************************************************************************
********************************************************************************
********************************************************************************
    This section contains implementation specific information that may vary
    between releases as the implementation needs to change.  This section is
    included for compilation reasons only.
********************************************************************************
********************************************************************************
*******************************************************************************/

#if defined(USB_POLLING)
    #define USB_VOLATILE
#else
    #define USB_VOLATILE volatile
#endif

#define CTRL_TRF_RETURN void
#define CTRL_TRF_PARAMS void

// Defintion of the PIPE structure
//  This structure is used to keep track of data that is sent out
//  of the stack automatically.
typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to
        // get the data from
        BYTE *bRam;
        ROM BYTE *bRom;
        WORD *wRam;
        ROM WORD *wRom;
    }pSrc;
    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            //is this transfer from RAM or ROM?
            BYTE ctrl_trf_mem          :1;
            BYTE reserved              :5;
            //include a zero length packet after
            //data is done if data_size%ep_size = 0?
            BYTE includeZero           :1;
            //is this PIPE currently in use
            BYTE busy                  :1;
        }bits;
        BYTE Val;
    }info;
    WORD_VAL wCount;
}IN_PIPE;

extern USB_VOLATILE IN_PIPE inPipes[];

typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to
        // get the data from
        BYTE *bRam;
        WORD *wRam;
    }pDst;
    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            BYTE reserved              :7;
            //is this PIPE currently in use
            BYTE busy                  :1;
        }bits;
        BYTE Val;
    }info;
    WORD_VAL wCount;
    CTRL_TRF_RETURN (*pFunc)(CTRL_TRF_PARAMS);
}OUT_PIPE;

/************* DWF - SHOULD BE REIMPLEMENTED AS AN EVENT *******************/
//#if defined(ENABLE_EP0_DATA_RECEIVED_CALLBACK)
//    void USBCBEP0DataReceived(void);
//    #define USBCB_EP0_DATA_RECEIVED() USBCBEP0DataReceived()
//#else
//    #define USBCB_EP0_DATA_RECEIVED()
//#endif

extern USB_VOLATILE BOOL RemoteWakeup;
extern USB_VOLATILE USB_DEVICE_STATE USBDeviceState;
extern USB_VOLATILE BYTE USBActiveConfiguration;
/******************************************************************************/
/* DOM-IGNORE-END */

#endif //USBD_H
