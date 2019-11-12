/* This file contains bullet-proof tested shit. Do not modify. Rewrite if malfunction. */

#include "BackEnd.h"

int isInside(int x, int y) {
    return (x <= 15 && x >= 1) && (y <= 15 && y >= 1);
}

int yA_t_ilContinuationBlackUnobstructedBeginningBySpace(GomokuState *self, int atLine, int atColumn, int atDirection, int consistsOf) {
    int i, line, column;

    line = atLine + verticalStep[atDirection];
    column = atColumn + horizontalStep[atDirection];
    if (!isInside(line, column) || stateAtPosition(self, line, column) != kGomokuGridStateUnoccupied) {
        return 0;
    }
    for (i = 0; i < consistsOf; i++) {
        line += verticalStep[atDirection];
        column += horizontalStep[atDirection];
        if (!isInside(line, column) || stateAtPosition(self, line, column) != kGomokuPlayerBlack) {
            return 0;
        }
    }
    line += verticalStep[atDirection];
    column += horizontalStep[atDirection];
    if (isInside(line, column) && stateAtPosition(self, line, column) == kGomokuPlayerBlack) {
        return 0;
    }

    return 1;
}

/* live: 2 half: 1 dead: 0 
   the situation in the four totally ignored can be pretended */
int detectBlackFour(GomokuState *self, int atLine, int atColumn, int atDirection, int offset) {
    int left, right, currentLine, currentColumn, leftObstructed, rightObstructed;

    left = 0;
    right = 0;
    leftObstructed = 0;
    rightObstructed = 0;

    right = offset + 4;
    left = -offset + 1;

    currentLine = atLine + right * verticalStep[atDirection];
    currentColumn = atColumn + right * horizontalStep[atDirection];
    if (!isInside(currentLine, currentColumn) || stateAtPosition(self, currentLine, currentColumn) != kGomokuGridStateUnoccupied) {
        rightObstructed = 1;
    } else {
        currentLine += verticalStep[atDirection];
        currentColumn += horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
            rightObstructed = 1;
        }
    }

    currentLine = atLine + left * verticalStep[atDirection + 4];
    currentColumn = atColumn + left * horizontalStep[atDirection + 4];
    if (!isInside(currentLine, currentColumn) || stateAtPosition(self, currentLine, currentColumn) != kGomokuGridStateUnoccupied) {
        leftObstructed = 1;
    } else {
        currentLine += verticalStep[atDirection + 4];
        currentColumn += horizontalStep[atDirection + 4];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
            leftObstructed = 1;
        }
    }

    return 2 - leftObstructed - rightObstructed;
}

int detectBlackDiscontinuingFour(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset) {
    int currentLine, currentColumn, result, i;
    result = 0;

    for (i = 1; i < 4; i++) {
        if (i == continuing) {
            currentLine = atLine;
            currentColumn = atColumn;
            currentLine += offset * verticalStep[atDirection];
            currentColumn += offset * horizontalStep[atDirection];
            result += yA_t_ilContinuationBlackUnobstructedBeginningBySpace(self, currentLine, currentColumn, atDirection + 4, 4 - i);
            currentLine = atLine;
            currentColumn = atColumn;
            currentLine += (offset + i - 1) * verticalStep[atDirection];
            currentColumn += (offset + i - 1) * horizontalStep[atDirection];
            result += yA_t_ilContinuationBlackUnobstructedBeginningBySpace(self, currentLine, currentColumn, atDirection, 4 - i);
        }
    }

    return result;
}

/* 1  on live three 0 otherwise */
int detectLiveThreeOfTheBlackPlayerOnDirections(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset) {
    int currentLine, currentColumn;

    switch (continuing) {
    case 3:
        currentLine = atLine + (offset - 1) * verticalStep[atDirection];
        currentColumn = atColumn + (offset - 1) * horizontalStep[atDirection];
        if (!isInside(currentLine, currentColumn) || stateAtPosition(self, currentLine, currentColumn) != kGomokuGridStateUnoccupied) {
            return 0;
        }
        currentLine = atLine + (offset + 3) * verticalStep[atDirection];
        currentColumn = atColumn + (offset + 3) * horizontalStep[atDirection];
        if (!isInside(currentLine, currentColumn) || stateAtPosition(self, currentLine, currentColumn) != kGomokuGridStateUnoccupied) {
            return 0;
        }
        if (detectBlackFour(self, atLine, atColumn, atDirection, offset - 1) == 2 || detectBlackFour(self, atLine, atColumn, atDirection, offset) == 2) {
            return 1;
        }
        return 0;
        break;
    case 2:
        currentLine = atLine + (offset - 1) * verticalStep[atDirection];
        currentColumn = atColumn + (offset - 1) * horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuGridStateUnoccupied) {
            currentLine -= verticalStep[atDirection];
            currentColumn -= horizontalStep[atDirection];
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                if (detectBlackFour(self, atLine, atColumn, atDirection, offset - 2) == 2) {
                    return 1;
                }
            }
        }
        currentLine = atLine + (offset + 2) * verticalStep[atDirection];
        currentColumn = atColumn + (offset + 2) * horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuGridStateUnoccupied) {
            currentLine += verticalStep[atDirection];
            currentColumn += horizontalStep[atDirection];
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                if (detectBlackFour(self, atLine, atColumn, atDirection, offset) == 2) {
                    return 1;
                }
            }
        }
        return 0;
        break;
    case 1:
        currentLine = atLine + (offset - 1) * verticalStep[atDirection];
        currentColumn = atColumn + (offset - 1) * horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuGridStateUnoccupied) {
            currentLine -= verticalStep[atDirection];
            currentColumn -= horizontalStep[atDirection];
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                currentLine -= verticalStep[atDirection];
                currentColumn -= horizontalStep[atDirection];
                if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                    if (detectBlackFour(self, atLine, atColumn, atDirection, offset - 3) == 2) {
                        return 1;
                    }
                }
            }
        }
        currentLine = atLine + (offset + 1) * verticalStep[atDirection];
        currentColumn = atColumn + (offset + 1) * horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuGridStateUnoccupied) {
            currentLine += verticalStep[atDirection];
            currentColumn += horizontalStep[atDirection];
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                currentLine += verticalStep[atDirection];
                currentColumn += horizontalStep[atDirection];
                if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerBlack) {
                    if (detectBlackFour(self, atLine, atColumn, atDirection, offset) == 2) {
                        return 1;
                    }
                }
            }
        }
        return 0;
        break;
    default:
        return 0;
        break;
    }
}

