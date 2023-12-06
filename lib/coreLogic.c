/*
 * coreLogic.c - part of the Ccheckers project
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

#include <stdlib.h>

struct checkedMoveResult {
    // 1 if the function failed
    int status;
    int currentPosition;
    int jumpablePosition;
    int desiredPosition;
};

struct userChosenMove {
    int currentPosition;
    int desiredPosition;
};


int checkMove(char (*pa)[64], struct checkedMoveResult *moveResult) {
    // Ensure the user doesn't try to move to a light square
    if ((*pa)[moveResult->desiredPosition] == 'l') {return 1;}


    // Ensure there isn't another piece in the desiredPosition
    if ((*pa)[moveResult->desiredPosition] != ' ') {return 1;}

    // The index of the space between the desired and current position
    int desiredMinusCurrent = moveResult->desiredPosition - moveResult->currentPosition;
    int jumpablePosition = (desiredMinusCurrent / 2) + moveResult->currentPosition;

    // Ensure the piece is only moving between a 19 and 6 index change
    //
    int absoluteDesiredMinusCurrent = desiredMinusCurrent;
    if (desiredMinusCurrent < 0) {
        absoluteDesiredMinusCurrent *= -1;
    }
    if (absoluteDesiredMinusCurrent > 19) {return 1;}
    if (absoluteDesiredMinusCurrent < 6) {return 1;}


    // if non-king white: it can only increase index
    if ((*pa)[moveResult->currentPosition] == 'w') {
        if (moveResult->currentPosition > moveResult->desiredPosition) {return 1;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition] == 'b' || (*pa)[jumpablePosition] == 'B') { 
                moveResult->jumpablePosition = jumpablePosition;
            } else {return 1;}
        }
    }


    // if non-king black: it can only decrease index
    if ((*pa)[moveResult->currentPosition] == 'b') {
        if (moveResult->currentPosition < moveResult->desiredPosition) {return 1;}

        // if trying to jump an opponents piece
        if ((absoluteDesiredMinusCurrent) > 13) {
            // Check the char of the between piece
            if ((*pa)[jumpablePosition] == 'w' || (*pa)[jumpablePosition] == 'W') {
                moveResult->jumpablePosition = jumpablePosition;
            } else {return 1;}
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
                } else {return 1;}
            // if black king
            } else if ((*pa)[moveResult->currentPosition] == 'B') {
                if ((*pa)[jumpablePosition] == 'w' || (*pa)[jumpablePosition] == 'W') {
                    // if an opponents piece, remove it
                    moveResult->jumpablePosition = jumpablePosition;
                } else {return 1;}
            }
        }
    }


    // Successful logic
    moveResult->status = 0;
    return 0;
}

int makeMove(char (*pa)[64], int currentPosition, int jumpablePosition, int desiredPosition) {
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

int checkMultiHop(char (*pa)[64], int desiredPosition) {
    // Run the squares that are 14 and 18 away from the new currentPosition
    //  through checkMove to see if they are jumpable to, if they are, return
    //  1, else return 0. Further logic is handled by the other functions

    struct checkedMoveResult theoreticalMoveResult;
    theoreticalMoveResult.status = 1;
    theoreticalMoveResult.jumpablePosition = 0;

    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition+18;
    checkMove(pa, &theoreticalMoveResult);
    // If the move was valid, return 0
    if (!theoreticalMoveResult.status) {return 0;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition-18;
    if (!theoreticalMoveResult.status) {return 0;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition+14;
    checkMove(pa, &theoreticalMoveResult);
    if (!theoreticalMoveResult.status) {return 0;}
    
    theoreticalMoveResult.currentPosition = desiredPosition;
    theoreticalMoveResult.desiredPosition = desiredPosition-14;
    checkMove(pa, &theoreticalMoveResult);
    if (!theoreticalMoveResult.status) {return 0;}

    return 1;
}

int run(int (*promptUser)(char (*)[64], struct userChosenMove *, char (*)[6])) {
    // 'l' = light (square the player can't move to)
    // ' ' = open square the player can move to
    char piecesArray[64] = {
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
        (*promptUser)(&piecesArray, &user, &playersMove);

        // Set the default moveResult values
        moveResult.status = 1; // Invalid Move
        moveResult.currentPosition = user.currentPosition;
        moveResult.desiredPosition = user.desiredPosition;
        moveResult.jumpablePosition = 0;

        checkMove(&piecesArray, &moveResult);

        // If the checked move above was valid, make the move
        if (!moveResult.status) {
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
        if (absoluteDesiredMinusCurrent > 13 && !checkMultiHop(&piecesArray, user.desiredPosition)) {
        }
        // If the last move was successful
        else if (!moveResult.status) {
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
