#ifndef GOMOKU_ALPHA_BETA_SEARCH_HEADER
#define GOMOKU_ALPHA_BETA_SEARCH_HEADER

#include "BackEnd.h"
#include "TranspositionTable.h"

typedef struct __________ALPHA_BETA_TREE_NODE {
    float value;
    GomokuState situation;
    struct __________ALPHA_BETA_TREE_NODE *children[225];
} AlphaBetaTreeNode;

typedef struct {
    AlphaBetaTreeNode *root;
    evaluationBasedOnCurrentStateOnly evaluate, fastEvaluate;
    int stepCount, pruned, issued;
    TranspositionTable *cache;
} AlphaBetaSupportingStructure;

AlphaBetaTreeNode *createAlphaBetaTreeWithState(GomokuState *self);
void destroyAlphaBetaEntireSubtree(AlphaBetaTreeNode *self);

AlphaBetaSupportingStructure *createAlphaBetaSupportingStructureWithState(GomokuState *self, evaluationBasedOnCurrentStateOnly heuristic, evaluationBasedOnCurrentStateOnly fastHeuristic);
void destroyAlphaBetaSupportingStructure(AlphaBetaSupportingStructure *self);

void alphaBeta(GomokuState *self, void *supporting);
void changeAlphaBetaSupportingStructure(AlphaBetaSupportingStructure *self, int opponentLine, int opponentColumn);
void alphaBetaStepCount(void *self);

void expandAlphaBetaTreeNode(AlphaBetaTreeNode *self, int index);

#endif