/*!
    \file    esc_ospi.h
    \brief   esc ospi driver basic configuration

    \version 2024-11-22, V1.0.0, firmware for esc
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

#ifndef ESC_OSPI_H
#define ESC_OSPI_H

#include "gd32h7xx_ospim.h"
#include "gd32h7xx_ospi.h"

#define CMD_EQIO            0x38U                                   /*!< enable QSPI command */
#define CMD_EOIO            0x3AU                                   /*!< enable OSPI command */
#define CMD_RSTIO           0xFFU                                   /*!< rese IO command */

#define CMD_READ            0x0BU                                   /*!< read command */
#define CMD_SDOR            0x3BU                                   /*!< SPI dual output read command */
#define CMD_SDIOR           0xBBU                                   /*!< SPI dual I/O read command */
#define CMD_SQOR            0x6BU                                   /*!< SPI quad output read command */
#define CMD_SQIOR           0xEBU                                   /*!< SPI quad I/O read command */

#define CMD_WRITE           0x02U                                   /*!< write command */
#define CMD_SDDW            0x32U                                   /*!< SPI dual data write command */
#define CMD_SDADW           0xB2U                                   /*!< SPI dual address/data write command */
#define CMD_SQDW            0x62U                                   /*!< SPI quad data write command */
#define CMD_SQADW           0xE2U                                   /*!< SPI quad address/data write command */

#define CMD_RSTQIO          0xFFU                                   /*!< reset QSPI command */
#define CMD_RSTOIO          0xFFU                                   /*!< reset OSPI command */

/* SPI mode select */
typedef enum {
    SPI_MODE = 0U,                                                  /*!< SPI mode */
    QSPI_MODE,                                                      /*!< QSPI mode */
    OSPI_MODE                                                       /*!< OSPI mode */
} interface_mode;

/* address increase mode select */
typedef enum {
    ADDR_NO_INC = 0U,                                               /*!< no increase mode */
    ADDR_INC = BIT(14),                                             /*!< increase mode */
    ADDR_DEC = BIT(15)                                              /*!< decrease mode */
} addr_inc_mode;

#define OSPI_INTERFACE             OSPI1
#define MODE_SELECT                OSPI_MODE

extern ospi_parameter_struct ospi_init_struct;

/* initialize OSPI/OSPIM and GPIO */
void ospi_hw_init(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct);
/* enable qspi mode */
void ospi_enable_qspi_mode(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct, interface_mode mode);
/* enable ospi mode */
void ospi_enable_ospi_mode(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct, interface_mode mode);
/* reset spi mode */
void ospi_reset_spi_mode(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct, interface_mode mode);
/* ospi write data */
void ospi_write(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct, interface_mode mode, uint8_t cmd, addr_inc_mode inc_mode,
                uint32_t addr, uint8_t *pdata, uint32_t data_size);
/* ospi read data */
void ospi_read(uint32_t ospi_periph, ospi_parameter_struct *ospi_struct, interface_mode mode, uint8_t cmd, addr_inc_mode inc_mode,
               uint32_t addr, uint8_t *pdata, uint32_t data_size);
/* write esc core reg by csr */
void ospi_write_reg(uint8_t *writebuffer, uint16_t address, uint8_t count, uint32_t ospi_periph, ospi_parameter_struct *ospi_struct,
                    interface_mode mode, uint8_t write_cmd, uint8_t read_cmd, addr_inc_mode inc_mode);
/* read esc core reg by csr */
void ospi_read_reg(uint8_t *readbuffer, uint16_t address, uint8_t count, uint32_t ospi_periph, ospi_parameter_struct *ospi_struct,
                   interface_mode mode, uint8_t write_cmd, uint8_t read_cmd, addr_inc_mode inc_mode);
/* read esc register */
void ospi_read_register(uint8_t *readbuffer, uint16_t address, uint16_t count);
/* write esc register */
void ospi_write_register(uint8_t *writebuffer, uint16_t address, uint16_t count);

#endif /* ESC_OSPI_H */
