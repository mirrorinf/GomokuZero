#ifndef GOMOKU_ALPHA_BETA_SEARCH_HEADER
#define GOMOKU_ALPHA_BETA_SEARCH_HEADER

#include "BackEnd.h"

typedef struct __________ALPHA_BETA_TREE_NODE {
    float value;
    GomokuState situation;
    struct __________ALPHA_BETA_TREE_NODE *children[225];
} AlphaBetaTreeNode;

typedef struct {
    AlphaBetaTreeNode *root;
} AlphaBetaSupportingStructure;

AlphaBetaTreeNode *createTreeWithState(GomokuState *self);
void destroyAlphaBetaEntireSubtree(AlphaBetaTreeNode *self);

player alphaBeta;
void changeAlphaBetaSupportingStructure(AlphaBetaSupportingStructure *self, int opponentLine, int opponentColumn);

#endif