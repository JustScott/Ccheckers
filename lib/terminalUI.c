/*
 * terminalUI.c - part of the Ccheckers project
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
#include "coreLogic.h"

int promptUser(char (*pa)[64], struct userChosenMove *user, char (*playersMove)[6]) {
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
        if (user->currentPosition < 64-1 && user->currentPosition >= 0) {
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
        if (user->desiredPosition < 64-1 && user->desiredPosition >= 0) {break;}
    }

    return 0;
}
