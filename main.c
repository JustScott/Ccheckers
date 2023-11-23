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
#define SQUARE_STR_LEN 1

char playersMove[5] = "black";


struct checkedMoveResult {
    // 0 if the function failed
    int status;
    // {currentPosition, jumpablePosition, desiredPosition}
    int moveInstructions[3];
};

struct checkedMoveResult checkMove(char (*pa)[TOTAL_SQUARES][SQUARE_STR_LEN], int currentPosition, int desiredPosition) {
    struct checkedMoveResult moveResult;
    moveResult.status = 0;
    moveResult.moveInstructions[0] = currentPosition;
    moveResult.moveInstructions[2] = desiredPosition;

    // Ensure the user doesn't try to move to a light square
    if ((*pa)[desiredPosition][0] == 'l') {return moveResult;}

    // Ensure there isn't another piece in the desiredPosition
    if ((*pa)[desiredPosition][0] != ' ') {return moveResult;}

    // The index of the space between the desired and current position
    int jumpablePosition = desiredPosition - currentPosition;
    jumpablePosition = (jumpablePosition / 2) + currentPosition;

    // Ensure the piece is only moving between a 19 and 6 index change
    int desiredMinusCurrent = desiredPosition-currentPosition;
    int absoluteDesiredMinusCurrent = desiredMinusCurrent;

    if (desiredMinusCurrent < 0) {
        absoluteDesiredMinusCurrent *= -1;
    }
    if (absoluteDesiredMinusCurrent > 19) {return moveResult;}
    if (absoluteDesiredMinusCurrent < 6) {return moveResult;}


    // if non-king white: it can only increase index
    if ((*pa)[currentPosition][0] == 'w') {
        if (currentPosition > desiredPosition) {return moveResult;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition][0] == 'b' || (*pa)[jumpablePosition][0] == 'B') { 
                moveResult.moveInstructions[1] = jumpablePosition;
            } else {return moveResult;}
        }
    }

    // if non-king black: it can only decrease index
    if ((*pa)[currentPosition][0] == 'b') {
        if (currentPosition < desiredPosition) {return moveResult;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition][0] == 'w' || (*pa)[jumpablePosition][0] == 'W') {
                moveResult.moveInstructions[1] = jumpablePosition;
            } else {return moveResult;}
        }
    }

    //  if black or white king piece
    if ((*pa)[currentPosition][0] == 'W' || (*pa)[currentPosition][0] == 'B') {
        // If moving more than one square
        if (absoluteDesiredMinusCurrent > 13) {
            // if white king
            if ((*pa)[currentPosition][0] == 'W') {
                if ((*pa)[jumpablePosition][0] == 'b' || (*pa)[jumpablePosition][0] == 'B') {
                    // if an opponents piece, remove it
                    moveResult.moveInstructions[1] = jumpablePosition;
                } else {return moveResult;}   
            // if black king
            } else if ((*pa)[currentPosition][0] == 'B') {
                if ((*pa)[jumpablePosition][0] == 'w' || (*pa)[jumpablePosition][0] == 'W') {
                    // if an opponents piece, remove it
                    moveResult.moveInstructions[1] = jumpablePosition;
                } else {return moveResult;}
            }
        }
    }

    // Successful logic
    moveResult.status = 1;
    return moveResult; 
}

int makeMove(char (*pa)[TOTAL_SQUARES][SQUARE_STR_LEN], int currentPosition, int jumpablePosition, int desiredPosition) {
    if (jumpablePosition) {
        (*pa)[jumpablePosition][0] = ' ';
    }

    // Move the piece out of the currentPosition and into the desiredPosition 
    (*pa)[desiredPosition][0] = (*pa)[currentPosition][0]; 
    (*pa)[currentPosition][0] = ' ';

    // Kinging a white piece
    if ((*pa)[56][0] == 'w') {(*pa)[56][0] = 'W';}
    if ((*pa)[58][0] == 'w') {(*pa)[58][0] = 'W';}
    if ((*pa)[60][0] == 'w') {(*pa)[60][0] = 'W';}
    if ((*pa)[62][0] == 'w') {(*pa)[62][0] = 'W';}

    // Kinging a black piece
    if ((*pa)[1][0] == 'b') {(*pa)[1][0] = 'B';}
    if ((*pa)[3][0] == 'b') {(*pa)[3][0] = 'B';}
    if ((*pa)[5][0] == 'b') {(*pa)[5][0] = 'B';}
    if ((*pa)[7][0] == 'b') {(*pa)[7][0] = 'B';}
}

