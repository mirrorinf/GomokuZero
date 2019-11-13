#include "Gomoku.h"
#include "FrontEnd.h"
#include "BackEnd.h"
#include "MCTSSeul.h"

int main(int argc, const char * argv[]) {
    GomokuState *s;
    player black, white;
    void *blackData, *whiteData;
    int state, j;
    char *buffer, c;
    MCTSNode *root;
    FILE *log;

    log = fopen("gomoku.log", "a");

    buffer = (char *)malloc(sizeof(char) * StringBufferLengthLimit);

    s = initGomokuState();
    black = changeStateBasedUponUserInput;
    blackData = NULL;
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