void continuingAtDirections(GomokuState *self, int atLine, int atColumn, int party, int *numberAtEachDirection, int *startingOffsetToTheCurrentPositionOnEachDirection) {
    int maxLengths[8];
    int i, currentLine, currentColumn;

    memset(maxLengths, 0, sizeof(int)*8);

    for (i = 0; i < 8; i++) {
        currentLine = atLine;
        currentColumn = atColumn;
        while (currentLine > 0 && currentLine < 16 && currentColumn > 0 && currentColumn < 16 && stateAtPosition(self, currentLine, currentColumn) == party) {
            maxLengths[i]++;
            currentLine += verticalStep[i];
            currentColumn += horizontalStep[i];
        }
    }

    for (i = 0; i < 4; i++) {
        numberAtEachDirection[i] = maxLengths[i] + maxLengths[i + 4] - 1;
        startingOffsetToTheCurrentPositionOnEachDirection[i] = -maxLengths[i + 4] + 1;
    }
}

int boardIsFull(GomokuState *self) {
    int i, j, full;
    full = 1;

    for (i = 1; i < 16; i++) {
        for (j = 1; j < 16; j++) {
            if (stateAtPosition(self, i, j) == kGomokuGridStateUnoccupied) {
                full = 0;
            }
        }
    }

    return full;
}

int gameTerminated(GomokuState *self) {
    int numberAtDirections[4], offsetOfDirections[4];
    int i, countOfUndeadFour, countOfLiveThree;

    continuingAtDirections(self, self->recentMoveLine, self->recentMoveColumn, -self->nextMoveParty, numberAtDirections, offsetOfDirections);

    if (-self->nextMoveParty == kGomokuPlayerBlack) {
        /* detect double four here */
        countOfUndeadFour = 0;
        for (i = 0; i < 4; i++) {
            if (numberAtDirections[i] == 4) {
                if (detectBlackFour(self, self->recentMoveLine, self->recentMoveColumn, i, offsetOfDirections[i]) > 0) {
                    countOfUndeadFour++;
                }
            } else {
                countOfUndeadFour += detectBlackDiscontinuingFour(self, self->recentMoveLine, self->recentMoveColumn, i, numberAtDirections[i], offsetOfDirections[i]);
            }
        }
        if (countOfUndeadFour > 1) {
            return kGameTerminatedBlackForbiddenDoubleFour;
        }
        /* detect double three here */
        countOfLiveThree = 0;
        for (i = 0; i < 4; i++) {
            if (numberAtDirections[i] <= 3) {
                if (detectLiveThreeOfTheBlackPlayerOnDirections(self, self->recentMoveLine, self->recentMoveColumn, i, numberAtDirections[i], offsetOfDirections[i]) > 0) {
                    countOfLiveThree++;
                }
            }
        }
        if (countOfLiveThree > 1) {
            return kGameTerminatedBlackForbiddenDoubleThree;
        }
        /* detect overline here */
        for (i = 0; i < 4; i++) {
            if (numberAtDirections[i] > 5) {
                return kGameTerminatedBlackForbiddenOverline;
            }
        }
        /* N y a plus forbidden move */
        for (i = 0; i < 4; i++) {
            if (numberAtDirections[i] == 5) {
                return kGameTerminatedBlackWon;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (numberAtDirections[i] >= 5) {
                return kGameTerminatedWhiteContinuedFive;
            }
        }
    }

    if (boardIsFull(self)) {
        return kGameTerminatedDraw;
    }

    return kGameHasNotYetTerminated;
}
