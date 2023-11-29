/*
 * main.c - part of the Ccheckers project
 * Copyright (C) 2023, Scott Wyman, development@scottwyman.me
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <stdio.h>


#define TOTAL_SQUARES 64


struct checkedMoveResult {
    // 0 if the function failed
    int status;
    int currentPosition;
    int jumpablePosition;
    int desiredPosition;
};

struct userChosenMove {
    int currentPosition;
    int desiredPosition;
};


void checkMove(char (*pa)[TOTAL_SQUARES], struct checkedMoveResult *moveResult) {
    // Ensure the user doesn't try to move to a light square
    if ((*pa)[moveResult->desiredPosition] == 'l') {return;}


    // Ensure there isn't another piece in the desiredPosition
    if ((*pa)[moveResult->desiredPosition] != ' ') {return;}

    // The index of the space between the desired and current position
    int desiredMinusCurrent = moveResult->desiredPosition - moveResult->currentPosition;
    int jumpablePosition = (desiredMinusCurrent / 2) + moveResult->currentPosition;

    // Ensure the piece is only moving between a 19 and 6 index change
    //
    int absoluteDesiredMinusCurrent = desiredMinusCurrent;
    if (desiredMinusCurrent < 0) {
        absoluteDesiredMinusCurrent *= -1;
    }
    if (absoluteDesiredMinusCurrent > 19) {return;}
    if (absoluteDesiredMinusCurrent < 6) {return;}


    // if non-king white: it can only increase index
    if ((*pa)[moveResult->currentPosition] == 'w') {
        if (moveResult->currentPosition > moveResult->desiredPosition) {return;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition] == 'b' || (*pa)[jumpablePosition] == 'B') { 
                moveResult->jumpablePosition = jumpablePosition;
            } else {return;}
        }
    }


    // if non-king black: it can only decrease index
    if ((*pa)[moveResult->currentPosition] == 'b') {
        if (moveResult->currentPosition < moveResult->desiredPosition) {return;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition] == 'w' || (*pa)[jumpablePosition] == 'W') {
                moveResult->jumpablePosition = jumpablePosition;
            } else {return;}
        }
    }


    //  if black or white king piece
    if ((*pa)[moveResult->currentPosition] == 'W' || (*pa)[moveResult->currentPosition] == 'B') {
        // If moving more than one square
        if (absoluteDesiredMinusCurrent > 13) {
            // if white king
            if ((*pa)[moveResult->currentPosition] == 'W') {
                if ((*pa)[jumpablePosition] == 'b' || (*pa)[jumpablePosition] == 'B') {
                    // if an opponents piece, remove it
                    moveResult->jumpablePosition = jumpablePosition;
                } else {return;}   
            // if black king
            } else if ((*pa)[moveResult->currentPosition] == 'B') {
                if ((*pa)[jumpablePosition] == 'w' || (*pa)[jumpablePosition] == 'W') {
                    // if an opponents piece, remove it
                    moveResult->jumpablePosition = jumpablePosition;
                } else {return;}
            }
        }
    }


    // Successful logic
    moveResult->status = 1;
}

int makeMove(char (*pa)[TOTAL_SQUARES], int currentPosition, int jumpablePosition, int desiredPosition) {
    if (jumpablePosition) {
        (*pa)[jumpablePosition] = ' ';
    }

    // Move the piece out of the currentPosition and into the desiredPosition 
    (*pa)[desiredPosition] = (*pa)[currentPosition]; 
    (*pa)[currentPosition] = ' ';


    // Kinging a white piece
    if ((*pa)[56] == 'w') {(*pa)[56] = 'W';}
    if ((*pa)[58] == 'w') {(*pa)[58] = 'W';}
    if ((*pa)[60] == 'w') {(*pa)[60] = 'W';}
    if ((*pa)[62] == 'w') {(*pa)[62] = 'W';}

    // Kinging a black piece
    if ((*pa)[1] == 'b') {(*pa)[1] = 'B';}
    if ((*pa)[3] == 'b') {(*pa)[3] = 'B';}
    if ((*pa)[5] == 'b') {(*pa)[5] = 'B';}
    if ((*pa)[7] == 'b') {(*pa)[7] = 'B';}
}

int checkMultiHop(char (*pa)[TOTAL_SQUARES], int desiredPosition) {
    // Run the squares that are 14 and 18 away from the new currentPosition
    //  through checkMove to see if they are jumpable to, if they are, return
    //  1, else return 0. Further logic is handled by the other functions

    struct checkedMoveResult theoreticalMoveResult;
    theoreticalMoveResult.status = 0;
    theoreticalMoveResult.jumpablePosition = 0;

    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition+18;
    checkMove(pa, &theoreticalMoveResult);
    if (theoreticalMoveResult.status) {return 1;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition-18;
    if (theoreticalMoveResult.status) {return 1;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition+14;
    checkMove(pa, &theoreticalMoveResult);
    if (theoreticalMoveResult.status) {return 1;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition-14;
    checkMove(pa, &theoreticalMoveResult);
    if (theoreticalMoveResult.status) {return 1;}

    return 0;
}

void promptUser(char (*pa)[TOTAL_SQUARES], struct userChosenMove *user, char (*playersMove)[6]) {
    int tryLimit = 10;

    // Ensure the player chooses one of their pieces
    while (tryLimit--) {
        printf("\e[1;1H\e[2J"); // Clears the screen
        if (tryLimit < 9) {
            printf("\n\t- Invalid Choice, Try Again -\n");
        }
        printf(
            "\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "|     |  1%c |     |  3%c |     |  5%c |     |  7%c |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "|  8%c |     | 10%c |     | 12%c |     | 14%c |     |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "|     | 17%c |     | 19%c |     | 21%c |     | 23%c |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "| 24%c |     | 26%c |     | 28%c |     | 30%c |     |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "|     | 33%c |     | 35%c |     | 37%c |     | 39%c |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "| 40%c |     | 42%c |     | 44%c |     | 46%c |     |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "|     | 49%c |     | 51%c |     | 53%c |     | 55%c |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n"
            "| 56%c |     | 58%c |     | 60%c |     | 62%c |     |\n"
            "+-----+-----+-----+-----+-----+-----+-----+-----+\n",

            (*pa)[1], (*pa)[3], (*pa)[5], (*pa)[7],
            (*pa)[8], (*pa)[10],(*pa)[12],(*pa)[14],
            (*pa)[17],(*pa)[19],(*pa)[21],(*pa)[23],
            (*pa)[24],(*pa)[26],(*pa)[28],(*pa)[30],
            (*pa)[33],(*pa)[35],(*pa)[37],(*pa)[39],
            (*pa)[40],(*pa)[42],(*pa)[44],(*pa)[46],
            (*pa)[49],(*pa)[51],(*pa)[53],(*pa)[55],
            (*pa)[56],(*pa)[58],(*pa)[60],(*pa)[62]
        );

        printf("\n(%s) Enter the square number of the piece you wish to move: ", *playersMove);
        scanf("%i", &user->currentPosition);

        // Ensure the desired position is within the board/array size to
        //  avoid index errors
        if (user->currentPosition < TOTAL_SQUARES-1 && user->currentPosition >= 0) {
            // If it's white's moves
            if ((*pa)[user->currentPosition] == 'w' || (*pa)[user->currentPosition] == 'W') {
                // If the player choose the opponent's color
                if ((*playersMove)[0] != 'w') {
                    continue;
                }
            // If it's black's move
            } else if ((*pa)[user->currentPosition] == 'b' || (*pa)[user->currentPosition] == 'B') {
                // If the player choose the opponent's color
                if ((*playersMove)[0] != 'b') {
                    continue;
                }
            } else {continue;} // If the player chooses a blank or white piece
        } else {continue;}

        // Picking a square to move to
        printf(
            "\n(%s) Which square number should we move '%i%c' to: ", 
            *playersMove, user->currentPosition, (*pa)[user->currentPosition]
        );

        scanf("%i", &user->desiredPosition);

        // Ensure the desired position is within the board/array size to
        //  avoid index errors
        if (user->desiredPosition < TOTAL_SQUARES-1 && user->desiredPosition >= 0) {break;}
    }
}

void main () {
    // 'l' = light (square the player can't move to)
    // ' ' = open square the player can move to
    char piecesArray[TOTAL_SQUARES] = {
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        'w', 'l', 'w', 'l', 'w', 'l', 'w', 'l',
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        ' ', 'l', ' ', 'l', ' ', 'l', ' ', 'l',
        'l', ' ', 'l', ' ', 'l', ' ', 'l', ' ',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l',
        'l', 'b', 'l', 'b', 'l', 'b', 'l', 'b',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l'
    };

    char playersMove[6] = "black";

    // Declared outside of the loop for readability
    //
    int desiredMinusCurrent;
    int absoluteDesiredMinusCurrent;
    struct userChosenMove user;
    struct checkedMoveResult moveResult;

    int moveLimit = 10000;
    while (moveLimit--) {
        promptUser(&piecesArray, &user, &playersMove);

        moveResult.status = 0;
        moveResult.currentPosition = user.currentPosition;
        moveResult.desiredPosition = user.desiredPosition;
        moveResult.jumpablePosition = 0;

        checkMove(&piecesArray, &moveResult);

        // If the checked move above was valid, make the move
        if (moveResult.status) {
            makeMove(
                &piecesArray, 
                moveResult.currentPosition,
                moveResult.jumpablePosition,
                moveResult.desiredPosition
            );
        }

        // Ensure the piece is only moving between a 19 and 6 index change
        desiredMinusCurrent = user.desiredPosition-user.currentPosition;
        absoluteDesiredMinusCurrent = desiredMinusCurrent;

        if (desiredMinusCurrent < 0) {
            absoluteDesiredMinusCurrent *= -1;
        }

        // To be able to skip switching players
        //  1. The current player had to of just jumped an opponent's piece
        //  2. Their has to be a spot open for another jump as the next move
        if (absoluteDesiredMinusCurrent > 13 && checkMultiHop(&piecesArray, user.desiredPosition)) {
        } else if (moveResult.status) {
            // switch players
            if (playersMove[0] == 'b') {
                playersMove[0] = 'w';
                playersMove[1] = 'h';
                playersMove[2] = 'i';
                playersMove[3] = 't';
                playersMove[4] = 'e';
            }
            else {
                playersMove[0] = 'b';
                playersMove[1] = 'l';
                playersMove[2] = 'a';
                playersMove[3] = 'c';
                playersMove[4] = 'k';
            }
        }
    }
}

