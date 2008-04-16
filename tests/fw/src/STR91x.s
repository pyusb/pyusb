;/*****************************************************************************/
;/* STR91x.S: Startup file for ST STR91x device series                        */
;/*****************************************************************************/
;/* <<< Use Configuration Wizard in Context Menu >>>                          */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2007 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/


;/*
; *  The STR91x.S code is executed after CPU Reset. This file may be
; *  translated with the following SET symbols. In uVision these SET
; *  symbols are entered under Options - ASM - Define.
; *
; *  BOOT_BANK1:     must be set when booting from Flash Bank1.
; *
; *  Startup code always boots from Flash address 0x00000000
; */


; Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F

I_Bit           EQU     0x80            ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40            ; when F bit is set, FIQ is disabled


;// <h> Stack Configuration (Stack Sizes in Bytes)
;//   <o0> Undefined Mode      <0x0-0xFFFFFFFF:8>
;//   <o1> Supervisor Mode     <0x0-0xFFFFFFFF:8>
;//   <o2> Abort Mode          <0x0-0xFFFFFFFF:8>
;//   <o3> Fast Interrupt Mode <0x0-0xFFFFFFFF:8>
;//   <o4> Interrupt Mode      <0x0-0xFFFFFFFF:8>
;//   <o5> User/System Mode    <0x0-0xFFFFFFFF:8>
;// </h>

UND_Stack_Size  EQU     0x00000000
SVC_Stack_Size  EQU     0x00000008
ABT_Stack_Size  EQU     0x00000000
FIQ_Stack_Size  EQU     0x00000000
IRQ_Stack_Size  EQU     0x00000100
USR_Stack_Size  EQU     0x00000400

ISR_Stack_Size  EQU     (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + \
                         FIQ_Stack_Size + IRQ_Stack_Size)

                AREA    STACK, NOINIT, READWRITE, ALIGN=3

Stack_Mem       SPACE   USR_Stack_Size
__initial_sp    SPACE   ISR_Stack_Size
Stack_Top


;// <h> Heap Configuration
;//   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF>
;// </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


;// <e0> System Configuration
;//   <h> System Configuration (SCU_SCR0)
;//     <o1.3..4> SRAM_SIZE: SRAM Size
;//       <i> Default: 32KB
;//                     <0=> 32KB
;//                     <1=> 64KB
;//                     <2=> 96KB
;//     <o1.0> EN_PFQBC: PFQBC Unit Enable
;//     <o1.1> WSR_DTCM: DTCM Wait State Enable
;//     <o1.2> WSR_AHB:  AHB Wait State Enable
;//   </h>
;// </e0>
SCR0_SETUP      EQU     1
SCU_SCR0_Val    EQU     0x00000197


; Flash Memory Interface (FMI) definitions (Flash banks sizes and addresses)
FMI_BASE        EQU     0x54000000      ; FMI Base Address (non-buffered)
FMI_BBSR_OFS    EQU     0x00            ; Boot Bank Size Register
FMI_NBBSR_OFS   EQU     0x04            ; Non-boot Bank Size Register
FMI_BBADR_OFS   EQU     0x0C            ; Boot Bank Base Address Register
FMI_NBBADR_OFS  EQU     0x10            ; Non-boot Bank Base Address Register
FMI_CR_OFS      EQU     0x18            ; Control Register

