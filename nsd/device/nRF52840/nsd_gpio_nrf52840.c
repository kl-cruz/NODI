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

#include "nsd_common.h"
#include "nsd_gpio.h"

inline void nsd_gpio_config(nsd_gpio_t *p_nsd_gpio, uint32_t pin, uint32_t config_val)
{
    NSD_DRV_CHECK(p_nsd_gpio != NULL);
    p_nsd_gpio->PIN_CNF[pin] = config_val;
}

inline void nsd_gpio_set(nsd_gpio_t *p_nsd_gpio, uint32_t pin)
{
    NSD_DRV_CHECK(p_nsd_gpio != NULL);
    NSD_DRV_CHECK(pin < 32);
    p_nsd_gpio->OUTSET = 1UL << pin;
}

inline void nsd_gpio_clr(nsd_gpio_t *p_nsd_gpio, uint32_t pin)
{
    NSD_DRV_CHECK(p_nsd_gpio != NULL);
    NSD_DRV_CHECK(pin < 32);
    p_nsd_gpio->OUTCLR = 1UL << pin;
}

uint32_t nsd_gpio_translate_periph(nsd_gpio_pin_t const *p_pin)
{
    NSD_DRV_CHECK(p_pin != NULL);
    NSD_DRV_CHECK(p_pin->pin < 32);
    if (p_pin->p_port == NSD_GPIO_P0)
    {
        return p_pin->pin;
    }
    if (p_pin->p_port == NSD_GPIO_P1)
    {
        /* Every peripheral has port value at bit 5. */
        return p_pin->pin | (1 << 5UL);
    }
    /* In case of troubles set pin as unused. */
    return 0xFFFFFFFF;
}
