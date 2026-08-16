/*!
    \file    esc_spi.h
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

#ifndef ESC_SPI_H
#define ESC_SPI_H

#include "gd32h7xx.h"

/* esc USE SPI definitions */
#define SPIx                           SPI3

/* esc USE SPI SCK definitions */
#define SPIx_SCK_PIN                   GPIO_PIN_2
#define SPIx_SCK_GPIO_PORT             GPIOE

/* esc USE SPI MISO definitions */
#define SPIx_MISO_PIN                  GPIO_PIN_5
#define SPIx_MISO_GPIO_PORT            GPIOE

/* esc USE SPI MOSI definitions */
#define SPIx_MOSI_PIN                  GPIO_PIN_6
#define SPIx_MOSI_GPIO_PORT            GPIOE

/* esc USE SPI CS definitions */
#define SELECT_SPI    gpio_bit_reset(GPIOE,GPIO_PIN_4)
#define DESELECT_SPI  gpio_bit_set(GPIOE,GPIO_PIN_4)

/* CS SELECT */
#define CSLOW()       SELECT_SPI
/* CS DESELECT */
#define CSHIGH()      DESELECT_SPI

/*  SPI CMD definitions */
#define CMD_FAST_READ                   0x0B
#define CMD_SERIAL_WRITE                0x02
#define CMD_FAST_READ_DUMMY             1

/* function declarations */
/* init spi gpio configuration */
void spi_gpio_init(uint32_t spi_psc_value);

/* write word data to esc */
void spi_write_word(uint16_t address, uint32_t val);
/* read word data from esc */
uint32_t spi_read_word(uint16_t address);

/* write data to esc by CCTL register */
void spi_write_reg(uint8_t *writebuffer, uint16_t address, uint8_t count);
/* Read data from esc by CCTL register */
void spi_read_reg(uint8_t *readbuffer, uint16_t address, uint8_t count);

/* write register data to esc */
void spi_write_register(uint8_t *writebuffer, uint16_t address, uint16_t count);
/* read register data from esc */
void spi_read_register(uint8_t *ReadBuffer, uint16_t address, uint16_t count);
#endif /* ESC_SPI_H */
