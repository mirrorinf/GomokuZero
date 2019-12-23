#include "AlphaBeta.h"
#include <assert.h>
#include "timing.h"

typedef int (*compareFunction)(int, int, void *);

typedef struct {
    int depth, decision;
} IDSearchDescriptor;

static int maximizingCompare(int x, int y, void *scoreBuffer) {
    float *score = (float *)scoreBuffer;
    return score[x] > score[y];
}

static int minimizingCompare(int x, int y, void *scoreBuffer) {
    float *score = (float *)scoreBuffer;
    return score[x] < score[y];
}

static void quicksortShit(int *array, int left, int right, compareFunction compare, void *compareFunctionAuxiliaire) {
    int i, j, pivot, temp;
    if (left >= right) {
        return;
    }

    pivot = array[right];
    i = left;
    for (j = left; j < right; j++) {
        if (compare(array[j], pivot, compareFunctionAuxiliaire)) {
            temp = array[j];
            array[j] = array[i];
            array[i] = temp;
            i++;
        }
    }
    temp = array[right];
    array[right] = array[i];
    array[i] = temp;

    quicksortShit(array, left, i - 1, compare, compareFunctionAuxiliaire);
    quicksortShit(array, i + 1, right, compare, compareFunctionAuxiliaire);
}

float alphaBetaMinimaxIDS(AlphaBetaSupportingStructure *environment, AlphaBetaTreeNode *node, int depth, int isMaxPlayer, float alpha, float beta, int round, unsigned long timeBound) {
    int terminal, i, indices[225], count = 0;
    float best, temp, buffer[225];

    environment->issued++;

    if (currentSystemTime() > timeBound) {
        environment->isCut = 1;
        return 50;
    }

    if (lookupInTranspositionTable(environment->cache, (node->situation).board, &best) == round) {
        return best;
    }

    terminal = gameTerminated(&(node->situation));
    if (terminal > 2000) {
        return 102;
    } else if (terminal < -2000) {
        return -2;
    }

    if (depth <= 0) {
        temp = environment->evaluate(&(node->situation));
        storeInTranspositionTableWithRound(environment->cache, (node->situation).board, temp, depth, round);
        return temp;
    }

    if (isMaxPlayer) {
        best = -1;
        for (i = 0; i < 225; i++) {
            if (stateAtPosition(&(node->situation), i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(&(node->situation), i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(node, i);
            if (lookupInTranspositionTable(environment->cache, node->children[i]->situation.board, &temp)) {
                buffer[i] = temp;
            }else {
                buffer[i] = environment->fastEvaluate(&(node->children[i]->situation));
            }
            indices[count] = i;
            count++;
        }
        quicksortShit(indices, 0, count - 1, maximizingCompare, buffer);
        assert(count <= 1 || buffer[indices[0]] >= buffer[indices[1]]);
        for (i = 0; i < count; i++) {
            temp = alphaBetaMinimaxIDS(environment, node->children[indices[i]], depth - 1, !isMaxPlayer, alpha, beta, round, timeBound);
            best = best > temp ? best : temp;
            alpha = alpha > best ? alpha : best;
            if (beta <= alpha) {
                break;
            }
        }
    } else {
        best = 101;
        for (i = 0; i < 225; i++) {
            if (stateAtPosition(&(node->situation), i / 15 + 1, i % 15 + 1) != kGomokuGridStateUnoccupied) {
                continue;
            }
            if (!isAdjecentToSituation(&(node->situation), i / 15 + 1, i % 15 + 1)) {
                continue;
            }
            expandAlphaBetaTreeNode(node, i);
            if (lookupInTranspositionTable(environment->cache, node->children[i]->situation.board, &temp)) {
                buffer[i] = temp;
            }else {
                buffer[i] = environment->fastEvaluate(&(node->children[i]->situation));
            }
            indices[count] = i;
            count++;
        }
        quicksortShit(indices, 0, count - 1, minimizingCompare, buffer);
        assert(count <= 1 || buffer[indices[0]] <= buffer[indices[1]]);
        for (i = 0; i < count; i++) {
            temp = alphaBetaMinimaxIDS(environment, node->children[indices[i]], depth - 1, !isMaxPlayer, alpha, beta, round, timeBound);
            best = best < temp ? best : temp;
            beta = beta < best ? beta : best;
            if (beta <= alpha) {
                break;
            }
        }
    }

    storeInTranspositionTableWithRound(environment->cache, (unsigned char *)&((node->situation).board), best, depth, round);

    return best;
}

/* depth 3 or 4 ? */
int alphaBetaIDS(GomokuState *self, AlphaBetaSupportingStructure *environment, int depth, unsigned long timeBound) {
    int i, bestIndex, thisStep, terminal, forcedChoice = 0, count = 0, indices[225];
    float temp, best, alpha, beta, buffer[225];
    AlphaBetaTreeNode *root;

    root = createAlphaBetaTreeWithState(self);
    alpha = 0;
    beta = 100;

    bestIndex = -1;

    if (self->nextMoveParty == kGomokuPlayerWhite) {
        best = -1;
        for (i = 0; i < 225; i++) {
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
                environment->winingMove = 1;
                return i;
            } else if (terminal < -2000) {
                continue;
            }
            if (lookupInTranspositionTable(environment->cache, root->children[i]->situation.board, &temp)) {
                buffer[i] = temp;
            }else {
                buffer[i] = environment->fastEvaluate(&(root->children[i]->situation));
            }
            indices[count] = i;
            count++;
        }
        if (!forcedChoice) {
            quicksortShit(indices, 0, count - 1, maximizingCompare, buffer);
            for (i = 0; i < count; i++) {
                temp = alphaBetaMinimaxIDS(environment, root->children[indices[i]], depth, 0, alpha, beta, depth, timeBound);
                destroyAlphaBetaEntireSubtree(root->children[indices[i]]);
                root->children[indices[i]] = NULL;
                if (temp > best) {
                    best = temp;
                    bestIndex = indices[i];
                }
                alpha = alpha > best ? alpha : best;
                if (beta <= alpha) {
                    break;
                }
            }
        }
    } else {
        best = 101;
        for (i = 0; i < 225; i++) {
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
                environment->winingMove = 1;
                return i;
            } else if (terminal > 2000) {
                continue;
            }
            if (lookupInTranspositionTable(environment->cache, root->children[i]->situation.board, &temp)) {
                buffer[i] = temp;
            }else {
                buffer[i] = environment->fastEvaluate(&(root->children[i]->situation));
            }
            indices[count] = i;
            count++;
        }
        if (!forcedChoice) {
            quicksortShit(indices, 0, count - 1, minimizingCompare, buffer);
            for (i = 0; i < count; i++) {
                temp = alphaBetaMinimaxIDS(environment, root->children[indices[i]], depth, 1, alpha, beta, depth, timeBound);
                destroyAlphaBetaEntireSubtree(root->children[indices[i]]);
                root->children[indices[i]] = NULL;
                if (temp < best) {
                    best = temp;
                    bestIndex = indices[i];
                }
                beta = beta < best ? beta : best;
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }

    destroyAlphaBetaEntireSubtree(root);

    return bestIndex;
}

void iterativeDeepening(GomokuState *self, void *supporting) {
    unsigned long timeBound;
    AlphaBetaSupportingStructure *environment = (AlphaBetaSupportingStructure *)supporting;
    int i, best, last, chosen;

    initializeTimingSystem();
    timeBound = currentSystemTime() + convertToRepresentation(14500);

    environment->strategyLevel = -1;
    resetTranspositionTable(environment->cache);
    environment->winingMove = 0;

    if (environment->stepCount == 0) {
        changeState(self, 8, 8, self->nextMoveParty);
        self->recentMoveLine = 8;
        self->recentMoveColumn = 8;
        self->nextMoveParty *= -1;
        return;
    }

    best = last = chosen = -1;
    for (i = 3; (!(environment->winingMove)) && (currentSystemTime() < timeBound); i++) {
        last = best;
        best = alphaBetaIDS(self, environment, i, timeBound);
    }

    if (environment->isCut) {
        chosen = last;
        environment->strategyLevel = i - 1;
    } else {
        chosen = best;
        environment->strategyLevel = i;
    }

    if (environment->winingMove) {
        chosen = best;
        environment->strategyLevel = -1;
    }

    if (chosen == -1) {
        printf("No possible move. Surrender!\n");
        exit(1);
    }

    changeState(self, chosen / 15 + 1, chosen % 15 + 1, self->nextMoveParty);
    self->recentMoveLine = chosen / 15 + 1;
    self->recentMoveColumn = chosen % 15 + 1;
    self->nextMoveParty *= -1;
}

