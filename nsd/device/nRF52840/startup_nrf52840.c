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

#define __DEFAULT_IRQ_WEAK __attribute__ ((weak, alias("default_irq_handler")))

#ifndef __STACK_SIZE
#define __STACK_SIZE 0x2000
#endif

#ifndef __HEAP_SIZE
#define __HEAP_SIZE 0x2000
#endif

/* Variables maps to memory in linker script. */
extern unsigned long __bss_start__;
extern unsigned long __bss_end__;
extern unsigned long __etext;
extern unsigned long __data_start__;
extern unsigned long __data_end__;
extern unsigned long __StackTop;

/* Functions using to init std lib, system init and start app. */
extern void __libc_init_array();
extern void SystemInit();
extern void main();

/* Set up stack and heap. */

char __stack[__STACK_SIZE] __attribute__ ((section(".stack")));
char __heap[__HEAP_SIZE] __attribute__ ((section(".heap")));

/**
 * @brief Default irq handler executing if other irq is not defined.
 */
__attribute__((weak)) void default_irq_handler()
{
    while (1)
    {
        /* Just while */
    }
}

/**
 * @brief Entry point, starting after reset.
 */
__attribute__((naked)) void Reset_Handler()
{
    /* Pointers to data section. */
    unsigned long *p_data_src;
    unsigned long *p_data_dest;

    /* Pointer to bss start point. */
    unsigned long *p_bss_start;

    /* Set up the stack pointer. */
    __asm("ldr sp,=__StackTop\n\t");

    /* Copy initialized data section. */
    p_data_src = &__etext;
    p_data_dest = &__data_start__;

    while (p_data_dest != &__data_end__)
    {
        *(p_data_dest++) = *(p_data_src++);
    }

    /* Clear BSS section. */
    p_bss_start = &__bss_start__;

    while (p_bss_start != &__bss_end__)
    {
        *(p_bss_start++) = 0;
    }

    /* Initialize MCU, apply erratas. */
    SystemInit();

    /* Run constructors. */
    __libc_init_array();

    /* Call the application's entry point. */
    main();

    /* If something goes wrong, catch execution in while loop. */
    while (1);
}


/* Point every IRQ to default_irq_handler if not implemented. */

/* ARM Cortex exceptions */
void NMI_Handler() __DEFAULT_IRQ_WEAK;
void HardFault_Handler() __DEFAULT_IRQ_WEAK;
void MemManage_Handler() __DEFAULT_IRQ_WEAK;
void BusFault_Handler() __DEFAULT_IRQ_WEAK;
void UsageFault_Handler() __DEFAULT_IRQ_WEAK;
void SVC_Handler() __DEFAULT_IRQ_WEAK;
void DebugMonitor_Handler() __DEFAULT_IRQ_WEAK;
void PendSV_Handler() __DEFAULT_IRQ_WEAK;
void SysTick_Handler() __DEFAULT_IRQ_WEAK;

/* nrf52840 Specific Peripheral Interrupts */
void POWER_CLOCK_IRQHandler() __DEFAULT_IRQ_WEAK;
void RADIO_IRQHandler() __DEFAULT_IRQ_WEAK;
void UARTE0_UART0_IRQHandler() __DEFAULT_IRQ_WEAK;
void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler() __DEFAULT_IRQ_WEAK;
void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler() __DEFAULT_IRQ_WEAK;
void NFCT_IRQHandler() __DEFAULT_IRQ_WEAK;
void GPIOTE_IRQHandler() __DEFAULT_IRQ_WEAK;
void SAADC_IRQHandler() __DEFAULT_IRQ_WEAK;
void TIMER0_IRQHandler() __DEFAULT_IRQ_WEAK;
void TIMER1_IRQHandler() __DEFAULT_IRQ_WEAK;
void TIMER2_IRQHandler() __DEFAULT_IRQ_WEAK;
void RTC0_IRQHandler() __DEFAULT_IRQ_WEAK;
void TEMP_IRQHandler() __DEFAULT_IRQ_WEAK;
void RNG_IRQHandler() __DEFAULT_IRQ_WEAK;
void ECB_IRQHandler() __DEFAULT_IRQ_WEAK;
void CCM_AAR_IRQHandler() __DEFAULT_IRQ_WEAK;
void WDT_IRQHandler() __DEFAULT_IRQ_WEAK;
void RTC1_IRQHandler() __DEFAULT_IRQ_WEAK;
void QDEC_IRQHandler() __DEFAULT_IRQ_WEAK;
void COMP_LPCOMP_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI0_EGU0_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI1_EGU1_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI2_EGU2_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI3_EGU3_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI4_EGU4_IRQHandler() __DEFAULT_IRQ_WEAK;
void SWI5_EGU5_IRQHandler() __DEFAULT_IRQ_WEAK;
void TIMER3_IRQHandler() __DEFAULT_IRQ_WEAK;
void TIMER4_IRQHandler() __DEFAULT_IRQ_WEAK;
void PWM0_IRQHandler() __DEFAULT_IRQ_WEAK;
void PDM_IRQHandler() __DEFAULT_IRQ_WEAK;
void MWU_IRQHandler() __DEFAULT_IRQ_WEAK;
void PWM1_IRQHandler() __DEFAULT_IRQ_WEAK;
void PWM2_IRQHandler() __DEFAULT_IRQ_WEAK;
void SPIM2_SPIS2_SPI2_IRQHandler() __DEFAULT_IRQ_WEAK;
void RTC2_IRQHandler() __DEFAULT_IRQ_WEAK;
void I2S_IRQHandler() __DEFAULT_IRQ_WEAK;
void FPU_IRQHandler() __DEFAULT_IRQ_WEAK;
void USBD_IRQHandler() __DEFAULT_IRQ_WEAK;
void UARTE1_IRQHandler() __DEFAULT_IRQ_WEAK;
void QSPI_IRQHandler() __DEFAULT_IRQ_WEAK;
void CRYPTOCELL_IRQHandler() __DEFAULT_IRQ_WEAK;
void SPIM3_IRQHandler() __DEFAULT_IRQ_WEAK;
void PWM3_IRQHandler() __DEFAULT_IRQ_WEAK;

