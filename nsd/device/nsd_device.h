#ifndef NSD_DEVICE_H
#define NSD_DEVICE_H

#if defined(NSD_CHIP_NRF52832)
#include "nsd_nRF52832.h"
#elif defined(NSD_CHIP_NRF52840)
#include "nsd_nRF52840.h"
#else
#error "Please define chip in C flags as NSD_CHIP=xxxxx"
#endif

#endif // NSD_NRF52832_H
