#include "MCTSSeul.h"

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
    created->isLeaf = 0;
    created->valuationForCurrentPlayer = 0;
    created->current = copied;
    created->parent = NULL;
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

int expandTree(MCTSNode *self, int atLine, int atColumn, evaluationBasedOnCurrentStateOnly evaluate) {
    MCTSNode *nee;
    if (stateAtPosition(self->current, atLine, atColumn) != kGomokuGridStateUnoccupied) {
        return -1;
    }
    nee = createRootNodeWithCurrentSituation(self->current);
    changeState(nee->current, atLine, atColumn, nee->current->nextMoveParty);
    nee->current->recentMoveLine = atLine;
    nee->current->recentMoveColumn = atColumn;
    nee->current->nextMoveParty *= -1;

    nee->parent = self;

    //nee->valuationForCurrentPlayer = evaluate(nee->current);

    self->children[serialNumber(atLine, atColumn)] = nee;
    return 0;
}

MCTSNode *newRootNodeTransistedWithMove(MCTSNode *self, int atLine, int atColumn) {
    int index;
    MCTSNode *temp;

    index = serialNumber(atLine, atColumn);

    if (self->children[index] == NULL) {
        expandTree(self, atLine, atColumn, quickEvaluationForTheCurrentPlayer);
    }

    temp = self->children[index];
    self->children[index] = NULL;
    destroyEntireSubtree(self);

    temp->parent = NULL;

    return temp;
}

void rolloutAndFeedback(MCTSNode *self, evaluationBasedOnCurrentStateOnly evaluate) {
    float scores[225], select;
    int i, j, round, c;
    GomokuState *temp;
    MCTSNode *loop;
    FILE *log;
    log = fopen("gomoku.log", "a");

    c = 0;
    temp = copyState(self->current);
    while ((round = gameTerminated(temp)) == 0) {
        c++;
        for (i = 0; i < 225; i++) {
            scores[i] = 0;
        }
        temp->nextMoveParty *= -1;
        for (i = 1; i <= 15; i++) {
            for (j = 1; j <= 15; j++) {
                if (stateAtPosition(temp, i, j) != kGomokuGridStateUnoccupied) {
                    continue;
                }
                changeState(temp, i, j, -temp->nextMoveParty);
                temp->recentMoveLine = i;
                temp->recentMoveColumn = j;
                scores[serialNumber(i, j)] = evaluate(temp) + 0.005;
                changeState(temp, i, j, kGomokuGridStateUnoccupied);
            }
        }

        for (i = 1; i < 225; i++) {
            scores[i] += scores[i - 1];
        }

        select = (float)random() / INT32_MAX * scores[224];
        for (i = 0; i < 225; i++) {
            if (scores[i] > select) {
                j = i;
                break;
            }
        }

        changeState(temp, j / 15 + 1, j % 15 + 1, temp->nextMoveParty);
        temp->recentMoveLine = j / 15 + 1;
        temp->recentMoveColumn = j % 15 + 1;
    }

    fprintf(log, "rounds: %d\n", c);
    fflush(log);

    loop = self;
    while (1) {
        loop->count++;
        if (-loop->current->nextMoveParty == kGomokuPlayerBlack) {
            if (round < 0) {
                loop->currentWin++;
            }
        } else {
            if (round > 2000) {
                loop->currentWin++;
            }
        }
        if (loop->parent == NULL) {
            break;
        } else {
            loop = loop->parent;
        }
    }

    destroyGomokuState(temp);
}

void minimaxSelectNextMove(MCTSNode *self, int *atLine, int *atColumn, evaluationBasedOnCurrentStateOnly evaluate) {
    int line, column, temp, index;
    float score[225], max;
    for (line = 1; line <= 15; line++) {
        for (column = 1; column <= 15; column++) {
            index = serialNumber(line, column);
            score[index] = 0;
            if (self->children[index] != NULL) {
                continue;
            }
            if (stateAtPosition(self->current, line, column) != kGomokuGridStateUnoccupied) {
                continue;
            }
            expandTree(self, line, column, evaluate);
            temp = gameTerminated(self->children[index]->current) * self->current->nextMoveParty;
            score[index] = (float)temp;
            if (temp > -2000 && temp < 2000) {
                score[index] = evaluate(self->children[index]->current);
            }
        }
    }
    max = -1000;
    for (line = 1; line <= 15; line++) {
        for (column = 1; column <= 15; column++) {
            index = serialNumber(line, column);
            if (score[index] > max) {
                max = score[index];
                *atLine = line;
                *atColumn = column;
            }
        }
    }
}

void minimaxStupid(GomokuState *self, void *tree) {
    MCTSNode *root;
    int line, column;

    root = (MCTSNode *)tree;
    minimaxSelectNextMove(root, &line, &column, quickEvaluationForTheCurrentPlayer);

    changeState(self, line, column, self->nextMoveParty);
    self->recentMoveLine = line;
    self->recentMoveColumn = column;
    self->nextMoveParty *= -1;
}
