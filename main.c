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
        displayGomokuState(s);
        if (s->nextMoveParty == kGomokuPlayerBlack) {
            black(s, blackData);
        } else {
            white(s, whiteData);
        }

        printf("issue rollout?\n");
        c = getchar();
        c = getchar();
        if (c == 'y') {
            fprintf(log, "issue\n");
            root = createRootNodeWithCurrentSituation(s);
            for (j = 0; j < 20000; j++) {
                rolloutAndFeedback(root, quickEvaluationForTheCurrentPlayer);
            }
            fprintf(log, "main: %d %d %f\n", root->count, root->currentWin, root->valuationForCurrentPlayer);
            fflush(log);
        }

        if ((state = gameTerminated(s)) != kGameHasNotYetTerminated) {
            displayGomokuState(s);
            encodeGameTerninationMessage(state, buffer);
            printf("%s\n", buffer);
            break;
        }
    }
    
    destroyGomokuState(s);
    free(buffer);

    return 0;
}
