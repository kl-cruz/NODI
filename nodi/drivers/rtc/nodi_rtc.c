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
#include "nodi_rtc.h"

#if (NODI_RTC_ENABLED == 1) || defined(__DOXYGEN__)

#if defined(NODI_CHIP_HAS_RTC0) && (NODI_RTC_USE_RTC0 == 1) || defined(__DOXYGEN__)
/** @brief RTC0 driver object.*/
nodi_rtc_drv_t NODI_RTC0;
#endif

#if defined(NODI_CHIP_HAS_RTC1) && (NODI_RTC_USE_RTC1 == 1) || defined(__DOXYGEN__)
/** @brief RTC1 driver object.*/
nodi_rtc_drv_t NODI_RTC1;
#endif

#if defined(NODI_CHIP_HAS_RTC2) && (NODI_RTC_USE_RTC2 == 1) || defined(__DOXYGEN__)
/** @brief RTC2 driver object.*/
nodi_rtc_drv_t NODI_RTC2;
#endif

void nodi_rtc_irq_routine(void *p_ctx);

void nodi_rtc_prepare(void)
{
#if (NODI_RTC_USE_RTC0 == 1)
    NODI_RTC0.state = NODI_RTC_DRV_STATE_UNINIT;
    NODI_RTC0.p_rtc_reg = NRF_RTC0;
    NODI_RTC0.irq = RTC0_IRQn;
    NODI_RTC0.irq_priority = NODI_RTC_RTC0_IRQ_PRIORITY;
#ifndef NODI_RTC_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_rtc_irq_routine, &NODI_RTC0, RTC0_IRQn);
#endif
#endif

#if (NODI_RTC_USE_RTC1 == 1)
    NODI_RTC1.state = NODI_RTC_DRV_STATE_UNINIT;
    NODI_RTC1.p_rtc_reg = NRF_RTC1;
    NODI_RTC1.irq = RTC1_IRQn;
    NODI_RTC1.irq_priority = NODI_RTC_RTC1_IRQ_PRIORITY;
#ifndef NODI_RTC_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_rtc_irq_routine, &NODI_RTC1, RTC1_IRQn);
#endif
#endif

#if (NODI_RTC_USE_RTC2 == 1)
    NODI_RTC1.state = NODI_RTC_DRV_STATE_UNINIT;
    NODI_RTC1.p_rtc_reg = NRF_RTC2;
    NODI_RTC1.irq = RTC2_IRQn;
    NODI_RTC1.irq_priority = NODI_RTC_RTC1_IRQ_PRIORITY;
#ifndef NODI_RTC_DISABLE_IRQ_CONNECT
    nodi_mnd_register(nodi_rtc_irq_routine, &NODI_RTC2, RTC2_IRQn);
#endif
#endif
}

void nodi_rtc_init(nodi_rtc_drv_t *p_rtc_drv)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_rtc_drv->config != NULL, "Driver's config pointer is NULL!");
    NODI_DRV_CHECK(p_rtc_drv->state == NODI_RTC_DRV_STATE_UNINIT,
                  "Driver already initialized!");
    NODI_DRV_CHECK((p_rtc_drv->config->prescaler & !RTC_PRESCALER_PRESCALER_Msk) == 0,
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

    nodi_common_irq_enable(p_rtc_drv->irq, p_rtc_drv->irq_priority);
    p_rtc_drv->state = NODI_RTC_DRV_STATE_STOPPED;

}

void nodi_rtc_evt_enable(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    uint32_t evts_reg = 0;
    switch (evt)
    {
    case NODI_RTC_DRV_CB_EVT_TICK:
        p_reg->EVENTS_TICK = 0;
        evts_reg = RTC_INTENSET_TICK_Set << RTC_INTENSET_TICK_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_OVERFLOW:
        p_reg->EVENTS_OVRFLW = 0;
        evts_reg = RTC_INTENSET_OVRFLW_Set << RTC_INTENSET_OVRFLW_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP0:
        p_reg->EVENTS_COMPARE[0] = 0;
        evts_reg = RTC_INTENSET_COMPARE0_Set << RTC_INTENSET_COMPARE0_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP1:
        p_reg->EVENTS_COMPARE[1] = 0;
        evts_reg = RTC_INTENSET_COMPARE1_Set << RTC_INTENSET_COMPARE1_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP2:
        p_reg->EVENTS_COMPARE[2] = 0;
        evts_reg = RTC_INTENSET_COMPARE2_Set << RTC_INTENSET_COMPARE2_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP3:
        p_reg->EVENTS_COMPARE[3] = 0;
        evts_reg = RTC_INTENSET_COMPARE3_Set << RTC_INTENSET_COMPARE3_Pos;
        break;
    default:
        NODI_DRV_CHECK(false, "Unhandled event!");
        break;
    }

    p_reg->EVTENSET = evts_reg;
    p_reg->INTENSET = evts_reg;
}

