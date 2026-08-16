/*!
    \file    esc_spi.c
    \brief   esc spi driver basic configuration

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
#include "esc_syscfg.h"
#include <string.h>

static void rcu_config(void);
static void spi_config(uint32_t spi_psc_value);

/*!
    \brief      SPI Port initialize
    \param[in]  spi_psc_value: the pcs of spi value
    \param[out] none
    \retval     none
*/
void spi_gpio_init(uint32_t spi_psc_value)
{
    /* peripheral clock enable */
    rcu_config();
    /* SPI config */
    spi_config(spi_psc_value);

    gpio_bit_reset(GPIOE, GPIO_PIN_4);

    /* SPI/I2S master start transfer */
    spi_master_transfer_start(SPI3, SPI_TRANS_START);
}

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_SPI3);
    rcu_spi_clock_config(IDX_SPI3, RCU_SPISRC_APB2);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(uint32_t spi_psc_value)
{
    spi_parameter_struct spi_init_struct;

    /* connect port to
    SPI0_NSS->PE4
    SPI0_SCK->PE2
    SPI0_MISO->PE5
    SPI0_MOSI->PE6 */

    gpio_af_set(GPIOE, GPIO_AF_5, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);

    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);

    gpio_mode_set(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_12MHZ, GPIO_PIN_4);
    gpio_bit_set(GPIOE, GPIO_PIN_4);

    /* deinitilize SPI and the parameters */
    spi_i2s_deinit(SPI3);
    spi_struct_para_init(&spi_init_struct);

    /* SPI parameter config */
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.data_size = SPI_DATASIZE_8BIT;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.prescale = spi_psc_value;
    spi_init(SPI3, &spi_init_struct);

    /* enable SPI byte access */
    spi_byte_access_enable(SPI3);
    /* enable SPI NSS output */
    spi_nss_output_enable(SPI3);

    spi_enable(SPI3);
}

/*!
    \brief      read and write data to esc
    \param[in]  cmd: the data send to esc
    \param[out] none
    \retval     temp: the data read from esc
*/
static uint8_t wr_cmd(uint8_t cmd)
{
    uint8_t temp;

    /* wait for SPI1 Tx buffer empty */
    while(spi_i2s_flag_get(SPI3, SPI_FLAG_TP) == RESET);
    /* send SPI1 data */
    spi_i2s_data_transmit(SPI3, cmd);
    /* wait for SPI1 data reception */
    while(spi_i2s_flag_get(SPI3, SPI_FLAG_RP) == RESET);
    /* read SPI1 received data */
    temp =  spi_i2s_data_receive(SPI3);
    return temp;
}

/*!
    \brief      read byte data from esc
    \param[in]  none
    \param[out] none
    \retval     data: read from esc
*/
static uint8_t spi_read(void)
{
    uint8_t data;
    data = wr_cmd(0);
    return (data);
}

/*!
    \brief      write byte data to esc
    \param[in]  data: the data send to esc
    \param[out] none
    \retval     none
*/
static void spi_write(uint8_t data)
{
    wr_cmd(data);
}

/*!
    \brief      read word data from esc
    \param[in]  address：the address to be read from esc
    \param[out] none
    \retval     data: word data from esc
*/
uint32_t spi_read_word(uint16_t address)
{
    UINT32_VAL dwresult;
    UINT16_VAL waddr;

    waddr.Val  = address;

    /* Assert CS line */
    CSLOW();

    /* Write Command */
    spi_write(CMD_FAST_READ);

    /* Write Address */
    spi_write(waddr.byte.HB);
    spi_write(waddr.byte.LB);

    /* Dummy Byte */
    spi_write(CMD_FAST_READ_DUMMY);

    /* Read Bytes */
    dwresult.byte.LB = spi_read();
    dwresult.byte.HB = spi_read();
    dwresult.byte.UB = spi_read();
    dwresult.byte.MB = spi_read();

    /* De-Assert CS line */
    CSHIGH();

    return dwresult.Val;
}

/*!
    \brief      write word data to esc
    \param[in]  address: the address write to esc
    \param[in]  val：the data write to esc
    \param[out] none
    \retval     none
*/
void spi_write_word(uint16_t address, uint32_t val)
{
    UINT32_VAL dwdata;
    UINT16_VAL waddr;

    waddr.Val  = address;
    dwdata.Val = val;

    /* assert CS line */
    CSLOW();

    /* write Command */
    spi_write(CMD_SERIAL_WRITE);

    /* write Address */
    spi_write(waddr.byte.HB);
    spi_write(waddr.byte.LB);

    /* write Bytes */
    spi_write(dwdata.byte.LB);
    spi_write(dwdata.byte.HB);
    spi_write(dwdata.byte.UB);
    spi_write(dwdata.byte.MB);

    /* de-assert CS line */
    CSHIGH();
}

