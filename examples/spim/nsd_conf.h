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

#ifndef NSD_CONF_H
#define NSD_CONF_H

/* Enable/Disable MCU peripherals */
#define NSD_SPIM_ENABLED                       TRUE

/* SPIM driver configuration */
#define NSD_SPIM_USE_SPIM0                     TRUE
#define NSD_SPIM_SPIM0_IRQ_PRIORITY            7

#define NSD_SPIM_USE_SPIM1                     FALSE
#define NSD_SPIM_SPIM1_IRQ_PRIORITY            7

#define NSD_SPIM_USE_SPIM2                     FALSE
#define NSD_SPIM_SPIM2_IRQ_PRIORITY            7

#define NSD_SPIM_USE_SPIM3                     FALSE
#define NSD_SPIM_SPIM3_IRQ_PRIORITY            7

//#define NSD_CHIP_NRF52840


#endif // NSD_CONF_H
