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

#define MINIMAX_WIDTH 3

void expandAtSingleNodeForDecisionWidth1(MCTSNode *self, int depth, evaluationBasedOnCurrentStateOnly evaluate) {
    float score[225];
    int i, maxIndex;

    if (depth <= 0) {
        return;
    }
    if (self->isLeaf) {
        return;
    }

    expandWithScoreBuffer(self, score, evaluate);
    selectMaxExact(score, &maxIndex, 225, 1);

    expandAtSingleNodeForDecisionWidth1(self->children[maxIndex], depth - 1, evaluate);
    self->count += self->children[maxIndex]->count;
    self->currentLose += self->children[maxIndex]->currentWin;
    self->currentWin += self->children[maxIndex]->currentLose;
}

void expandAtSingleNodeForDecision(MCTSNode *self, int depth, evaluationBasedOnCurrentStateOnly evaluate) {
    float score[225];
    int i, maxIndex[MINIMAX_WIDTH], wining, losing;

    if (depth <= 0) {
        return;
    }
    if (self->isLeaf) {
        return;
    }

    wining = 1;
    losing = 0;

    expandWithScoreBuffer(self, score, evaluate);
    selectMaxExact(score, maxIndex, 225, MINIMAX_WIDTH);

    for (i = 0; i < MINIMAX_WIDTH; i++) {
        expandAtSingleNodeForDecision(self->children[maxIndex[i]], depth - 1, evaluate);
        self->count += self->children[maxIndex[i]]->count;
        self->currentLose += self->children[maxIndex[i]]->currentWin;
        self->currentWin += self->children[maxIndex[i]]->currentLose;
        if (self->children[maxIndex[i]]->currentMustWin) {
            losing = 1;
        }
        if (!self->children[maxIndex[i]]->currentMustLose) {
            wining = 0;
        } 
    }
    self->currentMustLose = losing;
    self->currentMustWin = wining;
}

void minimaxSelectNextMove(MCTSNode *self, int *atLine, int *atColumn, evaluationBasedOnCurrentStateOnly evaluate) {
    MCTSNode *current;
    int i, selected, s, temp, tempf;
    float score[225];
    FILE *minimaxLog;

    minimaxLog = fopen("minimaxSelectNextMove.log", "a");

    if (self->isLeaf) {
        fprintf(stderr, "minimaxSelectNextMove:atLine:atColumn:evaluate: Reached leaf. Must be bug in sequence.\n");
        exit(-1);
    }

    expandAtSingleNodeForDecision(self, 5, evaluate);

    for (i = 0; i < 225; i++) {
        if (self->children[i] == NULL) {
            score[i] = -3000;
        } else {
            if (self->children[i]->isLeaf && self->children[i]->currentWin == 1) {
                selected = i;
                break;
            }
            if (self->children[i]->currentMustWin) {
                selected = i;
                break;
            }
            if (self->children[i]->currentMustLose) {
                score[i] = -3000;
                continue;
            }
            if (self->children[i]->count < 2) {
                score[i] = -3000;
                continue;
            }
            temp = self->children[i]->currentWin > 0 ? self->children[i]->currentWin : 1;
            tempf = self->children[i]->currentLose > 0 ? self->children[i]->currentLose : 1;
            score[i] = self->children[i]->valuationForCurrentPlayer + 0.7 * sqrtf(logf(temp) / logf(self->children[i]->count)) - 0.7 * sqrtf(logf(tempf) / logf(self->children[i]->count));
            fprintf(minimaxLog, "%c%d %f %d %d %d\n", i % 15 + 'A', i / 15 + 1, score[i], self->children[i]->currentLose, self->children[i]->currentWin, self->children[i]->count);
            fflush(minimaxLog);
        }
    }
    fclose(minimaxLog);

    selectMaxExact(score, &selected, 225, 1);
    *atLine = selected / 15 + 1;
    *atColumn = selected % 15 + 1;
}

void minimax(GomokuState *self, void *tree) {
    MCTSNode *root;
    int line, column;
    FILE *minimaxLog;

    minimaxLog = fopen("minimaxSelectNextMove.log", "a");
    fprintf(minimaxLog, "minimax:: start round\n");
    fclose(minimaxLog);

    root = (MCTSNode *)tree;
    minimaxSelectNextMove(root, &line, &column, quickEvaluationForTheCurrentPlayer);

    if (stateAtPosition(self, line, column) != kGomokuGridStateUnoccupied) {
        printf("Surrender!\n");
        exit(-1);
    }

    changeState(self, line, column, self->nextMoveParty);
    self->recentMoveLine = line;
    self->recentMoveColumn = column;
    self->nextMoveParty *= -1;
}