;// <e0> Flash Memory Interface (FMI)
;//   <e1.3> Flash Boot Bank Enable
;//   <h> Boot Bank Size Configuration (BBSR)
;//     <o2.0..3>  BBSIZE: Memory size 
;//       <i> Default 32KB
;//                     <0=>   32KB
;//                     <1=>   64KB
;//                     <2=>  128KB
;//                     <3=>  256KB
;//                     <4=>  512KB
;//                     <5=>    1MB
;//                     <6=>    2MB
;//                     <7=>    4MB
;//                     <8=>    8MB
;//                     <9=>   16MB
;//                     <10=>  32MB
;//                     <11=>  64MB
;//   </h>
;//   <h> Boot Bank Base Address Configuration (BBADR)
;//     <o3.0..23> BBADDR: Address <0x0-0xFFFFFF>
;//       <i> Default: 0x000000
;//   </h>
;//   </e>
;//   <e1.4> Flash Non-boot Bank Enable
;//   <h> Non-boot Bank Size Configuration (NBBSR)
;//     <o4.0..3>  NBBSIZE: Memory size 
;//       <i> Default 8KB
;//                     <0=>    8KB
;//                     <1=>   16KB
;//                     <2=>   32KB
;//                     <3=>   64KB
;//                     <4=>  128KB
;//                     <5=>  256KB
;//                     <6=>  512KB
;//                     <7=>    1MB
;//                     <8=>    2MB
;//                     <9=>    4MB
;//                     <10=>   8MB
;//                     <11=>  16MB
;//                     <12=>  32MB
;//                     <13=>  64MB
;//   </h>
;//   <h> Non-boot Bank Base Address Configuration (NBBADR)
;//     <o5.0..23> NBBADDR: Address <0x0-0xFFFFFF>
;//       <i> Default: 0x000000
;//   </h>
;//   </e>
;//   <o6.11..12> WSTATES: Wait states
;//     <i> 1 wait state is required for FMI bus clock frequency of < 66 MHz
;//     <i> 2 wait states are required for FMI bus clock frequency of 75 MHz
;//     <i> 3 wait states are required for maximum FMI bus clock frequency
;//     <i> Default: 1 Wait state
;//                     <0=> 1 Wait state 
;//                     <1=> 2 Wait states
;//                     <2=> 3 Wait states
;//   <o6.10> PWD: Power-down configuration    
;//     <i> Default: Enabled 
;//                     <0=> Disabled  
;//                     <1=> Enabled
;//   <o6.9> LVDEN: Low Voltage Detector enable
;//     <i> Default: Enabled 
;//                     <0=> Enabled   
;//                     <1=> Disabled
;//   <o6.4> BUSCFG: Flash bus clock configuration
;//     <i> Used for configuration of bus frequencies grater than 66 MHz    
;//     <i> Default: Low frequency bus clock
;//                     <0=> Low frequency bus clock
;//                     <1=> Bus clock speed > 66 Mhz
;// </e0> End of FMI
FMI_SETUP       EQU     1
FMI_CR_Val      EQU     0x00000018
FMI_BBSR_Val    EQU     0x00000004
FMI_BBADR_Val   EQU     0x00000000
FMI_NBBSR_Val   EQU     0x00000002
FMI_NBBADR_Val  EQU     0x00400000
FLASH_CFG_Val   EQU     0x00000000


; System Control Unit (SCU) definitions
SCU_BASE        EQU     0x5C002000      ; SCU Base Address (non-buffered)
SCU_CLKCNTR_OFS EQU     0x00            ; Clock Control register Offset
SCU_PLLCONF_OFS EQU     0x04            ; PLL Configuration register Offset
SCU_SYSSTAT_OFS EQU     0x08            ; System Status Register Offset
SCU_PCGR0_OFS   EQU     0x14            ; Peripheral Clock Gating Register 0 Offset
SCU_PCGR1_OFS   EQU     0x18            ; Peripheral Clock Gating Register 1 Offset
SCU_PRR0_OFS    EQU     0x1C            ; Peripheral Reset Register        0 Offset
SCU_PRR1_OFS    EQU     0x20            ; Peripheral Reset Register        1 Offset
SCU_SCR0_OFS    EQU     0x34            ; System Configuration Register 0 Offset

; Constants
SYSSTAT_LOCK    EQU     0x01            ; PLL Lock Status

