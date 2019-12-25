#ifndef GOMOKU_TRANSPOSITION_TABLE_HEADER
#define GOMOKU_TRANSPOSITION_TABLE_HEADER

#include "Gomoku.h"

typedef struct {
    unsigned char *boardStates;
    float *score;
    unsigned char *depth;
    size_t capacity;
    unsigned char *occupied, *isNew;
} TranspositionTable;

#define HISTORICAL_STORAGE 23333

TranspositionTable *createTranspositionTable(size_t capacity);
void destroyTranspositionTable(TranspositionTable *self);

void resetTranspositionTable(TranspositionTable *self);

/* 0 on not found; 1 on found */
int lookupInTranspositionTable(TranspositionTable *self, const unsigned char *board, float *output);
void storeInTranspositionTable(TranspositionTable *self, const unsigned char *board, float score, unsigned char depth);
void storeInTranspositionTableWithRound(TranspositionTable *self, const unsigned char *board, float score, unsigned char depth, int round);

#endif
