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

#ifndef NSD_NRF52840_H
#define NSD_NRF52840_H

/* Chip description */

#include "nrf52840.h"

/* SPIM subsystem */
#include "nrf_spim.h"
#define NSD_CHIP_HAS_SPIM0
#define SPIM0_IRQn          SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn
#define SPIM0_IRQHandler    SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
#define NSD_CHIP_HAS_SPIM1
#define SPIM1_IRQn          SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn
#define SPIM1_IRQHandler    SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
#define NSD_CHIP_HAS_SPIM2
#define SPIM2_IRQn          SPIM2_SPIS2_TWIM2_TWIS2_SPI2_TWI2_IRQn
#define SPIM2_IRQHandler    SPIM2_SPIS2_TWIM2_TWIS2_SPI2_TWI2_IRQHandler
#define NSD_CHIP_HAS_SPIM3
#define SPIM3_IRQn          SPIM3_SPIS3_TWIM3_TWIS3_SPI3_TWI3_IRQn
#define SPIM3_IRQHandler    SPIM3_SPIS3_TWIM3_TWIS3_SPI3_TWI3_IRQHandler


/* GPIO subsystem */
#include "nrf_gpio.h"
#define NSD_GPIO_PORT0      NRF_P0
#define NSD_GPIO_PORT1      NRF_P1


#endif // NSD_NRF52840_H
