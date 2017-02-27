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

#ifndef NODI_PWR_CLK_H
#define NODI_PWR_CLK_H

#include "nodi_common.h"

#if (NODI_PWR_CLK_ENABLED == 1) || defined(__DOXYGEN__)

typedef enum {
    NODI_PWR_CLK_HFCLK_STATE_RUNNING_RC,
    NODI_PWR_CLK_HFCLK_STATE_RUNNING_XTAL,
} nodi_pwr_clk_hfclk_state_t;

typedef enum {
    NODI_PWR_CLK_LFCLK_STATE_NOT_RUNNING,
    NODI_PWR_CLK_LFCLK_STATE_RUNNING_RC,
    NODI_PWR_CLK_LFCLK_STATE_RUNNING_XTAL,
    NODI_PWR_CLK_LFCLK_STATE_RUNNING_HFCLK_SYNTH,
    NODI_PWR_CLK_LFCLK_STATE_RUNNING_RC_LOW_POWER,
} nodi_pwr_clk_lfclk_state_t;

typedef enum {
    NODI_PWR_CLK_LFCLK_SRC_RC,
    NODI_PWR_CLK_LFCLK_SRC_XTAL,
    NODI_PWR_CLK_LFCLK_SRC_SYNTH,
    NODI_PWR_CLK_LFCLK_SRC_LOW_SWING_XL1_GND_XL2,
    NODI_PWR_CLK_LFCLK_SRC_FULL_SWING_XL1_NO_XL2,
} nodi_pwr_clk_lfclk_src_t;

typedef struct nodi_pwr_clk_drv nodi_pwr_clk_drv_t;

/**
 * @brief   POWER/CLOCK notification callback type.
 *
 * @param[in] p_uarte_drv      pointer to the nodi_uarte_drv_t object triggering the callback
 */
typedef void (*nodi_pwr_clk_irq_callback_t)(nodi_pwr_clk_drv_t *p_pwr_clk_drv);


struct nodi_pwr_clk_drv {
    volatile nodi_pwr_clk_hfclk_state_t hfclk_state;  ///< High frequency clock current state.
    volatile nodi_pwr_clk_lfclk_state_t lfclk_state;  ///< Low frequency clock driver current state.
    nodi_pwr_clk_irq_callback_t         hfclk_cb;     ///< HFCLK clock event callback.
    nodi_pwr_clk_irq_callback_t         lfclk_cb;     ///< LFCLK clock event callback.
    uint8_t                             irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern nodi_pwr_clk_drv_t NODI_PWR_CLK;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initializes structures of active drivers
 */
void nodi_pwr_clk_prepare();

/**
 * @brief Initializes selected peripheral.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 */
void nodi_pwr_clk_init(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Deinitializes POWER/CLOCK peripherals.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 */
void nodi_pwr_clk_deinit(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Starts HFCLK clock.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 *
 */
void nodi_clk_hfclk_start(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Stops HFCLK clock.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 *
 */
void nodi_clk_hfclk_stop(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Starts LFCLK clock.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 * @param[in] src               Pointer to structure representing POWER/CLOCK driver.
 *
 */
void nodi_clk_lfclk_start(nodi_pwr_clk_drv_t * p_pwr_clk_drv, nodi_pwr_clk_lfclk_src_t src);

/**
 * @brief Stops LFCLK clock.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 *
 */
void nodi_clk_lfclk_stop(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Checks if LFCLK is running.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 *
 * @return true if LFCLK is running.
 */
bool nodi_clk_lfclk_running(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

/**
 * @brief Checks if HFCLK is running.
 *
 * @param[in] p_pwr_clk_drv     Pointer to structure representing POWER/CLOCK driver.
 *
 * @return true if HFCLK is running.
 */
bool nodi_clk_hfclk_running(nodi_pwr_clk_drv_t * p_pwr_clk_drv);

#ifdef NODI_PWR_CLK_DISABLE_IRQ_CONNECT

/**
 * @brief POWER/CLOCK interrupt service routine.
 *
 * @details This interrupt routine should be connect to interrupt system used in specific
 *          environment.To use direct connection between IRQ and this function, undefine
 *          NODI_UARTE_DISABLE_IRQ_CONNECT define.
 *
 * @param[in] p_ctx             Pointer context internally casted to structure representing POWER/CLOCK driver.
 */
void nodi_pwr_clk_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NODI_PWR_CLK_ENABLED */

#endif /* NODI_PWR_CLK_H */
