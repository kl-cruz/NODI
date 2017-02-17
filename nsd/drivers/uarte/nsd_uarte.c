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

#include "nsd.h"

#if (NSD_UARTE_ENABLED == TRUE) || defined(__DOXYGEN__)

#if defined(NSD_CHIP_HAS_UARTE0) && (NSD_UARTE_USE_UARTE0 == TRUE) || defined(__DOXYGEN__)
/** @brief UARTE0 driver object.*/
nsd_uarte_drv_t NSD_UARTE0;
#endif

#if defined(NSD_CHIP_HAS_UARTE1) && (NSD_UARTE_USE_UARTE1 == TRUE) || defined(__DOXYGEN__)
/** @brief UARTE1 driver object.*/
nsd_uarte_drv_t NSD_UARTE1;
#endif

void nsd_uarte_irq_routine(void *p_ctx);

void nsd_uarte_prepare(void)
{
#if (NSD_UARTE_USE_UARTE0 == TRUE)
    NSD_UARTE0.uarte_state = NSD_UARTE_DRV_STATE_UNINIT;
    NSD_UARTE0.p_uarte_reg = NRF_UARTE0;
    NSD_UARTE0.irq = UARTE0_IRQn;
    NSD_UARTE0.irq_priority = NSD_UARTE_UARTE0_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_uarte_irq_routine, &NSD_UARTE0, UARTE0_IRQn);
#endif
#endif

#if (NSD_UARTE_USE_UARTE1 == TRUE)
    NSD_UARTE1.uarte_state = NSD_UARTE_DRV_STATE_UNINIT;
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
    NRF_UARTE_Type * p_uarte = p_uarte_drv->p_uarte_reg;

    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_uarte_drv->uarte_state != NSD_UARTE_DRV_STATE_UNINIT);

    /* Set peripheral's pins. */
    //TODO Consider to move pin configuration to pinmux manager
    nrf_uarte_txrx_pins_set(p_uarte, p_uarte_drv->config->tx_pin, p_uarte_drv->config->rx_pin);
    nrf_uarte_hwfc_pins_disconnect(p_uarte);

    nrf_uarte_baudrate_set(p_uarte, p_uarte_drv->config->baudrate);

    /* Set SPI mode and bitorder*/
    nrf_uarte_configure(p_uarte, p_uarte_drv->config->parity, p_uarte_drv->config->hwfc);

    /* Set interrupt, because driver is based on interrupts. */
    nrf_uarte_event_clear(p_uarte, NRF_UARTE_EVENT_ENDTX);
    nrf_uarte_event_clear(p_uarte, NRF_UARTE_EVENT_ENDRX);
    nrf_uarte_int_disable(p_uarte, 0xFFFFFFFF);
    nrf_uarte_int_enable(p_uarte, NRF_UARTE_INT_ENDRX_MASK);
    nrf_uarte_int_enable(p_uarte, NRF_UARTE_INT_ENDTX_MASK);

    /* Enable peripheral */
    nrf_uarte_enable(p_uarte);

    nsd_common_irq_enable(p_uarte_drv->irq, p_uarte_drv->irq_priority);

    p_uarte_drv->uarte_state = NSD_UARTE_DRV_STATE_READY;
}

void nsd_uarte_deinit(nsd_uarte_drv_t *p_uarte_drv)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_uarte_drv->uarte_state != NSD_SPIM_DRV_STATE_UNINIT);
    NRF_UARTE_Type * p_uarte_reg = p_uarte_drv->p_uarte_reg;

    nsd_common_irq_disable(p_uarte_drv->irq);
    nrf_uarte_int_disable(p_uarte_reg, 0xFFFFFFFF);
    nrf_uarte_disable(p_uarte_reg);

    p_uarte_drv->uarte_state = NSD_UARTE_DRV_STATE_UNINIT;
}

void nsd_uarte_send_start(nsd_uarte_drv_t *p_uarte_drv, size_t n, const void *p_txbuf)
{
    NSD_DRV_CHECK(p_uarte_drv != NULL);
    NSD_DRV_CHECK(p_txbuf != NULL);
    nrf_uarte_tx_buffer_set(p_uarte_drv->p_uarte_reg, p_txbuf, n);

    p_uarte_drv->uarte_state = NSD_UARTE_DRV_STATE_BUSY;
    nrf_uarte_event_clear(p_uarte_drv->p_uarte_reg, NRF_UARTE_EVENT_ENDTX);
    nrf_uarte_event_clear(p_uarte_drv->p_uarte_reg, NRF_UARTE_EVENT_TXSTOPPED);
    nrf_uarte_task_trigger(p_uarte_drv->p_uarte_reg, NRF_UARTE_TASK_STARTTX);
}

void nsd_uarte_send_stop(nsd_uarte_drv_t *p_uarte_drv)
{
    nrf_uarte_task_trigger(p_uarte_drv->p_uarte_reg, NRF_UARTE_TASK_STOPTX);
}

bool nsd_uarte_send_busy_check(nsd_uarte_drv_t *p_uarte_drv)
{
    return false;
}

void nsd_uarte_receive_start(nsd_uarte_drv_t *p_uarte_drv, size_t n, void *p_rxbuf)
{

}

void nsd_uarte_receive_stop(nsd_uarte_drv_t *p_uarte_drv)
{
    nrf_uarte_task_trigger(p_uarte_drv->p_uarte_reg, NRF_UARTE_TASK_STOPRX);
}

bool nsd_uarte_receive_busy_check(nsd_uarte_drv_t *p_uarte_drv)
{
    return false;
}

void nsd_uarte_irq_routine(void *p_ctx)
{
    nsd_uarte_drv_t *p_uarte_drv = (nsd_uarte_drv_t *) p_ctx;
    nrf_uarte_event_clear(p_uarte_drv->p_uarte_reg, NRF_UARTE_EVENT_ENDTX);
    p_uarte_drv->uarte_state = NSD_UARTE_DRV_STATE_BUSY;
    if (p_uarte_drv->config->end_cb)
    {
        p_uarte_drv->config->end_cb(p_uarte_drv);
    }
    p_uarte_drv->uarte_state = NSD_UARTE_DRV_STATE_READY;
}

#endif
