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

int moveLogic(char (*pa)[TOTAL_SQUARES][SQUARE_STR_LEN], int currentPosition, int desiredPosition) {
    // Ensure the user doesn't try to move to a light square
    if ((*pa)[desiredPosition][0] == 'l') {return 0;}

    // Ensure there isn't another piece in the desiredPosition
    if ((*pa)[desiredPosition][0] != ' ') {return 0;}

    // If the piece isn't kinged
    //
    //  if white: it can only increase index
    if ((*pa)[currentPosition][0] == 'w') {
        if (currentPosition > desiredPosition) {return 0;}

        // The user can only move an index of 7, unless there is an opponents piece
        //  between it and the open spot, then it can move 14
        if ((desiredPosition-currentPosition) > 19) {return 0;}
        if ((desiredPosition-currentPosition) < 6) {return 0;}
        
        // if trying to jump an opponents piece
        if ((desiredPosition-currentPosition) == 14) {
            // Check the char of the between piece
            if ((*pa)[desiredPosition-7][0] == 'b') { 
                // if an opponents piece, remove it
                (*pa)[desiredPosition-7][0] = ' ';
            } else {return 0;}
        }
        // if trying to jump an opponents piece
        if ((desiredPosition-currentPosition) == 18) {
            // Check the char of the between piece
            if ((*pa)[desiredPosition-9][0] == 'b') { 
                // if an opponents piece, remove it
                (*pa)[desiredPosition-9][0] = ' ';
            } else {return 0;}
        }
    }

    //  if black: it can only decrease index
    else if ((*pa)[currentPosition][0] == 'b') {
        if (currentPosition < desiredPosition) {return 0;}

        // The user can only move an index of 7, unless there is an opponents piece
        //  between it and the open spot, then it can move 14
        if ((currentPosition-desiredPosition) > 19) {return 0;}
        if ((currentPosition-desiredPosition) < 6) {return 0;}
        
        // if trying to jump an opponents piece
        if ((currentPosition-desiredPosition) == 14) {
            // Check the char of the between piece
            if ((*pa)[desiredPosition+7][0] == 'w') {
                // if an opponents piece, remove it
                (*pa)[desiredPosition+7][0] = ' ';
            } else {return 0;}
        }
        // if trying to jump an opponents piece
        if ((currentPosition-desiredPosition) == 18) {
            // Check the char of the between piece
            if ((*pa)[desiredPosition+9][0] == 'w') {
                // if an opponents piece, remove it
                (*pa)[desiredPosition+9][0] = ' ';
            } else {return 0;}
        }
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

    return 1; // Successful logic
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


    // If all the rules were followed and the piece was moved
    if (moveLogic(pa, currentPosition, desiredPosition)) {
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

