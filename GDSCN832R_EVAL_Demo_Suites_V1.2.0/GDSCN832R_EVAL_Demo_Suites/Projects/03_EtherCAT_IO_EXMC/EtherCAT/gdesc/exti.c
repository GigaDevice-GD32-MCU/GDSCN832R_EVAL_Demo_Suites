/*!
    \file    exti.c
    \brief   EtherCAT exti configuration

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
#include "exti.h"

/*!
    \brief      esc exti irq gpio configuration.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_irq_configuration(void)
{

    /** Enable the GPIO Clock */
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_15);

    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOF, EXTI_SOURCE_PIN15);

    /* Configure EXTI Line */
    exti_init(EXTI_15, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_15);

}
/*!
    \brief      esc exti sync0 gpio configuration.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_sync0_configuration(void)
{

    /** Enable the GPIO Clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_13);

    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOB, EXTI_SOURCE_PIN13);

    /* Configure EXTI Line */
    exti_init(EXTI_13, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_13);

}

/*!
    \brief      esc exti sync1 gpio configuration.
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_sync1_configuration(void)
{
    /** Enable the GPIO Clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /** Configure pin as input floating */
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_10);


    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 2U);

    /* connect  EXTI line to button GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN10);

    /* Configure EXTI Line */
    exti_init(EXTI_10, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_10);
}

/*!
    \brief      irq disable global int
    \param[in]  none
    \param[out] none
    \retval     none
*/
void disable_global_int(void)
{

    NVIC_DisableIRQ(EXTI10_15_IRQn);

    NVIC_DisableIRQ(TIMER2_IRQn);

}

/*!
    \brief      irq enable global int
    \param[in]  none
    \param[out] none
    \retval     none
*/
void enable_global_int(void)
{

    NVIC_EnableIRQ(EXTI10_15_IRQn);

    NVIC_EnableIRQ(TIMER2_IRQn);
}
