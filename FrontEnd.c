/* This file contains bullet-proof tested shit. Do not modify. Rewrite if malfunction. */

#include "FrontEnd.h"
#include "Auxiliaire.h"

#define boxCharacterUpperLeft "┌"
#define boxCharacterUpperHorizontal "┬"
#define boxCharacterUpperRight "┐"
#define boxCharacterVerticalLeft "├"
#define boxCharacterMiddle "┼"
#define boxCharacterVerticalRight "┤"
#define boxCharacterBottomLeft "└"
#define boxCharacterBottomHorizontal "┴"
#define boxCharacterBottomRight "┘"

#ifdef FRONTEND_BOARD_LARGE
#define boxCharacterHorizontalContiuation "───"
#define boxCharacterVerticalMiddle "│"
#endif
#ifdef FRONTEND_BOARD_SMALL
#define boxCharacterHorizontalContiuation "─"
#define boxCharacterVerticalMiddle ""
#endif

#define characterPlayerBlack "○"
#define characterPlayerBlackEmphasized "△"
#define characterPlayerWhite "●"
#define characterPlayerWhiteEmphasized "▲"

#ifdef WIN32
#define SystemCommandClearScreen "cls"
#else
#define SystemCommandClearScreen "clear"
#endif

int haveDisplayedPlayer(GomokuState *self, int atLine, int atColumn) {
    if (stateAtPosition(self, atLine, atColumn) == kGomokuGridStateUnoccupied) {
        return 0;
    }
    if (stateAtPosition(self, atLine, atColumn) == kGomokuGridStateOccupiedPlayerBlack) {
        if (self->recentMoveLine == atLine && self->recentMoveColumn == atColumn) {
            printf(characterPlayerBlackEmphasized);
        } else {
            printf(characterPlayerBlack);
        }
    } else if (stateAtPosition(self, atLine, atColumn) == kGomokuGridStateOccupiedPlayerWhite) {
        if (self->recentMoveLine == atLine && self->recentMoveColumn == atColumn) {
            printf(characterPlayerWhiteEmphasized);
        } else {
            printf(characterPlayerWhite);
        }
    }
    return 1;
}

void displayGomokuState(GomokuState *self, const char *additionalMessage) {
    int i, j;
    fflush(stdout);
    system(SystemCommandClearScreen);

    printf("Welcome to Gomoku Game!\nThis is intented to be played on black terminal white characters.\n\n\n");
    
    printf("15 ");

    if (!haveDisplayedPlayer(self, 15, 1)) {
        printf(boxCharacterUpperLeft);
    }
    printf(boxCharacterHorizontalContiuation);
    for (i = 0; i < 13; i++) {
        if (!haveDisplayedPlayer(self, 15, 2 + i)) {
            printf(boxCharacterUpperHorizontal);
        }
        printf(boxCharacterHorizontalContiuation);
    }
    if (!haveDisplayedPlayer(self, 15, 15)) {
        printf(boxCharacterUpperRight);
    }
    printf("\n");
    for (i = 0; i < 13; i++) {
#ifdef FRONTEND_BOARD_LARGE
        printf("   ");
        for (j = 0; j < 15; j++) {
            printf(boxCharacterVerticalMiddle);
            printf("   ");
        }
        printf("\n");
#endif
        printf("%2d ", 14 - i);
        if (!haveDisplayedPlayer(self, 14 - i, 1)) {
            printf(boxCharacterVerticalLeft);
        }
        printf(boxCharacterHorizontalContiuation);
        for (j = 0; j < 13; j++) {
            if (!haveDisplayedPlayer(self, 14 - i, 2 + j)) {
                printf(boxCharacterMiddle);
            }
            printf(boxCharacterHorizontalContiuation);
        }
        if (!haveDisplayedPlayer(self, 14 - i, 15)) {
            printf(boxCharacterVerticalRight);
        }
        printf("\n");
    }
#ifdef FRONTEND_BOARD_LARGE
    printf("   ");
    for (j = 0; j < 15; j++) {
        printf(boxCharacterVerticalMiddle);
        printf("   ");
    }
    printf("\n");
#endif
    printf(" 1 ");
    if (!haveDisplayedPlayer(self, 1, 1)) {
        printf(boxCharacterBottomLeft);
    }
    printf(boxCharacterHorizontalContiuation);
    for (i = 0; i < 13; i++) {
        if (!haveDisplayedPlayer(self, 1, 2 + i)) {
            printf(boxCharacterBottomHorizontal);
        }
        printf(boxCharacterHorizontalContiuation);
    }
    if (!haveDisplayedPlayer(self, 1, 15)) {
        printf(boxCharacterBottomRight);
    }
    printf("\n");

    printf("   ");
#ifdef FRONTEND_BOARD_LARGE
    for (i = 0; i < 15; i++) {
        printf("%c   ", 'A' + i);
    }
#endif
#ifdef FRONTEND_BOARD_SMALL
    for (i = 0; i < 15; i++) {
        printf("%c ", 'A' + i);
    }
#endif

    if (additionalMessage != NULL) {
        printf("%s\n", additionalMessage);
    }
    if (self->recentMoveLine != -1) {
        printf("\nrecent move: %c%d\n", 'A' + self->recentMoveColumn - 1, self->recentMoveLine);
    }

    fflush(stdout);
}

void changeStateBasedUponUserInput(GomokuState *self, void *meaninglessPlaceholder) {
    char columnName;
    int atLine, atColumn;
    char c;

    if (self->nextMoveParty == kGomokuPlayerBlack) {
        printf("\nMove of player black >>");
    } else {
        printf("\nMove of player white >>");
    }
    fflush(stdout);

    while (1) {
        columnName = '\n';
        while (columnName == '\n' || columnName == ' ' || columnName == '\t') {
            scanf("%c", &columnName);
        }

        if (scanf("%d", &atLine) != 1 || atLine < 1 || atLine > 15) {
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Input error. Try again >>");
            continue;
        }

        if (columnName >= 'A' && columnName <= 'O') {
            atColumn = columnName - 'A' + 1;
        } else if (columnName >= 'a' && columnName <= 'o') {
            atColumn = columnName - 'a' + 1;
        } else {
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Input error. Try again >>");
            continue;
        }

        if (stateAtPosition(self, atLine, atColumn) != kGomokuGridStateUnoccupied) {
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Position already occupied. Try again >>");
            continue;
        }

        break;
    }

    changeState(self, atLine, atColumn, self->nextMoveParty);
    self->nextMoveParty = -self->nextMoveParty;
    self->recentMoveLine = atLine;
    self->recentMoveColumn = atColumn;
}

void encodeGameTerninationMessage(int terminaison, char *buffer) {
    switch (terminaison) {
    case kGameTerminatedBlackWon:
        sprintf(buffer, "Black has won.");
        break;
    case kGameTerminatedWhiteContinuedFive:
        sprintf(buffer, "White has won.");
        break;
    case kGameTerminatedBlackForbiddenDoubleFour:
        sprintf(buffer, "White has won. Forbidden black move: double four.");
        break;
    case kGameTerminatedBlackForbiddenDoubleThree:
        sprintf(buffer, "White has won. Forbidden black move: double three.");
        break;
    case kGameTerminatedBlackForbiddenOverline:
        sprintf(buffer, "White has won. Forbidden black move: overline.");
        break;
    default:
        break;
    }
}

void meaninglessPlaceholder(void *meaningless) {

}
