#ifndef NSD_COMMON_H
#define NSD_COMMON_H

#include "nsd_device.h"

#define NSD_DRV_CHECK(state)

static inline void nsd_common_irq_enable(IRQn_Type IRQn, uint8_t priority)
{
    NVIC_SetPriority(IRQn, priority);
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_EnableIRQ(IRQn);
}

static inline void nsd_common_irq_disable(IRQn_Type IRQn)
{
    NVIC_ClearPendingIRQ(IRQn);
    NVIC_DisableIRQ(IRQn);
}

#endif
