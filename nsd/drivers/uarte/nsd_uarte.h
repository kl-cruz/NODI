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

#ifndef NSD_UARTE_H
#define NSD_UARTE_H

#include "nsd_device.h"

#if (NSD_UARTE_ENABLED == TRUE) || defined(__DOXYGEN__)

#if !defined(NSD_UARTE_USE_UARTE0) || defined(__DOXYGEN__)
#define NSD_UARTE_USE_UARTE0 FALSE
#endif

#if !defined(NSD_UARTE_USE_UARTE1) || defined(__DOXYGEN__)
#define NSD_UARTE_USE_UARTE1 FALSE
#endif

typedef struct nsd_uarte_drv nsd_uarte_drv_t;


/**
 * @brief   SPI notification callback type.
 *
 * @param[in] p_uarte_drv      pointer to the @p nsd_uarte_drv_t object triggering the
 *                            callback
 */
typedef void (*nsd_uarte_irq_callback_t)(nsd_uarte_drv_t *p_uarte_drv);


typedef struct {
    nsd_uarte_irq_callback_t end_cb; ///< Operation complete callback or @p NULL.
    uint8_t                  rx_pin;
    uint8_t                  tx_pin;
    uint8_t                  rts_pin;
    uint8_t                  cts_pin;
    nrf_uarte_hwfc_t         hwfc;               ///< Flow control configuration.
    nrf_uarte_parity_t       parity;             ///< Parity configuration.
    nrf_uarte_baudrate_t     baudrate;           ///< Baudrate.
} nsd_uarte_config_s;

/**
 * @brief   SPIM Driver state machine possible states.
 */
typedef enum {
    NSD_UARTE_DRV_STATE_UNINIT, ///< Driver is uninitialized.
    NSD_UARTE_DRV_STATE_READY,  ///< Driver is ready to start an operation.
    NSD_UARTE_DRV_STATE_BUSY,   ///< Driver is busy, executing operation.
} nsd_uarte_state_t;

/**
 * @brief   Structure representing a SPIM driver.
 */
struct nsd_uarte_drv {
    const nsd_uarte_config_s   *config;       ///< Current configuration data.
    volatile nsd_uarte_state_t  uarte_state;   ///< SPIM driver current state.
    NRF_UARTE_Type             *p_uarte_reg;          ///< Pointer to the SPIx registers block.
    IRQn_Type                  irq;          ///< SPI/SPIM peripheral instance IRQ number.
    uint8_t                    irq_priority; ///< Interrupt priority.
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (NSD_UARTE_USE_UARTE0 == TRUE) && !defined(__DOXYGEN__)
extern nsd_uarte_drv_t NSD_UARTE0;
#endif

#if (NSD_UARTE_USE_UARTE1 == TRUE) && !defined(__DOXYGEN__)
extern nsd_uarte_drv_t NSD_UARTE1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  /**
   * @brief Initializes structures of active drivers
   */
  void nsd_uarte_prepare();

  /**
   * @brief Initializes selected peripheral.
   *
   * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
   */
  void nsd_uarte_init(nsd_uarte_drv_t *p_uarte_drv);

  /**
   * @brief Sends data using UARTE peripheral.
   *
   * @details When using nRF52 Family remember about 255 bytes hardware limitation!
   *
   * @param[in]  p_uarte_drv      Pointer to structure representing UARTE driver.
   * @param[out] n                Output data length.
   * @param[out] p_txbuf          Output data buffer.
   */
  void nsd_uarte_send_start(nsd_uarte_drv_t *p_uarte_drv, size_t n, const void *p_txbuf);

  /**
   * @brief nsd_uarte_send_stop
   * @param p_uarte_drv
   */
  void nsd_uarte_send_stop(nsd_uarte_drv_t *p_uarte_drv);

  /**
   * @brief nsd_uarte_send_busy_check
   * @param p_uarte_drv
   * @return
   */
  bool nsd_uarte_send_busy_check(nsd_uarte_drv_t *p_uarte_drv);

  /**
   * @brief Receives data using UARTE peripheral.
   *
   * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
   * @param[in] n                 Input data length.
   * @param[in] p_rxbuf           Input data buffer.
   */
  void nsd_uarte_receive_start(nsd_uarte_drv_t *p_uarte_drv, size_t n, void *p_rxbuf);

  /**
   * @brief nsd_uarte_receive_stop
   * @param p_uarte_drv
   */
  void nsd_uarte_receive_stop(nsd_uarte_drv_t *p_uarte_drv);

  /**
   * @brief nsd_uarte_receive_busy_check
   * @param p_uarte_drv
   * @return
   */
  bool nsd_uarte_receive_busy_check(nsd_uarte_drv_t *p_uarte_drv);

  /**
   * @brief Deinitializes UARTE peripheral.
   *
   * @param[in] p_uarte_drv       Pointer to structure representing UARTE driver.
   */
  void nsd_uarte_deinit(nsd_uarte_drv_t *p_uarte_drv);

#ifdef NSD_UARTE_DISABLE_IRQ_CONNECT

  /**
   * @brief UARTE interrupt service routine.
   *
   * @details This interrupt routine should be connect to interrupt system used in specific
   *          environment.To use direct connection between IRQ and this function, undefine
   *          NSD_UARTE_DISABLE_IRQ_CONNECT define.
   *
   * @param[in] p_ctx             Pointer context internally casted to structure representing UARTE driver.
   */
  void nsd_uarte_irq_routine(void *p_ctx);

#endif

#ifdef __cplusplus
}
#endif


#endif /* NSD_UARTE_ENABLED */

#endif /* NSD_UARTE_H */
