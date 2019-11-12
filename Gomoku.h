#ifndef Gomoku_h
#define Gomoku_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define kGomokuGridStateUnoccupied 0
#define kGomokuGridStateOccupiedPlayerBlack -1
#define kGomokuGridStateOccupiedPlayerWhite 1

#define kGomokuPlayerBlack -1
#define kGomokuPlayerWhite 1

typedef struct __GomokuState {
    char *grid;
    int recentMoveLine, recentMoveColumn, nextMoveParty;
} GomokuState;

/* negatif if black won greater than 2000 if white won */
#define kGameTerminatedDraw 100
#define kGameHasNotYetTerminated 0
#define kGameTerminatedBlackWon -2333
#define kGameTerminatedWhiteContinuedFive 2333
#define kGameTerminatedBlackForbiddenDoubleFour 2334
#define kGameTerminatedBlackForbiddenDoubleThree 2335
#define kGameTerminatedBlackForbiddenOverline 2336

typedef void (*player)(GomokuState *, void *);

GomokuState *initGomokuState();
void destroyGomokuState(GomokuState *self);

GomokuState *copyState(GomokuState *self);
void UNSAFEcopyImage(GomokuState *self, float *destination, int party);/* not yet implemented */

char stateAtPosition(GomokuState *self, int atLine, int atColumn);
void changeState(GomokuState *self, int atLine, int atColumn, char toState);

#endif /* Gomoku_h */
