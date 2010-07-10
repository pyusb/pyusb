#ifndef _BBT_TRANSFER_H
#define _BBT_TRANSFER_H

#include "USB/usb.h"
#include "USB/usb_function_generic.h"
#include "HardwareProfile.h"

// Externs from usb_device.c
extern volatile BDT_ENTRY* pBDTEntryOut[USB_MAX_EP_NUMBER+1];
extern volatile BDT_ENTRY* pBDTEntryIn[USB_MAX_EP_NUMBER+1];

/////////////////////////
// BDT transfer macros //
/////////////////////////
//Toggle the DTS bit if required
#if ((USB_PING_PONG_MODE==USB_PING_PONG__FULL_PING_PONG) || (USB_PING_PONG_MODE==USB_PING_PONG__ALL_BUT_EP0))
	#define PP_COUNT (2)

	// Copied from usb_device.h ///////////////
	#if defined (__18CXX) || defined(__C30__)
		#define USB_NEXT_PING_PONG 0x0004
	#elif defined(__C32__)
		#define USB_NEXT_PING_PONG 0x0008
	#else
		#error "Not defined for this compiler"
	#endif
	///////////////////////////////////////////

	#define mBDT_MaskAndToggleDTS(BdtPtr)  (BdtPtr->STAT.Val & _DTSMASK)
	#define mBDT_IsOdd(BdtPtr)             ((((BYTE_VAL*)&BdtPtr)->Val & USB_NEXT_PING_PONG)?1:0)
	#define mBDT_TogglePP(BdtPtr)          ((BYTE_VAL*)&BdtPtr)->Val ^= USB_NEXT_PING_PONG

#else
	#define PP_COUNT (1)

	#define mBDT_MaskAndToggleDTS(BdtPtr)  ((BdtPtr->STAT.Val & _DTSMASK) ^ _DTSMASK)
	#define mBDT_IsOdd(BdtPtr)             (0)
	#define mBDT_TogglePP(BdtPtr)
#endif

#if defined(__C32__)
	#define mBDT_FillTransfer(BdtPtr, BufferPtr, BufferLength)	\
	{															\
	    BdtPtr->ADR = ConvertToPhysicalAddress(BufferPtr);		\
	    BdtPtr->CNT = BufferLength;								\
	}
	
	#define mBDT_GetLength(BdtPtr) (BdtPtr->CNT)

#else
	#define mBDT_FillTransfer(BdtPtr, BufferPtr, BufferLength)	\
	{															\
	    BdtPtr->ADR = ConvertToPhysicalAddress(BufferPtr);		\
	    BdtPtr->CNT = (BYTE)BufferLength;						\
	}

	#define mBDT_GetLength(BdtPtr) (BdtPtr->CNT + ((BdtPtr->STAT.Val << 8) & 0x300))

#endif

// Masks STAT with DTS, Set _USIE, _DTSEN
#define mBDT_SubmitTransfer(BdtPtr) BdtPtr->STAT.Val = (mBDT_MaskAndToggleDTS(BdtPtr)) | _USIE | _DTSEN

#if defined (__18CXX) || defined(__C30__)
	// Sets BC8 & BC9 with high transfer length bits, _USIE
	#define mBDT_SubmitIsoTransfer(BdtPtr, BufferLength) BdtPtr->STAT.Val = (((BufferLength >> 8) & 0x3) | _USIE)
#elif defined(__C32__)
	// Sets _USIE (CNT is 16bit for PIC32)
	#define mBDT_SubmitIsoTransfer(BdtPtr, BufferLength) BdtPtr->STAT.Val = _USIE
#else
    #error "Not defined for this compiler"
#endif

#if USB_MAX_EP_NUMBER > 0
	#define pBdtRxEp1	pBDTEntryOut[1]
	#define pBdtOutEp1	pBDTEntryOut[1]
	#define pBdtTxEp1	pBDTEntryIn[1]
	#define pBdtInEp1	pBDTEntryIn[1]
#endif

#if USB_MAX_EP_NUMBER > 1
	#define pBdtRxEp2	pBDTEntryOut[2]
	#define pBdtOutEp2	pBDTEntryOut[2]
	#define pBdtTxEp2	pBDTEntryIn[2]
	#define pBdtInEp2	pBDTEntryIn[2]
