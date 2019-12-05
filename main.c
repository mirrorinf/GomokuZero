#include "Gomoku.h"
#include "FrontEnd.h"
#include "BackEnd.h"
#include "MCTSSeul.h"
#include <time.h>
#include "AlphaBeta.h"

int main(int argc, const char * argv[]) {
    GomokuState *s;
    player black, white;
    void *blackData, *whiteData;
    int state, j;
    char *buffer, c;
    MCTSNode *root;

    srand(time(NULL));

    buffer = (char *)malloc(sizeof(char) * StringBufferLengthLimit);

    s = initGomokuState();
    black = alphaBeta;
    blackData = createAlphaBetaSupportingStructureWithState(s, quickEvaluationForTheWhitePlayer);
    white = changeStateBasedUponUserInput;
    whiteData = NULL;
    
    while (1) {
        sprintf(buffer, "%f", quickEvaluationForTheCurrentPlayer(s));
        displayGomokuState(s, buffer);
        if (s->nextMoveParty == kGomokuPlayerBlack) {
            black(s, blackData);
        } else {
            white(s, whiteData);
        }

        ((AlphaBetaSupportingStructure *)blackData)->stepCount++;

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
