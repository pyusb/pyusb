/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_endp.h
* Author             : MCD Application Team
* Date First Issued  : 05/18/2006 : Version 1.0
* Description        : Non default control endpoints service routines 
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
#include "USB_lib.h"
#include "usb_conf.h"
#include "ring_buffer.h"

#ifndef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif

rb_data_t bulk_buffer[RB_BUFFER_SIZE];
rb_data_t interrupt_buffer[RB_BUFFER_SIZE];
rb_data_t isochronous_buffer[RB_BUFFER_SIZE];

ring_buffer_t rb_bulk;
ring_buffer_t rb_interrupt;
ring_buffer_t rb_isochronous;

/*
 * This routine handle the irq processing in loopback. All data sent to the
 * endpoint OUT is read from endpoint IN
 */
static void ep_handle_loopback(BYTE ep, WORD ep_txaddr, WORD ep_rxaddr, ring_buffer_t *rb)
{
    if (wIstr & ISTR_DIR) { /* data received */

        rb_put_ptr(rb, (unsigned char *) ep_rxaddr, GetEPRxCount(ep));
        SetEPRxCount(ep, 0x40);
	    SetEPRxStatus(ep, EP_RX_VALID);

    } else { /* data sent */

        SetEPTxCount(ep, rb_get_ptr(rb, (unsigned char *) ep_txaddr, min(rb_size(rb), 0x40)));
        SetEPTxStatus(ep, EP_TX_VALID);
    }
}

/* ENDP1 service routine*/ 
void EP1_isr(void)
{
    ep_handle_loopback(ENDP1, ENDP1_TXADDR, ENDP1_RXADDR, &rb_bulk);
}

/* ENDP2 service routine */
void EP2_isr(void)
{
    ep_handle_loopback(ENDP2, ENDP2_TXADDR, ENDP2_RXADDR, &rb_interrupt);
}

/* ENDP3 service routine */
void EP3_isr(void)
{
    if(GetENDPOINT(ENDP3)&EP_DTOG_TX) {
        rb_put_ptr(&rb_isochronous, (unsigned char *) ENDP3_BUFF0ADDR, GetEPDblBuf0Count(ENDP3));
        SetEPDblBuf0Count(ENDP3, EP_DBUF_OUT, 0x40);
    } else {
        rb_put_ptr(&rb_isochronous, (unsigned char *) ENDP3_BUFF1ADDR, GetEPDblBuf1Count(ENDP3));
        SetEPDblBuf1Count(ENDP3, EP_DBUF_OUT, 0x40);
    }

    FreeUserBuffer(ENDP3,EP_DBUF_OUT);
}

/* ENDP4 service routine */
void EP4_isr(void)
{
    if(GetENDPOINT(ENDP1)&EP_DTOG_RX)
        SetEPDblBuf1Count(ENDP4, EP_DBUF_IN, rb_get_ptr(&rb_isochronous, (unsigned char *) ENDP4_BUFF1ADDR, min(0x40, rb_size(&rb_isochronous))));
    else
        SetEPDblBuf0Count(ENDP4, EP_DBUF_IN, rb_get_ptr(&rb_isochronous, (unsigned char *) ENDP4_BUFF0ADDR, min(0x40, rb_size(&rb_isochronous))));
    
    FreeUserBuffer(ENDP4,EP_DBUF_IN);
}
