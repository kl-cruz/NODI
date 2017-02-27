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

#include "nsd_common.h"
#include "nsd_spim.h"

#if (NSD_SPIM_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NSD_CHIP_HAS_SPIM0) && (NSD_SPIM_USE_SPIM0 == 1) || defined(__DOXYGEN__)
/** @brief SPIM0 driver object.*/
nsd_spim_drv_t NSD_SPIM0;
#endif

#if defined(NSD_CHIP_HAS_SPIM1) && (NSD_SPIM_USE_SPIM1 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM1;
#endif

#if defined(NSD_CHIP_HAS_SPIM2) && (NSD_SPIM_USE_SPIM2 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM2;
#endif

#if defined(NSD_CHIP_HAS_SPIM3) && (NSD_SPIM_USE_SPIM3 == 1) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM3;
#endif

void nsd_spim_irq_routine(void *p_ctx);

void nsd_spim_prepare(void)
{
#if (NSD_SPIM_USE_SPIM0 == 1)
    NSD_SPIM0.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM0.p_spim_reg = NRF_SPIM0;
    NSD_SPIM0.irq = SPIM0_IRQn;
    NSD_SPIM0.irq_priority = NSD_SPIM_SPIM0_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM0, SPIM0_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM1 == 1)
    NSD_SPIM1.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM1.p_spim_reg = NRF_SPIM1;
    NSD_SPIM1.irq = SPIM1_IRQn;
    NSD_SPIM1.irq_priority = NSD_SPIM_SPIM1_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM1, SPIM1_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM2 == 1)
    NSD_SPIM2.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM2.p_spim_reg = NRF_SPIM2;
    NSD_SPIM2.irq = SPIM2_IRQn;
    NSD_SPIM2.irq_priority = NSD_SPIM_SPIM2_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM2, SPIM2_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM1 == 1)
    NSD_SPIM3.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM3.p_spim_reg = NRF_SPIM3;
    NSD_SPIM3.irq = SPIM3_IRQn;
    NSD_SPIM3.irq_priority = NSD_SPIM_SPIM3_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM3, SPIM3_IRQn);
#endif
#endif
}

void nsd_spim_init(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_UNINIT,
                  "Driver already initialized!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    /* Set pins. */
    p_reg->PSEL.SCK  = nsd_gpio_translate_periph(&p_spim_drv->config->sck_pin),
    p_reg->PSEL.MOSI = nsd_gpio_translate_periph(&p_spim_drv->config->mosi_pin),
    p_reg->PSEL.MISO = nsd_gpio_translate_periph(&p_spim_drv->config->miso_pin);

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
    nsd_common_irq_enable(p_spim_drv->irq, p_spim_drv->irq_priority);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_READY;
}

void nsd_spim_deinit(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_READY,
                  "Driver is not initialized!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    /* Disable peripheral irq in NVIC. */
    nsd_common_irq_disable(p_spim_drv->irq);
    /* Disable interrupts. */
    p_reg->INTENCLR = 0xFFFFFFFF;
    /* Disable peripheral. */
    p_reg->ENABLE = SPIM_ENABLE_ENABLE_Disabled;

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_UNINIT;
}

inline void nsd_spim_select(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_READY,
                  "Driver is not initialized!");
    nsd_gpio_clr(p_spim_drv->config->cs_pin.p_port, p_spim_drv->config->cs_pin.pin);
}

inline void nsd_spim_unselect(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(((p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_FINISH) ||
                   (p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_READY)),
                    "Driver is in bad state!");
    nsd_gpio_set(p_spim_drv->config->cs_pin.p_port, p_spim_drv->config->cs_pin.pin);
}

void nsd_spim_exchange(nsd_spim_drv_t *p_spim_drv,
                       uint32_t n_tx,
                       const void *p_txbuf,
                       uint32_t n_rx,
                       void *p_rxbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(n_tx <= 255, "TX length is too long!");
    NSD_DRV_CHECK(n_rx <= 255, "RX length is too long!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n_tx;
    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n_rx;

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nsd_spim_send(nsd_spim_drv_t *p_spim_drv, uint32_t n, const void *p_txbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n;
    p_reg->RXD.MAXCNT = 0;

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nsd_spim_receive(nsd_spim_drv_t *p_spim_drv, uint32_t n, void *p_rxbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL, "Driver pointer is NULL!");

    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;
    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n;
    p_reg->TXD.MAXCNT = 0;

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    p_reg->TASKS_START = 1;
}

void nsd_spim_irq_routine(void *p_ctx)
{
    NSD_DRV_CHECK(p_ctx != NULL, "Context is NULL!");

    nsd_spim_drv_t *p_spim_drv = (nsd_spim_drv_t *)p_ctx;
    NRF_SPIM_Type * p_reg = p_spim_drv->p_spim_reg;

    if (p_reg->EVENTS_END == 1)
    {
        p_reg->EVENTS_END = 0;
        /* Set finish state to indicate operation end. */
        p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_spim_drv->config->end_cb)
        {
            p_spim_drv->config->end_cb(p_spim_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_FINISH)
        {
            p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_READY;
        }
    }
}

#endif
