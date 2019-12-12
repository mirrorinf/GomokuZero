#ifndef GOMOKU_BACKEND_HEADER
#define GOMOKU_BACKEND_HEADER

#include "Gomoku.h"

const static int horizontalStep[8] = {1, 1, 0, -1, -1, -1, 0, 1}; 
const static int verticalStep[8] = {0, 1, 1, 1, 0, -1, -1, -1};

void continuingAtCurrentPositionOfTheLastPlayerAtDirections(GomokuState *self, int *numberAtEachDirection, int *startingOffsetToTheCurrentPositionOnEachDirection);

/* fonctions utiles pour l'évaluation */
int isInside(int x, int y);
int detectBlackFour(GomokuState *self, int atLine, int atColumn, int atDirection, int offset);
int detectBlackDiscontinuingFour(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset);
int detectLiveThreeOfTheBlackPlayerOnDirections(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset);
void continuingAtDirections(GomokuState *self, int atLine, int atColumn, int party, int *numberAtEachDirection, int *startingOffsetToTheCurrentPositionOnEachDirection);
int boardIsFull(GomokuState *self);

float quickEvaluationForTheCurrentPlayer(GomokuState *self);
float quickEvaluationForTheWhitePlayer(GomokuState *self);
float shortsightedEvaluationForTheWhitePlayer(GomokuState *self);

int gameTerminated(GomokuState *self);

typedef float (*evaluationBasedOnCurrentStateOnly)(GomokuState *);

#endif