int checkMultiHop(char (*pa)[TOTAL_SQUARES][SQUARE_STR_LEN], int currentPosition) {
    // Run the squares that are 14 and 18 away from the new currentPosition
    //  through checkMove to see if they are jumpable to, if they are, return
    //  1, else return 0. Further logic is handled by the other functions
    struct checkedMoveResult moveDownRight = checkMove(pa, currentPosition, currentPosition+18);
    struct checkedMoveResult moveUpLeft = checkMove(pa, currentPosition, currentPosition-18);
    struct checkedMoveResult moveDownLeft = checkMove(pa, currentPosition, currentPosition+14);
    struct checkedMoveResult moveUpRight = checkMove(pa, currentPosition, currentPosition-14);

    return moveDownRight.status || moveUpLeft.status || moveDownLeft.status || moveUpRight.status;
}

void promptUser(char (*pa)[TOTAL_SQUARES][SQUARE_STR_LEN]) {
    int currentPosition;
    int desiredPosition;

    int tryLimit = 10;

    // Ensure the player chooses one of their pieces
    while (tryLimit > 0) {
        printf("\e[1;1H\e[2J"); // Clears the screen
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

            (*pa)[1][0], (*pa)[3][0], (*pa)[5][0], (*pa)[7][0],
            (*pa)[8][0], (*pa)[10][0],(*pa)[12][0],(*pa)[14][0],
            (*pa)[17][0],(*pa)[19][0],(*pa)[21][0],(*pa)[23][0],
            (*pa)[24][0],(*pa)[26][0],(*pa)[28][0],(*pa)[30][0],
            (*pa)[33][0],(*pa)[35][0],(*pa)[37][0],(*pa)[39][0],
            (*pa)[40][0],(*pa)[42][0],(*pa)[44][0],(*pa)[46][0],
            (*pa)[49][0],(*pa)[51][0],(*pa)[53][0],(*pa)[55][0],
            (*pa)[56][0],(*pa)[58][0],(*pa)[60][0],(*pa)[62][0]
        );

        tryLimit--;
        printf("\n(%s) Enter the square number of the piece you wish to move: ", playersMove);
        scanf("%i", &currentPosition);

        // If it's white move
        if ((*pa)[currentPosition][0] == 'w' || (*pa)[currentPosition][0] == 'W') {
            if (playersMove[0] == 'w') {
                break;
            }
        // If it's blacks move
        } else if ((*pa)[currentPosition][0] == 'b' || (*pa)[currentPosition][0] == 'B') {
            if (playersMove[0] == 'b') {
                break;
            }
        // If the player chooses an incorrect piece
        } else {
            continue;
        }
    }

    tryLimit = 10;
    while (tryLimit > 0) {
        tryLimit--;
        // Picking a square to move to
        printf(
            "\n(%s) Which square number should we move '%i%c' to: ", 
            playersMove, currentPosition, (*pa)[currentPosition][0]
        );
        scanf("%i", &desiredPosition);
        // Ensure the desired position is within the board/array size to
        //  avoid index errors
        if (desiredPosition < TOTAL_SQUARES-1 && desiredPosition >= 0) {break;}
    }


    struct checkedMoveResult checkedMoveResult1 = checkMove(pa, currentPosition, desiredPosition);
    
    // If the checked move above was valid, make the move
    if (checkedMoveResult1.status) {
        makeMove(
            pa, 
            checkedMoveResult1.moveInstructions[0],
            checkedMoveResult1.moveInstructions[1],
            checkedMoveResult1.moveInstructions[2]
        );
    }


    // Ensure the piece is only moving between a 19 and 6 index change
    int desiredMinusCurrent = desiredPosition-currentPosition;
    int absoluteDesiredMinusCurrent = desiredMinusCurrent;

    if (desiredMinusCurrent < 0) {
        absoluteDesiredMinusCurrent *= -1;
    }

    // To be able to skip switching players
    //  1. The current player had to of just jumped an opponent's piece
    //  2. Their has to be a spot open for another jump as the next move
    if (absoluteDesiredMinusCurrent > 13 && checkMultiHop(pa, desiredPosition)) {
        
    } else if (checkedMoveResult1.status) {
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

    promptUser(pa);
}

void main () {
    // Each piece will take up 3 spaces, replacing chars with empty
    //  space when needed
    char pieces_array[TOTAL_SQUARES][SQUARE_STR_LEN] = {
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        'w', 'l', 'w', 'l', 'w', 'l', 'w', 'l',
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        ' ', 'l', ' ', 'l', ' ', 'l', ' ', 'l',
        'l', ' ', 'l', ' ', 'l', ' ', 'l', ' ',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l',
        'l', 'b', 'l', 'b', 'l', 'b', 'l', 'b',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l'
    };
    promptUser(&pieces_array);
}

