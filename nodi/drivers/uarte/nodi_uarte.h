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

#ifndef NODI_UARTE_H
#define NODI_UARTE_H

#include "nodi_common.h"

#if (NODI_UARTE_ENABLED == 1) || defined(__DOXYGEN__)
#include "nodi_uarte_const.h"

#if !defined(NODI_UARTE_USE_UARTE0) || defined(__DOXYGEN__)
#define NODI_UARTE_USE_UARTE0 0
#endif

#if !defined(NODI_UARTE_USE_UARTE1) || defined(__DOXYGEN__)
#define NODI_UARTE_USE_UARTE1 0
#endif

typedef struct nodi_uarte_drv nodi_uarte_drv_t;

/**
 * @brief   UARTE notification callback type.
 *
 * @param[in] p_uarte_drv      pointer to the nodi_uarte_drv_t object triggering the callback
 */
typedef void (*nodi_uarte_irq_callback_t)(nodi_uarte_drv_t *p_uarte_drv);


typedef struct {
    nodi_uarte_irq_callback_t tx_end_cb; ///< Transmit operation complete callback or NULL.
    nodi_uarte_irq_callback_t rx_end_cb; ///< Receive operation complete callback or NULL.
    nodi_gpio_pin_t           rx_pin;    ///< RX pin config structure
    nodi_gpio_pin_t           tx_pin;    ///< TX pin config structure
    nodi_gpio_pin_t           rts_pin;   ///< RTS pin config structure
    nodi_gpio_pin_t           cts_pin;   ///< CTS pin config structure
    uint32_t                  hwfc;      ///< Flow control configuration.
    uint32_t                  parity;    ///< Parity configuration.
    uint32_t                  baudrate;  ///< Baudrate.
} nodi_uarte_config_s;

/**
 * @brief   UARTE Driver state machine possible states.
 */
typedef enum {
    NODI_UARTE_DRV_STATE_UNINIT, ///< Driver is uninitialized.
    NODI_UARTE_DRV_STATE_READY,  ///< Driver is ready to start an operation.
    NODI_UARTE_DRV_STATE_BUSY,   ///< Driver is busy, executing operation.
    NODI_UARTE_DRV_STATE_FINISH, ///< Driver finished operation.
} nodi_uarte_state_t;

/**
 * @brief   Structure representing a UARTE driver.
 */
struct nodi_uarte_drv {
    const nodi_uarte_config_s  *config;         ///< Current configuration data.
    volatile nodi_uarte_state_t uarte_rx_state; ///< UARTE driver current state.
    volatile nodi_uarte_state_t uarte_tx_state; ///< UARTE driver current state.
    NRF_UARTE_Type             *p_uarte_reg;    ///< Pointer to the UARTE registers block.
    IRQn_Type                   irq;            ///< UARTE peripheral instance IRQ number.
    uint8_t                     irq_priority;   ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NODI_UARTE_USE_UARTE0 == 1) && !defined(__DOXYGEN__)
extern nodi_uarte_drv_t NODI_UARTE0;
#endif

#if (NODI_UARTE_USE_UARTE1 == 1) && !defined(__DOXYGEN__)
extern nodi_uarte_drv_t NODI_UARTE1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initializes structures of active drivers
 */
void nodi_uarte_prepare();

/**
 * @brief Initializes selected peripheral.
 *
 * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
 */
void nodi_uarte_init(nodi_uarte_drv_t *p_uarte_drv);

/**
 * @brief Sends data using UARTE peripheral.
 *
 * @details When using nRF52 Family remember about 255 bytes hardware limitation!
 *
 * @param[in]  p_uarte_drv      Pointer to structure representing UARTE driver.
 * @param[out] n                Output data length.
 * @param[out] p_txbuf          Output data buffer.
 */
void nodi_uarte_send_start(nodi_uarte_drv_t *p_uarte_drv, uint32_t n, const void *p_txbuf);

/**
 * @brief nodi_uarte_send_stop
 * @param p_uarte_drv
 */
void nodi_uarte_send_stop(nodi_uarte_drv_t *p_uarte_drv);

/**
 * @brief nodi_uarte_send_busy_check
 * @param p_uarte_drv
 * @return
 */
uint32_t nodi_uarte_send_busy_check(nodi_uarte_drv_t *p_uarte_drv);

/**
 * @brief Receives data using UARTE peripheral.
 *
 * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
 * @param[in] n                 Input data length.
 * @param[in] p_rxbuf           Input data buffer.
 */
void nodi_uarte_receive_start(nodi_uarte_drv_t *p_uarte_drv, uint32_t n, void *p_rxbuf);

/**
 * @brief nodi_uarte_receive_stop
 * @param p_uarte_drv
 */
void nodi_uarte_receive_stop(nodi_uarte_drv_t *p_uarte_drv);

/**
 * @brief nodi_uarte_receive_busy_check
 * @param p_uarte_drv
 * @return
 */
uint32_t nodi_uarte_receive_busy_check(nodi_uarte_drv_t *p_uarte_drv);

/**
 * @brief Deinitializes UARTE peripheral.
 *
 * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
 */
void nodi_uarte_deinit(nodi_uarte_drv_t *p_uarte_drv);

#ifdef NODI_UARTE_DISABLE_IRQ_CONNECT

/**
 * @brief UARTE interrupt service routine.
 *
 * @details This interrupt routine should be connect to interrupt system used in specific
 *          environment.To use direct connection between IRQ and this function, undefine
 *          NODI_UARTE_DISABLE_IRQ_CONNECT define.
 *
 * @param[in] p_ctx             Pointer context internally casted to structure representing UARTE driver.
 */
void nodi_uarte_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NODI_UARTE_ENABLED */

#endif /* NODI_UARTE_H */