/**
  * Definition of interrupt vector table.
  */
void (* const irq_vector[])() __attribute__ ((section(".isr_vector"))) = {
    /* ARM Cortex exceptions */
    (void (*)(void))&__StackTop,                   /*       Stack top pointer                */
    Reset_Handler,                                 /* -15 - Reset                            */
    NMI_Handler,                                   /* -14 - NMI                              */
    HardFault_Handler,                             /* -13 - Hard fault                       */
    MemManage_Handler,                             /* -12 - Memory management fault          */
    BusFault_Handler,                              /* -11 - Bus fault                        */
    UsageFault_Handler,                            /* -10 - Usage fault                      */
    0,                                             /* -9  - Reserved                         */
    0,                                             /* -8  - Reserved                         */
    0,                                             /* -7  - Reserved                         */
    0,                                             /* -6  - Reserved                         */
    SVC_Handler,                                   /* -5  - SVCall                           */
    DebugMonitor_Handler,                          /* -4  - Reservered for Debug             */
    0,                                             /* -3  - Reserved                         */
    PendSV_Handler,                                /* -2  - PendSV                           */
    SysTick_Handler,                               /* -1  - Systick                          */

    /* nrf52840 Specific Peripheral Interrupts */
    POWER_CLOCK_IRQHandler,                        /* 0   - POWER_CLOCK                      */
    RADIO_IRQHandler,                              /* 1   - RADIO                            */
    UARTE0_UART0_IRQHandler,                       /* 2   - UARTE0_UART0                     */
    SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler,  /* 3   - SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0*/
    SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler,  /* 4   - SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1*/
    NFCT_IRQHandler,                               /* 5   - NFCT                             */
    GPIOTE_IRQHandler,                             /* 6   - GPIOTE                           */
    SAADC_IRQHandler,                              /* 7   - SAADC                            */
    TIMER0_IRQHandler,                             /* 8   - TIMER0                           */
    TIMER1_IRQHandler,                             /* 9   - TIMER1                           */
    TIMER2_IRQHandler,                             /* 10  - TIMER2                           */
    RTC0_IRQHandler,                               /* 11  - RTC0                             */
    TEMP_IRQHandler,                               /* 12  - TEMP                             */
    RNG_IRQHandler,                                /* 13  - RNG                              */
    ECB_IRQHandler,                                /* 14  - ECB                              */
    CCM_AAR_IRQHandler,                            /* 15  - CCM_AAR                          */
    WDT_IRQHandler,                                /* 16  - WDT                              */
    RTC1_IRQHandler,                               /* 17  - RTC1                             */
    QDEC_IRQHandler,                               /* 18  - QDEC                             */
    COMP_LPCOMP_IRQHandler,                        /* 19  - COMP_LPCOMP                      */
    SWI0_EGU0_IRQHandler,                          /* 20  - SWI0_EGU0                        */
    SWI1_EGU1_IRQHandler,                          /* 21  - SWI1_EGU1                        */
    SWI2_EGU2_IRQHandler,                          /* 22  - SWI2_EGU2                        */
    SWI3_EGU3_IRQHandler,                          /* 23  - SWI3_EGU3                        */
    SWI4_EGU4_IRQHandler,                          /* 24  - SWI4_EGU4                        */
    SWI5_EGU5_IRQHandler,                          /* 25  - SWI5_EGU5                        */
    TIMER3_IRQHandler,                             /* 26  - TIMER3                           */
    TIMER4_IRQHandler,                             /* 27  - TIMER4                           */
    PWM0_IRQHandler,                               /* 28  - PWM0                             */
    PDM_IRQHandler,                                /* 29  - PDM                              */
    0,                                             /* 30  - Reserved                         */
    0,                                             /* 31  - Reserved                         */
    MWU_IRQHandler,                                /* 32  - MWU                              */
    PWM1_IRQHandler,                               /* 33  - PWM1                             */
    PWM2_IRQHandler,                               /* 34  - PWM2                             */
    SPIM2_SPIS2_SPI2_IRQHandler,                   /* 35  - SPIM2_SPIS2_SPI2                 */
    RTC2_IRQHandler,                               /* 36  - RTC2                             */
    I2S_IRQHandler,                                /* 37  - I2S                              */
    FPU_IRQHandler,                                /* 38  - FPU                              */
    USBD_IRQHandler,                               /* 39  - USBD                             */
    UARTE1_IRQHandler,                             /* 40  - UARTE1                           */
    QSPI_IRQHandler,                               /* 41  - QSPI                             */
    CRYPTOCELL_IRQHandler,                         /* 42  - CRYPTOCELL                       */
    SPIM3_IRQHandler,                              /* 43  - SPIM3                            */
    0,                                             /* 44  - Reserved                         */
    PWM3_IRQHandler,                               /* 45  - PWM3                             */
};
