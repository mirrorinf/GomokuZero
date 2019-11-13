#ifndef MCTS_SEUL_HEADER
#define MCTS_SEUL_HEADER

#include "BackEnd.h"
#define MAX_NODES 100000
#define ROLLOUT_COUNT 1000

/* Ne pas penser de la performance dans cette implementation */
typedef struct __MCTS_Node {
    GomokuState *current;
    struct __MCTS_Node *children[225];
    struct __MCTS_Node *parent;
    int count, currentWin, isLeaf;
    float valuationForCurrentPlayer;
} MCTSNode;

MCTSNode *createRootNodeWithCurrentSituation(GomokuState *self);
void destroyEntireSubtree(MCTSNode *root);

/* -1 on already occupied; do not detect for alredy expanded */
int expandTree(MCTSNode *self, int atLine, int atColumn, evaluationBasedOnCurrentStateOnly evaluate);
/* if the position in children is NULL fatal error */
MCTSNode *newRootNodeTransistedWithMove(MCTSNode *self, int atLine, int atColumn);

void rolloutAndFeedback(MCTSNode *self, evaluationBasedOnCurrentStateOnly evaluate);
void minimaxStupid(GomokuState *self, void *tree);

#endif