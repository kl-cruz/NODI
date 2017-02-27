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

void irq_routine(nodi_spim_drv_t *p_spim_drv)
{
    (void)(p_spim_drv);
    nodi_spim_unselect(&NODI_SPIM0);
    data_sent = true;
}

nodi_spim_config_s cfg = {
    .bit_order = NODI_SPIM_BIT_ORDER_MSB_FIRST,
    .end_cb = irq_routine,
    .frequency = NODI_SPIM_FREQ_125K,
    .mode = NODI_SPIM_MODE_0,
    .orc = 0x00,
    .cs_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 3),
    .miso_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 4),
    .mosi_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 28),
    .sck_pin = NODI_GPIO_PIN(NODI_GPIO_P0, 29),
};


void pin_config(void)
{
    if ((cfg.mode == NODI_SPIM_MODE_0) || (cfg.mode == NODI_SPIM_MODE_1))
    {
        nodi_gpio_clr(NODI_GPIO_P0, cfg.sck_pin.pin);
    }
    else
    {
        nodi_gpio_set(NODI_GPIO_P0, cfg.sck_pin.pin);
    }

    nodi_gpio_config(cfg.sck_pin.p_port, cfg.sck_pin.pin, NODI_GPIO_CFG_SPI_SCK);
    nodi_gpio_config(cfg.mosi_pin.p_port, cfg.mosi_pin.pin, NODI_GPIO_CFG_SPI_MOSI);
    nodi_gpio_config(cfg.miso_pin.p_port, cfg.miso_pin.pin, NODI_GPIO_CFG_SPI_MISO);

    nodi_gpio_set(cfg.cs_pin.p_port, cfg.cs_pin.pin);
    nodi_gpio_config(cfg.cs_pin.p_port, cfg.cs_pin.pin, NODI_GPIO_CFG_SPI_CS);
}


int main(void)
{
    uint8_t data[2] = {0x03,0xAA};

    /* Configure nodi subsystem */
    nodi_init();

    NODI_SPIM0.config = &cfg;

    pin_config();

    nodi_spim_init(&NODI_SPIM0);
    nodi_spim_select(&NODI_SPIM0);
    nodi_spim_send(&NODI_SPIM0, 2, data);

    while (!data_sent);
    nodi_spim_deinit(&NODI_SPIM0);

    while (1)
    {
    }
}

