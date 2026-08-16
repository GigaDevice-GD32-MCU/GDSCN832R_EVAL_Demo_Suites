/*!
    \file    esc_timer.c
    \brief   esc timer driver basic configuration

    \version 2024-11-22, V1.0.0, demo for GDSCN832xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

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

#include "esc_spi.h"
#include "esc_ospi.h"
#include "esc_exmc.h"
#include "esc_timer.h"

/*!
    \brief      enable a ESC BASIC TIMER
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_timer_enable(void)
{
    uint32_t temp_value = 0U;

#if USE_SPI
    temp_value = spi_read_word(ESC_TIMER_CTL0) | BIT(29);
    spi_write_word(ESC_TIMER_CTL0, temp_value);
#endif

#if USE_OSPI
    ospi_read(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_READ, ADDR_NO_INC, ESC_TIMER_CTL0, (uint8_t *)&temp_value, 4U);
    temp_value = temp_value | BIT(29);
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_NO_INC, ESC_TIMER_CTL0, (uint8_t *)&temp_value, 4U);
#endif

#if USE_EXMC
    temp_value = exmc_read(ESC_TIMER_CTL0) | BIT(29);
    exmc_write(ESC_TIMER_CTL0, temp_value);
#endif
}

/*!
    \brief      disable a ESC BASIC TIMER
    \param[in]  none
    \param[out] none
    \retval     none
*/
void esc_timer_disable(void)
{
    uint32_t temp_value = 0U;

    temp_value = 0x0000FFFFU;
#if USE_SPI
    spi_write_word(ESC_TIMER_CTL0, temp_value);
#endif

#if USE_OSPI
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_NO_INC, ESC_TIMER_CTL0, (uint8_t *)&temp_value, 4U);
#endif

#if USE_EXMC
    exmc_write(ESC_TIMER_CTL0, temp_value);
#endif
}

/*!
    \brief      configure ESC BASIC TIMER pre-load register value
    \param[in]  preload: the counter pre-load value,0~65535
    \param[out] none
    \retval     none
*/
void esc_timer_preload_value_config(uint16_t preload)
{
    uint32_t temp_value = 0U;

    temp_value = 0x20000000U | (uint32_t)preload;

#if USE_SPI
    spi_write_word(ESC_TIMER_CTL0, temp_value);
#endif

#if USE_OSPI
    ospi_write(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_WRITE, ADDR_NO_INC, ESC_TIMER_CTL0, (uint8_t *)&temp_value, 4U);
#endif

#if USE_EXMC
    exmc_write(ESC_TIMER_CTL0, temp_value);
#endif
}

/*!
    \brief      read ESC BASIC TIMER counter value
    \param[in]  none
    \param[out] none
    \retval     counter value
*/
uint32_t esc_timer_counter_read(void)
{

    uint32_t counter_value = 0U;

#if USE_SPI
    counter_value = spi_read_word(ESC_TIMER_CNT);
#endif

#if USE_OSPI
    ospi_read(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_READ, ADDR_NO_INC, ESC_TIMER_CNT, (uint8_t *)&counter_value, 4U);
#endif

#if USE_EXMC
    counter_value = exmc_read(ESC_TIMER_CNT);
#endif

    return counter_value;
}

/*!
    \brief      read ESC FRC counter value
    \param[in]  none
    \param[out] none
    \retval     counter value
*/
uint32_t esc_frc_counter_read(void)
{
    uint32_t counter_value = 0U;

#if USE_SPI
    counter_value = spi_read_word(ESC_FRC_CNT);
#endif

#if USE_OSPI
    ospi_read(OSPI_INTERFACE, &ospi_init_struct, MODE_SELECT, CMD_READ, ADDR_NO_INC, ESC_FRC_CNT, (uint8_t *)&counter_value, 4U);
#endif

#if USE_EXMC
    counter_value = exmc_read(ESC_FRC_CNT);
#endif

    return counter_value;

}
