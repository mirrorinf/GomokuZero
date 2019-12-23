#include "Gomoku.h"
#include "FrontEnd.h"
#include "BackEnd.h"
#include "AlphaBeta.h"
#include "Auxiliaire.h"
#include <assert.h>

int isBadSetting(const char **argv) {
    return !( (strcmp(argv[1], "computer") == 0 && strcmp(argv[2], "human") == 0) || (strcmp(argv[1], "human") == 0 && strcmp(argv[2], "human") == 0) || (strcmp(argv[1], "human") == 0 && strcmp(argv[2], "computer") == 0) );
}

int main(int argc, const char **argv) {
    GomokuState *s;
    player black, white;
    opponentCallback blackCallback, whiteCallback;
    void *blackData, *whiteData;
    int state;
    char *buffer;

    if (argc != 3 || isBadSetting(argv)) {
        printf("Usage:\n  \t%s computer human\nor\t%s human computer\nor\t%s human human\n", argv[0], argv[0], argv[0]);
        exit(1);
    }
    /* Because of definition this does not need a semicolon */
    SRANDOM

    buffer = (char *)malloc(sizeof(char) * StringBufferLengthLimit);
    assert(buffer != NULL);
    s = initGomokuState();

    black = changeStateBasedUponUserInput;
    blackData = NULL;
    blackCallback = meaninglessPlaceholder;
    if (strcmp(argv[1], "computer") == 0) {
        black = alphaBeta;
        blackData = createAlphaBetaSupportingStructureWithState(s, quickEvaluationForTheWhitePlayer, shortsightedEvaluationForTheWhitePlayer);
        blackCallback = alphaBetaStepCount;
    }
    
    white = changeStateBasedUponUserInput;
    whiteData = NULL;
    whiteCallback = meaninglessPlaceholder;
    if (strcmp(argv[2], "computer") == 0) {
        white = alphaBeta;
        whiteData = createAlphaBetaSupportingStructureWithState(s, quickEvaluationForTheWhitePlayer, shortsightedEvaluationForTheWhitePlayer);
        whiteCallback = alphaBetaStepCount;
    }
    
    while (1) {
        sprintf(buffer, "");
        displayGomokuState(s, buffer);
        if (s->nextMoveParty == kGomokuPlayerBlack) {
            black(s, blackData);
        } else {
            white(s, whiteData);
        }

        /* If NULL is ever argument to these two calls, the function called must be meaninglessPlaceholder 
           check here to ensuring no accident happens */
        if (blackData == NULL) {
            assert(blackCallback == meaninglessPlaceholder);
        }
        if (whiteData == NULL) {
            assert(whiteCallback == meaninglessPlaceholder);
        }
        blackCallback(blackData);
        whiteCallback(whiteData);

        if ((state = gameTerminated(s)) != kGameHasNotYetTerminated) {
            displayGomokuState(s, NULL);
            encodeGameTerninationMessage(state, buffer);
            printf("%s\n", buffer);
            break;
        }
    }
    
    destroyGomokuState(s);
    free(buffer);

    return 0;
}
