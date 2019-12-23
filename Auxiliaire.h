#ifndef AUXILIAIRE_HEADER
#define AUXILIAIRE_HEADER

#define DARWIN
#define FRONTEND_BOARD_SMALL
#define FRONTEND_CLAIRE

#ifdef DARWIN
#define RANDOM_UNIFORM arc4random_uniform
#define SRANDOM
#define FRONTEND_CHARSET_UTF8
#else
#ifdef WIN32
#error "No support for Windows"
#else /* Support of other platforms not tested */
#include <time.h>
#define RANDOM_UNIFOREM(x) (rand() % (x))
#define SRANDOM srand(time(NULL));
#define FRONTEND_CHARSET_UTF8
#endif
#endif

#endif
