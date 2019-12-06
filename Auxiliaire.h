#ifndef AUXILIAIRE_HEADER
#define AUXILIAIRE_HEADER

#define DARWIN

#ifdef DARWIN
#define RANDOM_UNIFORM arc4random_uniform
#define PARALLELISM_LIBDISPATCH
#else
#define RANDOM_UNIFOREM(x) (rand() % x)
#define PARALLELISM_PTHREAD
#endif

#ifdef PARALLELISM_LIBDISPATCH
#include <dispatch/dispatch.h>
#endif

#endif