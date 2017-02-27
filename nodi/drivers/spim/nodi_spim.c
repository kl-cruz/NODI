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

#include "nodi_common.h"
#include "nodi_spim.h"

#if (NODI_SPIM_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NODI_CHIP_HAS_SPIM0) && (NODI_SPIM_USE_SPIM0 == 1) || defined(__DOXYGEN__)
/** @brief SPIM0 driver object.*/
nodi_spim_drv_t NODI_SPIM0;
#endif

#if defined(NODI_CHIP_HAS_SPIM1) && (NODI_SPIM_USE_SPIM1 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nodi_spim_drv_t NODI_SPIM1;
#endif

#if defined(NODI_CHIP_HAS_SPIM2) && (NODI_SPIM_USE_SPIM2 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nodi_spim_drv_t NODI_SPIM2;
#endif

#if defined(NODI_CHIP_HAS_SPIM3) && (NODI_SPIM_USE_SPIM3 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nodi_spim_drv_t NODI_SPIM3;
#endif

void nodi_spim_irq_routine(void *p_ctx);

void nodi_spim_prepare(void)
{
#if (NODI_SPIM_USE_SPIM0 == 1)
    NODI_SPIM0.spim_state = NODI_SPIM_DRV_STATE_UNINIT;
    NODI_SPIM0.p_spim_reg = NRF_SPIM0;
    NODI_SPIM0.irq = SPIM0_IRQn;
    NODI_SPIM0.irq_priority = NODI_SPIM_SPIM0_IRQ_PRIORITY;
#ifndef NODI_SPIM_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_spim_irq_routine, &NODI_SPIM0, SPIM0_IRQn);
#endif
#endif

#if (NODI_SPIM_USE_SPIM1 == 1)
    NODI_SPIM1.spim_state = NODI_SPIM_DRV_STATE_UNINIT;
    NODI_SPIM1.p_spim_reg = NRF_SPIM1;
    NODI_SPIM1.irq = SPIM1_IRQn;
    NODI_SPIM1.irq_priority = NODI_SPIM_SPIM1_IRQ_PRIORITY;
#ifndef NODI_SPIM_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_spim_irq_routine, &NODI_SPIM1, SPIM1_IRQn);
#endif
#endif

#if (NODI_SPIM_USE_SPIM2 == 1)
    NODI_SPIM2.spim_state = NODI_SPIM_DRV_STATE_UNINIT;
    NODI_SPIM2.p_spim_reg = NRF_SPIM2;
    NODI_SPIM2.irq = SPIM2_IRQn;
    NODI_SPIM2.irq_priority = NODI_SPIM_SPIM2_IRQ_PRIORITY;
#ifndef NODI_SPIM_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_spim_irq_routine, &NODI_SPIM2, SPIM2_IRQn);
#endif
#endif

#if (NODI_SPIM_USE_SPIM1 == 1)
    NODI_SPIM3.spim_state = NODI_SPIM_DRV_STATE_UNINIT;
    NODI_SPIM3.p_spim_reg = NRF_SPIM3;
    NODI_SPIM3.irq = SPIM3_IRQn;
    NODI_SPIM3.irq_priority = NODI_SPIM_SPIM3_IRQ_PRIORITY;
#ifndef NODI_SPIM_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_spim_irq_routine, &NODI_SPIM3, SPIM3_IRQn);
#endif
#endif
}

void nodi_spim_init(nodi_spim_drv_t *p_spim_drv)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_UNINIT,
                  "Driver already initialized!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    /* Set pins. */
    p_reg->PSEL.SCK  = nodi_gpio_translate_periph(&p_spim_drv->config->sck_pin),
    p_reg->PSEL.MOSI = nodi_gpio_translate_periph(&p_spim_drv->config->mosi_pin),
    p_reg->PSEL.MISO = nodi_gpio_translate_periph(&p_spim_drv->config->miso_pin);

    /* Set SPIM frequency. */
    p_reg->FREQUENCY = p_spim_drv->config->frequency;

    /* Set SPI mode and bitorder. */
    p_reg->CONFIG = p_spim_drv->config->bit_order | p_spim_drv->config->mode;

    /* Set overrun character. */
    p_reg->ORC = p_spim_drv->config->orc;

    /* Set interrupt, because driver is based on interrupts. */
    p_reg->INTENCLR = 0xFFFFFFFF;
    p_reg->EVENTS_END = 0;
    p_reg->INTENSET = SPIM_INTENSET_END_Msk;

    /* Enable peripheral. */
    p_reg->ENABLE = SPIM_ENABLE_ENABLE_Enabled;

    /* Enable peripheral irq in NVIC. */
    nodi_common_irq_enable(p_spim_drv->irq, p_spim_drv->irq_priority);

    p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_READY;
}

void nodi_spim_deinit(nodi_spim_drv_t *p_spim_drv)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_READY,
                  "Driver is not initialized!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    /* Disable peripheral irq in NVIC. */
    nodi_common_irq_disable(p_spim_drv->irq);
    /* Disable interrupts. */
    p_reg->INTENCLR = 0xFFFFFFFF;
    /* Disable peripheral. */
    p_reg->ENABLE = SPIM_ENABLE_ENABLE_Disabled;

    p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_UNINIT;
}

inline void nodi_spim_select(nodi_spim_drv_t *p_spim_drv)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_READY,
                  "Driver is not initialized!");
    nodi_gpio_clr(p_spim_drv->config->cs_pin.p_port, p_spim_drv->config->cs_pin.pin);
}

inline void nodi_spim_unselect(nodi_spim_drv_t *p_spim_drv)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(((p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_FINISH) ||
                   (p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_READY)),
                    "Driver is in bad state!");
    nodi_gpio_set(p_spim_drv->config->cs_pin.p_port, p_spim_drv->config->cs_pin.pin);
}

void nodi_spim_exchange(nodi_spim_drv_t *p_spim_drv,
                       uint32_t n_tx,
                       const void *p_txbuf,
                       uint32_t n_rx,
                       void *p_rxbuf)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(n_tx <= 255, "TX length is too long!");
    NODI_DRV_CHECK(n_rx <= 255, "RX length is too long!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n_tx;
    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n_rx;

    p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nodi_spim_send(nodi_spim_drv_t *p_spim_drv, uint32_t n, const void *p_txbuf)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n;
    p_reg->RXD.MAXCNT = 0;

    p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nodi_spim_receive(nodi_spim_drv_t *p_spim_drv, uint32_t n, void *p_rxbuf)
{
    NODI_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n;
    p_reg->TXD.MAXCNT = 0;

    p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nodi_spim_irq_routine(void *p_ctx)
{
    NODI_DRV_CHECK(p_ctx != NULL, "Context is NULL!");

    nodi_spim_drv_t *p_spim_drv = (nodi_spim_drv_t *)p_ctx;
    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    if (p_reg->EVENTS_END == 1)
    {
        p_reg->EVENTS_END = 0;
        /* Set finish state to indicate operation end. */
        p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_spim_drv->config->end_cb)
        {
            p_spim_drv->config->end_cb(p_spim_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_spim_drv->spim_state == NODI_SPIM_DRV_STATE_FINISH)
        {
            p_spim_drv->spim_state = NODI_SPIM_DRV_STATE_READY;
        }
    }
}

#endif
