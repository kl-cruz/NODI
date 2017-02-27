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

#include "nsd_mnd.h"

/*===========================================================================*/
/* Micro NVIC Dispatcher data structures and types.                          */
/*===========================================================================*/

typedef struct {
    nsd_nmd_irq_routine_t irq_routine;
    void * p_ctx;
} nsd_mnd_irq_pair_t;

typedef void (*nsd_nmd_nvic_irq_t)(void);

/*===========================================================================*/
/* Micro NVIC Dispatcher local definitions.                                  */
/*===========================================================================*/

#define NSD_MND_IRQ_OFFSET        (-NonMaskableInt_IRQn)

#define NSD_MND_FIRST_IRQ_NUMBER  (-Reset_IRQn)
#define NSD_MND_LAST_IRQ_NUMBER   (PWM3_IRQn)
/* Count without reset handler */
#define NSD_MND_IRQ_COUNT         (NSD_MND_IRQ_OFFSET + NSD_MND_LAST_IRQ_NUMBER + 1)
/* Count without reset handler */
#define NSD_MND_NVIC_IRQ_COUNT    (NSD_MND_FIRST_IRQ_NUMBER + NSD_MND_LAST_IRQ_NUMBER + 1)

#define NSD_MND_PAIR_IDX(irq_idx) (irq_idx + NSD_MND_IRQ_OFFSET)
/* MND starts from NMI irq. ISPR has NMI at position 2. Translate it: */
#define NSD_MND_IPSR_TO_IDX(ipsr_val) (ipsr_val - 2)

/*===========================================================================*/
/* Micro NVIC Dispatcher local variables and types.                          */
/*===========================================================================*/

static nsd_mnd_irq_pair_t nsd_mnd_irq_pair_array[NSD_MND_IRQ_COUNT];

/* Interrupt vector table has to be aligned to 6th bit according to:
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/CIHFDJCA.html
 */
static nsd_nmd_nvic_irq_t nvic_irq_array[NSD_MND_NVIC_IRQ_COUNT] __attribute__((aligned(0x80)));

/*===========================================================================*/
/* Micro NVIC Dispatcher local functions.                                    */
/*===========================================================================*/

/* Extern default handler to change it in init function. */
extern void Default_Handler(void);

/**
 * @brief Redefined weak default interrupt handler - Default_Handler
 */
void nsd_mnd_nvic_default_handler(void)
{
    uint32_t curr_irq = __get_IPSR();
    nsd_mnd_irq_pair_array[NSD_MND_IPSR_TO_IDX(curr_irq)].irq_routine(
            nsd_mnd_irq_pair_array[NSD_MND_IPSR_TO_IDX(curr_irq)].p_ctx);
}

static void nsd_mnd_default_routine(void * p_ctx)
{
    (void)(p_ctx);
    while(1);
}


/*===========================================================================*/
/* Micro Nvic Dispatcher exported functions.                                 */
/*===========================================================================*/

void nsd_mnd_init(void)
{
    uint32_t i;

    const nsd_nmd_nvic_irq_t * p_irq_vtor = (const nsd_nmd_nvic_irq_t *)SCB->VTOR;
    /* Address 0x00 has value of stack top. We have to take next element. */
    p_irq_vtor++;

    /* Copy existing vector table. */
    for (i = 0; i < NSD_MND_NVIC_IRQ_COUNT; ++i)
    {
        /* Change default handler into mnd default handler. Keep other handlers. */
        nvic_irq_array[i] = p_irq_vtor[i] == Default_Handler ?
                    nsd_mnd_nvic_default_handler :
                    p_irq_vtor[i];
    }

    /* Set new vector table. */
    SCB->VTOR = ((uint32_t)(nvic_irq_array) & (uint32_t)0xFFFFFFC0);

    /* Connect default handler for every interrupt in dispatcher. */
    for (i = 0; i < NSD_MND_IRQ_COUNT; ++i)
    {
        nsd_mnd_irq_pair_array[i].irq_routine = nsd_mnd_default_routine;
    }


}

void nsd_mnd_register(nsd_nmd_irq_routine_t p_func, void * p_ctx, nsd_nmd_irq_t irq_num)
{
    /* TODO: Add assert to check if irq_routine equals nsd_mnd_default_routine. */

    nsd_mnd_irq_pair_array[NSD_MND_PAIR_IDX(irq_num)].irq_routine = p_func;
    nsd_mnd_irq_pair_array[NSD_MND_PAIR_IDX(irq_num)].p_ctx = p_ctx;
}

void nsd_mnd_unregister(nsd_nmd_irq_t irq_num)
{
    nsd_mnd_irq_pair_array[NSD_MND_PAIR_IDX(irq_num)].irq_routine = nsd_mnd_default_routine;
}
