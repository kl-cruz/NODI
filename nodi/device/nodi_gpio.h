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

#ifndef NODI_GPIO_H
#define NODI_GPIO_H

#include "nodi_device.h"

#define NODI_GPIO_PIN(_port, _pin) {.p_port = _port, .pin=_pin}

typedef struct {
    nodi_gpio_t *p_port;
    uint32_t pin;
} nodi_gpio_pin_t;

void nodi_gpio_config(nodi_gpio_t *p_nodi_gpio, uint32_t pin, uint32_t config_val);

void nodi_gpio_set(nodi_gpio_t *p_nodi_gpio, uint32_t pin);

void nodi_gpio_clr(nodi_gpio_t *p_nodi_gpio, uint32_t pin);

uint32_t nodi_gpio_translate_periph(nodi_gpio_pin_t const *p_pin);

#endif // NODI_GPIO_H
