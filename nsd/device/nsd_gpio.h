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

#ifndef NSD_GPIO_H
#define NSD_GPIO_H

#include "nsd_device.h"

#define NSD_GPIO_PIN(_port, _pin) {.p_port = _port, .pin=_pin}

typedef struct {
    nsd_gpio_t *p_port;
    uint32_t pin;
} nsd_gpio_pin_t;

void nsd_gpio_config(nsd_gpio_t *p_nsd_gpio, uint32_t pin, uint32_t config_val);

void nsd_gpio_set(nsd_gpio_t *p_nsd_gpio, uint32_t pin);

void nsd_gpio_clr(nsd_gpio_t *p_nsd_gpio, uint32_t pin);

uint32_t nsd_gpio_translate_periph(nsd_gpio_pin_t const *p_pin);

#endif // NSD_GPIO_H