;// <e0> Clock Configuration
;//   <h> Clock Control Register Configuration (SCU_CLKCNTR)
;//     <o1.17..18> EMIRATIO: External Memory Interface Ratio  
;//       <i> Default: fBCLK=HCLK/2
;//                     <0=> fBCLK=HCLK  
;//                     <1=> fBCLK=HCLK/2
;//     <o1.16> FMISEL: Flash Memory Interface Clock Divider   
;//       <i> Default: FMICLK=RCLK
;//                     <0=> FMICLK=RCLK
;//                     <1=> FMICLK=RCLK/2
;//     <o1.14> TIM23SEL: Timers 2 and 3 Clock Selection
;//       <i> Default: Master Clock divided by prescaler PRSC_TIM23+1
;//                     <0=> Master Clock divided by prescaler PRSC_TIM23+1
;//                     <1=> External Clock from EXTCLK_T2T3 pin GPIO P2.5
;//     <o1.13> TIM01SEL: Timers 0 and 1 Clock Selection
;//       <i> Default: Master Clock divided by prescaler PRSC_TIM01+1
;//                     <0=> Master Clock divided by prescaler PRSC_TIM01+1
;//                     <1=> External Clock from EXTCLK_T0T1 pin GPIO P2.4
;//     <o1.12> PHYSEL: MII_PHYCLK Enable
;//       <i> Default: MII_PHYCLK output disabled
;//                     <0=> MII_PHYCLK output disabled
;//                     <1=> Fosc output on MII_PHYCLK pin GPIO P5.2
;//     <o1.10..11> USBSEL: USB 48 MHz Clock Selection         
;//       <i> Default: fMSTR
;//                     <0=> fMSTR
;//                     <1=> fMSTR/2
;//                     <2=> External Clock from USB_CLK48M pin GPIO P2.7
;//     <o1.9> BRSEL: Baud Rate Clock Selection
;//       <i> Default: fMSTR/2
;//                     <0=> fMSTR/2
;//                     <1=> fMSTR
;//     <o1.7..8> APBDIV: PCLK Divider
;//       <i> Default: PCLK=RCLK
;//                     <0=> PCLK=RCLK
;//                     <1=> PCLK=RCLK/2    
;//                     <2=> PCLK=RCLK/4    
;//                     <3=> PCLK=RCLK/8
;//     <o1.5..6> AHBDIV: HCLK Divider 
;//       <i> Default: HCLK=RCLK
;//                     <0=> HCLK=RCLK
;//                     <1=> HCLK=RCLK/2 
;//                     <2=> HCLK=RCLK/4 
;//     <o1.2..4> RCLKDIV: RCLK Divider 
;//       <i> Default: RCLK=fMSTR
;//                     <0=> RCLK=fMSTR
;//                     <1=> RCLK=fMSTR/2
;//                     <2=> RCLK=fMSTR/4
;//                     <3=> RCLK=fMSTR/8
;//                     <4=> RCLK=fMSTR/16
;//                     <5=> RCLK=fMSTR/1024
;//     <o1.0..1> MCLKSEL: Main Clock Source
;//       <i> Default: fMSTR=fOSC
;//                     <0=> fMSTR=fPLL
;//                     <1=> fMSTR=fRTC
;//                     <2=> fMSTR=fOSC
;//   </h>
;//   <h> PLL Configuration Register Configuration (SCU_PLLCONF)
;//     <i> fPLL = (2 * PLL_NDIV * fOSC) / (PLL_MDIV * (2 ^ PLL_PDIV))
;//     <o2.19> PLLEN: PLL Enable
;//       <i> Default: PLL Disabled
;//                     <0=> PLL Disabled
;//                     <1=> PLL Enabled
;//     <o2.16..18> PLL_PDIV: PLL Post-divider <0-5>
;//       <i>  Default: 0
;//     <o2.8..15> PLL_NDIV: PLL Feedback divider <1-255>
;//       <i>  Default: 24
;//     <o2.0..7> PLL_MDIV: PLL Pre-divider <1-255>
;//       <i>  Default: 25
;//   </h>
;//   <h> Peripheral Clock Gating Register 0 Configuration (SCU_PCGR0)
;//     <o3.11> MAC: Ethernet peripheral clock gating
;//       <i> Default: Stopped
;//     <o3.10> USB48M : USB 48 MHz clock gating
;//       <i> Default: Stopped
;//     <o3.9>  USB : USB peripheral clock gating
;//       <i> Default: Stopped
;//     <o3.8>  DMA : DMA peripheral clock gating
;//       <i> Default: Stopped
;//     <o3.7>  EXT_MEM_CLK : External memory clock gating
;//       <i> Default: Running
;//     <o3.6>  EMI : EMI peripheral clock gating
;//       <i> Default: Running
;//     <o3.5>  VIC : VIC peripheral clock gating
;//       <i> Default: Stopped
;//     <o3.4>  SRAM_ARBITER : SRAM arbiter clock gating
;//       <i> Default: Running
;//     <o3.3>  SRAM : SRAM clock gating
;//       <i> Default: Running
;//     <o3.1>  PQFBC : PQFBC clock gating
;//       <i> Default: Running
;//     <o3.0>  FMI : FMI clock gating
;//       <i> Default: Running
;//   </h>
;//   <h> Peripheral Clock Gating Register 1 Configuration (SCU_PCGR1)
;//     <o4.24> RTC: RTC clock gating
;//       <i> Default: Stopped
;//     <o4.23> GPIO9: GPIO9 Port clock gating
;//       <i> Default: Stopped
;//     <o4.22> GPIO8: GPIO8 Port clock gating
;//       <i> Default: Stopped
;//     <o4.21> GPIO7: GPIO7 Port clock gating
;//       <i> Default: Stopped
;//     <o4.20> GPIO6: GPIO6 Port clock gating
;//       <i> Default: Stopped
;//     <o4.19> GPIO5: GPIO5 Port clock gating
;//       <i> Default: Stopped
;//     <o4.18> GPIO4: GPIO4 Port clock gating
;//       <i> Default: Stopped
;//     <o4.17> GPIO3: GPIO3 Port clock gating
;//       <i> Default: Stopped
;//     <o4.16> GPIO2: GPIO2 Port clock gating
;//       <i> Default: Stopped
;//     <o4.15> GPIO1: GPIO1 Port clock gating
;//       <i> Default: Stopped
;//     <o4.14> GPIO0: GPIO0 Port clock gating
;//       <i> Default: Stopped
;//     <o4.13> WIU: WIU peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.12> WDG: WDG peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.11> ADC: ADC clock gating
;//       <i> Default: Stopped
;//     <o4.10> CAN: CAN clock gating
;//       <i> Default: Stopped
;//     <o4.9>  SSP1: SSP1 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.8>  SSP0: SSP0 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.7>  I2C1: I2C1 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.6>  I2C0: I2C0 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.5>  UART2: UART2 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.4>  UART1: UART1 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.3>  UART0: UART0 peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.2>  MC: Motor Control peripheral clock gating
;//       <i> Default: Stopped
;//     <o4.1>  TIM23: Timers 2 and 3 clock gating
;//       <i> Default: Stopped
;//     <o4.0>  TIM01: Timers 0 and 1 clock gating
;//       <i> Default: Stopped
;//   </h>
;// </e1> End of Clock Configuration
CLOCK_SETUP     EQU     1
SCU_CLKCNTR_Val EQU     0x00030600
SCU_PLLCONF_Val EQU     0x000AC019
SCU_PCGR0_Val   EQU     0x0000001B
SCU_PCGR1_Val   EQU     0x00000000


