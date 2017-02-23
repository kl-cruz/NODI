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
#include "nsd_uarte.h"

#if (NSD_UARTE_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NSD_CHIP_HAS_UARTE0) && (NSD_UARTE_USE_UARTE0 == 1) || defined(__DOXYGEN__)
/** @brief UARTE0 driver object.*/
nsd_uarte_drv_t NSD_UARTE0;
#endif

#if defined(NSD_CHIP_HAS_UARTE1) && (NSD_UARTE_USE_UARTE1 == 1) || defined(__DOXYGEN__)
/** @brief UARTE1 driver object.*/
nsd_uarte_drv_t NSD_UARTE1;
#endif

void nsd_uarte_irq_routine(void *p_ctx);

void nsd_uarte_prepare(void)
{
#if (NSD_UARTE_USE_UARTE0 == 1)
    NSD_UARTE0.uarte_rx_state = NSD_UARTE_DRV_STATE_UNINIT;
    NSD_UARTE0.uarte_tx_state = NSD_UARTE_DRV_STATE_UNINIT;
    NSD_UARTE0.p_uarte_reg = NRF_UARTE0;
    NSD_UARTE0.irq = UARTE0_IRQn;
    NSD_UARTE0.irq_priority = NSD_UARTE_UARTE0_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_uarte_irq_routine, &NSD_UARTE0, UARTE0_IRQn);
#endif
#endif

#if (NSD_UARTE_USE_UARTE1 == 1)
    NSD_UARTE1.uarte_rx_state = NSD_UARTE_DRV_STATE_UNINIT;
    NSD_UARTE1.uarte_tx_state = NSD_UARTE_DRV_STATE_UNINIT;
    NSD_UARTE1.p_uarte_reg = NRF_UARTE1;
    NSD_UARTE1.irq = UARTE1_IRQn;
    NSD_UARTE1.irq_priority = NSD_UARTE_UARTE1_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_uarte_irq_routine, &NSD_UARTE1, UARTE1_IRQn);
#endif
#endif
}

void nsd_uarte_init(nsd_uarte_drv_t *p_uarte_drv)
{
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_uarte_drv->uarte_state != NSD_UARTE_DRV_STATE_UNINIT);

    /* Set peripheral's pins. */
    p_reg->PSEL.TXD = nsd_gpio_translate_periph(&p_uarte_drv->config->tx_pin);
    p_reg->PSEL.RXD = nsd_gpio_translate_periph(&p_uarte_drv->config->rx_pin);

    /* Currently driver is not support flow control. Disable pins. */
    p_reg->PSEL.RTS = 0xFFFFFFFF;
    p_reg->PSEL.CTS = 0xFFFFFFFF;

    p_reg->BAUDRATE = p_uarte_drv->config->baudrate;

    /* Set SPI mode and bitorder*/
    p_reg->CONFIG =p_uarte_drv->config->parity | p_uarte_drv->config->hwfc;

    /* Set interrupt, because driver is based on interrupts. */
    p_reg->EVENTS_ENDRX = 0;
    p_reg->EVENTS_ENDTX = 0;

    p_reg->INTENCLR = 0xFFFFFFFF;
    p_reg->INTENSET = UARTE_INTENSET_ENDRX_Msk |
                      UARTE_INTENSET_ENDTX_Msk;

    /* Enable peripheral */
    p_reg->ENABLE = UARTE_ENABLE_ENABLE_Enabled;

    nsd_common_irq_enable(p_uarte_drv->irq, p_uarte_drv->irq_priority);

    p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_READY;
    p_uarte_drv->uarte_rx_state = NSD_UARTE_DRV_STATE_READY;

}

void nsd_uarte_deinit(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_uarte_drv->uarte_state != NSD_SPIM_DRV_STATE_UNINIT);
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    nsd_common_irq_disable(p_uarte_drv->irq);
    p_reg->INTENCLR = 0xFFFFFFFF;
    p_reg->ENABLE = UARTE_ENABLE_ENABLE_Disabled;

    p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_UNINIT;
    p_uarte_drv->uarte_rx_state = NSD_UARTE_DRV_STATE_UNINIT;
}

void nsd_uarte_send_start(nsd_uarte_drv_t *p_uarte_drv, uint32_t n, const void *p_txbuf)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_txbuf != NULL);
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n;

    p_reg->EVENTS_ENDTX = 0;
    p_reg->EVENTS_TXSTOPPED = 0;
    p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_BUSY;
    p_reg->TASKS_STARTTX = 1;
}

void nsd_uarte_send_stop(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->INTENCLR = UARTE_INTENCLR_ENDRX_Msk |
                      UARTE_INTENCLR_ERROR_Msk;
    p_reg->TASKS_STOPTX = 1;
    p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_READY;
}

uint32_t nsd_uarte_send_busy_check(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    return p_uarte_drv->uarte_tx_state == NSD_UARTE_DRV_STATE_BUSY ? 1 : 0;
}

void nsd_uarte_receive_start(nsd_uarte_drv_t *p_uarte_drv, uint32_t n, void *p_rxbuf)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_rxbuf != NULL);
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n;

    p_reg->EVENTS_ENDRX = 0;
    p_uarte_drv->uarte_rx_state = NSD_UARTE_DRV_STATE_BUSY;
    p_reg->TASKS_STARTRX = 1;
}

void nsd_uarte_receive_stop(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->INTENCLR = UARTE_INTENCLR_ENDTX_Msk |
                      UARTE_INTENCLR_ERROR_Msk;

    p_reg->TASKS_STOPRX = 1;
}

uint32_t nsd_uarte_receive_busy_check(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    return p_uarte_drv->uarte_rx_state == NSD_UARTE_DRV_STATE_BUSY ? 1 : 0;
}

void nsd_uarte_irq_routine(void *p_ctx)
{
    NSD_DRV_CHECK(p_ctx != NULL);
    nsd_uarte_drv_t *p_uarte_drv = (nsd_uarte_drv_t *) p_ctx;
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;
    if (p_reg->EVENTS_ENDTX == 1)
    {
        p_reg->EVENTS_ENDTX = 0;
        /* Set finish state to indicate operation end. */
        p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_uarte_drv->config->tx_end_cb)
        {
            p_uarte_drv->config->tx_end_cb(p_uarte_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_uarte_drv->uarte_tx_state == NSD_UARTE_DRV_STATE_FINISH)
        {
            p_uarte_drv->uarte_tx_state = NSD_UARTE_DRV_STATE_READY;
        }
    }

    if (p_reg->EVENTS_ENDRX == 1)
    {
        p_reg->EVENTS_ENDRX = 0;
        /* Set finish state to indicate operation end. */
        p_uarte_drv->uarte_rx_state = NSD_UARTE_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_uarte_drv->config->rx_end_cb)
        {
            p_uarte_drv->config->rx_end_cb(p_uarte_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_uarte_drv->uarte_rx_state == NSD_UARTE_DRV_STATE_FINISH)
        {
            p_uarte_drv->uarte_rx_state = NSD_UARTE_DRV_STATE_READY;
        }
    }
}

#endif
