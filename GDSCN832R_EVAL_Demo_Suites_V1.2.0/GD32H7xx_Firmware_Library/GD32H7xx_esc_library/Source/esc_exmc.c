/*!
    \file    esc_exmc.c
    \brief   esc exmc  driver basic configuration

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

#include "esc_exmc.h"
#include  "esc_syscfg.h"
#include <string.h>

exmc_norsram_parameter_struct nor_init_struct;
exmc_norsram_timing_parameter_struct nor_timing_init_struct;
uint32_t nor_region;
uint32_t exmc_width = EXMC_NOR_DATABUS_WIDTH_8B;

/*!
    \brief      nor flash peripheral gpio initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exmc_nor_gpio_config(void)
{
    nor_region = EXMC_BANK0_NORSRAM_REGION0;
    /* EXMC clock enable */
    rcu_periph_clock_enable(RCU_EXMC);
    /* EXMC gpio clock enable */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    /*
    PD14 - EXMC_D0
    PD15 - EXMC_D1
    PD0  - EXMC_D2
    PD1  - EXMC_D3
    PD8  - EXMC_D13
    PD9  - EXMC_D14
    PD10 - EXMC_D15
    */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 |
                GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 |
                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1 |
                            GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    /*
    PE7  - EXMC_D4
    PE8  - EXMC_D5
    PE9  - EXMC_D6
    PE10 - EXMC_D7
    PE11 - EXMC_D8
    PE12 - EXMC_D9
    PE13 - EXMC_D10
    PE14 - EXMC_D11
    PE15 - EXMC_D12
    */
    gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                  GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                            GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    /*
    PD3 - EXMC_CLK
    PD4 - EXMC_NOE
    PD5 - EXMC_NWE
    */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    /*
    PC7 - EXMC_NE0
    */
    gpio_af_set(GPIOC, GPIO_AF_9, GPIO_PIN_7);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_7);
    /*
    PC6 - EXMC_NWAIT
    */
    gpio_af_set(GPIOC, GPIO_AF_12, GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_6);
    /*
    PB7 - EXMC_NADV
    */
    gpio_af_set(GPIOB, GPIO_AF_12, GPIO_PIN_7);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_85MHZ, GPIO_PIN_7);
}
/*!
    \brief      nor flash peripheral async mode initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exmc_nor_async_init(void)
{
    nor_region = EXMC_BANK0_NORSRAM_REGION0;

    exmc_norsram_enable(nor_region);

    nor_timing_init_struct.asyn_access_mode       = EXMC_ACCESS_MODE_B;

    nor_timing_init_struct.bus_latency            = 15;
    nor_timing_init_struct.asyn_data_setuptime    = 30;
    nor_timing_init_struct.asyn_address_holdtime  = 15;
    nor_timing_init_struct.asyn_address_setuptime = 15;
    nor_timing_init_struct.syn_clk_division       = EXMC_SYN_CLOCK_RATIO_2_CLK;

    nor_init_struct.norsram_region   = nor_region;
    nor_init_struct.write_mode       = EXMC_ASYN_WRITE;
    nor_init_struct.extended_mode    = DISABLE;

    nor_init_struct.asyn_wait        = ENABLE;

    nor_init_struct.nwait_signal     = DISABLE;

    nor_init_struct.memory_write     = ENABLE;
    nor_init_struct.nwait_config     = EXMC_NWAIT_CONFIG_DURING;
    nor_init_struct.nwait_polarity   = EXMC_NWAIT_POLARITY_LOW;

    nor_init_struct.burst_mode       = DISABLE;
    nor_init_struct.databus_width    = exmc_width;
    nor_init_struct.memory_type      = EXMC_MEMORY_TYPE_NOR;
    nor_init_struct.address_data_mux = ENABLE;

    nor_init_struct.read_write_timing = &nor_timing_init_struct;
    nor_init_struct.write_timing     = &nor_timing_init_struct;

    exmc_norsram_init(&nor_init_struct);
    exmc_norsram_enable(nor_region);
}

/*!
    \brief      nor flash peripheral write sync mode initialize
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exmc_nor_write_sync_init(void)
{
    nor_region = EXMC_BANK0_NORSRAM_REGION0;

    exmc_norsram_enable(nor_region);
    nor_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;

    nor_timing_init_struct.bus_latency = 0;
    nor_timing_init_struct.asyn_data_setuptime = 4;
    nor_timing_init_struct.asyn_address_holdtime = 1;
    nor_timing_init_struct.asyn_address_setuptime = 1;
    nor_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
    nor_timing_init_struct.syn_data_latency = EXMC_DATALAT_4_CLK;

    nor_init_struct.norsram_region   = nor_region;
    nor_init_struct.write_mode       = EXMC_SYN_WRITE;
    nor_init_struct.extended_mode    = DISABLE;
    nor_init_struct.asyn_wait        = DISABLE;
    nor_init_struct.nwait_signal     = ENABLE;

    nor_init_struct.memory_write     = ENABLE;
    nor_init_struct.nwait_config     = EXMC_NWAIT_CONFIG_BEFORE;
    nor_init_struct.nwait_polarity   = EXMC_NWAIT_POLARITY_LOW;
    nor_init_struct.burst_mode       = ENABLE;
    nor_init_struct.databus_width    = exmc_width;
    nor_init_struct.memory_type      = EXMC_MEMORY_TYPE_NOR;
    nor_init_struct.address_data_mux = ENABLE;

    nor_init_struct.write_timing = &nor_timing_init_struct;
    nor_init_struct.read_write_timing = &nor_timing_init_struct;

    exmc_norsram_init(&nor_init_struct);
    exmc_norsram_enable(nor_region);
}

/*!
    \brief      nor flash peripheral initialize for read operate
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exmc_nor_read_sync_init(void)
{
    nor_region = EXMC_BANK0_NORSRAM_REGION0;

    exmc_norsram_enable(nor_region);
    nor_timing_init_struct.asyn_access_mode = EXMC_ACCESS_MODE_A;

    nor_timing_init_struct.bus_latency = 0;
    nor_timing_init_struct.asyn_data_setuptime = 4;
    nor_timing_init_struct.asyn_address_holdtime = 1;
    nor_timing_init_struct.asyn_address_setuptime = 1;
    nor_timing_init_struct.syn_clk_division = EXMC_SYN_CLOCK_RATIO_2_CLK;
    nor_timing_init_struct.syn_data_latency = EXMC_DATALAT_6_CLK;

    nor_init_struct.norsram_region   = nor_region;
    nor_init_struct.write_mode       = EXMC_SYN_WRITE;
    nor_init_struct.extended_mode    = DISABLE;
    nor_init_struct.asyn_wait        = DISABLE;
    nor_init_struct.nwait_signal     = ENABLE;

    nor_init_struct.memory_write     = ENABLE;
    nor_init_struct.nwait_config     = EXMC_NWAIT_CONFIG_BEFORE;
    nor_init_struct.nwait_polarity   = EXMC_NWAIT_POLARITY_LOW;
    nor_init_struct.burst_mode       = ENABLE;
    nor_init_struct.databus_width    = exmc_width;
    nor_init_struct.memory_type      = EXMC_MEMORY_TYPE_NOR;
    nor_init_struct.address_data_mux = ENABLE;

    nor_init_struct.write_timing = &nor_timing_init_struct;
    nor_init_struct.read_write_timing = &nor_timing_init_struct;

    exmc_norsram_init(&nor_init_struct);
    exmc_norsram_enable(nor_region);
}

/*!
    \brief      Asynchronous read word data from esc
    \param[in]  address:the address to be read from esc
    \param[out] none
    \retval     data: word data from esc
*/
uint32_t exmc_read(uint32_t address)
{
    UINT32_DATA result = {0};

    if(exmc_width == EXMC_NOR_DATABUS_WIDTH_8B) {
        result.byte.LB = nor_read(address);
        result.byte.HB = nor_read(address + 1);
        result.byte.UB = nor_read(address + 2);
        result.byte.MB = nor_read(address + 3);
    } else {
        result.half_word.LW = nor_16bit_read(address);
        result.half_word.HW = nor_16bit_read((address + 2));
    }
    return result.Val;
}

