#ifndef FrontEnd_h
#define FrontEnd_h

#include "Gomoku.h"

#define StringBufferLengthLimit 2048

void displayGomokuState(GomokuState *self, const char *additionalMessage);
void changeStateBasedUponUserInput(GomokuState *self, void *meaninglessPlaceholder);
void encodeGameTerninationMessage(int terminaison, char *buffer);
void meaninglessPlaceholder(void *meaningless);

#endif /* FrontEnd_h */
