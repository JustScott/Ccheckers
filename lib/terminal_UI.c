/*
 * terminal_UI.c - part of the Ccheckers project
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
#include "core_logic.h"

/**
 * @brief handles the UI/frontend for prompting the user for moves
 *         and displaying moves to them. 
 *
 * @param peices_array    A pointer to the checker board array
 * @param userChosenMove  A pointer to a struct that contains variables for the
 *                         players chosen pieces current index and desired index
 * @param players_move    A an array of chars that is either 'black' or 'white'
 *                         depending on whose move it is
 *
 * @return  0 if the function succeeds
 */
int prompt_user(char (*pieces_array)[64], struct userChosenMove *user, char (*players_move)[6]) {
    int try_limit = 10;

    // Ensure the player chooses one of their pieces
    while (try_limit--) {
        printf("\e[1;1H\e[2J"); // Clears the screen
        if (try_limit < 9) {
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

            (*pieces_array)[1], (*pieces_array)[3], (*pieces_array)[5], (*pieces_array)[7],
            (*pieces_array)[8], (*pieces_array)[10],(*pieces_array)[12],(*pieces_array)[14],
            (*pieces_array)[17],(*pieces_array)[19],(*pieces_array)[21],(*pieces_array)[23],
            (*pieces_array)[24],(*pieces_array)[26],(*pieces_array)[28],(*pieces_array)[30],
            (*pieces_array)[33],(*pieces_array)[35],(*pieces_array)[37],(*pieces_array)[39],
            (*pieces_array)[40],(*pieces_array)[42],(*pieces_array)[44],(*pieces_array)[46],
            (*pieces_array)[49],(*pieces_array)[51],(*pieces_array)[53],(*pieces_array)[55],
            (*pieces_array)[56],(*pieces_array)[58],(*pieces_array)[60],(*pieces_array)[62]
        );

        printf("\n(%s) Enter the square number of the piece you wish to move: ", *players_move);
        scanf("%i", &user->current_position);

        // Ensure the desired position is within the board/array size to
        //  avoid index errors
        if (user->current_position < 64-1 && user->current_position >= 0) {
            // If it's white's moves
            if ((*pieces_array)[user->current_position] == 'w' || (*pieces_array)[user->current_position] == 'W') {
                // If the player choose the opponent's color
                if ((*players_move)[0] != 'w') {
                    continue;
                }
            // If it's black's move
            } else if ((*pieces_array)[user->current_position] == 'b' || (*pieces_array)[user->current_position] == 'B') {
                // If the player choose the opponent's color
                if ((*players_move)[0] != 'b') {
                    continue;
                }
            } else {continue;} // If the player chooses a blank or white piece
        } else {continue;}

        // Picking a square to move to
        printf(
            "\n(%s) Which square number should we move '%i%c' to: ", 
            *players_move, user->current_position, (*pieces_array)[user->current_position]
        );

        scanf("%i", &user->desired_position);

        // Ensure the desired position is within the board/array size to
        //  avoid index errors
        if (user->desired_position < 64-1 && user->desired_position >= 0) {break;}
    }

    return 0;
}
