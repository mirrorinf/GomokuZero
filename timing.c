#include "timing.h"

#ifdef DARWIN
#include <mach/mach_time.h>

static mach_timebase_info_data_t timeInfo;

void initializeTimingSystem() {
    mach_timebase_info(&timeInfo);
}

unsigned long currentSystemTime() {
    return mach_absolute_time();
}

float convertTimeIntevalToMilliSeconds(unsigned long start, unsigned long end) {
    unsigned long t = start - end;
    unsigned long nano = t * timeInfo.numer / timeInfo.denom;
    return (float)nano / 1e6;
}

unsigned long convertToRepresentation(float milliseconds) {
    unsigned long ns = milliseconds * 1e6;
    return ns * timeInfo.denom / timeInfo.numer;
}
#else
#ifdef LINUX
#endif
#endif

/* If this is a more formal project then here should be implementations for other platforms */
