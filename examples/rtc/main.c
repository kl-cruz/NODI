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

volatile static bool is_started = false;
static bool led_toggle;
static nodi_gpio_pin_t pca10056_led0 = NODI_GPIO_PIN(NODI_GPIO_P0, 13);

void lfclk_handler(nodi_pwr_clk_drv_t *p_pwr_clk_drv)
{
    is_started = true;
}

void irq_routine(nodi_rtc_drv_t *p_rtc_drv, nodi_rtc_cb_evt_t evt)
{
    led_toggle = !led_toggle;
    if (led_toggle)
    {
        nodi_gpio_clr(pca10056_led0.p_port, pca10056_led0.pin);
    }
    else
    {
        nodi_gpio_set(pca10056_led0.p_port, pca10056_led0.pin);
    }
}

nodi_rtc_config_s cfg = {
    .evt_cb = irq_routine,
    .prescaler = 4095,    // To achieve one second tick
};


void pin_config(void)
{
    led_toggle = false;
    nodi_gpio_set(pca10056_led0.p_port, pca10056_led0.pin);
    nodi_gpio_config(pca10056_led0.p_port, pca10056_led0.pin, NODI_GPIO_CFG_STD_OUTPUT);
}

int main(void)
{
    /* Configure nodi subsystem */
    nodi_init();

    /* Start HF clock to get ~115200 bauds */
    NODI_PWR_CLK.lfclk_cb = lfclk_handler;
    nodi_pwr_clk_init(&NODI_PWR_CLK);
    nodi_clk_lfclk_start(&NODI_PWR_CLK, NODI_PWR_CLK_LFCLK_SRC_XTAL);
    while (!nodi_clk_lfclk_running(&NODI_PWR_CLK));

    /* Configure RTC driver */

    NODI_RTC0.config = &cfg;

    pin_config();

    nodi_rtc_init(&NODI_RTC0);
    nodi_rtc_evt_enable(&NODI_RTC0, NODI_RTC_DRV_CB_EVT_TICK);
    nodi_rtc_start(&NODI_RTC0);

    while (true)
    {
    }
}

