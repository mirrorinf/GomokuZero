#ifndef AUXILIAIRE_HEADER
#define AUXILIAIRE_HEADER

#define DARWIN
#define FRONTEND_BOARD_SMALL
/* black terminal: FRONTEND_FONCEE white terminal: FRONTEND_CLAIRE */
#define FRONTEND_FONCEE

#ifdef DARWIN
#define RANDOM_UNIFORM arc4random_uniform
#define SRANDOM
#define FRONTEND_CHARSET_UTF8
#else
#include <time.h>
#define RANDOM_UNIFOREM(x) (rand() % (x))
#define SRANDOM srand(time(NULL));
#define FRONTEND_CHARSET_UTF8
#endif

#endif
