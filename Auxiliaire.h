#ifndef AUXILIAIRE_HEADER
#define AUXILIAIRE_HEADER

#define DARWIN

#ifdef DARWIN
#define RANDOM_UNIFORM arc4random_uniform
#define SRANDOM
#define PARALLELISM_LIBDISPATCH
#else
#include <time.h>
#define RANDOM_UNIFOREM(x) (rand() % (x))
#define SRANDOM srand(time(NULL));
#define PARALLELISM_PTHREAD
#endif

#ifdef PARALLELISM_LIBDISPATCH
#include <dispatch/dispatch.h>
#endif

#endif