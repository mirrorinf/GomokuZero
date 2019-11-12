//
//  GomokuPublic.c
//  Gomoku
//
//  Created by ChuXiaoMin on 2019/9/20.
//  Copyright © 2019 ChuXiaoMin. All rights reserved.
//

#include "Gomoku.h"

GomokuState *initGomokuState() {
    GomokuState *self = (GomokuState *)malloc(sizeof(GomokuState));

    self->grid = (char *)malloc(sizeof(char)*15*15);
    memset(self->grid, 0, sizeof(char)*15*15);

    self->recentMoveLine = -1;
    self->recentMoveColumn = -1;
    self->nextMoveParty = -1;
    
    return self;
}

void destroyGomokuState(GomokuState *self) {
    free(self->grid);
    free(self);
}

char stateAtPosition(GomokuState *self, int atLine, int atColumn) {
    return self->grid[(atLine - 1) * 15 + (atColumn - 1)];
}

void changeState(GomokuState *self, int atLine, int atColumn, char toState) {
    self->grid[(atLine - 1) * 15 + (atColumn - 1)] = toState;
}

GomokuState *copyState(GomokuState *self) {
    GomokuState *newSelf;
    newSelf = (GomokuState *)malloc(sizeof(GomokuState));
    newSelf->grid = (char *)malloc(sizeof(char)*15*15);

    memcpy(newSelf->grid, self->grid, sizeof(char)*15*15);
    newSelf->recentMoveLine = self->recentMoveLine;
    newSelf->recentMoveColumn = self->recentMoveColumn;
    newSelf->nextMoveParty = self->nextMoveParty;

    return newSelf;
}
