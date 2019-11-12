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
    created->whiteWin = 0;
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

    self->children[serialNumber(atLine, atColumn)] = nee;
    return 0;
}

MCTSNode *newRootNodeTransistedWithMove(MCTSNode *self, int atLine, int atColumn) {
    int index;
    MCTSNode *temp;
    index = serialNumber(atLine, atColumn);

    temp = self->children[index];
    self->children[index] = NULL;
    destroyEntireSubtree(self);

    return temp;
}
