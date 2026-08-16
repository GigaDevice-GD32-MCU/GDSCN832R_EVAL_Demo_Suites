/*!
    \file    gdesc.c
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

#include <string.h>
#include "systick.h"
#include "ecat_def.h"
#include "ecatslv.h"
#include "ecatappl.h"
#include "gdesc.h"

/* UALEVENT union */
typedef union {
    UINT8           byte[2];
    UINT16          word;
}
UALEVENT;

UALEVENT
esc_alevent;            /*!< contains the content of the ALEvent register (0x220), this variable is updated on each Access to the Esc  */

/*!
    \brief      read the AL_Event register (0x220),interrupts are supported.
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void get_interrupt_register(void)
{
    DISABLE_AL_EVENT_INT

    HW_EscReadIsr((MEM_ADDR *)&esc_alevent.word, 0x220, 2);

    ENABLE_AL_EVENT_INT

}

/*!
    \brief      read the AL_Event register (0x220).
                Shall be implemented if interrupts are supported else this function is equal to "get_interrupt_register()"
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if !INTERRUPTS_SUPPORTED
#define isr_get_interrupt_register get_interrupt_register
#else
static void isr_get_interrupt_register(void)
{

    HW_EscReadIsr((MEM_ADDR *)&esc_alevent.word, 0x220, 2);

}
#endif

/*!
    \brief      enable esc func
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void esc_enable(void)
{

    volatile uint32_t temp = 0;

    /*ByteTest and  PMU ready*/
#if USE_SPI
    do {
        temp = spi_read_word(PMU_PDIREFVAL);
    } while(temp != 0x76543210);
#endif

#if USE_OSPI
    do {
        ospi_read(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_READ, 0, PMU_PDIREFVAL, (uint8_t *)&temp, 4);
    } while(temp != 0x76543210);
#endif

#if USE_EXMC
    do {
        temp = exmc_read(PMU_PDIREFVAL);
    } while(temp != 0x76543210);
#endif

#if USE_SPI
    spi_write_word(ESC_RCU_RSTCFG, 0x301);
    delay_1ms(4);
#endif

#if USE_OSPI
    temp = 0x301;
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_INC, ESC_RCU_RSTCFG, (uint8_t *)&temp, 4);
    delay_1ms(4);
    if(MODE_SELECT == OSPI_MODE) {
        ospi_enable_ospi_mode(OSPI_INTERFACE, &ospi_init_struct, SPI_MODE);
    }
#endif

#if USE_EXMC
    /*phyrstmode initilaze*/
    exmc_write(ESC_RCU_RSTCFG, 0x301);
    delay_1ms(4);
#endif

#if USE_SPI
    do {
        temp = spi_read_word(PMU_CTRL0);
    } while((temp & BIT(0)) != BIT(0));
#endif

#if USE_OSPI
    do {
        ospi_read(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_READ, 0, PMU_CTRL0, (uint8_t *)&temp, 4);
    } while((temp & BIT(0)) != BIT(0));
#endif

#if USE_EXMC
    do {
        temp = exmc_read(PMU_CTRL0);
    } while((temp & BIT(0)) != BIT(0));
#endif

}

/*!
    \brief      esc hardware init
    \param[in]  none
    \param[out] none
    \retval     none
*/
UINT8 HW_Init(void)
{
    UINT16 intMask;

#if USE_SPI
    /* initialize the ESC SPI */
    spi_gpio_init(SPI_PSC_32);
#endif
#if USE_OSPI
    ospi_hw_init(OSPI_INTERFACE, &ospi_init_struct);
    if(MODE_SELECT == OSPI_MODE) {
        ospi_enable_ospi_mode(OSPI_INTERFACE, &ospi_init_struct, SPI_MODE);
    }
#endif
#if USE_EXMC
    exmc_nor_gpio_config();
    exmc_nor_async_init();
#endif

    /*enabe esc*/
    esc_enable();

    do {
        intMask = 0x0093;
        HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
        intMask = 0x02;
        HW_EscReadDWord(intMask, ESC_AL_EVENTMASK_OFFSET);
    } while(intMask != 0x0093);

    intMask = 0x0;
    HW_EscWriteDWord(intMask, ESC_AL_EVENTMASK_OFFSET);

    /*Write in Interrupt Enable register.*/
#if USE_SPI
    spi_write_word(INTC_CTL, INTC_CTL_IRQEN | INTC_CTL_IRQMODE);
#endif
#if USE_OSPI
    intMask = INTC_CTL_IRQEN | INTC_CTL_IRQMODE;
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_INC, INTC_CTL, (uint8_t *)&intMask, 4);
#endif
#if USE_EXMC
    exmc_write(INTC_CTL, INTC_CTL_IRQEN | INTC_CTL_IRQMODE);
