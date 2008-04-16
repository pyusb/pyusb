/*
 * PyUSB Test Firmware
 *
 * Startup code
 *
 * Author: Wander Lairson Costa
 */

/* Program constants */

    .equ    Mode_USR,       0x10	/* User mode */
    .equ    Mode_FIQ,       0x11	/* FIQ mode */
    .equ    Mode_IRQ,       0x12	/* IRQ mode */
    .equ    Mode_SVC,       0x13	/* Supervisor mode */
    .equ    Mode_ABT,       0x17	/* Abort mode */
    .equ    Mode_UND,       0x1B	/* Undefined mode */
    .equ    Mode_SYS,       0x1F	/* System mode */

    .equ    I_Bit,          0x80	/* IRQ enable mask (set the bit to disable) */
    .equ    F_Bit,          0x40	/* FIQ enable mask (set the bit to disable) */

/* Memory Map definition */

    .equ    RAM_Base,	    0x04000000
    .equ    RAM_Limit,	    (RAM_Base + 96*1024 - 4)

/* Stack definition */

    .equ    USR_StackSize,  0x00000400
    .equ    FIQ_StackSize,  0x00000008
    .equ    IRQ_StackSize,  0x00000400
    .equ    SVC_StackSize,  0x00000100
    .equ    ABT_StackSize,  0x00000008
    .equ    UND_StackSize,  0x00000008

    .equ    StackTop,	    RAM_Limit

/* --- STR9X SCU specific definitions */

    .equ SCU_BASE_Address,    0x5C002000
    .equ SCU_CLKCNTR_OFST,    0x00000000
    .equ SCU_PLLCONF_OFST,    0x00000004
    .equ SCU_SYSSTATUS_OFST,  0x00000008
    .equ SCU_SCR0_OFST,       0x00000034

/* --- STR9X FMI specific definitions */

    .equ FMI_BASE_Address,    0x54000000
    .equ FMI_BBSR_OFST,       0x00000000
    .equ FMI_NBBSR_OFST,      0x00000004
    .equ FMI_BBADR_OFST,      0x0000000C
    .equ FMI_NBBADR_OFST,     0x00000010
    .equ FMI_CR_OFST,         0x00000018


    .text
    .code 32
    .align 4

    .global _start

_start:
 
    LDR     PC, Reset_Addr
    LDR     PC, Undefined_Addr
    LDR     PC, SWI_Addr
    LDR     PC, Prefetch_Addr
    LDR     PC, Abort_Addr
    NOP                          /*; Reserved vector*/
    LDR     PC, IRQ_Addr
    LDR     PC, FIQ_Addr

Reset_Addr	:   .long Startup
Undefined_Addr	:   .long Undefined
SWI_Addr	:   .long Swi
Prefetch_Addr	:   .long Prefetch
Abort_Addr	:   .long Abort
FIQ_Addr	:   .long Fiq
IRQ_Addr	:   .long Irq

Undefined:  b	Undefined
Swi:	    b	Swi
Prefetch:   b	Prefetch
Abort:	    b	Abort
Fiq:	    b	Fiq

/* Program Startup */

Startup:
/* This code is based on FreeRTOS startup code */
    nop	    /* Wait For OSC */
    nop 
    nop 
    nop 
    nop 
    nop 
    nop 
    nop 
    nop 
    nop 

/* FMI Configuration */

    ldr	    r6, =FMI_BASE_Address
    ldr	    r7, =0x4
    str	    r7, [r6, #FMI_BBSR_OFST]
    ldr	    r7, =0x2
    str	    r7, [r6, #FMI_NBBSR_OFST]
    ldr	    r7, =0x0
    str	    r7, [r6, #FMI_BBADR_OFST]
    ldr	    r7, =0x20000
    str	    r7, [r6, #FMI_NBBADR_OFST]
    ldr	    r7, =0x18
    str	    r7, [r6, #FMI_CR_OFST]

/* Enable 96K RAM */

    ldr	    r0, =SCU_BASE_Address
    ldr	    r1, =0x0191
    str	    r1, [r0, #SCU_SCR0_OFST]

/* */

    ldr	    r6, =0x00080000
    ldr	    r7, =0x60
    strh    r7, [r6]
    ldr	    r6, =0x00083040
    ldr	    r7, =0x3
    strh    r7, [r6]

/* PLL Configuration */

    ldr	    r1, =0x00020002
    strh    r1, [r0, #SCU_CLKCNTR_OFST]
    nop					    /* Wait OSC */
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ldr	    r1, =0x000ac019
    str	    r1, [r0, #SCU_PLLCONF_OFST]

wait_loop:
    ldr	    r1, [r0, #SCU_SYSSTATUS_OFST]
    ands    r1, r1, #0x01
    beq	    wait_loop
    ldr	    r1, =0x00020080
    str	    r1, [r0, #SCU_CLKCNTR_OFST]
    
/* Stack setup */
/* Based on TNKernel code */

    ldr	    r0, =StackTop
    msr	    cpsr_c, #Mode_UND|I_Bit|F_Bit
    mov	    sp, r0
    sub	    r0, r0, #UND_StackSize
    msr	    cpsr_c, #Mode_ABT|I_Bit|F_Bit
    mov	    sp, r0
    sub	    r0, r0, #ABT_StackSize
    msr	    cpsr_c, #Mode_FIQ|I_Bit|F_Bit
    mov	    sp, r0
    sub	    r0, r0, #FIQ_StackSize
    msr	    cpsr_c, #Mode_IRQ|I_Bit|F_Bit
    mov	    sp, r0
    sub	    r0, r0, #IRQ_StackSize
    msr	    cpsr_c, #Mode_SVC|I_Bit|F_Bit
    mov	    sp, r0
    sub	    r0, r0, #SVC_StackSize
    msr	    cpsr_c, #Mode_SYS|I_Bit|F_Bit
    mov	    sp, r0
    msr	    cpsr_c, #Mode_USR

/* Main */

    .extern main
    ldr	    r0, =main
    b	    main
    b	    .

    .equ VIC0_VAR, 0xFFFFF030
    .equ VIC1_VAR, 0xFC000030

Irq:
    sub     lr, lr, #4
    stmfd   sp!, {r0,lr}
    ldr     r0, =VIC0_VAR
    ldr     r0, [r0]
    mov     lr, pc
    bx      r0
    ldr     r0, =VIC0_VAR
    str     r1, [r0]
    ldr     r0, =VIC1_VAR
    str     r1, [r0]
    ldmfd   sp!,{r0,pc}^

