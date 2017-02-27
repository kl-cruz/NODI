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
#include "nsd_rtc.h"

#if (NSD_RTC_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NSD_CHIP_HAS_RTC0) && (NSD_RTC_USE_RTC0 == 1) || defined(__DOXYGEN__)
/** @brief RTC0 driver object.*/
nsd_rtc_drv_t NSD_RTC0;
#endif

#if defined(NSD_CHIP_HAS_RTC1) && (NSD_RTC_USE_RTC1 == 1) || defined(__DOXYGEN__)
/** @brief RTC1 driver object.*/
nsd_rtc_drv_t NSD_RTC1;
#endif

#if defined(NSD_CHIP_HAS_RTC2) && (NSD_RTC_USE_RTC2 == 1) || defined(__DOXYGEN__)
/** @brief RTC2 driver object.*/
nsd_rtc_drv_t NSD_RTC2;
#endif

void nsd_rtc_irq_routine(void *p_ctx);

void nsd_rtc_prepare(void)
{
#if (NSD_RTC_USE_RTC0 == 1)
    NSD_RTC0.state = NSD_RTC_DRV_STATE_UNINIT;
    NSD_RTC0.p_rtc_reg = NRF_RTC0;
    NSD_RTC0.irq = RTC0_IRQn;
    NSD_RTC0.irq_priority = NSD_RTC_RTC0_IRQ_PRIORITY;
#ifndef NSD_RTC_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_rtc_irq_routine, &NSD_RTC0, RTC0_IRQn);
#endif
#endif

#if (NSD_RTC_USE_RTC1 == 1)
    NSD_RTC1.state = NSD_RTC_DRV_STATE_UNINIT;
    NSD_RTC1.p_rtc_reg = NRF_RTC1;
    NSD_RTC1.irq = RTC1_IRQn;
    NSD_RTC1.irq_priority = NSD_RTC_RTC1_IRQ_PRIORITY;
#ifndef NSD_RTC_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_rtc_irq_routine, &NSD_RTC1, RTC1_IRQn);
#endif
#endif

#if (NSD_RTC_USE_RTC2 == 1)
    NSD_RTC1.state = NSD_RTC_DRV_STATE_UNINIT;
    NSD_RTC1.p_rtc_reg = NRF_RTC2;
    NSD_RTC1.irq = RTC2_IRQn;
    NSD_RTC1.irq_priority = NSD_RTC_RTC1_IRQ_PRIORITY;
#ifndef NSD_RTC_DISABLE_IRQ_CONNECT
    nsd_mnd_register(nsd_rtc_irq_routine, &NSD_RTC2, RTC2_IRQn);
#endif
#endif
}

void nsd_rtc_init(nsd_rtc_drv_t *p_rtc_drv)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_rtc_drv->config != NULL, "Driver's config pointer is NULL!");
    NSD_DRV_CHECK(p_rtc_drv->state == NSD_RTC_DRV_STATE_UNINIT,
                  "Driver already initialized!");
    NSD_DRV_CHECK(p_rtc_drv->config->prescaler & RTC_PRESCALER_PRESCALER_Msk == 0,
                  "RTC prescaler out of band!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;

    /* NRF52840 ERRATA 20 */
    p_reg->TASKS_STOP = 0;
    /* NRF52840 ERRATA 20 */

    /* Set up prescaler. */
    p_reg->PRESCALER = p_rtc_drv->config->prescaler;

    /* Set interrupt, because driver is based on interrupts. */
    p_reg->EVENTS_TICK = 0;
    p_reg->EVENTS_OVRFLW = 0;
    p_reg->EVENTS_COMPARE[0] = 0;
    p_reg->EVENTS_COMPARE[1] = 0;
    p_reg->EVENTS_COMPARE[2] = 0;
    p_reg->EVENTS_COMPARE[3] = 0;

    p_reg->INTENCLR = 0xFFFFFFFF;

    nsd_common_irq_enable(p_rtc_drv->irq, p_rtc_drv->irq_priority);
    p_rtc_drv->state = NSD_RTC_DRV_STATE_STOPPED;

}

