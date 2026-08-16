/*!
    \file    gdesc.h
    \brief   gdesc driver basic configuration

    \version 2025-5-14, V1.1.0, demo for GDSCN832xx
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32h7xx.h"
#include  "esc.h"
#include  "exti.h"
#include  "timer.h"
#include  "esc_spi.h"
#include  "esc_ospi.h"
#include  "esc_exmc.h"
#include  "esc_phy.h"
#include  "esc_intc.h"
#include  "esc_gpio.h"
#include  "esc_pmu.h"
#include  "esc_rcu.h"
#include  "esc_syscfg.h"
#include  "esc_timer.h"

#define ESC_RD                                    0x02                                         /*!< Indicates a read access to ESC or EEPROM */
#define ESC_WR                                    0x04                                         /*!< Indicates a write access to ESC or EEPROM. */

#define ECAT_TIMER_INC_P_MS

#define    DISABLE_AL_EVENT_INT                   disable_global_int();                        /*!< disable global interrupt */
#define    ENABLE_AL_EVENT_INT                    enable_global_int();                         /*!< enable global interrupt */

#ifndef DISABLE_ESC_INT
#define    DISABLE_ESC_INT()                      NVIC_DisableIRQ(EXTI10_15_IRQn)              /*! Disable IRQ interrupt */
#endif
#ifndef ENABLE_ESC_INT
#define    ENABLE_ESC_INT()                       NVIC_EnableIRQ(EXTI10_15_IRQn)               /*! Enable IRQ interrupt */
#endif

#if AL_EVENT_ENABLED
#define    INIT_ESC_INT                           exti_irq_configuration();                     /*! esc exti irq gpio configuration */
#define    ecat_isr                               EXTI10_15_IRQHandler                          /*! primary interrupt vector name */
#define    ACK_ESC_INT                            exti_interrupt_flag_clear(EXTI_15);           /*! clear sync0 interrupt */
#endif /* AL_EVENT_ENABLED */

#if DC_SUPPORTED
#define    INIT_SYNC0_INT                         exti_sync0_configuration();                    /*! esc exti sync0 gpio configuration */
//#define    sync0_isr                              EXTI10_15_IRQHandler                           /*! primary interrupt vector name */
#define    DISABLE_SYNC0_INT                      NVIC_DisableIRQ(EXTI10_15_IRQn);               /*! disable sync0 interrupt */
#define    ENABLE_SYNC0_INT                       NVIC_EnableIRQ(EXTI10_15_IRQn);                /*! enable sync0 interrupt */
#define    ACK_SYNC0_INT                          exti_interrupt_flag_clear(EXTI_13);            /*! clear sync0 interrupt */

#define    INIT_SYNC1_INT                         exti_sync0_configuration();                    /*! esc exti sync1 gpio configuration */
//#define    sync1_isr                              EXTI3_IRQHandler                               /*! primary interrupt vector name */
#define    DISABLE_SYNC1_INT                      NVIC_DisableIRQ(EXTI10_15_IRQn);               /*! disable sync1 interrupt */
#define    ENABLE_SYNC1_INT                       NVIC_EnableIRQ(EXTI10_15_IRQn);                /*! enable sync1 interrupt */
#define    ACK_SYNC1_INT                          exti_interrupt_flag_clear(EXTI_10);            /*! clear sync1 interrupt */

#endif /* DC_SUPPORTED */

#define    ECAT_TIMER_ACK_INT           timer_interrupt_flag_clear(TIMER2 , TIMER_INT_FLAG_UP);  /*! clear timer interrupt flag */
#define    timer_isr                    TIMER2_IRQHandler                                        /*! primary interrupt vector name */
#define    ENABLE_ECAT_TIMER_INT        NVIC_EnableIRQ(TIMER2_IRQn);                             /*! enable timer interrupt */
#define    DISABLE_ECAT_TIMER_INT       NVIC_DisableIRQ(TIMER2_IRQn);                            /*! disable timer interrupt */

#define INIT_ECAT_TIMER                 esc_timer_configuration();                               /*! configure esc timer */