void nodi_rtc_evt_disable(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    uint32_t evts_reg = 0;
    switch( evt)
    {
    case NODI_RTC_DRV_CB_EVT_TICK:
        evts_reg = RTC_INTENCLR_TICK_Clear << RTC_INTENCLR_TICK_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_OVERFLOW:
        evts_reg = RTC_INTENCLR_OVRFLW_Clear << RTC_INTENCLR_OVRFLW_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP0:
        evts_reg = RTC_INTENCLR_COMPARE0_Clear << RTC_INTENCLR_COMPARE0_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP1:
        evts_reg = RTC_INTENCLR_COMPARE1_Clear << RTC_INTENCLR_COMPARE1_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP2:
        evts_reg = RTC_INTENCLR_COMPARE2_Clear << RTC_INTENCLR_COMPARE2_Pos;
        break;
    case NODI_RTC_DRV_CB_EVT_COMP3:
        evts_reg = RTC_INTENCLR_COMPARE3_Clear << RTC_INTENCLR_COMPARE3_Pos;
        break;
    default:
        NODI_DRV_CHECK(false, "Unhandled event!");
        break;
    }

    p_reg->EVTENCLR = evts_reg;
    p_reg->INTENCLR = evts_reg;
}

void nodi_rtc_start(nodi_rtc_drv_t *p_rtc_drv)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_rtc_drv->state == NODI_RTC_DRV_STATE_STOPPED,
                  "Driver is already started!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_START = 1;
    p_rtc_drv->state = NODI_RTC_DRV_STATE_STARTED;
}

void nodi_rtc_stop(nodi_rtc_drv_t *p_rtc_drv)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");
    NODI_DRV_CHECK(p_rtc_drv->state != NODI_RTC_DRV_STATE_STOPPED,
                  "Driver is already stopped!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_STOP = 1;
    p_rtc_drv->state = NODI_RTC_DRV_STATE_STOPPED;
}

inline void nodi_rtc_clear(nodi_rtc_drv_t *p_rtc_drv)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_CLEAR = 1;
}

inline void nodi_rtc_overflow_trigger(nodi_rtc_drv_t *p_rtc_drv)
{
    NODI_DRV_CHECK(p_rtc_drv != NULL, "Driver pointer is NULL!");

    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    p_reg->TASKS_TRIGOVRFLW = 1;
}

void nodi_rtc_irq_routine(void *p_ctx)
{
    NODI_DRV_CHECK(p_ctx != NULL, "Context is NULL!");

    nodi_rtc_drv_t *p_rtc_drv = (nodi_rtc_drv_t *) p_ctx;
    NRF_RTC_Type * p_reg = p_rtc_drv->p_rtc_reg;
    if (p_reg->EVENTS_TICK == 1)
    {
        p_reg->EVENTS_TICK = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_TICK);
        }
    }

    if (p_reg->EVENTS_OVRFLW == 1)
    {
        p_reg->EVENTS_OVRFLW = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_OVERFLOW);
        }
    }

    if (p_reg->EVENTS_COMPARE[0] == 1)
    {
        p_reg->EVENTS_COMPARE[0] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_COMP0);
        }
    }

    if (p_reg->EVENTS_COMPARE[1] == 1)
    {
        p_reg->EVENTS_COMPARE[1] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_COMP1);
        }
    }

    if (p_reg->EVENTS_COMPARE[2] == 1)
    {
        p_reg->EVENTS_COMPARE[2] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_COMP2);
        }
    }

    if (p_reg->EVENTS_COMPARE[3] == 1)
    {
        p_reg->EVENTS_COMPARE[3] = 0;

        /* Call callback if not null. */
        if (p_rtc_drv->config->evt_cb)
        {
            p_rtc_drv->config->evt_cb(p_rtc_drv, NODI_RTC_DRV_CB_EVT_COMP3);
        }
    }


}

#endif
