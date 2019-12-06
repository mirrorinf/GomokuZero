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
    unsigned char board[57];
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
typedef void (*opponentCallback)(void *);

GomokuState *initGomokuState();
void destroyGomokuState(GomokuState *self);

GomokuState *copyState(GomokuState *self);

char stateAtPosition(GomokuState *self, int atLine, int atColumn);
void changeState(GomokuState *self, int atLine, int atColumn, char toState);

int serialNumber(int atLine, int atColumn);

/* Consider only positions that are adjacent to the current stones */
int isAdjecentToSituation(GomokuState *self, int line, int column);

int isInside(int x, int y);

#endif /* Gomoku_h */