/*!
    \brief      Asynchronous write word data to esc
    \param[in]  address: the address write to esc
    \param[in]  val:the data write to esc
    \param[out] none
    \retval     none
*/
void exmc_write(uint32_t address, uint32_t val)
{
    UINT32_DATA result = {0};

    result.Val = val;

    if(exmc_width == EXMC_NOR_DATABUS_WIDTH_8B) {
        nor_write(address, result.byte.LB);
        nor_write(address + 1, result.byte.HB);
        nor_write(address + 2, result.byte.UB);
        nor_write(address + 3, result.byte.MB);
    } else {
        nor_16bit_write(address, result.half_word.LW);
        nor_16bit_write((address + 2), result.half_word.HW);
    }
}

/*!
    \brief      synchronous read
    \param[in]
    \param[out]
    \retval
*/
uint32_t exmc_sy_read(uint32_t address)
{
    return nor_sy_read(address);
}

uint64_t exmc_sy_read_64bit(uint32_t address)
{
    return nor_64bit_sy_read(address);
}

uint32_t exmc_16bit_sy_read(uint32_t address)
{

    return nor_sy_read(address * 2);
}
uint64_t exmc_16bit_sy_read_64bit(uint32_t address)
{

    return nor_64bit_sy_read(address * 2);
}

