#include "AlphaBeta.h"
#include <assert.h>

AlphaBetaTreeNode *createAlphaBetaTreeWithState(GomokuState *self) {
    AlphaBetaTreeNode *newSelf;
    int i;

    newSelf = (AlphaBetaTreeNode *)malloc(sizeof(AlphaBetaTreeNode));
    memcpy(&(newSelf->situation), self, sizeof(GomokuState));
    newSelf->value = 50;

    for (i = 0; i < 225; i++) {
        newSelf->children[i] = NULL;
    }

    return newSelf;
}

void destroyAlphaBetaEntireSubtree(AlphaBetaTreeNode *self) {
    int i;

    for (i = 0; i < 225; i++) {
        if (self->children[i] == NULL) {
            continue;
        }
        destroyAlphaBetaEntireSubtree(self->children[i]);
    }

    free(self);
}

AlphaBetaSupportingStructure *createAlphaBetaSupportingStructureWithState(GomokuState *self, evaluationBasedOnCurrentStateOnly heuristic) {
    AlphaBetaSupportingStructure *newSelf;

    newSelf = (AlphaBetaSupportingStructure *)malloc(sizeof(AlphaBetaSupportingStructure));
    newSelf->root = createAlphaBetaTreeWithState(self);
    newSelf->evaluate = heuristic;
    newSelf->stepCount = 0;

    return newSelf;
}

void destroyAlphaBetaSupportingStructure(AlphaBetaSupportingStructure *self) {
    destroyAlphaBetaEntireSubtree(self->root);
    free(self);
}

void changeAlphaBetaSupportingStructure(AlphaBetaSupportingStructure *self, int opponentLine, int opponentColumn) {
    int index;
    AlphaBetaTreeNode *temp;

    index = serialNumber(opponentLine, opponentColumn);

    if (self->root->children[index] == NULL) {
        expandAlphaBetaTreeNode(self->root, index);
    }

    temp = self->root;
    self->root = self->root->children[index];
    temp->children[index] = NULL;
    destroyAlphaBetaEntireSubtree(temp);
}

void expandAlphaBetaTreeNode(AlphaBetaTreeNode *self, int index) {
    if (self->children[index] != NULL) {
        return;
    }
    self->children[index] = createAlphaBetaTreeWithState(&(self->situation));
    assert(self->children[index] != NULL);
    changeState(&(self->children[index]->situation), index / 15 + 1, index % 15 + 1, self->children[index]->situation.nextMoveParty);
    self->children[index]->situation.recentMoveLine = index / 15 + 1;
    self->children[index]->situation.recentMoveColumn = index % 15 + 1;
    self->children[index]->situation.nextMoveParty *= -1;
}

float alphaBetaMinimax(AlphaBetaSupportingStructure *environment, AlphaBetaTreeNode *node, int depth, int isMaxPlayer, float alpha, float beta) {
    int terminal, i;
    float best, temp;

    terminal = gameTerminated(&(node->situation));
    if (terminal > 2000) {
        return 100;
    } else if (terminal < -2000) {
        return 0;
    }

    if (depth <= 0) {
        return environment->evaluate(&(node->situation));
    }

    if (isMaxPlayer) {
        best = 0;
        for (i = 0; i < 225; i++) {
            if (stateAtPosition(&(node->situation), i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(&(node->situation), i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(node, i);
            temp = alphaBetaMinimax(environment, node->children[i], depth - 1, !isMaxPlayer, alpha, beta);
            best = best > temp ? best : temp;
            alpha = alpha > best ? alpha : best;
            if (beta <= alpha) {
                break;
            }
        }
    } else {
        best = 100;
        for (i = 0; i < 225; i++) {
            if (stateAtPosition(&(node->situation), i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(&(node->situation), i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(node, i);
            temp = alphaBetaMinimax(environment, node->children[i], depth - 1, !isMaxPlayer, alpha, beta);
            best = best < temp ? best : temp;
            beta = beta < best ? beta : best;
            if (beta <= alpha) {
                break;
            }
        }
    }

    return best;
}

void alphaBeta(GomokuState *self, void *supporting) {
    AlphaBetaSupportingStructure *environment = (AlphaBetaSupportingStructure *)supporting;
    int i, bestIndex, thisStep, terminal;
    float temp, best;
    AlphaBetaTreeNode *root;
    const int step[8] = {1, 59, 4, 97, 101, 141, 17, 223};

    if (environment->stepCount == 0) {
        changeState(self, 8, 8, self->nextMoveParty);
        self->recentMoveLine = 8;
        self->recentMoveColumn = 8;
        self->nextMoveParty *= -1;
        return;
    }

    root = createAlphaBetaTreeWithState(self);

    if (self->nextMoveParty == kGomokuPlayerWhite) {
        best = 0;
        thisStep = step[rand() % 8];
        for (i = thisStep; i != 0; i = (i + thisStep) % 225) {
            if (stateAtPosition(self, i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(self, i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(root, i);
            assert(&(root->children[i]) != NULL);
            terminal = gameTerminated(&(root->children[i]->situation));
            if (terminal > 2000) {
                bestIndex = i;
                break;
            } else if (terminal < -2000) {
                continue;
            }
            temp = alphaBetaMinimax(environment, root->children[i], 3, 0, 0, 100);
            destroyAlphaBetaEntireSubtree(root->children[i]);
            root->children[i] = NULL;
            if (temp > best) {
                best = temp;
                bestIndex = i;
            }
        }
    } else {
        best = 100;
        thisStep = step[rand() % 8];
        for (i = thisStep; i != 0; i = (i + thisStep) % 225) {
            if (stateAtPosition(self, i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(self, i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(root, i);
            assert(&(root->children[i]) != NULL);
            terminal = gameTerminated(&(root->children[i]->situation));
            if (terminal < -2000) {
                bestIndex = i;
                break;
            } else if (terminal > 2000) {
                continue;
            }
            temp = alphaBetaMinimax(environment, root->children[i], 3, 1, 0, 100);
            destroyAlphaBetaEntireSubtree(root->children[i]);
            root->children[i] = NULL;
            if (temp < best) {
                best = temp;
                bestIndex = i;
            }
        }
    }

    changeState(self, bestIndex / 15 + 1, bestIndex % 15 + 1, self->nextMoveParty);
    self->recentMoveLine = bestIndex / 15 + 1;
    self->recentMoveColumn = bestIndex % 15 + 1;
    self->nextMoveParty *= -1;

    destroyAlphaBetaEntireSubtree(root);
}
