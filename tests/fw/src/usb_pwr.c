/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_pwr.c
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : connection/disconnection & power management
********************************************************************************
* History:
* 05/24/2006 : Version 1.1
* 05/18/2006 : Version 1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "91x_lib.h"
#include "USB_lib.h"
#include "USB_conf.h"
#include "USB_pwr.h"

volatile BOOL fCellSuspended;
volatile BYTE bDeviceState=UNCONNECTED; /* USB device status */
volatile BOOL fSuspendEnabled=TRUE;  /* true when suspend is possible */

struct {
	volatile RESUME_STATE eState;
	volatile BYTE bESOFcnt;
} ResumeS;


/*==========================================================================*/
/* PowerOn() */
/* handles switch-on conditions */
/* INPUT : */
/* OUTPUT: */
/*==========================================================================*/
RESULT PowerOn()
{
 WORD wRegVal;

	/*** cable plugged-in ? ***/
	/*while(!CablePluggedIn());*/
		
	/*** CNTR_PWDN = 0 ***/
	wRegVal = CNTR_FRES;
	_SetCNTR(wRegVal);

	/*** CNTR_FRES = 0 ***/
    wInterrupt_Mask = 0;
    _SetCNTR(wInterrupt_Mask);
	/*** Clear pending interrupts ***/
    _SetISTR(0);
	/*** Set interrupt mask ***/
    wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
    _SetCNTR(wInterrupt_Mask);

	return USB_SUCCESS;
} /* PowerOn */

/*==========================================================================*/
/* PowerOff() */
/* handles switch-off conditions */
/* INPUT : */
/* OUTPUT: */
/*==========================================================================*/
RESULT PowerOff()
{
	/* disable all ints and force USB reset */
    _SetCNTR(CNTR_FRES);
    /* clear interrupt status register */
    _SetISTR(0);

      /* Disable the Pull-Up*/

    // GPIO_BitWrite(GPIO0,2,0);

    /* switch-off device */
    _SetCNTR(CNTR_FRES+CNTR_PDWN);
    /* sw variables reset */
	/* ... */

	return USB_SUCCESS;
} /* PowerOff */
/*==========================================================================*/
/* Suspend() */
/* sets suspend mode operating conditions */
/* INPUT : */
/* OUTPUT: */
/*==========================================================================*/
void Suspend(void)
{
 WORD wCNTR;
    fCellSuspended= TRUE;
	/* suspend preparation */
	/* ... */

	/* macrocell enters suspend mode */
    wCNTR = _GetCNTR();
    wCNTR |= CNTR_FSUSP;
    _SetCNTR(wCNTR);

/* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
	/* power reduction */
	/* ... on connected devices */

    /* force low-power mode in the macrocell */
    wCNTR = _GetCNTR();
    wCNTR |= CNTR_LPMODE;
    _SetCNTR(wCNTR);

    /* switch-off the clocks */
    /* ... */


}/* Suspend */

/*==========================================================================*/
/* Resume_Init() */
/* handles wake-up restoring normal operations */
/* INPUT : */
/* OUTPUT: */
/*==========================================================================*/
void Resume_Init(void)
{
//  WORD i;
  WORD wCNTR;

    fCellSuspended= FALSE;

/* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
    /* restart the clocks */
    /* ...  */

    /* CNTR_LPMODE = 0 */
    wCNTR = _GetCNTR();
    wCNTR &= (~CNTR_LPMODE);
    _SetCNTR(wCNTR);

	/* restore full power */
	/* ... on connected devices */

   	/* reset FSUSP bit */
   	_SetCNTR(IMR_MSK);

	/* reverse suspend preparation */
	/* ... */

}/* Resume_Init() */

/*==========================================================================*/
/* Resume() */
/* This is the state machine handling resume operations and timing sequence. */
/* The control is based on the ResumeS structure variables and on the ESOF */
/* interrupt calling this subroutine without changing machine state.*/
/* */
/* IN  : a state machine value (RESUME_STATE) */
/*       RESUME_ESOF does'nt change ResumeS.eState */
/*		  allowing decrementing of the ESOF counter in different states */
/* OUT : none */
/*==========================================================================*/
void Resume(RESUME_STATE eResumeSetVal)
{
 WORD wCNTR;
 WORD wRegVal;

	if(eResumeSetVal != RESUME_ESOF)
			ResumeS.eState = eResumeSetVal;

	switch(ResumeS.eState)
	{
		case RESUME_EXTERNAL:
		Resume_Init();
	  wRegVal = GetFNR();
    		if(wRegVal & FNR_RXDP) //10 & 11 false conditions
     		{
    	     		Suspend();
     		}
		    ResumeS.eState = RESUME_OFF;
			break;
		case RESUME_INTERNAL:
			Resume_Init();
		    ResumeS.eState = RESUME_START;
			break;
		case RESUME_LATER:
			ResumeS.bESOFcnt = 2;
		    ResumeS.eState = RESUME_WAIT;
			break;
		case RESUME_WAIT:
			ResumeS.bESOFcnt--;
			if(ResumeS.bESOFcnt == 0)
			    ResumeS.eState = RESUME_START;
			break;
		case RESUME_START:
		    wCNTR = _GetCNTR();
		    wCNTR |= CNTR_RESUME;
		    _SetCNTR(wCNTR);
		    ResumeS.eState = RESUME_ON;
		    ResumeS.bESOFcnt = 10;
			break;
		case RESUME_ON:
			ResumeS.bESOFcnt--;
			if(ResumeS.bESOFcnt == 0)
			{
			    wCNTR = _GetCNTR();
			    wCNTR &= (~CNTR_RESUME);
			    _SetCNTR(wCNTR);
			    ResumeS.eState = RESUME_OFF;
			}
			break;
		case RESUME_OFF:
		case RESUME_ESOF:
		default:
		    ResumeS.eState = RESUME_OFF;
			break;
	}
}/* Resume() */