;// <e0> Peripheral Reset Configuration
;//   <h> Peripheral Reset Register 0 Configuration (SCU_PRR0)
;//     <o1.12>   RST_PFQBC_AHB: PFQBC AHB Reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: Not in reset
;//     <o1.11>         RST_MAC: Ethernet peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o1.9>          RST_USB: USB peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o1.8>          RST_DMA: DMA peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o1.6>          RST_EMI: EMI peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: Not in reset
;//     <o1.5>          RST_VIC: VIC peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o1.4> RST_SRAM_ARBITER: SRAM arbiter reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: Not in reset
;//     <o1.1>        RST_PQFBC: PQFBC reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: Not in reset
;//     <o1.0>          RST_FMI: FMI reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: Not in reset
;//   </h>
;//   <h> Peripheral Reset Register 1 Configuration (SCU_PRR1)
;//     <o2.24>         RST_RTC: RTC reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.23>       RST_GPIO9: GPIO9 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.22>       RST_GPIO8: GPIO8 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.21>       RST_GPIO7: GPIO7 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.20>       RST_GPIO6: GPIO6 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.19>       RST_GPIO5: GPIO5 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.18>       RST_GPIO4: GPIO4 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.17>       RST_GPIO3: GPIO3 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.16>       RST_GPIO2: GPIO2 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.15>       RST_GPIO1: GPIO1 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.14>       RST_GPIO0: GPIO0 Port reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.13>         RST_WIU: WIU peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.12>         RST_WDG: WDG peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.11>         RST_ADC: ADC reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.10>         RST_CAN: CAN peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.9>         RST_SSP1: SSP1 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.8>         RST_SSP0: SSP0 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.7>         RST_I2C1: I2C1 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.6>         RST_I2C0: I2C0 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.5>        RST_UART2: UART2 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.4>        RST_UART1: UART1 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.3>        RST_UART0: UART0 peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.2>           RST_MC: Motor Control peripheral reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.1>        RST_TIM23: Timers 2 and 3 reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//     <o2.0>        RST_TIM01: Timers 0 and 1 reset
;//       <0=> In reset <1=> Not in reset
;//       <i> Default: In reset
;//   </h>
;// </e1> End of Clock Configuration
P_RESET_SETUP   EQU     0
SCU_PRR0_Val    EQU     0x00001053
SCU_PRR1_Val    EQU     0x00000000


