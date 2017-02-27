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

#ifndef NODI_RTC_H
#define NODI_RTC_H

#include "nodi_common.h"

#if (NODI_RTC_ENABLED == 1) || defined(__DOXYGEN__)

#if !defined(NODI_RTC_USE_RTC0) || defined(__DOXYGEN__)
#define NODI_RTC_USE_RTC0 0
#endif

#if !defined(NODI_RTC_USE_RTC1) || defined(__DOXYGEN__)
#define NODI_RTC_USE_RTC1 0
#endif

#if !defined(NODI_RTC_USE_RTC2) || defined(__DOXYGEN__)
#define NODI_RTC_USE_RTC2 0
#endif

/**
 * @brief   RTC callback events.
 */
typedef enum {
    NODI_RTC_DRV_CB_EVT_TICK,     ///< Event generated after TICK event.
    NODI_RTC_DRV_CB_EVT_OVERFLOW, ///< Event generated after OVRFLW event.
    NODI_RTC_DRV_CB_EVT_COMP0,    ///< Event generated after COMPARE0 event.
    NODI_RTC_DRV_CB_EVT_COMP1,    ///< Event generated after COMPARE1 event.
    NODI_RTC_DRV_CB_EVT_COMP2,    ///< Event generated after COMPARE2 event.
    NODI_RTC_DRV_CB_EVT_COMP3,    ///< Event generated after COMPARE3 event.
} nodi_rtc_cb_evt_t;

typedef struct nodi_rtc_drv nodi_rtc_drv_t;

/**
 * @brief   RTC notification callback type.
 *
 * @param[in] p_rtc_drv         Pointer to the nodi_rtc_drv_t object triggering the callback.
 */
typedef void (*nodi_rtc_irq_callback_t)(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt);


typedef struct {
    nodi_rtc_irq_callback_t evt_cb;   ///< Event callback or NULL.
    uint32_t               prescaler; ///< RTC prescaler to gain period 32768/(PRESCALER+1).
} nodi_rtc_config_s;

/**
 * @brief   RTC Driver state machine possible states.
 */
typedef enum {
    NODI_RTC_DRV_STATE_UNINIT,  ///< Driver is uninitialized.
    NODI_RTC_DRV_STATE_STOPPED, ///< Driver is stopped and configured.
    NODI_RTC_DRV_STATE_STARTED, ///< Driver is working. Cannot configure PRESCALER.
} nodi_rtc_state_t;

/**
 * @brief   Structure representing a RTC driver.
 */
struct nodi_rtc_drv {
    const nodi_rtc_config_s  *config;       ///< Current configuration data.
    volatile nodi_rtc_state_t state;        ///< RTC driver current state.
    NRF_RTC_Type             *p_rtc_reg;    ///< Pointer to the RTC registers block.
    IRQn_Type                 irq;          ///< RTC peripheral instance IRQ number.
    uint8_t                   irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NODI_RTC_USE_RTC0 == 1) && !defined(__DOXYGEN__)
extern nodi_rtc_drv_t NODI_RTC0;
#endif

#if (NODI_RTC_USE_RTC1 == 1) && !defined(__DOXYGEN__)
extern nodi_rtc_drv_t NODI_RTC1;
#endif

#if (NODI_RTC_USE_RTC2 == 1) && !defined(__DOXYGEN__)
extern nodi_rtc_drv_t NODI_RTC2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initializes structures of active drivers.
 */
void nodi_rtc_prepare();

/**
 * @brief Initializes selected peripheral.
 *
 * Remember to start LFCLK before using RTC peripheral.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 */
void nodi_rtc_init(nodi_rtc_drv_t *p_rtc_drv);

/**
 * @brief Enables choosen event to generate interrupt.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 * @param[in] p_evt             Event to enable in interrupt and event systems in RTC peripheral.
 *
 */
void nodi_rtc_evt_enable(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt);

/**
 * @brief Disables choosen event.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 * @param[in] p_evt             Event to disable in interrupt and event systems in RTC peripheral.
 *
 */
void nodi_rtc_evt_disable(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt);

/**
 * @brief Starts RTC peripheral.
 *
 * @param[in]  p_rtc_drv        Pointer to structure representing RTC driver.
 *
 */
void nodi_rtc_start(nodi_rtc_drv_t *p_rtc_drv);

/**
 * @brief Stops RTC peripheral.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 *
 */
void nodi_rtc_stop(nodi_rtc_drv_t *p_rtc_drv);

/**
 * @brief Clears RTC counter.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 *
 */
void nodi_rtc_clear(nodi_rtc_drv_t *p_rtc_drv);

/**
 * @brief Triggers overflow task to generate overflow event by the software.
 *
 * @param[in] p_rtc_drv         Pointer to structure representing RTC driver.
 *
 */
void nodi_rtc_overflow_trigger(nodi_rtc_drv_t *p_rtc_drv);


#ifdef NODI_RTC_DISABLE_IRQ_CONNECT

/**
 * @brief RTC interrupt service routine.
 *
 * @details This interrupt routine should be connect to interrupt system used in specific
 *          environment.To use direct connection between IRQ and this function, undefine
 *          NODI_RTC_DISABLE_IRQ_CONNECT define.
 *
 * @param[in] p_ctx             Pointer context internally casted to structure representing RTC driver.
 */
void nodi_rtc_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NODI_RTC_ENABLED */

#endif /* NODI_RTC_H */
