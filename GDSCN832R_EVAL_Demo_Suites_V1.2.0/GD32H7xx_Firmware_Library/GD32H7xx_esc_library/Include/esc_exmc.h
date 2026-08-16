/*!
    \file    esc_exmc.h
    \brief   esc exmc driver basic configuration

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

#ifndef ESC_EXMC_H
#define ESC_EXMC_H

#include "gd32h7xx.h"

/* max read and write address  */
#define BANK0_REGION0_ADDR                     ((uint32_t)0x60000000U)
#define NOR_MAX_ADDRESS                        ((uint32_t)0x00FFFFFFU)
#define EXMC_NOR_BASE_ADDRESS                  BANK0_REGION0_ADDR

/* BUS WIDTH SELECT 8BIT/16BIT */
#define ESC_PRAM_READ_BUSY                      ((unsigned int)1 << 31)
#define ESC_PRAM_READ_ABORT                    ((unsigned int)1 << 30)
#define ESC_PRAM_READ_AVAIL                    ((unsigned int)1)

#define ESC_PRAM_WRITE_BUSY                    ((unsigned int)1 << 31)
#define ESC_PRAM_WRITE_ABORT                   ((unsigned int)1 << 30)
#define ESC_PRAM_WRITE_AVAIL                   ((unsigned int)1)

#define PRAM_AVBL_COUNT_MASK                   0x1F

#define AXI_ADDR_TRANS(A)                      (EXMC_NOR_BASE_ADDRESS + (A))

#define nor_16bit_write(address, data)         (*(volatile uint16_t *)(AXI_ADDR_TRANS(address*2)) = (uint16_t)(data))
#define nor_16bit_read(address)                (*(volatile uint16_t *)(AXI_ADDR_TRANS(address*2)))
#define nor_write(address, data)               (*(volatile uint8_t *)(AXI_ADDR_TRANS(address)) = (uint8_t)(data))
#define nor_read(address)                      (*(volatile uint8_t *)(AXI_ADDR_TRANS(address)))


#define   nor_sy_write(address, data)          (*(volatile uint32_t *)(AXI_ADDR_TRANS(address)) = (uint32_t)(data))
#define   nor_sy_read(address)                 (*(volatile uint32_t *)(AXI_ADDR_TRANS(address)))


#define   nor_64bit_sy_write(address, data)    (*(volatile uint64_t *)(AXI_ADDR_TRANS(address))) = (uint64_t)(data)
#define   nor_64bit_sy_read(address)           (*(volatile uint64_t *)(AXI_ADDR_TRANS(address)))

/* 32bit value union */
typedef union {
    uint32_t Val;
    struct {
        uint8_t LB;
        uint8_t HB;
        uint8_t UB;
        uint8_t MB;
    } byte;
    struct {
        uint16_t LW;
        uint16_t HW;
    } half_word;
} UINT32_DATA;

extern uint32_t exmc_width;

/* function declarations */
/* init exmc gpio configuration */
void exmc_nor_gpio_config(void);
/* exmc nor async init configuration */
void exmc_nor_async_init(void);

/* exmc nor write sync init configuration */
void exmc_nor_write_sync_init(void);
/* exmc nor read sync init configuration */
void exmc_nor_read_sync_init(void);

/* asynchronous EXMC16bit read/writ function  */
uint32_t exmc_16bit_read(uint32_t address);
void exmc_16bit_write(uint32_t address, uint32_t data);
/* synchronous EXMC8bit read/writfunction  */
uint32_t exmc_sy_read(uint32_t address);
uint64_t exmc_sy_read_64bit(uint32_t address);
void exmc_sy_write(uint32_t address, uint32_t data);
void exmc_sy_write_64bit(uint32_t address, uint64_t data);
/* synchronous EXMC16bit read/writfunction  */
uint32_t exmc_16bit_sy_read(uint32_t address);
uint64_t exmc_16bit_sy_read_64bit(uint32_t address);
void exmc_16bit_sy_write(uint32_t address, uint32_t data);
void exmc_16bit_sy_write_64bit(uint32_t address, uint64_t data);


/* Asynchronous write word data to esc */
void exmc_write(uint32_t address, uint32_t val);
/* Asynchronous read data from esc */
uint32_t exmc_read(uint32_t address);

/* write data to esc by CCTL register */
void exmc_write_reg(uint8_t *writebuffer, uint16_t address, uint8_t count);
/* Read data from esc by CCTL register */
void exmc_read_reg(uint8_t *readbuffer, uint16_t address, uint8_t count);

/* write register data to esc */
void exmc_write_register(uint8_t *writebuffer, uint16_t address, uint16_t count);
/* read register data from esc */
void exmc_read_register(uint8_t *readbuffer, uint16_t address, uint16_t count);
#endif
