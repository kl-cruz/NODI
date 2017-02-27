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
#include "nsd.h"

static bool data_sent = false;

void irq_routine(nsd_uarte_drv_t *p_uarte_drv)
{
    (void)(p_uarte_drv);
    data_sent = true;
}

nsd_uarte_config_s cfg = {
    .tx_end_cb = irq_routine,
    .rx_end_cb = irq_routine,
    .rx_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 3),
    .tx_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 28),
    .hwfc = 0,
    .parity = 0,
    .baudrate = NSD_UARTE_BAUD_115200,
};


void pin_config(void)
{
    nsd_gpio_set(NSD_GPIO_P0, cfg.tx_pin.pin);
    nsd_gpio_config(NSD_GPIO_P0, cfg.tx_pin.pin, NSD_GPIO_CFG_UART_TX);
    nsd_gpio_config(NSD_GPIO_P0, cfg.rx_pin.pin, NSD_GPIO_CFG_UART_RX);
}

int main(void)
{
    uint8_t data[2] = {0x03,0xAA};

    /* Configure nsd subsystem */
    nsd_init();

    /* Start HF clock to get ~115200 bauds */
    nsd_pwr_clk_init(&NSD_PWR_CLK);
    nsd_clk_hfclk_start(&NSD_PWR_CLK);
    while (!nsd_clk_hfclk_running(&NSD_PWR_CLK));

    /* Configure UARTE driver */

    NSD_UARTE0.config = &cfg;

    pin_config();

    nsd_uarte_init(&NSD_UARTE0);
    nsd_uarte_send_start(&NSD_UARTE0, 2, data);

    while (!data_sent);
    nsd_uarte_deinit(&NSD_UARTE0);

    while (1)
    {
    }
}