void nsd_rtc_evt_enable(nsd_rtc_drv_t *p_rtc_drv, nsd_rtc_cb_evt_t evt)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    uint32_t evts_reg = 0;
    switch (evt)
    {
    case NSD_RTC_DRV_CB_EVT_TICK:
        p_reg->EVENTS_TICK = 0;
        evts_reg = RTC_INTENSET_TICK_Set << RTC_INTENSET_TICK_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_OVERFLOW:
        p_reg->EVENTS_OVRFLW = 0;
        evts_reg = RTC_INTENSET_OVRFLW_Set << RTC_INTENSET_OVRFLW_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP0:
        p_reg->EVENTS_COMPARE[0] = 0;
        evts_reg = RTC_INTENSET_COMPARE0_Set << RTC_INTENSET_COMPARE0_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP1:
        p_reg->EVENTS_COMPARE[1] = 0;
        evts_reg = RTC_INTENSET_COMPARE1_Set << RTC_INTENSET_COMPARE1_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP2:
        p_reg->EVENTS_COMPARE[2] = 0;
        evts_reg = RTC_INTENSET_COMPARE2_Set << RTC_INTENSET_COMPARE2_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP3:
        p_reg->EVENTS_COMPARE[3] = 0;
        evts_reg = RTC_INTENSET_COMPARE3_Set << RTC_INTENSET_COMPARE3_Pos;
        break;
    default:
        NSD_DRV_CHECK(false, "Unhandled event!");
        break;
    }

    p_reg->EVTENSET = evts_reg;
    p_reg->INTENSET = evts_reg;
}

void nsd_rtc_evt_disable(nsd_rtc_drv_t *p_rtc_drv, nsd_rtc_cb_evt_t evt)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    uint32_t evts_reg = 0;
    switch( evt)
    {
    case NSD_RTC_DRV_CB_EVT_TICK:
        evts_reg = RTC_INTENCLR_TICK_Clear << RTC_INTENCLR_TICK_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_OVERFLOW:
        evts_reg = RTC_INTENCLR_OVRFLW_Clear << RTC_INTENCLR_OVRFLW_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP0:
        evts_reg = RTC_INTENCLR_COMPARE0_Clear << RTC_INTENCLR_COMPARE0_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP1:
        evts_reg = RTC_INTENCLR_COMPARE1_Clear << RTC_INTENCLR_COMPARE1_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP2:
        evts_reg = RTC_INTENCLR_COMPARE2_Clear << RTC_INTENCLR_COMPARE2_Pos;
        break;
    case NSD_RTC_DRV_CB_EVT_COMP3:
        evts_reg = RTC_INTENCLR_COMPARE3_Clear << RTC_INTENCLR_COMPARE3_Pos;
        break;
    default:
        NSD_DRV_CHECK(false, "Unhandled event!");
        break;
    }

    p_reg->EVTENCLR = evts_reg;
    p_reg->INTENCLR = evts_reg;
}

void nsd_rtc_start(nsd_rtc_drv_t *p_rtc_drv)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_rtc_drv->state == NSD_RTC_DRV_STATE_STOPPED,
                  "Driver is already started!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_START = 1;
    p_rtc_drv->state = NSD_RTC_DRV_STATE_STARTED;
}

void nsd_rtc_stop(nsd_rtc_drv_t *p_rtc_drv)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NSD_DRV_CHECK(p_rtc_drv->state != NSD_RTC_DRV_STATE_STOPPED,
                  "Driver is already stopped!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_STOP = 1;
    p_rtc_drv->state = NSD_RTC_DRV_STATE_STOPPED;
}

inline void nsd_rtc_clear(nsd_rtc_drv_t *p_rtc_drv)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_CLEAR = 1;
}

inline void nsd_rtc_overflow_trigger(nsd_rtc_drv_t *p_rtc_drv)
{
    NSD_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_TRIGOVRFLW = 1;
}

void nsd_rtc_irq_routine(void *p_ctx)
{
    NSD_DRV_CHECK(p_ctx != NULL, "Context is NULL!");

    nsd_rtc_drv_t *p_rtc_drv = (nsd_rtc_drv_t *) p_ctx;
    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    if (p_reg->EVENTS_TICK == 1)
    {
        p_reg->EVENTS_TICK = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_TICK);
        }
    }

    if (p_reg->EVENTS_OVRFLW == 1)
    {
        p_reg->EVENTS_OVRFLW = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_OVERFLOW);
        }
    }

    if (p_reg->EVENTS_COMPARE[0] == 1)
    {
        p_reg->EVENTS_COMPARE[0] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_COMP0);
        }
    }

    if (p_reg->EVENTS_COMPARE[1] == 1)
    {
        p_reg->EVENTS_COMPARE[1] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_COMP1);
        }
    }

    if (p_reg->EVENTS_COMPARE[2] == 1)
    {
        p_reg->EVENTS_COMPARE[2] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_COMP2);
        }
    }

    if (p_reg->EVENTS_COMPARE[3] == 1)
    {
        p_reg->EVENTS_COMPARE[3] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NSD_RTC_DRV_CB_EVT_COMP3);
        }
    }


}

#endif
