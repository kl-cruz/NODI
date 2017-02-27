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

void irq_routine(nsd_spim_drv_t *p_spim_drv)
{
    (void)(p_spim_drv);
    nsd_spim_unselect(&NSD_SPIM0);
    data_sent = true;
}

nsd_spim_config_s cfg = {
    .bit_order = NSD_SPIM_BIT_ORDER_MSB_FIRST,
    .end_cb = irq_routine,
    .frequency = NSD_SPIM_FREQ_125K,
    .mode = NSD_SPIM_MODE_0,
    .orc = 0x00,
    .cs_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 3),
    .miso_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 4),
    .mosi_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 28),
    .sck_pin = NSD_GPIO_PIN(NSD_GPIO_P0, 29),
};


void pin_config(void)
{
    if ((cfg.mode == NSD_SPIM_MODE_0) || (cfg.mode == NSD_SPIM_MODE_1))
    {
        nsd_gpio_clr(NSD_GPIO_P0, cfg.sck_pin.pin);
    }
    else
    {
        nsd_gpio_set(NSD_GPIO_P0, cfg.sck_pin.pin);
    }

    nsd_gpio_config(cfg.sck_pin.p_port, cfg.sck_pin.pin, NSD_GPIO_CFG_SPI_SCK);
    nsd_gpio_config(cfg.mosi_pin.p_port, cfg.mosi_pin.pin, NSD_GPIO_CFG_SPI_MOSI);
    nsd_gpio_config(cfg.miso_pin.p_port, cfg.miso_pin.pin, NSD_GPIO_CFG_SPI_MISO);

    nsd_gpio_set(cfg.cs_pin.p_port, cfg.cs_pin.pin);
    nsd_gpio_config(cfg.cs_pin.p_port, cfg.cs_pin.pin, NSD_GPIO_CFG_SPI_CS);
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

    while (!data_sent);
    nsd_spim_deinit(&NSD_SPIM0);

    while (1)
    {
    }
}

