#ifndef GOMOKU_BACKEND_HEADER
#define GOMOKU_BACKEND_HEADER

#include "Gomoku.h"

const static int horizontalStep[8] = {1, 1, 0, -1, -1, -1, 0, 1}; 
const static int verticalStep[8] = {0, 1, 1, 1, 0, -1, -1, -1};

void continuingAtCurrentPositionOfTheLastPlayerAtDirections(GomokuState *self, int *numberAtEachDirection, int *startingOffsetToTheCurrentPositionOnEachDirection);

/* The whole chunk of quick evaluation is going to be deprecated when CNN-based evaluation is complete */
/* fonctions utiles pour l'évaluation */
int isInside(int x, int y);
int detectBlackFour(GomokuState *self, int atLine, int atColumn, int atDirection, int offset);
int detectBlackDiscontinuingFour(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset);
int detectLiveThreeOfTheBlackPlayerOnDirections(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset);
void continuingAtDirections(GomokuState *self, int atLine, int atColumn, int party, int *numberAtEachDirection, int *startingOffsetToTheCurrentPositionOnEachDirection);
int boardIsFull(GomokuState *self);

float quickEvaluationForTheCurrentPlayer(GomokuState *self);

int gameTerminated(GomokuState *self);

#endif