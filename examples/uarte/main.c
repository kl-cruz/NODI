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

#include <stdbool.h>
#include <stdint.h>
#include "nodi.h"

volatile static bool data_sent = false;
volatile static bool is_started = false;

void hfclk_handler(nodi_pwr_clk_drv_t *p_pwr_clk_drv)
{
    (void)(p_pwr_clk_drv);
    is_started = true;
}

void irq_routine(nodi_uarte_drv_t *p_uarte_drv)
{
    (void)(p_uarte_drv);
    data_sent = true;
}

nodi_uarte_config_s cfg = {
    .tx_end_cb = irq_routine,
    .rx_end_cb = irq_routine,
    .rx_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 3),
    .tx_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 28),
    .hwfc = 0,
    .parity = 0,
    .baudrate = NODI_UARTE_BAUD_115200,
};


void pin_config(void)
{
    nodi_gpio_set(NODI_GPIO_P0, cfg.tx_pin.pin);
    nodi_gpio_config(NODI_GPIO_P0, cfg.tx_pin.pin, NODI_GPIO_CFG_UART_TX);
    nodi_gpio_config(NODI_GPIO_P0, cfg.rx_pin.pin, NODI_GPIO_CFG_UART_RX);
}

int main(void)
{
    uint8_t data[2] = {0x03,0xAA};

    /* Configure nodi subsystem */
    nodi_init();

    /* Start HF clock to get ~115200 bauds */
    NODI_PWR_CLK.hfclk_cb = hfclk_handler;
    nodi_pwr_clk_init(&NODI_PWR_CLK);
    nodi_clk_hfclk_start(&NODI_PWR_CLK);
    while (!is_started);

    /* Configure UARTE driver */

    NODI_UARTE0.config = &cfg;

    pin_config();

    nodi_uarte_init(&NODI_UARTE0);
    nodi_uarte_send_start(&NODI_UARTE0, 2, data);

    while (!data_sent);
    nodi_uarte_deinit(&NODI_UARTE0);

    while (1)
    {
    }
}

