#include "TranspositionTable.h"

TranspositionTable *createTranspositionTable(size_t capacity) {
    TranspositionTable *self;
    self = (TranspositionTable *)malloc(sizeof(TranspositionTable));
    if (self == NULL) {
        return NULL;
    }

    self->capacity = capacity;

    self->boardStates = (unsigned char *)malloc(sizeof(unsigned char) * 57 * capacity);
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

    self->occupied = (unsigned char *)malloc(sizeof(unsigned char *) * capacity);
    if (self->occupied == NULL) {
        free(self->boardStates);
        free(self->depth);
        free(self->score);
        free(self);
        return NULL;
    }
    memset(self->occupied, 0, sizeof(unsigned char) * capacity);

    return self;
}

void destroyTranspositionTable(TranspositionTable *self) {
    free(self->boardStates);
    free(self->depth);
    free(self->score);
    free(self);
}

/* 32-bit PJW hash function for fixed length */
static unsigned long hashFunction(const unsigned char *str) {
    unsigned long hash = 0, high;
    int i;

    for (i = 0; i < 57; i++) {
        hash = (hash << 4) + str[i];
        if ((high = hash & 0xF0000000))
            hash ^= high >> 24;
        hash &= ~high;
    }

    return hash;
}

int lookupInTranspositionTable(TranspositionTable *self, const unsigned char *board, float *output) {
    unsigned long address = hashFunction(board) % self->capacity;

    if (!self->occupied[address]) {
        return 0;
    }
    if (memcmp(board, &(self->boardStates[address]), sizeof(unsigned char) * 57)) {
        *output = self->score[address];
        return 1;
    }
    return 0;
}

void storeInTranspositionTable(TranspositionTable *self, const unsigned char *board, float score, unsigned char depth) {
    unsigned long address = hashFunction(board) % self->capacity;

    if (self->occupied[address]) {
        if (self->depth[address] <= depth) {
            return;
        }
    }

    memcpy(&(self->boardStates[address]), board, sizeof(unsigned char) * 57);
    self->depth[address] = depth;
    self->occupied[address] = 1;
    self->score[address] = score;
}