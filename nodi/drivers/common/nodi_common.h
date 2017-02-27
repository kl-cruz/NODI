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

#ifndef NODI_COMMON_H
#define NODI_COMMON_H

#include <stdbool.h>
#include <stdlib.h>
#include "nodi_conf.h"
#include "nodi_device.h"
#include "nodi_gpio.h"

#ifdef NODI_DEBUG
#define NODI_DRV_CHECK(statement, fail_text)   nodi_common_assert(statement)

__attribute__ ((used)) static void nodi_common_assert(bool statement)
{
    if (!statement)
    {
        __BKPT();
        while(1);
    }
}
#else
#define NODI_DRV_CHECK(statement, text)
#endif

static inline void nodi_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{
    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}

static inline void nodi_common_irq_disable(IRQn_Type IRQn)
{
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_DisableIRQ(IRQn);
}

#endif