#endif

#if USB_MAX_EP_NUMBER > 2
	#define pBdtRxEp3	pBDTEntryOut[3]
	#define pBdtOutEp3	pBDTEntryOut[3]
	#define pBdtTxEp3	pBDTEntryIn[3]
	#define pBdtInEp3	pBDTEntryIn[3]
#endif

#if USB_MAX_EP_NUMBER > 3
	#define pBdtRxEp4	pBDTEntryOut[4]
	#define pBdtOutEp4	pBDTEntryOut[4]
	#define pBdtTxEp4	pBDTEntryIn[4]
	#define pBdtInEp4	pBDTEntryIn[4]
#endif

#if USB_MAX_EP_NUMBER > 4
	#define pBdtRxEp5	pBDTEntryOut[5]
	#define pBdtOutEp5	pBDTEntryOut[5]
	#define pBdtTxEp5	pBDTEntryIn[5]
	#define pBdtInEp5	pBDTEntryIn[5]
#endif

#if USB_MAX_EP_NUMBER > 5
	#define pBdtRxEp6	pBDTEntryOut[6]
	#define pBdtOutEp6	pBDTEntryOut[6]
	#define pBdtTxEp6	pBDTEntryIn[6]
	#define pBdtInEp6	pBDTEntryIn[6]
#endif
#if USB_MAX_EP_NUMBER > 6
	#define pBdtRxEp7	pBDTEntryOut[7]
	#define pBdtOutEp7	pBDTEntryOut[7]
	#define pBdtTxEp7	pBDTEntryIn[7]
	#define pBdtInEp7	pBDTEntryIn[7]
#endif
#if USB_MAX_EP_NUMBER > 7
	#define pBdtRxEp8	pBDTEntryOut[8]
	#define pBdtOutEp8	pBDTEntryOut[8]
	#define pBdtTxEp8	pBDTEntryIn[8]
	#define pBdtInEp8	pBDTEntryIn[8]
#endif
#if USB_MAX_EP_NUMBER > 8
	#define pBdtRxEp9	pBDTEntryOut[9]
	#define pBdtOutEp9	pBDTEntryOut[9]
	#define pBdtTxEp9	pBDTEntryIn[9]
	#define pBdtInEp9	pBDTEntryIn[9]
#endif
#if USB_MAX_EP_NUMBER > 9
	#define pBdtRxEp10	pBDTEntryOut[10]
	#define pBdtOutEp10	pBDTEntryOut[10]
	#define pBdtTxEp10	pBDTEntryIn[10]
	#define pBdtInEp10	pBDTEntryIn[10]
#endif
#if USB_MAX_EP_NUMBER > 10
	#define pBdtRxEp11	pBDTEntryOut[11]
	#define pBdtOutEp11	pBDTEntryOut[11]
	#define pBdtTxEp11	pBDTEntryIn[11]
	#define pBdtInEp11	pBDTEntryIn[11]
#endif
#if USB_MAX_EP_NUMBER > 11
	#define pBdtRxEp12	pBDTEntryOut[12]
	#define pBdtOutEp12	pBDTEntryOut[12]
	#define pBdtTxEp12	pBDTEntryIn[12]
	#define pBdtInEp12	pBDTEntryIn[12]
#endif
#if USB_MAX_EP_NUMBER > 12
	#define pBdtRxEp13	pBDTEntryOut[13]
	#define pBdtOutEp13	pBDTEntryOut[13]
	#define pBdtTxEp13	pBDTEntryIn[13]
	#define pBdtInEp13	pBDTEntryIn[13]
#endif
#if USB_MAX_EP_NUMBER > 13
	#define pBdtRxEp14	pBDTEntryOut[14]
	#define pBdtOutEp14	pBDTEntryOut[14]
	#define pBdtTxEp14	pBDTEntryIn[14]
	#define pBdtInEp14	pBDTEntryIn[14]
#endif
#if USB_MAX_EP_NUMBER > 14
	#define pBdtRxEp15	pBDTEntryOut[15]
	#define pBdtOutEp15	pBDTEntryOut[15]
	#define pBdtTxEp15	pBDTEntryIn[15]
	#define pBdtInEp15	pBDTEntryIn[15]
#endif

#endif