#endif

    /*EtherCAT interrupt enable*/
#if USE_SPI
    spi_write_word(INTC_EN, INTC_EN_ECATIE);
#endif
#if USE_OSPI
    intMask = INTC_EN_ECATIE;
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_INC, INTC_EN, (uint8_t *)&intMask, 4);
#endif
#if USE_EXMC
    exmc_write(INTC_EN, INTC_EN_ECATIE);
#endif

    /*IRQ init*/
#if AL_EVENT_ENABLED
    INIT_ESC_INT
    HW_ResetALEventMask(0x0);
    ENABLE_ESC_INT();
#endif

    /*DC SYNC0/SYNC1 init*/
#if DC_SUPPORTED
    INIT_SYNC0_INT
    INIT_SYNC1_INT

    ENABLE_SYNC0_INT
    ENABLE_SYNC1_INT
#endif

    /*Timer init*/
    INIT_ECAT_TIMER
    START_ECAT_TIMER

    return 0;
}

/*!
    \brief      esc hardware to release
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HW_Release(void)
{

}

/*!
    \brief      gets the current content of ALEvent register (0x220).
    \param[in]  none
    \param[out] none
    \retval     none
*/
UINT16 HW_GetALEventRegister(void)
{
    get_interrupt_register();
    return esc_alevent.word;
}

/*!
    \brief      gets the current content of ALEvent register (0x220), interrupts are supported.
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if INTERRUPTS_SUPPORTED
UINT16 HW_GetALEventRegister_Isr(void)
{
    isr_get_interrupt_register();
    return esc_alevent.word;
}
#endif

/*!
    \brief      this function makes an logical and with the AL Event Mask register (0x204)
    \param[in]  intMask - interrupt mask
    \param[out] none
    \retval     none
*/
void HW_ResetALEventMask(UINT16 intmask)
{
    UINT16 mask;
    UINT16 nal_event_mask;

    HW_EscReadWord(mask, ESC_AL_EVENTMASK_OFFSET);

    mask &= intmask;
    DISABLE_AL_EVENT_INT
    HW_EscWriteWord(mask, ESC_AL_EVENTMASK_OFFSET);
    HW_EscReadWord(nal_event_mask, ESC_AL_EVENTMASK_OFFSET);
    ENABLE_AL_EVENT_INT
}

/*!
    \brief      This function operates the PDI read access to the esc.
    \param[in]  pdata: Pointer to a byte array which holds data to write or saves read data.
    \param[in]  address: esc address ( upper limit is 0x1FFF ) for access.
    \param[in]  len: Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscRead(MEM_ADDR *pdata, UINT16 address, UINT16 len)
{
    UINT16 i;
    UINT8 *ptmp_data = (UINT8 *)pdata;

    /* loop for all bytes to be read */
    while(len > 0) {
        if(address >= 0x1000) {
            i = len;
        } else {
            i = (len > 4) ? 4 : len;

            if(address & 01) {
                i = 1;
            } else if(address & 02) {
                i = (i & 1) ? 1 : 2;
            } else if(i == 03) {
                i = 1;
            }
        }

        DISABLE_AL_EVENT_INT

#if USE_SPI
        spi_read_register(ptmp_data, address, i);
#endif
#if USE_OSPI
        ospi_read_register(ptmp_data, address, i);
#endif
#if USE_EXMC
        exmc_read_register(ptmp_data, address, i);
#endif

        ENABLE_AL_EVENT_INT

        len -= i;
        ptmp_data += i;
        address += i;
    }
}