/*!
    \brief      synchronous write
    \param[in]
    \param[out]
    \retval
*/
void exmc_sy_write(uint32_t address, uint32_t data)
{
    nor_sy_write(address, data);
}
void exmc_sy_write_64bit(uint32_t address, uint64_t data)
{
    nor_64bit_sy_write(address, data);
}
void exmc_16bit_sy_write(uint32_t address, uint32_t data)
{
    nor_sy_write(address * 2, data);
}
void exmc_16bit_sy_write_64bit(uint32_t address, uint64_t data)
{
    nor_64bit_sy_write(address * 2, data);
}

/*!
    \brief      write esc core reg by CCTL register
    \param[in]  writebuffer: pointer to data to be written
    \param[in]  address: write start address
    \param[in]  count: size of data to write
    \param[out] none
    \retval     none
*/
void exmc_write_reg(uint8_t *writebuffer, uint16_t address, uint8_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0;
    UINT16_VAL waddr;

    /*Reset/Stop any previous commands.*/
    param32_1.Val = (unsigned long int)ESC_CCTL_CMD_CCTL_STOP;
    exmc_write(ESC_CCTL_CMD, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_CCTL_CMD);
    } while((param32_1.v[3] & BUSY_MASK));

    for(i = 0; i < count; i++) {
        param32_1.v[i] = writebuffer[i];
    }

    /*write buffer data to ESC_CCTL_DATA register.*/
    exmc_write(ESC_CCTL_DATA, param32_1.Val);

    waddr.Val = address;

    param32_1.v[0] = waddr.byte.LB;
    param32_1.v[1] = waddr.byte.HB;
    param32_1.v[2] = count;
    param32_1.v[3] = ESC_WRITE_BYTE;

    /*write Address to ESC_CCTL_CMD register.*/
    exmc_write(ESC_CCTL_CMD, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_CCTL_CMD);

    } while(param32_1.v[3] & ESC_CSR_BUSY);

    return;
}

/*!
    \brief      read esc core reg by CCTL register
    \param[in]  readbuffer: pointer to data to be read
    \param[in]  address: read start address
    \param[in]  count: size of data to read
\param[out] none
\retval     none
*/
void exmc_read_reg(uint8_t *readbuffer, uint16_t address, uint8_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0;
    UINT16_VAL waddr;

    waddr.Val = address;

    /*Reset/Stop any previous commands.*/
    param32_1.Val = (unsigned long int)ESC_CCTL_CMD_CCTL_STOP;
    exmc_write(ESC_CCTL_CMD, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_CCTL_CMD);
    } while((param32_1.v[3] & BUSY_MASK));

    param32_1.v[0] = waddr.byte.LB;
    param32_1.v[1] = waddr.byte.HB;
    param32_1.v[2] = count;
    param32_1.v[3] = ESC_READ_BYTE;

    /*write Address to ESC_CCTL_CMD register.*/
    exmc_write(ESC_CCTL_CMD, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_CCTL_CMD);

    } while(param32_1.v[3] & ESC_CSR_BUSY);

    /*read buffer data from ESC_CCTL_DATA register.*/
    param32_1.Val = exmc_read(ESC_CCTL_DATA);

    for(i = 0; i < count; i++) {
        readbuffer[i] = param32_1.v[i];
    }

    return;
}

