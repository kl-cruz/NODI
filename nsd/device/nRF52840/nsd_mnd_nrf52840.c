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

/* TODO:
 *
 * void my_default_handler(void)
 * {
 *     while(1);
 * }
 *
 * irq_pairs_t my_pairs[IRQ_numbers] = {my_default_handler};
 *
 * void default_handler(void)
 * {
 *     int32_t curr_irq = __IPSR(); // get_ipsr something like this
 *     my_pairs[curr_irq].func(my_pairs[curr_irq].p_ctx);
 * }
 *
 * vector_array[IRQ_numbers]={default_handler}
 *
 * init()
 * {
 *    NVIC_set_IRQ_offset(vector_array); // SCB->VTOR = (address & (uint32_t)0x1FFFFF80);
 * }
 *
 */

/*===========================================================================*/
/* Micro NVIC Dispatcher data structures and types.                          */
/*===========================================================================*/

typedef struct {
    nsd_nmd_irq_routine_t irq_routine;
    void * p_ctx;
} nsd_mnd_irq_pair_t;

/*===========================================================================*/
/* Micro NVIC Dispatcher local definitions.                                  */
/*===========================================================================*/

#define NSD_MND_IRQ_OFFSET        (-NonMaskableInt_IRQn)
#define NSD_MND_LAST_IRQ_NUMBER   (PWM3_IRQn)
#define NSD_MND_IRQ_COUNT         (NSD_MND_IRQ_OFFSET + NSD_MND_LAST_IRQ_NUMBER + 1)

#define NSD_MND_PAIR_IDX(irq_idx) (irq_idx + NSD_MND_IRQ_OFFSET)

#define NSD_MND_CALL_ROUTINE(routine) do {                               \
    nsd_mnd_irq_pair_array[routine + NSD_MND_IRQ_OFFSET].irq_routine(    \
            nsd_mnd_irq_pair_array[routine + NSD_MND_IRQ_OFFSET].p_ctx); \
    } while(0)

/*===========================================================================*/
/* Micro NVIC Dispatcher local variables and types.                          */
/*===========================================================================*/

static nsd_mnd_irq_pair_t nsd_mnd_irq_pair_array[NSD_MND_IRQ_COUNT];

/*===========================================================================*/
/* Micro NVIC Dispatcher local functions.                                    */
/*===========================================================================*/

void nsd_mnd_default_routine(void * p_ctx)
{
    (void)(p_ctx);
    while(1);
}

/*===========================================================================*/
/* Micro NVIC Dispatcher interrupts functions.                               */
/*===========================================================================*/

void NMI_Handler(void)
{
    NSD_MND_CALL_ROUTINE(NonMaskableInt_IRQn);
}

void HardFault_Handler(void)
{
    NSD_MND_CALL_ROUTINE(HardFault_IRQn);
}

void MemoryManagement_Handler(void)
{
    NSD_MND_CALL_ROUTINE(MemoryManagement_IRQn);
}

void BusFault_Handler(void)
{
    NSD_MND_CALL_ROUTINE(BusFault_IRQn);
}

void UsageFault_Handler(void)
{
    NSD_MND_CALL_ROUTINE(UsageFault_IRQn);
}

void SVC_Handler(void)
{
    NSD_MND_CALL_ROUTINE(SVCall_IRQn);
}

void DebugMon_Handler(void)
{
    NSD_MND_CALL_ROUTINE(DebugMonitor_IRQn);
}

void PendSV_Handler(void)
{
    NSD_MND_CALL_ROUTINE(PendSV_IRQn);
}

void SysTick_Handler(void)
{
    NSD_MND_CALL_ROUTINE(SysTick_IRQn);
}


  /* External Interrupts */
void POWER_CLOCK_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(POWER_CLOCK_IRQn);
}

void RADIO_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(RADIO_IRQn);
}

void UARTE0_UART0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(UARTE0_UART0_IRQn);
}

void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn);
}

void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn);
}

void NFCT_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(NFCT_IRQn);
}

void GPIOTE_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(GPIOTE_IRQn);
}

void SAADC_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SAADC_IRQn);
}

void TIMER0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TIMER0_IRQn);
}

void TIMER1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TIMER1_IRQn);
}

void TIMER2_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TIMER2_IRQn);
}

void RTC0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(RTC0_IRQn);
}

void TEMP_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TEMP_IRQn);
}

void RNG_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(RNG_IRQn);
}

void ECB_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(ECB_IRQn);
}

void CCM_AAR_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(CCM_AAR_IRQn);
}

void WDT_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(WDT_IRQn);
}

void RTC1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(RTC1_IRQn);
}

void QDEC_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(QDEC_IRQn);
}

void COMP_LPCOMP_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(COMP_LPCOMP_IRQn);
}

void SWI0_EGU0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI0_EGU0_IRQn);
}

void SWI1_EGU1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI1_EGU1_IRQn);
}

void SWI2_EGU2_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI2_EGU2_IRQn);
}

void SWI3_EGU3_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI3_EGU3_IRQn);
}

void SWI4_EGU4_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI4_EGU4_IRQn);
}

void SWI5_EGU5_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SWI5_EGU5_IRQn);
}

void TIMER3_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TIMER3_IRQn);
}

void TIMER4_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(TIMER4_IRQn);
}

void PWM0_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(PWM0_IRQn);
}

void PDM_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(PDM_IRQn);
}

void MWU_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(MWU_IRQn);
}

void PWM1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(PWM1_IRQn);
}

void PWM2_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(PWM2_IRQn);
}

void SPIM2_SPIS2_SPI2_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SPIM2_SPIS2_SPI2_IRQn);
}

void RTC2_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(RTC2_IRQn);
}

void I2S_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(I2S_IRQn);
}

void FPU_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(FPU_IRQn);
}

void USBD_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(USBD_IRQn);
}

void UARTE1_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(UARTE1_IRQn);
}

void QSPI_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(QSPI_IRQn);
}

void CRYPTOCELL_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(CRYPTOCELL_IRQn);
}

void SPIM3_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(SPIM3_IRQn);
}

void PWM3_IRQHandler(void)
{
    NSD_MND_CALL_ROUTINE(PWM3_IRQn);
}

/*===========================================================================*/
/* Micro Nvic Dispatcher exported functions.                                 */
/*===========================================================================*/

void nsd_mnd_init(void)
{
    uint32_t i;
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
