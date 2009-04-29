/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : ISTR events interrupt service routines
********************************************************************************
* History:
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
* CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS
* A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
* OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
* OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
* CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "91x_lib.h"
#include "USB_lib.h"
#include "USB_conf.h"
#include "USB_prop.h"
#include "USB_pwr.h"
#include "util.h"

volatile WORD wIstr;  /* ISTR register last read value */
void USB_Istr(void);

/* function prototypes */
/* automatically built defining related macros */
#ifdef CTR_Callback
	void CTR_Callback(void);
#endif
#ifdef DOVR_Callback
	void DOVR_Callback(void);
#endif
#ifdef ERR_Callback
	void ERR_Callback(void);
#endif
#ifdef WKUP_Callback
	void WKUP_Callback(void);
#endif
#ifdef SUSP_Callback
	void SUSP_Callback(void);
#endif
#ifdef RESET_Callback
	void RESET_Callback(void);
#endif
#ifdef SOF_Callback
	void SOF_Callback(void);
#endif
#ifdef ESOF_Callback
	void ESOF_Callback(void);
#endif

extern void EP1_isr(void);                 /*empty routine*/
extern void EP2_isr(void);
extern void EP3_isr(void);
extern void EP4_isr(void);
extern void EP5_Callback(void);
extern void EP6_Callback(void);
extern void EP7_Callback(void);
extern void EP8_Callback(void);
extern void EP9_Callback(void);


/* function pointers to non-control endpoints service routines */
void (*pEpInt[9])(void)={
	EP1_isr,
	EP2_isr,
	EP3_isr,
	EP4_isr,
	EP5_Callback,
	EP6_Callback,
	EP7_Callback,
	EP8_Callback,
	EP9_Callback,
};

/* USB_Istr */
void USB_Istr(void)
{
	wIstr = _GetISTR();
	#if (IMR_MSK & ISTR_RESET)
	if (wIstr & ISTR_RESET & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_RESET);
		Device_Property.Reset();
		#ifdef RESET_Callback
		RESET_Callback();
		#endif
	}
	#endif
	
	#if	(IMR_MSK & ISTR_DOVR)
	if (wIstr & ISTR_DOVR & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_DOVR);
		#ifdef DOVR_Callback
		DOVR_Callback();
		#endif
	}
	#endif
	
	#if	(IMR_MSK & ISTR_ERR)
	if (wIstr & ISTR_ERR & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_ERR);
		#ifdef ERR_Callback
		ERR_Callback();
		#endif
	}
	#endif
	
	#if	(IMR_MSK & ISTR_WKUP)
	if (wIstr & ISTR_WKUP & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_WKUP);
		Resume(RESUME_EXTERNAL);
		#ifdef WKUP_Callback
		WKUP_Callback();
		#endif
	}
	#endif
	
	#if	(IMR_MSK & ISTR_SUSP)
	if (wIstr & ISTR_SUSP & wInterrupt_Mask)
	{
		/* check if SUSPEND is possible */
		if(fSuspendEnabled)
		{
			Suspend();
		}
		else
		{
			/* if not possible then resume after xx ms */
			Resume(RESUME_LATER);
		}
		/* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
		_SetISTR((WORD)CLR_SUSP);
		#ifdef SUSP_Callback
		SUSP_Callback();
		#endif
	}
	#endif
	
	#if (IMR_MSK & ISTR_SOF)
	if (wIstr & ISTR_SOF & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_SOF);
		#ifdef SOF_Callback
		SOF_Callback();
		#endif
	}
	#endif
	
	#if (IMR_MSK & ISTR_ESOF)
	if (wIstr & ISTR_ESOF & wInterrupt_Mask)
	{
		_SetISTR((WORD)CLR_ESOF);
		/* resume handling timing is made with ESOFs */
		Resume(RESUME_ESOF); /* request without change of the machine state */
		#ifdef ESOF_Callback
		ESOF_Callback();
		#endif
	}
	#endif
	
	#if	(IMR_MSK & ISTR_CTR)
	if (wIstr & ISTR_CTR & wInterrupt_Mask)
	{
		/* servicing of the endpoint correct transfer interrupt */
		/* clear of the CTR flag into the sub */
		CTR_ISR();
		#ifdef CTR_Callback
		CTR_Callback();
		#endif
	}
	#endif

}/* USB_Istr */



