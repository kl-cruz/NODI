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
#include "nodi_uarte.h"

#if (NODI_UARTE_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NODI_CHIP_HAS_UARTE0) && (NODI_UARTE_USE_UARTE0 == 1) || defined(__DOXYGEN__)
/** @brief UARTE0 driver object.*/
nodi_uarte_drv_t NODI_UARTE0;
#endif

#if defined(NODI_CHIP_HAS_UARTE1) && (NODI_UARTE_USE_UARTE1 == 1) || defined(__DOXYGEN__)
/** @brief UARTE1 driver object.*/
nodi_uarte_drv_t NODI_UARTE1;
#endif

void nodi_uarte_irq_routine(void *p_ctx);

void nodi_uarte_prepare(void)
{
#if (NODI_UARTE_USE_UARTE0 == 1)
    NODI_UARTE0.uarte_rx_state = NODI_UARTE_DRV_STATE_UNINIT;
    NODI_UARTE0.uarte_tx_state = NODI_UARTE_DRV_STATE_UNINIT;
    NODI_UARTE0.p_uarte_reg = NRF_UARTE0;
    NODI_UARTE0.irq = UARTE0_IRQn;
    NODI_UARTE0.irq_priority = NODI_UARTE_UARTE0_IRQ_PRIORITY;
#ifndef NODI_UARTE_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_uarte_irq_routine, &NODI_UARTE0, UARTE0_IRQn);
#endif
#endif

#if (NODI_UARTE_USE_UARTE1 == 1)
    NODI_UARTE1.uarte_rx_state = NODI_UARTE_DRV_STATE_UNINIT;
    NODI_UARTE1.uarte_tx_state = NODI_UARTE_DRV_STATE_UNINIT;
    NODI_UARTE1.p_uarte_reg = NRF_UARTE1;
    NODI_UARTE1.irq = UARTE1_IRQn;
    NODI_UARTE1.irq_priority = NODI_UARTE_UARTE1_IRQ_PRIORITY;
#ifndef NODI_UARTE_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_uarte_irq_routine, &NODI_UARTE1, UARTE1_IRQn);
#endif
#endif
}

void nodi_uarte_init(nodi_uarte_drv_t *p_uarte_drv)
{
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_rx_state == NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver already initialized!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_tx_state == NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver already initialized!");

    /* Set peripheral's pins. */
    p_reg->PSEL.TXD = nodi_gpio_translate_periph(&p_uarte_drv->config->tx_pin);
    p_reg->PSEL.RXD = nodi_gpio_translate_periph(&p_uarte_drv->config->rx_pin);

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

    nodi_common_irq_enable(p_uarte_drv->irq, p_uarte_drv->irq_priority);

    p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_READY;
    p_uarte_drv->uarte_rx_state = NODI_UARTE_DRV_STATE_READY;

}

void nodi_uarte_deinit(nodi_uarte_drv_t *p_uarte_drv)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_rx_state != NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver is not initialized!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_tx_state != NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver is not initialized!");

    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    nodi_common_irq_disable(p_uarte_drv->irq);
    p_reg->INTENCLR = 0xFFFFFFFF;
    p_reg->ENABLE = UARTE_ENABLE_ENABLE_Disabled;

    p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_UNINIT;
    p_uarte_drv->uarte_rx_state = NODI_UARTE_DRV_STATE_UNINIT;
}

void nodi_uarte_send_start(nodi_uarte_drv_t *p_uarte_drv, uint32_t n, const void *p_txbuf)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_txbuf != NULL, "Buffer pointer is NULL!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_tx_state != NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver is not initialized!");

    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->TXD.PTR    = (uint32_t)p_txbuf;
    p_reg->TXD.MAXCNT = n;

    p_reg->EVENTS_ENDTX = 0;
    p_reg->EVENTS_TXSTOPPED = 0;
    p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_BUSY;
    p_reg->TASKS_STARTTX = 1;
}

void nodi_uarte_send_stop(nodi_uarte_drv_t *p_uarte_drv)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_uarte_drv->uarte_tx_state != NODI_UARTE_DRV_STATE_UNINIT,
                  "Driver is not initialized!");

    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->INTENCLR = UARTE_INTENCLR_ENDRX_Msk |
                      UARTE_INTENCLR_ERROR_Msk;
    p_reg->TASKS_STOPTX = 1;
    p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_READY;
}

uint32_t nodi_uarte_send_busy_check(nodi_uarte_drv_t *p_uarte_drv)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    return p_uarte_drv->uarte_tx_state == NODI_UARTE_DRV_STATE_BUSY ? 1 : 0;
}

void nodi_uarte_receive_start(nodi_uarte_drv_t *p_uarte_drv, uint32_t n, void *p_rxbuf)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_rxbuf != NULL, "Buffer pointer is NULL!");
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->RXD.PTR    = (uint32_t)p_rxbuf;
    p_reg->RXD.MAXCNT = n;

    p_reg->EVENTS_ENDRX = 0;
    p_uarte_drv->uarte_rx_state = NODI_UARTE_DRV_STATE_BUSY;
    p_reg->TASKS_STARTRX = 1;
}

void nodi_uarte_receive_stop(nodi_uarte_drv_t *p_uarte_drv)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;

    p_reg->INTENCLR = UARTE_INTENCLR_ENDTX_Msk |
                      UARTE_INTENCLR_ERROR_Msk;

    p_reg->TASKS_STOPRX = 1;
}

uint32_t nodi_uarte_receive_busy_check(nodi_uarte_drv_t *p_uarte_drv)
{
    NODI_DRV_CHECK(p_uarte_drv != NULL, "Driver pointer is NULL!");
    return p_uarte_drv->uarte_rx_state == NODI_UARTE_DRV_STATE_BUSY ? 1 : 0;
}

void nodi_uarte_irq_routine(void *p_ctx)
{
    NODI_DRV_CHECK(p_ctx != NULL, "Context is NULL!");
    nodi_uarte_drv_t *p_uarte_drv = (nodi_uarte_drv_t *) p_ctx;
    NRF_UARTE_Type * p_reg = p_uarte_drv->p_uarte_reg;
    if (p_reg->EVENTS_ENDTX == 1)
    {
        p_reg->EVENTS_ENDTX = 0;
        /* Set finish state to indicate operation end. */
        p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_uarte_drv->config->tx_end_cb)
        {
            p_uarte_drv->config->tx_end_cb(p_uarte_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_uarte_drv->uarte_tx_state == NODI_UARTE_DRV_STATE_FINISH)
        {
            p_uarte_drv->uarte_tx_state = NODI_UARTE_DRV_STATE_READY;
        }
    }

    if (p_reg->EVENTS_ENDRX == 1)
    {
        p_reg->EVENTS_ENDRX = 0;
        /* Set finish state to indicate operation end. */
        p_uarte_drv->uarte_rx_state = NODI_UARTE_DRV_STATE_FINISH;

        /* Call callback if not null. */
        if (p_uarte_drv->config->rx_end_cb)
        {
            p_uarte_drv->config->rx_end_cb(p_uarte_drv);
        }

        /* Callback can start next transmission. Checking... */
        if (p_uarte_drv->uarte_rx_state == NODI_UARTE_DRV_STATE_FINISH)
        {
            p_uarte_drv->uarte_rx_state = NODI_UARTE_DRV_STATE_READY;
        }
    }
}

#endif
