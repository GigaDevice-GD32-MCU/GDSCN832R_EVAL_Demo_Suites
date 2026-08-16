/*!
    \file    esc_phy.h
    \brief   ESC phy driver basic configuration

    \version 2024-11-22, V1.0.0, firmware for ESC
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

#ifndef ESC_PHY_H
#define ESC_PHY_H

#include "gd32h7xx.h"

/* registers definitions */
#define ESC_MII_CONTROL                         0x0510                              /*!< ESC MII Management control register */
#define ESC_MII_STATUS                          0x0511                              /*!< ESC MII Management status register */
#define ESC_PHY_ADDR                            0x0512                              /*!< ESC PHY address register */
#define ESC_PHY_DATA                            0x0514                              /*!< ESC PHY data register */
#define ESC_MII_PDI_STATE                       0x0517                              /*!< MII Management ECAT access state register */
#define ESC_PHY_PORT0_STA                       0x0518                              /*!< PHY Port 0 Status register */
#define ESC_PHY_PORT1_STA                       0x0519                              /*!< PHY Port 1 Status register */

/*ESC MII Management Control register 0x0510*/
#define MII_WRITE_EN                            BIT(1)                              /*!< MII write enable*/

/*ESC MII Management Status register 0x0511*/
#define MII_READ_CMD                            0x1                                 /*!< MII read  command*/
#define MII_WRITE_CMD                           0x2                                 /*!< MII write command*/

/*ESC MII Management Status register 0x0511*/
#define MII_BUSY_MASK                           0x0080                              /*!< MII busy*/

/*MII Management ECAT Access State register: 0x0517*/
#define MII_ACCESS                              BIT(0)                              /*!< access to MII management bit*/
#define PDI_ACCESS                              BIT(1)                              /*!< force PDI access state bit*/

/*Enable register (0x3A08) */
#define PHYAIE_MASK                             BIT(26)                             /*!< Ethernet PHY B interrupt enable bit */
#define PHYBIE_MASK                             BIT(27)                             /*!< Ethernet PHY A interrupt enable bit */

#define BIT(x)                                  ((uint32_t)((uint32_t)0x01U << (x)))
/*PHY register num*/
#define PHY_MMD_CTL                             0x0D
#define PHY_MMD_ADDR_DATA                       0x0E

/*MMD Access Control Register 0x0D*/
#define MMD_ADDR_FUN_MASK                       0x0000
#define MMD_DATA_FUN_MASK                       0x4000

/* internal PHY address select */
typedef enum {
    PHYA = 0x00,                                      /*!< PHY A address */
    PHYB = 0x01                                       /*!< PHY B address */
} PHY_ADDR;

/* read PHY data from specified register */
uint32_t esc_phy_read(uint8_t phy_addr, uint8_t phy_reg);
/* write PHY data to specified register */
void esc_phy_write(uint8_t phy_addr, uint8_t phy_reg, uint16_t phy_data);
/* read data from specified MMD register */
uint32_t esc_mmd_read(uint8_t phy_addr, uint8_t MMD_addr, uint16_t MMD_addr_index);
/* write data to specified MMD register */
void esc_mmd_write(uint8_t phy_addr, uint8_t MMD_addr, uint16_t MMD_addr_index, uint16_t MMD_data);

#endif/* #define ESC_PHY_H */