; APB Bridge 1 & 2 definitions (Peripherals)
APB0_BUF_BASE   EQU     0x48001802      ; APB Bridge 0 Buffered Base Address
APB0_NBUF_BASE  EQU     0x58000000      ; APB Bridge 0 Non-buffered Base Address
APB1_BUF_BASE   EQU     0x4C000000      ; APB Bridge 1 Buffered Base Address
APB1_NBUF_BASE  EQU     0x5C000000      ; APB Bridge 1 Non-buffered Base Address


;// <e> Setup Library Exception Handlers
LEH_SETUP       EQU     0
;// </e>


                PRESERVE8


; Area Definition and Entry Point
;  Startup Code must be linked first at Address at which it expects to run.

                AREA    Reset, CODE, READONLY
                ARM

; Exception Vectors
;  Mapped to Address 0.
;  Absolute addressing mode must be used.
;  Dummy Handlers are implemented as infinite loops which can be modified.


Vectors         LDR     PC, Reset_Addr         
                LDR     PC, Undef_Addr
                LDR     PC, SWI_Addr
                LDR     PC, PAbt_Addr
                LDR     PC, DAbt_Addr
                NOP                         ; Reserved Vector 
                LDR     PC, IRQ_Addr
;                LDR     PC, [PC, #-0x0FF0]
                LDR     PC, FIQ_Addr

                IF      LEH_SETUP <> 0
                EXTERN  UndefHandler
                EXTERN  SWIHandler
                EXTERN  PAbtHandler
                EXTERN  DAbtHandler
                EXTERN  IRQHandler
                EXTERN  FIQHandler
                ENDIF
                
Reset_Addr      DCD     Reset_Handler
Undef_Addr      DCD     UndefHandler
SWI_Addr        DCD     SWIHandler
PAbt_Addr       DCD     PAbtHandler
DAbt_Addr       DCD     DAbtHandler
                DCD     0                   ; Reserved Address 
IRQ_Addr        DCD     IRQHandler
FIQ_Addr        DCD     FIQHandler

                
                IF      LEH_SETUP = 0
                
UndefHandler    B       UndefHandler
SWIHandler      B       SWIHandler
PAbtHandler     B       PAbtHandler
DAbtHandler     B       DAbtHandler
FIQHandler      B       FIQHandler

                ENDIF


; Reset Handler

                EXPORT  Reset_Handler
Reset_Handler   

                NOP     ; Wait for OSC stabilization
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP


; Setup System Configuration (and SRAM Size)
                IF      SCR0_SETUP == 1

                LDR     R0, =SCU_BASE
                LDR     R1, =SCU_SCR0_Val
                STR     R1, [R0, #SCU_SCR0_OFS]

                ENDIF


; Setup Flash Memory Interface (FMI)
                IF      FMI_SETUP == 1

                LDR     R0, =FMI_BASE
                LDR     R1, =FMI_BBSR_Val
                STR     R1, [R0, #FMI_BBSR_OFS]
                LDR     R1, =FMI_NBBSR_Val
                STR     R1, [R0, #FMI_NBBSR_OFS]
                LDR     R1, =(FMI_BBADR_Val:SHR:2)
                STR     R1, [R0, #FMI_BBADR_OFS]
                LDR     R2, =(FMI_NBBADR_Val:SHR:2)
                STR     R2, [R0, #FMI_NBBADR_OFS]
                LDR     R3, =FMI_CR_Val
                STR     R3, [R0, #FMI_CR_OFS]

                ; Write "Write flash configuration" command (60h)
                IF      :DEF:BOOT_BANK1
                MOV     R0, R1, LSL #2
                ELSE
                MOV     R0, R2, LSL #2
                ENDIF
                MOV     R1, #0x60
                STRH    R1, [R0, #0]

                ; Write "Write flash configuration confirm" command (03h)
                LDR     R2, =(FLASH_CFG_Val:SHL:2)
                ADD     R0, R0, R2
                MOV     R1, #0x03
                STRH    R1, [R0, #0]

                ENDIF


; Setup Clock
                IF      CLOCK_SETUP == 1

                LDR     R0, =SCU_BASE
                LDR     R1, =0x00020002
                STR     R1, [R0, #SCU_CLKCNTR_OFS]    ; Select OSC as clk src

                NOP     ; Wait for OSC stabilization
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP

                LDR     R1, =0x0003C019               ; PLL to default
                STR     R1, [R0, #SCU_PLLCONF_OFS]
                LDR     R1, =SCU_PLLCONF_Val          ; PLL to requested value
                STR     R1, [R0, #SCU_PLLCONF_OFS]

                ; Wait until PLL is stabilized (if PLL enabled)
                IF      (SCU_PLLCONF_Val:AND:0x80000) != 0
PLL_Loop        LDR     R2, [R0, #SCU_SYSSTAT_OFS]
                ANDS    R2, R2, #SYSSTAT_LOCK
                BEQ     PLL_Loop
                ENDIF

                LDR     R1, =SCU_CLKCNTR_Val          ; Setup clock control
                STR     R1, [R0, #SCU_CLKCNTR_OFS]

                LDR     R1, =SCU_PCGR0_Val            ; Enable clock gating
                STR     R1, [R0, #SCU_PCGR0_OFS]
                LDR     R1, =SCU_PCGR1_Val
                STR     R1, [R0, #SCU_PCGR1_OFS]

                ENDIF


; Setup Peripheral Reset
                IF      P_RESET_SETUP != 0
                LDR     R1, =SCU_PRR0_Val
                STR     R1, [R0, #SCU_PRR0_OFS]
                LDR     R1, =SCU_PRR1_Val
                STR     R1, [R0, #SCU_PRR1_OFS]
                ENDIF


; Setup Stack for each mode

                LDR     R0, =Stack_Top

;  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

;  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

;  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

;  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

;  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC:OR:I_Bit:OR:F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

;  Enter User Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_USR
                IF      :DEF:__MICROLIB

                EXPORT __initial_sp

                ELSE

                MOV     SP, R0
                SUB     SL, SP, #USR_Stack_Size

                ENDIF


; Enter the C code

                IMPORT  __main
                LDR     R0, =__main
                BX      R0


                IF      :DEF:__MICROLIB

                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE
; User Initial Stack & Heap
                AREA    |.text|, CODE, READONLY

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + USR_Stack_Size)
                LDR     R2, = (Heap_Mem +      Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDIF

VIC0_VAR    EQU     0xFFFFF030
VIC1_VAR    EQU     0xFC000030

IRQHandler
                sub     lr, lr, #4
                stmfd   sp!, {r0,lr}
                ldr     r0, =VIC0_VAR
                ldr     r0, [r0]
                blx     r0
                ldr     r0, =VIC0_VAR
                str     r1, [r0]
                ldr     r0, =VIC1_VAR
                str     r1, [r0]
                ldmfd   sp!,{r0,pc}^

                END
