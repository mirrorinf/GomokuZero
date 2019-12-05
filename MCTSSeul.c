#include "MCTSSeul.h"
#include <math.h>

int serialNumber(int atLine, int atColumn) {
    return (atLine - 1) * 15 + (atColumn - 1);
}

MCTSNode *createRootNodeWithCurrentSituation(GomokuState *self) {
    GomokuState *copied;
    MCTSNode *created;

    copied = copyState(self);
    created = (MCTSNode *)malloc(sizeof(MCTSNode));
    created->count = 0;
    created->currentWin = 0;
    created->currentLose = 0;
    created->isLeaf = 0;
    created->valuationForCurrentPlayer = 0;
    created->current = copied;
    created->parent = NULL;
    created->currentMustWin = 0;
    created->currentMustLose = 0;
    memset(created->children, 0, sizeof(void *)*225);

    return created;
}

void destroyEntireSubtree(MCTSNode *root) {
    int i;
    for (i = 0; i < 225; i++) {
        if (root->children[i] != NULL) {
            destroyEntireSubtree(root->children[i]);
        }
    }
    destroyGomokuState(root->current);
    free(root);
}

/* no setting: isLeaf; valuation :property */
int expandTree(MCTSNode *self, int atLine, int atColumn) {
    MCTSNode *nee;
    if (stateAtPosition(self->current, atLine, atColumn) != kGomokuGridStateUnoccupied) {
        return -1;
    }
    nee = createRootNodeWithCurrentSituation(self->current);
    changeState(nee->current, atLine, atColumn, nee->current->nextMoveParty);
    nee->current->recentMoveLine = atLine;
    nee->current->recentMoveColumn = atColumn;
    nee->current->nextMoveParty *= -1;
    nee->isLeaf = 0;

    nee->currentWin = 0;
    nee->currentLose = 0;

    nee->parent = self;

    self->children[serialNumber(atLine, atColumn)] = nee;

    return 0;
}

MCTSNode *newRootNodeTransistedWithMove(MCTSNode *self, int atLine, int atColumn) {
    int index;
    MCTSNode *temp;

    index = serialNumber(atLine, atColumn);

    if (self->children[index] == NULL) {
        expandTree(self, atLine, atColumn);
    }

    temp = self->children[index];
    self->children[index] = NULL;
    destroyEntireSubtree(self);

    temp->parent = NULL;

    return temp;
}

void expandWithScoreBuffer(MCTSNode *self, float *score,  evaluationBasedOnCurrentStateOnly evaluate) {
    int line, column, index, temp;

    for (line = 1; line <= 15; line++) {
        for (column = 1; column <= 15; column++) {
            index = serialNumber(line, column);
            score[index] = -1000;
            if (self->children[index] != NULL) {
                score[index] = self->children[index]->valuationForCurrentPlayer / (float)self->children[index]->count;
                continue;
            }
            if (stateAtPosition(self->current, line, column) != kGomokuGridStateUnoccupied) {
                continue;
            }
            expandTree(self, line, column);
            temp = gameTerminated(self->children[index]->current) * self->current->nextMoveParty;
            score[index] = (float)temp;
            if (temp > -2000 && temp < 2000) {
                score[index] = evaluate(self->children[index]->current);
            } else {
                self->children[index]->isLeaf = 1;
                if (temp >= 2000) {
                    /*score[index] = 4;*/
                    self->children[index]->currentWin += 1;
                    self->children[index]->currentMustWin = 1;
                } else {
                    /*score[index] = 0;*/
                    self->children[index]->currentLose += 1;
                    self->children[index]->currentMustLose = 1;
                }
            }
            self->children[index]->count = 1;
            self->children[index]->valuationForCurrentPlayer = score[index];
        }
    }
}

void selectMaxMonteCarloDestroy(float *from, int *index, int originalCount, int toBeSelected) {
    int i, j;
    float min, r;

    min = 1e10;

    for (i = 0; i < originalCount; i++) {
        if (from[i] < 0) {
            from[i] = 0;
            continue;
        }
        if (min > from[i]) {
            min = from[i];
        }
    }
    min -= 0.001;
    for (i = 0; i < originalCount; i++) {
        if (from[i] > min) {
            from[i] -= min;
        }
    }

    for (i = 1; i < originalCount; i++) {
        from[i] += from[i - 1];
    }

    for (j = 0; j < toBeSelected; j++) {
        if (from[originalCount - 1] < 1e-6) {
            printf("Surrender!\n");
            exit(-1);
        }
        r = (float)rand() / (float)RAND_MAX * from[originalCount - 1];
        for (i = 0; i < originalCount; i++) {
            if (from[i] > r) {
                index[j] = i;
                r = from[i] - (i > 0 ? from[i - 1] : 0);
                break;
            }
        }
        for (i = index[j]; i < originalCount; i++) {
            from[i] -= r;
        }
    }
}

