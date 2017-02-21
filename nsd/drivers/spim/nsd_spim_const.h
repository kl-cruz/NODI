/*
 * MIT License
 *
 * Copyright (c) 2017 Karol Lasonczyk (kl-cruz - https://github.com/kl-cruz)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef NSD_SPIM_CONST_H
#define NSD_SPIM_CONST_H

#include "nsd_device.h"

/**
 * @brief SPI master data rates.
 */
#define NSD_SPIM_FREQ_125K    SPIM_FREQUENCY_FREQUENCY_K125
#define NSD_SPIM_FREQ_250K    SPIM_FREQUENCY_FREQUENCY_K250
#define NSD_SPIM_FREQ_500K    SPIM_FREQUENCY_FREQUENCY_K500
#define NSD_SPIM_FREQ_1M      SPIM_FREQUENCY_FREQUENCY_M1
#define NSD_SPIM_FREQ_2M      SPIM_FREQUENCY_FREQUENCY_M2
#define NSD_SPIM_FREQ_4M      SPIM_FREQUENCY_FREQUENCY_M4
#define NRF_SPIM_FREQ_8M      SPIM_FREQUENCY_FREQUENCY_M8

#define NSD_SPIM_BIT_ORDER_MSB_FIRST SPIM_CONFIG_ORDER_MsbFirst
#define NSD_SPIM_BIT_ORDER_LSB_FIRST SPIM_CONFIG_ORDER_LsbFirst

#define NSD_SPIM_MODE_0 (\
    (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) | \
    (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos))

#define NSD_SPIM_MODE_1 (\
    (SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos) | \
    (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos))

#define NSD_SPIM_MODE_2 (\
    (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) | \
    (SPIM_CONFIG_CPHA_Leading    << SPIM_CONFIG_CPHA_Pos))

#define NSD_SPIM_MODE_3 (\
    (SPIM_CONFIG_CPOL_ActiveLow  << SPIM_CONFIG_CPOL_Pos) | \
    (SPIM_CONFIG_CPHA_Trailing   << SPIM_CONFIG_CPHA_Pos))


#endif /* NSD_SPIM_CONST_H */
