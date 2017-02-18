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

void irq_routine(nsd_spim_drv_t *p_spim_drv)
{
    (void)(p_spim_drv);
    nsd_spim_unselect(&NSD_SPIM0);
}

nsd_spim_config_s cfg = {
    .bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST,
    .cs_pin = 3,
    .end_cb = irq_routine,
    .frequency = NRF_SPIM_FREQ_125K,
    .miso_pin = 4,
    .mode = NRF_SPIM_MODE_0,
    .mosi_pin = 28,
    .orc = 0x00,
    .sck_pin = 29,
};


void pin_config(void)
{
    if (cfg.mode <= NRF_SPIM_MODE_1)
    {
        nrf_gpio_pin_clear(cfg.sck_pin);
    }
    else
    {
        nrf_gpio_pin_set(cfg.sck_pin);
    }

    nrf_gpio_cfg_output(cfg.sck_pin);
    // - MOSI (optional) - output with initial value 0,
     nrf_gpio_pin_clear(cfg.mosi_pin);
     nrf_gpio_cfg_output(cfg.mosi_pin);
    // - MISO (optional) - input,

    nrf_gpio_cfg_input(cfg.miso_pin, NRF_GPIO_PIN_NOPULL);
    // - Slave Select (optional) - output with initial value 1 (inactive).
    nrf_gpio_pin_set(cfg.cs_pin);
    nrf_gpio_cfg_output(cfg.cs_pin);
}


int main(void)
{
    uint8_t data[2] = {0x03,0xAA};

    /* Configure nsd subsystem */
    nsd_init();

    NSD_SPIM0.config = &cfg;

    pin_config();

    nsd_spim_init(&NSD_SPIM0);
    nsd_spim_select(&NSD_SPIM0);
    nsd_spim_send(&NSD_SPIM0, 2, data);
    nsd_spim_deinit(&NSD_SPIM0);

    while (true)
    {
    }
}