#define STOP_ECAT_TIMER                 DISABLE_ECAT_TIMER_INT                                   /*! disable timer interrupt */

#define START_ECAT_TIMER                ENABLE_ECAT_TIMER_INT                                    /*! enable timer interrupt */

#ifndef HW_GetTimer
#define HW_GetTimer()                   timer_counter_read(TIMER2);                              /*! get timer counter */
#endif

#ifndef HW_ClearTimer
#define HW_ClearTimer()                timer_counter_value_config(TIMER2,0);                     /*! clear timer counter */
#endif

#define HW_EscReadByte(bytevalue,address)           HW_EscRead(((MEM_ADDR *)&(bytevalue)),((UINT16)(address)),1) /*! 8Bit ESC read access */
#define HW_EscReadWord(wordvalue, address)          HW_EscRead(((MEM_ADDR *)&(wordvalue)),((UINT16)(address)),2) /*! 16Bit ESC read access */
#define HW_EscReadDWord(dwordvalue, address)        HW_EscRead(((MEM_ADDR *)&(dwordvalue)),((UINT16)(address)),4) /*! 32Bit ESC read access */
#define HW_EscReadMbxMem(pdata,address,len)         HW_EscRead(((MEM_ADDR *)(pdata)),((UINT16)(address)),(len)) /*! The mailbox data is stored in the local uC memory therefore the default read function is used. */

#define HW_EscReadByteIsr(bytevalue,address)        HW_EscReadIsr(((MEM_ADDR *)&(bytevalue)),((UINT16)(address)),1) /*! Interrupt specific 8Bit ESC read access */
#define HW_EscReadWordIsr(wordvalue, address)       HW_EscReadIsr(((MEM_ADDR *)&(wordvalue)),((UINT16)(address)),2) /*! Interrupt specific 16Bit ESC read access */
#define HW_EscReadDWordIsr(dwordvalue, address)     HW_EscReadIsr(((MEM_ADDR *)&(dwordvalue)),((UINT16)(address)),4) /*! Interrupt specific 32Bit ESC read access */

#define HW_EscWriteByte(wordvalue, address)         HW_EscWrite(((MEM_ADDR *)&(wordvalue)),((UINT16)(address)),1) /*! 8Bit ESC write access */
#define HW_EscWriteWord(wordvalue, address)         HW_EscWrite(((MEM_ADDR *)&(wordvalue)),((UINT16)(address)),2) /*! 16Bit ESC write access */
#define HW_EscWriteDWord(dwordvalue, address)       HW_EscWrite(((MEM_ADDR *)&(dwordvalue)),((UINT16)(address)),4) /*! 32Bit ESC write access */
#define HW_EscWriteMbxMem(pdata,address,len)        HW_EscWrite(((MEM_ADDR *)(pdata)),((UINT16)(address)),(len)) /*! The mailbox data is stored in the local uC memory therefore the default write function is used. */

#define HW_EscWriteWordIsr(wordvalue, address)      HW_EscWriteIsr(((MEM_ADDR *)&(wordvalue)),((UINT16)(address)),2) /*! Interrupt specific 16Bit ESC write access */
#define HW_EscWriteDWordIsr(dwordvalue, address)    HW_EscWriteIsr(((MEM_ADDR *)&(dwordvalue)),((UINT16)(address)),4) /*! Interrupt specific 32Bit ESC write access */


extern UINT8 HW_Init(void);
extern void HW_Release(void);
extern UINT16 HW_GetALEventRegister(void);
extern UINT16 HW_GetALEventRegister_Isr(void);

extern void HW_ResetALEventMask(UINT16 intmask);

extern void HW_EscRead(MEM_ADDR *pdata, UINT16 address, UINT16 len);
extern void HW_EscReadIsr(MEM_ADDR *pdata, UINT16 address, UINT16 len);

extern void HW_EscWrite(MEM_ADDR *pdata, UINT16 address, UINT16 len);
extern void HW_EscWriteIsr(MEM_ADDR *pdata, UINT16 address, UINT16 len);