void selectMaxExact(float *from, int *index, int originalCount, int toBeSelected) {
    float max;
    int i, j, k, selected;

    for (i = 0; i < toBeSelected; i++) {
        max = -1e10;
        for (j = 0; j < originalCount; j++) {
            selected = 0;
            for (k = 0; k < i; k++) {
                if (j == index[k]) {
                    selected = 1;
                }
            }
            if (!selected) {
                if (from[j] > max) {
                    index[i] = j;
                    max = from[j];
                }
            }
        }
    }
}

void minimaxSelectNextMoveStupid(MCTSNode *self, int *atLine, int *atColumn, evaluationBasedOnCurrentStateOnly evaluate) {
    int temp, i;
    float score[225], max;
    
    expandWithScoreBuffer(self, score, evaluate);
    max = -100000;
    for (i = 0; i < 225; i++) {
        if (score[i] > max) {
            max = score[i];
            temp = i;
        }
    }
    *atLine = temp / 15 + 1;
    *atColumn = temp % 15 + 1;
}

void minimaxStupid(GomokuState *self, void *tree) {
    MCTSNode *root;
    int line, column;

    root = (MCTSNode *)tree;
    minimaxSelectNextMoveStupid(root, &line, &column, quickEvaluationForTheCurrentPlayer);

    changeState(self, line, column, self->nextMoveParty);
    self->recentMoveLine = line;
    self->recentMoveColumn = column;
    self->nextMoveParty *= -1;
}

void expandWithoutScoring(MCTSNode *self) {
    int line, column, index, temp;

    for (line = 1; line <= 15; line++) {
        for (column = 1; column <= 15; column++) {
            index = serialNumber(line, column);
            if (self->children[index] != NULL) {
                continue;
            }
            if (stateAtPosition(self->current, line, column) != kGomokuGridStateUnoccupied) {
                continue;
            }
            expandTree(self, line, column);
            temp = gameTerminated(self->children[index]->current) * self->current->nextMoveParty;
            if (!(temp > -2000 && temp < 2000)) {
                self->children[index]->isLeaf = 1;
                if (temp >= 2000) {
                    self->children[index]->currentWin += 1;
                    self->children[index]->currentMustWin = 1;
                } else {
                    self->children[index]->currentLose += 1;
                    self->children[index]->currentMustLose = 1;
                }
            }
            self->children[index]->count = 1;
        }
    }
}

#define MINIMAX_EXPAND_DEPTH 4
#define MINIMAX_EXPAND_WIDTH 5

float minimaxExpandEvaluate(MCTSNode *root, int depth, evaluationBasedOnCurrentStateOnly staticEvaluation) {
    float temp, choose;
    int i;

    float score[225];
    int chosen[MINIMAX_EXPAND_WIDTH];

    if (root->isLeaf) {
        if (root->currentMustWin) {
            return 4.0;
        } else if (root->currentMustLose) {
            return 0.0;
        } else {
            return 2.0;
        }
    }

    if (depth >= MINIMAX_EXPAND_DEPTH) {
        return staticEvaluation(root->current);
    }

    expandWithScoreBuffer(root, score, quickEvaluationForTheCurrentPlayer);

    selectMaxExact(score, chosen, 225, MINIMAX_EXPAND_WIDTH);

    choose = 4;
    for (i = 0; i < MINIMAX_EXPAND_WIDTH; i++) {
        if (root->children[chosen[i]] == NULL) {
            continue;
        }
        temp = 4.0 - minimaxExpandEvaluate(root->children[chosen[i]], depth + 1, staticEvaluation);
        if (temp < choose) {
            choose = temp;
        }
    }

    return choose;
}

#define FIRST_WIDTH 16

void minimaxTrue(GomokuState *self, void *tree) {
    float score[225], bests[FIRST_WIDTH];
    int i, selected, line, column, chosen[FIRST_WIDTH];
    MCTSNode *root;

    root = createRootNodeWithCurrentSituation(self);

    expandWithScoreBuffer(root, score, quickEvaluationForTheCurrentPlayer);
    selectMaxExact(score, chosen, 225, FIRST_WIDTH);

    for (i = 0; i < FIRST_WIDTH; i++) {
        if (root->children[chosen[i]] == NULL) {
            bests[i] = -1e5;
            continue;
        }
        bests[i] = minimaxExpandEvaluate(root->children[chosen[i]], 0, quickEvaluationForTheCurrentPlayer);
    }

    selectMaxExact(score, &selected, FIRST_WIDTH, 1);

    line = chosen[selected] / 15 + 1;
    column = chosen[selected] % 15 + 1;

    changeState(self, line, column, self->nextMoveParty);
    self->recentMoveLine = line;
    self->recentMoveColumn = column;
    self->nextMoveParty *= -1;

    destroyEntireSubtree(root);
}
