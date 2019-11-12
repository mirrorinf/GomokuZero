#include "Gomoku.h"
#include "FrontEnd.h"
#include "BackEnd.h"

int main(int argc, const char * argv[]) {
    GomokuState *s;
    player black, white;
    void *blackData, *whiteData;
    int state;
    char *buffer;

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
