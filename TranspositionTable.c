#include "TranspositionTable.h"

TranspositionTable *createTranspositionTable(size_t capacity) {
    TranspositionTable *self;
    self = (TranspositionTable *)malloc(sizeof(TranspositionTable));
    if (self == NULL) {
        return NULL;
    }

    self->capacity = capacity;

    self->boardStates = (unsigned char *)malloc(sizeof(unsigned char) * 225 * capacity);
    if (self->boardStates == NULL) {
        free(self);
        return NULL;
    }

    self->depth = (unsigned char *)malloc(sizeof(unsigned char) * capacity);
    if (self->depth == NULL) {
        free(self->boardStates);
        free(self);
        return NULL;
    }

    self->score = (float *)malloc(sizeof(float) * capacity);
    if (self->score == NULL) {
        free(self->boardStates);
        free(self->depth);
        free(self);
        return NULL;
    }

    self->occupied = (unsigned char *)malloc(sizeof(unsigned char) * capacity);
    if (self->occupied == NULL) {
        free(self->boardStates);
        free(self->depth);
        free(self->score);
        free(self);
        return NULL;
    }

    self->cacheHit = 0;
    self->cacheLookup = 0;

    return self;
}

void destroyTranspositionTable(TranspositionTable *self) {
    free(self->boardStates);
    free(self->depth);
    free(self->score);
    free(self);
}

void resetTranspositionTable(TranspositionTable *self) {
    memset(self->occupied, 0, sizeof(unsigned char) * self->capacity);
}

/* 32-bit PJW hash function for fixed length */
static unsigned long hashFunction(const unsigned char *str) {
    unsigned long hash = 0, high;
    int i;

    for (i = 0; i < 225; i++) {
        hash = (hash << 4) + str[i];
        if ((high = hash & 0xF0000000))
            hash ^= high >> 24;
        hash &= ~high;
    }

    return hash;
}

int lookupInTranspositionTable(TranspositionTable *self, const unsigned char *board, float *output) {
    unsigned long address = hashFunction(board) % self->capacity;

    self->cacheLookup++;

    if (!self->occupied[address]) {
        return 0;
    }
    if (memcmp(board, &(self->boardStates[225 * address]), sizeof(unsigned char) * 225) == 0) {
        *output = self->score[address];
        self->cacheHit++;
        return 1;
    }
    return 0;
}

void storeInTranspositionTable(TranspositionTable *self, const unsigned char *board, float score, unsigned char depth) {
    unsigned long address = hashFunction(board) % self->capacity;

    if (self->occupied[address] && self->depth[address] <= depth) {
        return;
    }

    memcpy(&(self->boardStates[225 * address]), board, sizeof(unsigned char) * 225);
    self->depth[address] = depth;
    self->occupied[address] = 1;
    self->score[address] = score;
}
