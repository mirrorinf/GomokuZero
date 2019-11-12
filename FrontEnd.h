#ifndef FrontEnd_h
#define FrontEnd_h

#include "Gomoku.h"

#define StringBufferLengthLimit 2048

void displayGomokuState(GomokuState *self);
void changeStateBasedUponUserInput(GomokuState *self, void *meaninglessPlaceholder);
void encodeGameTerninationMessage(int terminaison, char *buffer);

#endif /* FrontEnd_h */
