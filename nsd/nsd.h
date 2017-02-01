#ifndef NSD_H
#define NSD_H

#define TRUE 1
#define FALSE 0

#include <stdint.h>
#include <stdlib.h>
#include "nsd_conf.h"
#include "nsd_default.h"
#include "nsd_device.h"
#include "nsd_common.h"


/* Drivers */
#include "nsd_spim.h"

void nsd_init(void);

#endif /* NSD_H */