#if INTERRUPTS_SUPPORTED
/*!
    \brief      This function operates the PDI read access to the esc.
    \param[in]  pdata: Pointer to a byte array which holds data to write or saves read data.
    \param[in]  address: esc address ( upper limit is 0x1FFF ) for access.
    \param[in]  len: Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscReadIsr(MEM_ADDR *pdata, UINT16 address, UINT16 len)
{
    UINT16 i;
    UINT8 *ptmp_data = (UINT8 *)pdata;

    /* send the address and command to the ESC */

    /* loop for all bytes to be read */
    while(len > 0) {

        if(address >= 0x1000) {
            i = len;
        } else {
            i = (len > 4) ? 4 : len;

            if(address & 01) {
                i = 1;
            } else if(address & 02) {
                i = (i & 1) ? 1 : 2;
            } else if(i == 03) {
                i = 1;
            }
        }

#if USE_SPI
        spi_read_register(ptmp_data, address, i);
#endif
#if USE_OSPI
        ospi_read_register(ptmp_data, address, i);
#endif
#if USE_EXMC
        exmc_read_register(ptmp_data, address, i);
#endif

        len -= i;
        ptmp_data += i;
        address += i;
    }
}
#endif /*#if INTERRUPTS_SUPPORTED */

/*!
    \brief      This function operates the PDI write access to the esc.
    \param[in]  pdata: Pointer to a byte array which holds data to write or saves read data.
    \param[in]  address: esc address ( upper limit is 0x1FFF ) for access.
    \param[in]  len: Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscWrite(MEM_ADDR *pdata, UINT16 address, UINT16 len)
{
    UINT16 i;
    UINT8 *ptmp_data = (UINT8 *)pdata;

    /* loop for all bytes to be written */
    while(len) {

        if(address >= 0x1000) {
            i = len;
        } else {
            i = (len > 4) ? 4 : len;

            if(address & 01) {
                i = 1;
            } else if(address & 02) {
                i = (i & 1) ? 1 : 2;
            } else if(i == 03) {
                i = 1;
            }
        }

        DISABLE_AL_EVENT_INT

        /* start transmission */
#if USE_SPI
        spi_write_register(ptmp_data, address, i);
#endif
#if USE_OSPI
        ospi_write_register(ptmp_data, address, i);
#endif
#if USE_EXMC
        exmc_write_register(ptmp_data, address, i);
#endif

        ENABLE_AL_EVENT_INT

        /* next address */
        len -= i;
        ptmp_data += i;
        address += i;
    }
}

#if INTERRUPTS_SUPPORTED
/*!
    \brief      This function operates the PDI write access to the esc.
    \param[in]  pdata: Pointer to a byte array which holds data to write or saves read data.
    \param[in]  address: esc address ( upper limit is 0x1FFF ) for access.
    \param[in]  len: Access size in Bytes.
    \param[out] none
    \retval     none
*/
void HW_EscWriteIsr(MEM_ADDR *pdata, UINT16 address, UINT16 len)
{
    UINT16 i ;
    UINT8 *ptmp_data = (UINT8 *)pdata;


    /* loop for all bytes to be written */
    while(len) {
        if(address >= 0x1000) {
            i = len;
        } else {
            i = (len > 4) ? 4 : len;

            if(address & 01) {
                i = 1;
            } else if(address & 02) {
                i = (i & 1) ? 1 : 2;
            } else if(i == 03) {
                i = 1;
            }
        }

        /* start transmission */
#if USE_SPI
        spi_write_register(ptmp_data, address, i);
#endif

#if USE_OSPI
        ospi_write_register(ptmp_data, address, i);
#endif

#if USE_EXMC
        exmc_write_register(ptmp_data, address, i);
#endif

        /* next address */
        len -= i;
        ptmp_data += i;
        address += i;
    }
}
#endif

