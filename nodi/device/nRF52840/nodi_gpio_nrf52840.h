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

#ifndef NODI_GPIO_NRF52840_H
#define NODI_GPIO_NRF52840_H

#include "nrf52840_bitfields.h"

/* Bit 0 */
#define NODI_GPIO_DIR_INPUT             GPIO_PIN_CNF_DIR_Input
#define NODI_GPIO_DIR_OUTPUT            GPIO_PIN_CNF_DIR_Output

/* Bit 1 */
#define NODI_GPIO_INPUT_CONNECT         GPIO_PIN_CNF_INPUT_Connect
#define NODI_GPIO_INPUT_DISCONNECT      GPIO_PIN_CNF_INPUT_Disconnect

/* Bits 2-3 */
#define NODI_GPIO_PULL_DISABLED         GPIO_PIN_CNF_PULL_Disabled
#define NODI_GPIO_PULL_PULLDOWN         GPIO_PIN_CNF_PULL_Pulldown
#define NODI_GPIO_PULL_PULLUP           GPIO_PIN_CNF_PULL_Pullup

/* Bits 8-10 */
#define NODI_GPIO_DRIVE_S0S1            GPIO_PIN_CNF_DRIVE_S0S1
#define NODI_GPIO_DRIVE_H0S1            GPIO_PIN_CNF_DRIVE_H0S1
#define NODI_GPIO_DRIVE_S0H1            GPIO_PIN_CNF_DRIVE_S0H1
#define NODI_GPIO_DRIVE_H0H1            GPIO_PIN_CNF_DRIVE_H0H1
#define NODI_GPIO_DRIVE_D0S1            GPIO_PIN_CNF_DRIVE_D0S1
#define NODI_GPIO_DRIVE_D0H1            GPIO_PIN_CNF_DRIVE_D0H1
#define NODI_GPIO_DRIVE_S0D1            GPIO_PIN_CNF_DRIVE_S0D1
#define NODI_GPIO_DRIVE_H0D1            GPIO_PIN_CNF_DRIVE_H0D1

/* Bits 16-17 */
#define NODI_GPIO_SENSE_DISABLED        GPIO_PIN_CNF_SENSE_Disabled
#define NODI_GPIO_SENSE_HIGH            GPIO_PIN_CNF_SENSE_High
#define NODI_GPIO_SENSE_LOW             GPIO_PIN_CNF_SENSE_Low

#define NODI_GPIO_CONFIG(dir, input, pull, drive, sense) (             \
    (((dir)   & GPIO_PIN_CNF_DIR_Msk)   << GPIO_PIN_CNF_DIR_Pos)   || \
    (((input) & GPIO_PIN_CNF_INPUT_Msk) << GPIO_PIN_CNF_INPUT_Pos) || \
    (((pull)  & GPIO_PIN_CNF_PULL_Msk)  << GPIO_PIN_CNF_PULL_Pos)  || \
    (((drive) & GPIO_PIN_CNF_DRIVE_Msk) << GPIO_PIN_CNF_DRIVE_Pos) || \
    (((sense) & GPIO_PIN_CNF_SENSE_Msk) << GPIO_PIN_CNF_SENSE_Pos))

/* Predefined configurations */

#define NODI_GPIO_CFG_STD_OUTPUT        NODI_GPIO_CONFIG(NODI_GPIO_DIR_OUTPUT,       \
                                                       NODI_GPIO_INPUT_DISCONNECT, \
                                                       NODI_GPIO_PULL_DISABLED,    \
                                                       NODI_GPIO_DRIVE_S0S1,       \
                                                       NODI_GPIO_SENSE_DISABLED)

#define NODI_GPIO_CFG_STD_INPUT         NODI_GPIO_CONFIG(NODI_GPIO_DIR_INPUT,        \
                                                       NODI_GPIO_INPUT_CONNECT,    \
                                                       NODI_GPIO_PULL_DISABLED,    \
                                                       NODI_GPIO_DRIVE_S0S1,       \
                                                       NODI_GPIO_SENSE_DISABLED)

/* SPI PINS */
#define NODI_GPIO_CFG_SPI_CS            NODI_GPIO_CFG_STD_OUTPUT
#define NODI_GPIO_CFG_SPI_SCK           NODI_GPIO_CFG_STD_OUTPUT
#define NODI_GPIO_CFG_SPI_MISO          NODI_GPIO_CFG_STD_INPUT
#define NODI_GPIO_CFG_SPI_MOSI          NODI_GPIO_CFG_STD_OUTPUT

#define NODI_GPIO_PINVAL_SPI_SCK_MODE0  1
#define NODI_GPIO_PINVAL_SPI_SCK_MODE1  0

/* UART PINS */
#define NODI_GPIO_CFG_UART_RX           NODI_GPIO_CFG_STD_INPUT
#define NODI_GPIO_CFG_UART_TX           NODI_GPIO_CFG_STD_OUTPUT
#define NODI_GPIO_CFG_UART_RTS          NODI_GPIO_CFG_STD_OUTPUT
#define NODI_GPIO_CFG_UART_CTS          NODI_GPIO_CFG_STD_INPUT

#define NODI_GPIO_PINVAL_UART_TX        1
#define NODI_GPIO_PINVAL_UART_RTS       1

#endif // NODI_GPIO_NRF52840_H
