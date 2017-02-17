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

#if (NSD_SPIM_ENABLED == TRUE) || defined(__DOXYGEN__)

#if defined(NSD_CHIP_HAS_SPIM0) && (NSD_SPIM_USE_SPIM0 == TRUE) || defined(__DOXYGEN__)
/** @brief SPIM0 driver object.*/
nsd_spim_drv_t NSD_SPIM0;
#endif

#if defined(NSD_CHIP_HAS_SPIM1) && (NSD_SPIM_USE_SPIM1 == TRUE) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM1;
#endif

#if defined(NSD_CHIP_HAS_SPIM2) && (NSD_SPIM_USE_SPIM2 == TRUE) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM2;
#endif

#if defined(NSD_CHIP_HAS_SPIM3) && (NSD_SPIM_USE_SPIM3 == TRUE) || defined(__DOXYGEN__)
/** @brief SPIM1 driver object.*/
nsd_spim_drv_t NSD_SPIM3;
#endif

void nsd_spim_irq_routine(void *p_ctx);

void nsd_spim_prepare(void)
{
#if (NSD_SPIM_USE_SPIM0 == TRUE)
    NSD_SPIM0.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM0.spi = NRF_SPIM0;
    NSD_SPIM0.irq = SPIM0_IRQn;
    NSD_SPIM0.irq_priority = NSD_SPIM_SPIM0_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM0, SPIM0_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM1 == TRUE)
    NSD_SPIM1.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM1.spi = NRF_SPIM1;
    NSD_SPIM1.irq = SPIM1_IRQn;
    NSD_SPIM1.irq_priority = NSD_SPIM_SPIM1_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM1, SPIM1_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM2 == TRUE)
    NSD_SPIM2.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM2.spi = NRF_SPIM2;
    NSD_SPIM2.irq = SPIM2_IRQn;
    NSD_SPIM2.irq_priority = NSD_SPIM_SPIM2_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM2, SPIM2_IRQn);
#endif
#endif

#if (NSD_SPIM_USE_SPIM1 == TRUE)
    NSD_SPIM3.spim_state = NSD_SPIM_DRV_STATE_UNINIT;
    NSD_SPIM3.spi = NRF_SPIM3;
    NSD_SPIM3.irq = SPIM3_IRQn;
    NSD_SPIM3.irq_priority = NSD_SPIM_SPIM3_IRQ_PRIORITY;
#ifndef NSD_SPIM_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_spim_irq_routine, &NSD_SPIM3, SPIM3_IRQn);
#endif
#endif
}

void nsd_spim_init(nsd_spim_drv_t *p_spim_drv)
{
    NRF_SPIM_Type * p_spim = p_spim_drv->spi;

    NSD_DRV_CHECK(p_spim_drv != NULL);
    NSD_DRV_CHECK(p_spim_drv->spim_state != NSD_SPIM_DRV_STATE_UNINIT);

    /* Set peripheral's pins. */
    //TODO Consider to move pin configuration to pinmux manager
    nrf_spim_pins_set(p_spim,
                      p_spim_drv->config->sck_pin,
                      p_spim_drv->config->mosi_pin,
                      p_spim_drv->config->miso_pin);

    nrf_spim_frequency_set(p_spim,
                           p_spim_drv->config->frequency);

    /* Set SPI mode and bitorder*/
    nrf_spim_configure(p_spim,
                       p_spim_drv->config->mode,
                       p_spim_drv->config->bit_order);

    /* Set overrun character. */
    nrf_spim_orc_set(p_spim, p_spim_drv->config->orc);

    /* Set interrupt, because driver is based on interrupts. */
    nrf_spim_event_clear(p_spim_drv->spi, NRF_SPIM_EVENT_END);
    nrf_spim_int_disable(p_spim, 0xFFFFFFFF);
    nrf_spim_int_enable(p_spim, NRF_SPIM_INT_END_MASK);

    /* Enable peripheral */
    nrf_spim_enable(p_spim);

    nsd_common_irq_enable(p_spim_drv->irq, p_spim_drv->irq_priority);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_READY;
}

void nsd_spim_deinit(nsd_spim_drv_t *p_spim_drv)
{
    NRF_SPIM_Type * p_spim = p_spim_drv->spi;
    NSD_DRV_CHECK(p_spim_drv != NULL);
    NSD_DRV_CHECK(p_spim_drv->spim_state != NSD_SPIM_DRV_STATE_UNINIT);

    while (p_spim_drv->spim_state != NSD_SPIM_DRV_STATE_READY);

    nsd_common_irq_disable(p_spim_drv->irq);
    nrf_spim_int_disable(p_spim, 0xFFFFFFFF);
    nrf_spim_disable(p_spim);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_UNINIT;
}

void nsd_spim_select(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL);
    nrf_gpio_pin_clear(p_spim_drv->config->cs_pin);
}

void nsd_spim_unselect(nsd_spim_drv_t *p_spim_drv)
{
    NSD_DRV_CHECK(p_spim_drv != NULL);
    while (p_spim_drv->spim_state == NSD_SPIM_DRV_STATE_BUSY);
    nrf_gpio_pin_set(p_spim_drv->config->cs_pin);
}

void nsd_spim_exchange(nsd_spim_drv_t *p_spim_drv,
                      size_t n_tx,
                      const void *p_txbuf,
                      size_t n_rx,
                      void *p_rxbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL);
    nrf_spim_tx_buffer_set(p_spim_drv->spi, p_txbuf, n_tx);
    nrf_spim_rx_buffer_set(p_spim_drv->spi, p_rxbuf, n_rx);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    nrf_spim_task_trigger(p_spim_drv->spi, NRF_SPIM_TASK_START);
}

void nsd_spim_send(nsd_spim_drv_t *p_spim_drv, size_t n, const void *p_txbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL);
    nrf_spim_tx_buffer_set(p_spim_drv->spi, p_txbuf, n);
    nrf_spim_rx_buffer_set(p_spim_drv->spi, NULL, 0);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    nrf_spim_task_trigger(p_spim_drv->spi, NRF_SPIM_TASK_START);
}

void nsd_spim_receive(nsd_spim_drv_t *p_spim_drv, size_t n, void *p_rxbuf)
{
    NSD_DRV_CHECK(p_spim_drv != NULL);
    nrf_spim_tx_buffer_set(p_spim_drv->spi, NULL, 0);
    nrf_spim_rx_buffer_set(p_spim_drv->spi, p_rxbuf, n);

    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_BUSY;
    nrf_spim_task_trigger(p_spim_drv->spi, NRF_SPIM_TASK_START);
}

void nsd_spim_irq_routine(void *p_ctx)
{
    nsd_spim_drv_t *p_spim_drv = (nsd_spim_drv_t *)p_ctx;
    nrf_spim_event_clear(p_spim_drv->spi, NRF_SPIM_EVENT_END);
    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_ENDTRX;
    if (p_spim_drv->config->end_cb)
    {
        p_spim_drv->config->end_cb(p_spim_drv);
    }
    p_spim_drv->spim_state = NSD_SPIM_DRV_STATE_READY;
}

#endif