/*!
    \brief      write data to esc pdram by CCTL register
    \param[in]  writebuffer: pointer to data to be written
    \param[in]  address: write start address
    \param[in]  count: size of data to write
    \param[out] none
    \retval     none
*/
void exmc_write_pdram(uint8_t *writebuffer, uint16_t address, uint16_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0U, nlength, nbyte_position, nwrt_spcavl_count;

    /*Reset or Stop any previous commands.*/
    param32_1.Val = ESC_PRAM_CR_PRAM_STOP_WRITE;
    exmc_write(ESC_PRAM_CW, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_PRAM_CW);
    } while((param32_1.v[3] & BUSY_MASK));

    /*Write address and length in the ESC PRAM address and length write register (ESC_PRAM_ALW)*/
    param32_1.w[0] = address;
    param32_1.w[1] = count;
    exmc_write(ESC_PRAM_ALW, param32_1.Val);

    /*Set PRAM Write Busy (PRAM_BUSY_WRITE) bit(ESC PRAM command write register) to start write operatrion*/
    param32_1.Val = ESC_PRAM_CR_PRAM_BUSY_WRITE;
    exmc_write(ESC_PRAM_CW, param32_1.Val);

    /*Read PRAM valid data write(PRAM_VALID_DATA_WRITE) bit is set*/
    do {
        param32_1.Val = exmc_read(ESC_PRAM_CW);
    } while(!(param32_1.v[0] & PRAM_VALID_DATA_MASK));

    /*Check write data available count*/
    nwrt_spcavl_count = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Write data to ESC PRAM FIFO data write register */
    nbyte_position = (address & 0x03);
    nlength = (4U - nbyte_position) > count ? count : (4U - nbyte_position);
    param32_1.Val = 0U;
    memcpy(&param32_1.v[nbyte_position], writebuffer + i, nlength);

    exmc_write(ESC_PRAM_FIFO_DW, param32_1.Val);

    nwrt_spcavl_count--;
    count -= nlength;
    i += nlength;

    while(nwrt_spcavl_count && count) {
        nlength = count > 4 ? 4 : count;
        param32_1.Val = 0;
        memcpy(&param32_1, (writebuffer + i), nlength);

        exmc_write(ESC_PRAM_FIFO_DW, param32_1.Val);

        i += nlength;
        count -= nlength;
        nwrt_spcavl_count--;
    }

    return;
}

/*!
    \brief      read data from esc pdram by CCTL register
    \param[in]  readbuffer: pointer to data to be read
    \param[in]  address: read start address
    \param[in]  count: size of data to read
    \param[out] none
    \retval     none
*/
void exmc_read_pdram(uint8_t *readbuffer, uint16_t address, uint16_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0, nlength, nbyte_position, nread_spaceavbl_count;

    /*Reset/stop any previous commands.*/
    param32_1.Val = (unsigned long int)ESC_PRAM_CR_PRAM_STOP_READ;
    exmc_write(ESC_PRAM_CR, param32_1.Val);

    /*wait the BUSY bit clear.*/
    do {
        param32_1.Val = exmc_read(ESC_PRAM_CR);
    } while((param32_1.v[3] & BUSY_MASK));

    /*Write address and length in the ESC PRAM address and length read register (ESC_PRAM_ALR)*/
    param32_1.w[0] = address;
    param32_1.w[1] = count;
    exmc_write(ESC_PRAM_ALR, param32_1.Val);

    /*Set PRAM Read Busy (PRAM_BUSY_READ) bit(ESC PRAM command read register) to start read operatrion*/
    param32_1.Val = ESC_PRAM_CR_PRAM_BUSY_READ;
    exmc_write(ESC_PRAM_CR, param32_1.Val);

    /*Read PRAM valid data read (PRAM_VALID_DATA_READ) bit is set*/
    do {
        param32_1.Val = exmc_read(ESC_PRAM_CR);
    } while(!(param32_1.v[0] & PRAM_VALID_DATA_MASK));

    /*Check read data available count*/
    nread_spaceavbl_count = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /*Read data from ESC PRAM FIFO data read register */
    param32_1.Val = exmc_read(ESC_PRAM_FIFO_DR);
    nread_spaceavbl_count--;
    nbyte_position = (address & 0x03);
    nlength = (4U - nbyte_position) > count ? count : (4U - nbyte_position);
    memcpy(readbuffer + i, &param32_1.v[nbyte_position], nlength);
    count -= nlength;
    i += nlength;

    while(count && nread_spaceavbl_count) {
        param32_1.Val = exmc_read(ESC_PRAM_FIFO_DR);
        nlength = count > 4 ? 4 : count;
        memcpy((readbuffer + i), &param32_1, nlength);
        i += nlength;
        count -= nlength;
        nread_spaceavbl_count--;
    }

    return;
}

/*!
    \brief      write esc register
    \param[in]  writebuffer: pointer to data to be write
    \param[in]  address: write start address
    \param[in]  count: size of data to write
    \param[out] none
    \retval     none
*/
void exmc_write_register(uint8_t *writebuffer, uint16_t address, uint16_t count)
{
    uint32_t tempbuff;
    if(address >= 0x1000) {
        exmc_write_pdram(writebuffer, address, count);
    } else {
        exmc_write_reg(writebuffer, address, count);
    }
}

/*!
    \brief      read esc register
    \param[in]  readbuffer: pointer to data to be read
    \param[in]  address: read start address
    \param[in]  count: size of data to read
    \param[out] none
    \retval     none
*/
void exmc_read_register(uint8_t *readbuffer, uint16_t address, uint16_t count)
{
    uint32_t tempbuff;

    if(address >= 0x1000) {
        exmc_read_pdram(readbuffer, address, count);
    } else {
        exmc_read_reg(readbuffer, address, count);
    }
}
