#include "Gomoku.h"

GomokuState *initGomokuState() {
    GomokuState *self = (GomokuState *)malloc(sizeof(GomokuState));
    int i;

    for (i = 0; i < 57; i++) {
        self->board[i] = 0x55;
    }

    self->recentMoveLine = -1;
    self->recentMoveColumn = -1;
    self->nextMoveParty = -1;
    
    return self;
}

void destroyGomokuState(GomokuState *self) {
    free(self);
}

char stateAtPosition(GomokuState *self, int atLine, int atColumn) {
    int index = (atLine - 1) * 15 + (atColumn - 1);
    int position = index / 4;
    int offset = index % 4 * 2;
    return (int)(((0x3 << offset) & (self->board[position])) >> offset) - 1;
}

void changeState(GomokuState *self, int atLine, int atColumn, char toState) {
    int index = (atLine - 1) * 15 + (atColumn - 1);
    int position = index / 4;
    int offset = index % 4 * 2;
    self->board[position] |= 0x3 << offset;
    self->board[position] &= ~(( ((unsigned char) (toState + 1) ) ^ 0x3) << offset);
}

GomokuState *copyState(GomokuState *self) {
    GomokuState *newSelf;
    newSelf = (GomokuState *)malloc(sizeof(GomokuState));

    memcpy(newSelf->board, self->board, sizeof(unsigned char)*57);
    newSelf->recentMoveLine = self->recentMoveLine;
    newSelf->recentMoveColumn = self->recentMoveColumn;
    newSelf->nextMoveParty = self->nextMoveParty;

    return newSelf;
}

int serialNumber(int atLine, int atColumn) {
    return (atLine - 1) * 15 + (atColumn - 1);
}

int isInside(int x, int y) {
    return (x <= 15 && x >= 1) && (y <= 15 && y >= 1);
}

int isAdjecentToSituation(GomokuState *self, int line, int column) {
    const int horizontalStep[8] = {1, 1, 0, -1, -1, -1, 0, 1}; 
    const int verticalStep[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    int i = 0;
    for (i = 0; i < 8; i++) {
        if (isInside(line + verticalStep[i], column + horizontalStep[i]) && stateAtPosition(self, line + verticalStep[i], column + horizontalStep[i]) != kGomokuGridStateUnoccupied) {
            return 1;
        }
    }
    return 0;
}