/*!
    \brief      write address to esc
    \param[in]  address：the address write to esc
    \param[out] none
    \retval     none
*/
static void spi_send_addr(uint16_t address)
{
    UINT16_VAL waddr;

    waddr.Val  = address;

    /* write address */
    spi_write(waddr.byte.HB);
    spi_write(waddr.byte.LB);
}

/*!
    \brief      read word from esc in burst mode
    \param[in]  none
    \param[out] none
    \retval     word data from esc
*/
static uint32_t spi_read_burst_mode()
{
    UINT32_VAL dwresult;

    /* read bytes */
    dwresult.byte.LB = spi_read();
    dwresult.byte.HB = spi_read();
    dwresult.byte.UB = spi_read();
    dwresult.byte.MB = spi_read();

    return dwresult.Val;
}

/*!
\brief      write data to esc in burst mode
\param[in]  val：the data write to esc
\param[out] none
\retval     none
*/
static void spi_write_burst_mode(uint32_t val)
{
    UINT32_VAL dwdata;
    dwdata.Val = val;

    /* write bytes */
    spi_write(dwdata.byte.LB);
    spi_write(dwdata.byte.HB);
    spi_write(dwdata.byte.UB);
    spi_write(dwdata.byte.MB);
}

/*!
    \brief      write esc core reg by CCTL register
    \param[in]  writebuffer: pointer to data to be written
    \param[in]  address: write start address
    \param[in]  count: size of data to write
    \param[out] none
    \retval     none
*/
void spi_write_reg(uint8_t *writebuffer, uint16_t address, uint8_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0;
    UINT16_VAL waddr;

    /* reset/stop any previous commands */
    param32_1.Val = (unsigned long int)ESC_CCTL_CMD_CCTL_STOP;
    spi_write_word(ESC_CCTL_CMD, param32_1.Val);

    /* wait the BUSY bit clear.*/
    do {
        param32_1.Val = spi_read_word(ESC_CCTL_CMD);
    } while((param32_1.v[3] & BUSY_MASK));

    for(i = 0; i < count; i++) {
        param32_1.v[i] = writebuffer[i];
    }

    /* write buffer data to ESC_CCTL_DATA register */
    spi_write_word(ESC_CCTL_DATA, param32_1.Val);

    waddr.Val = address;

    param32_1.v[0] = waddr.byte.LB;
    param32_1.v[1] = waddr.byte.HB;
    param32_1.v[2] = count;
    param32_1.v[3] = ESC_WRITE_BYTE;

    /* write address to ESC_CCTL_CMD register */
    spi_write_word(ESC_CCTL_CMD, param32_1.Val);

    /* wait the BUSY bit clear */
    do {
        param32_1.Val = spi_read_word(ESC_CCTL_CMD);

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
void spi_read_reg(uint8_t *readbuffer, uint16_t address, uint8_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0;
    UINT16_VAL waddr;
    waddr.Val = address;

    /* reset/stop any previous commands.*/
    param32_1.Val = (unsigned long int)ESC_CCTL_CMD_CCTL_STOP;
    spi_write_word(ESC_CCTL_CMD, param32_1.Val);

    /* wait the BUSY bit clear */
    do {
        param32_1.Val = spi_read_word(ESC_CCTL_CMD);
    } while((param32_1.v[3] & BUSY_MASK));

    param32_1.v[0] = waddr.byte.LB;
    param32_1.v[1] = waddr.byte.HB;
    param32_1.v[2] = count;
    param32_1.v[3] = ESC_READ_BYTE;

    /* write address to ESC_CCTL_CMD register */
    spi_write_word(ESC_CCTL_CMD, param32_1.Val);

    /* wait the BUSY bit clear */
    do {
        param32_1.Val = spi_read_word(ESC_CCTL_CMD);

    } while(param32_1.v[3] & ESC_CSR_BUSY);

    /* read buffer data from ESC_CCTL_DATA register */
    param32_1.Val = spi_read_word(ESC_CCTL_DATA);

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
void spi_write_pdram(uint8_t *writebuffer, uint16_t address, uint16_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0, nlength, nbyte_position, nwrt_spcavl_count;

    /* reset or Stop any previous commands */
    param32_1.Val = ESC_PRAM_CR_PRAM_STOP_WRITE;

    spi_write_word(ESC_PRAM_CW, param32_1.Val);

    /* wait the BUSY bit clear */
    do {
        param32_1.Val = spi_read_word(ESC_PRAM_CW);
    } while((param32_1.v[3] & BUSY_MASK));


    /* write address and length in the ESC PRAM address and length write register (ESC_PRAM_ALW) */
    param32_1.w[0] = address;
    param32_1.w[1] = count;
    spi_write_word(ESC_PRAM_ALW, param32_1.Val);

    /* set PRAM Write Busy (PRAM_BUSY_WRITE) bit(ESC PRAM command write register) to start write operatrion */
    param32_1.Val = ESC_PRAM_CR_PRAM_BUSY_WRITE;
    spi_write_word(ESC_PRAM_CW, param32_1.Val);

    /* read PRAM valid data write(PRAM_VALID_DATA_WRITE) bit is set */
    do {
        param32_1.Val = spi_read_word(ESC_PRAM_CW);
    } while(!(param32_1.v[0] & PRAM_VALID_DATA_MASK));

    /* check write data available count*/
    nwrt_spcavl_count = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /* write data to ESC PRAM FIFO data write register */
    nbyte_position = (address & 0x03);
    nlength = (4 - nbyte_position) > count ? count : (4 - nbyte_position);
    param32_1.Val = 0;

    memcpy(&param32_1.v[nbyte_position], writebuffer + i, nlength);

    spi_write_word(ESC_PRAM_FIFO_DW, param32_1.Val);

    nwrt_spcavl_count--;
    count -= nlength;
    i += nlength;

    /* auto increment mode */
    CSLOW();

    /* write command */
    spi_write(CMD_SERIAL_WRITE);

    spi_send_addr(ESC_PRAM_FIFO_DW);

    while(count) {
        nlength = count > 4 ? 4 : count;
        param32_1.Val = 0;
        memcpy(&param32_1, (writebuffer + i), nlength);

        spi_write_burst_mode(param32_1.Val);
        i += nlength;
        count -= nlength;
        nwrt_spcavl_count--;
    }
    CSHIGH();

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
void spi_read_pdram(uint8_t *readbuffer, uint16_t address, uint16_t count)
{
    UINT32_VAL param32_1 = {0};
    uint8_t i = 0, nlength, nbyte_position;
    uint8_t nread_space_avbl_count;

    /* reset/stop any previous commands */
    param32_1.Val = (unsigned long int)ESC_PRAM_CR_PRAM_STOP_READ;
    spi_write_word(ESC_PRAM_CR, param32_1.Val);

    /* wait the BUSY bit clear.*/
    do {
        param32_1.Val = spi_read_word(ESC_PRAM_CR);
    } while((param32_1.v[3] & BUSY_MASK));

    /* write address and length in the ESC PRAM address and length read register (ESC_PRAM_ALR) */
    param32_1.w[0] = address;
    param32_1.w[1] = count;
    spi_write_word(ESC_PRAM_ALR, param32_1.Val);

    /* set PRAM Read Busy (PRAM_BUSY_READ) bit(ESC PRAM command read register) to start read operatrion */
    param32_1.Val = ESC_PRAM_CR_PRAM_BUSY_READ;
    spi_write_word(ESC_PRAM_CR, param32_1.Val);

    /* read PRAM valid data read (PRAM_VALID_DATA_READ) bit is set */
    do {
        param32_1.Val = spi_read_word(ESC_PRAM_CR);
    } while(!(param32_1.v[0] & PRAM_VALID_DATA_MASK));

    /* check read data available count*/
    nread_space_avbl_count = param32_1.v[1] & PRAM_SPACE_AVBL_COUNT_MASK;

    /* read data from ESC PRAM FIFO data read register */
    param32_1.Val = spi_read_word(ESC_PRAM_FIFO_DR);
    nread_space_avbl_count--;
    nbyte_position = (address & 0x03);
    nlength = (4 - nbyte_position) > count ? count : (4 - nbyte_position);
    memcpy(readbuffer + i, &param32_1.v[nbyte_position], nlength);
    count -= nlength;
    i += nlength;

    /* auto increment mode */
    CSLOW();

    /* write Command */
    spi_write(CMD_FAST_READ);

    /* send address */
    spi_send_addr(ESC_PRAM_FIFO_DR);

    /* dummy byte */
    spi_write(CMD_FAST_READ_DUMMY);

    while(count) {
        param32_1.Val = spi_read_burst_mode();
        nlength = count > 4 ? 4 : count;
        memcpy((readbuffer + i), &param32_1, nlength);

        i += nlength;
        count -= nlength;
        nread_space_avbl_count --;
    }
    CSHIGH();

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
void spi_write_register(uint8_t *writebuffer, uint16_t address, uint16_t count)
{
    if(address >= 0x1000) {
        spi_write_pdram(writebuffer, address, count);
    } else {
        spi_write_reg(writebuffer, address, count);
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
void spi_read_register(uint8_t *readBuffer, uint16_t address, uint16_t count)
{
    if(address >= 0x1000) {
        spi_read_pdram(readBuffer, address, count);
    } else {
        spi_read_reg(readBuffer, address, count);
    }
}
