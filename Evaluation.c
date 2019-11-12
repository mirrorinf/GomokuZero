#include "BackEnd.h"

/*  There are diffrences in detecting black against white shapes mainly due to forbidden overline
    Split into dedicated functions to reduce the size of a single function 
    This approach increases code length but improves reduces developing times 
    These evaluation functions are going to be deprecated when CNN-based evaluation completes
    So it is not worth while changing gameover judging code for its sake */
int yA_t_ilContinuationWhiteBeginningBySpace(GomokuState *self, int atLine, int atColumn, int atDirection, int consistsOf) {
    int i, line, column;

    line = atLine + verticalStep[atDirection];
    column = atColumn + horizontalStep[atDirection];
    if (!isInside(line, column) || stateAtPosition(self, line, column) != kGomokuGridStateUnoccupied) {
        return 0;
    }
    for (i = 0; i < consistsOf; i++) {
        line += verticalStep[atDirection];
        column += horizontalStep[atDirection];
        if (!isInside(line, column) || stateAtPosition(self, line, column) != kGomokuPlayerWhite) {
            return 0;
        }
    }

    return 1;
}

int detectWhiteFour(GomokuState *self, int atLine, int atColumn, int atDirection, int offset) {
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
    }

    currentLine = atLine + left * verticalStep[atDirection + 4];
    currentColumn = atColumn + left * horizontalStep[atDirection + 4];
    if (!isInside(currentLine, currentColumn) || stateAtPosition(self, currentLine, currentColumn) != kGomokuGridStateUnoccupied) {
        leftObstructed = 1;
    }

    return 2 - leftObstructed - rightObstructed;
}

int detectWhiteDiscontinuingFour(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset) {
    int currentLine, currentColumn, result, i;
    result = 0;

    for (i = 1; i < 4; i++) {
        if (i == continuing) {
            currentLine = atLine;
            currentColumn = atColumn;
            currentLine += offset * verticalStep[atDirection];
            currentColumn += offset * horizontalStep[atDirection];
            result += yA_t_ilContinuationWhiteBeginningBySpace(self, currentLine, currentColumn, atDirection + 4, 4 - i);
            currentLine = atLine;
            currentColumn = atColumn;
            currentLine += (offset + i - 1) * verticalStep[atDirection];
            currentColumn += (offset + i - 1) * horizontalStep[atDirection];
            result += yA_t_ilContinuationWhiteBeginningBySpace(self, currentLine, currentColumn, atDirection, 4 - i);
        }
    }

    return result;
}

int detectLiveThreeOfTheWhitePlayerOnDirections(GomokuState *self, int atLine, int atColumn, int atDirection, int continuing, int offset) {
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
        if (detectWhiteFour(self, atLine, atColumn, atDirection, offset - 1) == 2 || detectWhiteFour(self, atLine, atColumn, atDirection, offset) == 2) {
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
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                if (detectWhiteFour(self, atLine, atColumn, atDirection, offset - 2) == 2) {
                    return 1;
                }
            }
        }
        currentLine = atLine + (offset + 2) * verticalStep[atDirection];
        currentColumn = atColumn + (offset + 2) * horizontalStep[atDirection];
        if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuGridStateUnoccupied) {
            currentLine += verticalStep[atDirection];
            currentColumn += horizontalStep[atDirection];
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                if (detectWhiteFour(self, atLine, atColumn, atDirection, offset) == 2) {
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
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                currentLine -= verticalStep[atDirection];
                currentColumn -= horizontalStep[atDirection];
                if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                    if (detectWhiteFour(self, atLine, atColumn, atDirection, offset - 3) == 2) {
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
            if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                currentLine += verticalStep[atDirection];
                currentColumn += horizontalStep[atDirection];
                if (isInside(currentLine, currentColumn) && stateAtPosition(self, currentLine, currentColumn) == kGomokuPlayerWhite) {
                    if (detectWhiteFour(self, atLine, atColumn, atDirection, offset) == 2) {
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

/* stupid? ça va */
float quickEvaluationForTheCurrentPlayer(GomokuState *self) {
    int result, tempNumber[4], tempOffset[4];
    int i, j, r, s;
    result = 0;

    for (i = 0; i < 225; i++) {
        r = i / 15 + 1;
        s = i % 15 + 1;
        continuingAtDirections(self, r, s, kGomokuPlayerWhite, tempNumber, tempOffset);
        for (j = 0; j < 4; j++) {
            if (tempNumber[j] == 4) {
                result += detectWhiteFour(self, r, s, j, tempOffset[j]) * 5;           
            } else {
                result += detectWhiteDiscontinuingFour(self, r, s, j, tempNumber[j], tempOffset[j]) * 5;
                result += detectLiveThreeOfTheWhitePlayerOnDirections(self, r, s, j, tempNumber[j], tempOffset[j]) * 8;
            }
        }
        continuingAtDirections(self, r, s, kGomokuPlayerBlack, tempNumber, tempOffset);
        for (j = 0; j < 4; j++) {
            if (tempNumber[j] == 4) {
                result -= detectBlackFour(self, r, s, j, tempOffset[j]) * 5;           
            } else {
                result -= detectBlackDiscontinuingFour(self, r, s, j, tempNumber[j], tempOffset[j]) * 5;
                result -= detectLiveThreeOfTheBlackPlayerOnDirections(self, r, s, j, tempNumber[j], tempOffset[j]) * 8;
            }
        }
    }

    return -1.0/500 * result * self->nextMoveParty;
}