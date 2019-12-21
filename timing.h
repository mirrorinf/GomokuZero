#ifndef GOMOKUZERO_TIMING_HEADER
#define GOMOKUZERO_TIMING_HEADER

#include "Auxiliaire.h"

void initializeTimingSystem();
unsigned long currentSystemTime();
float convertTimeIntevalToMilliSeconds(unsigned long start, unsigned long end);

#endif