#if AL_EVENT_ENABLED
/*!
    \brief      Interrupt service routine for the PDI interrupt from ESC.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ecat_isr(void)
{
    if(exti_interrupt_flag_get(EXTI_15) != RESET) {
        NVIC_DisableIRQ(EXTI10_15_IRQn);
        PDI_Isr();
        ACK_ESC_INT
        /* reset the interrupt flag */
        NVIC_EnableIRQ(EXTI10_15_IRQn);

        if(gpio_input_bit_get(GPIOB, GPIO_PIN_13) == RESET) {
            Sync0_Isr();
            ACK_SYNC0_INT
        }

        if(gpio_input_bit_get(GPIOA, GPIO_PIN_10) == RESET) {
            Sync1_Isr();
            ACK_SYNC1_INT
        }
    }

    if(exti_interrupt_flag_get(EXTI_13) != RESET) {
        NVIC_DisableIRQ(EXTI10_15_IRQn);
        Sync0_Isr();
        ACK_SYNC0_INT
        NVIC_EnableIRQ(EXTI10_15_IRQn);

        if(gpio_input_bit_get(GPIOF, GPIO_PIN_15) == RESET) {
            DISABLE_ESC_INT();
            PDI_Isr();
            ACK_ESC_INT
        }

        if(gpio_input_bit_get(GPIOA, GPIO_PIN_10) == RESET) {
            Sync1_Isr();
            ACK_SYNC1_INT
        }
    }

    if(exti_interrupt_flag_get(EXTI_10) != RESET) {
        NVIC_DisableIRQ(EXTI10_15_IRQn);
        Sync1_Isr();
        ACK_SYNC1_INT
        NVIC_EnableIRQ(EXTI10_15_IRQn);

        if(gpio_input_bit_get(GPIOF, GPIO_PIN_15) == RESET) {
            DISABLE_ESC_INT();
            PDI_Isr();
            ACK_ESC_INT
        }

        if(gpio_input_bit_get(GPIOB, GPIO_PIN_13) == RESET) {
            Sync0_Isr();
            ACK_SYNC0_INT
        }
    }
}
#endif

#if DC_SUPPORTED
/*!
    \brief      Interrupt service routine for the interrupts from SYNC0.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void sync0_isr(void)
{
    if(exti_interrupt_flag_get(EXTI_13) != RESET) {
        NVIC_DisableIRQ(EXTI10_15_IRQn);
        Sync0_Isr();
        ACK_SYNC0_INT
        NVIC_EnableIRQ(EXTI10_15_IRQn);

        if(gpio_input_bit_get(GPIOF, GPIO_PIN_8) == RESET) {

            DISABLE_ESC_INT();
            PDI_Isr();
            ACK_ESC_INT
        }

        if(gpio_input_bit_get(GPIOA, GPIO_PIN_10) == RESET) {
            Sync1_Isr();
            ACK_SYNC1_INT
        }
    }

    if(exti_interrupt_flag_get(EXTI_10) != RESET) {
        NVIC_DisableIRQ(EXTI10_15_IRQn);
        Sync1_Isr();
        ACK_SYNC1_INT
        NVIC_EnableIRQ(EXTI10_15_IRQn);

        if(gpio_input_bit_get(GPIOF, GPIO_PIN_8) == RESET) {
            DISABLE_ESC_INT();
            PDI_Isr();
            ACK_ESC_INT
        }

        if(gpio_input_bit_get(GPIOB, GPIO_PIN_13) == RESET) {
            Sync0_Isr();
            ACK_SYNC0_INT
        }
    }
}

/*!
    \brief      Interrupt service routine for the interrupts from SYNC1.
    \param[in]  none
    \param[out] none
    \retval     none
*/
#if 0
void sunc1_isr(void)
{
    if(exti_interrupt_flag_get(EXTI_3) != RESET) {
        NVIC_DisableIRQ(EXTI3_IRQn);
        Sync1_Isr();
        ACK_SYNC1_INT
        NVIC_EnableIRQ(EXTI3_IRQn);

        if(gpio_input_bit_get(GPIOF, GPIO_PIN_8) == RESET) {
            DISABLE_ESC_INT();
            PDI_Isr();
            ACK_ESC_INT
        }

        if(gpio_input_bit_get(GPIOC, GPIO_PIN_13) == RESET) {
            Sync0_Isr();
            ACK_SYNC0_INT
        }
    }
}
#endif
#endif

#if ECAT_TIMER_INT
/*!
    \brief      Timer 2 ISR.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_isr(void)
{
    if(timer_interrupt_flag_get(TIMER2, TIMER_INT_FLAG_UP) != RESET) {
        ECAT_CheckTimer();
        ECAT_TIMER_ACK_INT
    }
}
#endif
