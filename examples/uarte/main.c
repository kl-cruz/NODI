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

void irq_routine(nsd_uarte_drv_t *p_uarte_drv)
{
    (void)(p_uarte_drv);
}

nsd_uarte_config_s cfg = {
    .end_cb = irq_routine,
    .rx_pin = 3,
    .tx_pin = 28,
    .hwfc = NRF_UARTE_HWFC_DISABLED,
    .parity = NRF_UARTE_PARITY_EXCLUDED,
    .baudrate = NRF_UARTE_BAUDRATE_115200,
};


void pin_config(void)
{
    nrf_gpio_pin_set(cfg.tx_pin);
    nrf_gpio_cfg_output(cfg.tx_pin);
    nrf_gpio_cfg_input(cfg.rx_pin, NRF_GPIO_PIN_NOPULL);
}


int main(void)
{
    uint8_t data[2] = {0x03,0xAA};

    /* Configure nsd subsystem */
    nsd_init();

    NSD_UARTE0.config = &cfg;

    pin_config();

    nsd_uarte_init(&NSD_UARTE0);
    nsd_uarte_send_start(&NSD_UARTE0, 2, data);

    while (true)
    {
    }
}

