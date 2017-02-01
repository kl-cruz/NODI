#include "nsd.h"

void nsd_init(void)
{
#if (NSD_SPIM_ENABLED == TRUE) || defined(__DOXYGEN__)
    nsd_spim_prepare();
#endif
}
