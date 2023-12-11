/*
 * core_logic.c - part of the Ccheckers project
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
    int current_position;
    int jumpable_position;
    int desired_position;
};

struct userChosenMove {
    int current_position;
    int desired_position;
};


/** 
 * @brief Check that the users desired move is valid 
 *
 * @param pa           A pointer to the checker board array
 * @param move_result  Pointer to an instance of the checkedMoveResult struct
 *                      that holds information about the players move and whether
 *                      the move is valid
 *
 * @return             Integer 1 for invalid, 0 for success
 */
int check_move(char (*pieces_array)[64], struct checkedMoveResult *move_result) {
    // Ensure the user doesn't try to move to a light square
    if ((*pieces_array)[move_result->desired_position] == 'l') {return 1;}


    // Ensure there isn't another piece in the desired_position
    if ((*pieces_array)[move_result->desired_position] != ' ') {return 1;}

    // The index of the space between the desired and current position
    int desired_minus_current = move_result->desired_position - move_result->current_position;
    int jumpable_position = (desired_minus_current / 2) + move_result->current_position;

    // Ensure the piece is only moving between a 19 and 6 index change
    //
    int absolute_desired_minus_current = desired_minus_current;
    if (desired_minus_current < 0) {
        absolute_desired_minus_current *= -1;
    }
    if (absolute_desired_minus_current > 19) {return 1;}
    if (absolute_desired_minus_current < 6) {return 1;}


    // if non-king white: it can only increase index
    if ((*pieces_array)[move_result->current_position] == 'w') {
        if (move_result->current_position > move_result->desired_position) {return 1;}

        // if trying to jump an opponents piece
        if ((absolute_desired_minus_current) > 13) {
            // Check the char of the between piece
            if ((*pieces_array)[jumpable_position] == 'b' || (*pieces_array)[jumpable_position] == 'B') { 
                move_result->jumpable_position = jumpable_position;
            } else {return 1;}
        }
    }


    // if non-king black: it can only decrease index
    if ((*pieces_array)[move_result->current_position] == 'b') {
        if (move_result->current_position < move_result->desired_position) {return 1;}

        // if trying to jump an opponents piece
        if ((absolute_desired_minus_current) > 13) {
            // Check the char of the between piece
            if ((*pieces_array)[jumpable_position] == 'w' || (*pieces_array)[jumpable_position] == 'W') {
                move_result->jumpable_position = jumpable_position;
            } else {return 1;}
        }
    }


    //  if black or white king piece
    if ((*pieces_array)[move_result->current_position] == 'W' || (*pieces_array)[move_result->current_position] == 'B') {
        // If moving more than one square
        if (absolute_desired_minus_current > 13) {
            // if white king
            if ((*pieces_array)[move_result->current_position] == 'W') {
                if ((*pieces_array)[jumpable_position] == 'b' || (*pieces_array)[jumpable_position] == 'B') {
                    // if an opponents piece, remove it
                    move_result->jumpable_position = jumpable_position;
                } else {return 1;}
            // if black king
            } else if ((*pieces_array)[move_result->current_position] == 'B') {
                if ((*pieces_array)[jumpable_position] == 'w' || (*pieces_array)[jumpable_position] == 'W') {
                    // if an opponents piece, remove it
                    move_result->jumpable_position = jumpable_position;
                } else {return 1;}
            }
        }
    }


    // Successful logic
    move_result->status = 0;
    return 0;
}


/**
 * @brief Actually move the pieces around the board
 *
 * @param pieces_array       A pointer to the checker board array
 * @param current_position   The current index of the piece to move
 * @param jumpable_position  An opponents pieces index to jump over to the 
 *                            desired_position if applicable
 * @param desired_position   The index of the open space your piece is moving to
 *
 * @return 0 if move was made successfully
 */
int make_move(char (*pieces_array)[64], int current_position, int jumpable_position, int desired_position) {
    if (jumpable_position) {
        (*pieces_array)[jumpable_position] = ' ';
    }

    // Move the piece out of the current_position and into the desired_position 
    (*pieces_array)[desired_position] = (*pieces_array)[current_position]; 
    (*pieces_array)[current_position] = ' ';


    // Kinging a white piece
    if ((*pieces_array)[56] == 'w') {(*pieces_array)[56] = 'W';}
    if ((*pieces_array)[58] == 'w') {(*pieces_array)[58] = 'W';}
    if ((*pieces_array)[60] == 'w') {(*pieces_array)[60] = 'W';}
    if ((*pieces_array)[62] == 'w') {(*pieces_array)[62] = 'W';}

    // Kinging a black piece
    if ((*pieces_array)[1] == 'b') {(*pieces_array)[1] = 'B';}
    if ((*pieces_array)[3] == 'b') {(*pieces_array)[3] = 'B';}
    if ((*pieces_array)[5] == 'b') {(*pieces_array)[5] = 'B';}
    if ((*pieces_array)[7] == 'b') {(*pieces_array)[7] = 'B';}

    return 0;
}

/**
 * @breif Check that
 *
 * Runs all the squares 14/-14 & 18/-18 index away from the desired_position
 *  to see if the player can jump an enemy piece from them. This is helpful for
 *  knowing when to switch turns.
 *
 * @param pieces_array      A pointer to the checker board array
 * @param desired_position  The possible next position for the current piece  
 *
 * Returns  0 if any of the possible jumpable squares from the desired position can
 *           be jumped, else 1.
 */
int check_multi_hop(char (*pieces_array)[64], int desired_position) {
    struct checkedMoveResult theoretical_move_result;
    theoretical_move_result.status = 1;
    theoretical_move_result.jumpable_position = 0;

    theoretical_move_result.current_position = desired_position;
    theoretical_move_result.desired_position = desired_position+18;
    check_move(pieces_array, &theoretical_move_result);
    // If the move was valid, return 0
    if (!theoretical_move_result.status) {return 0;}
    
    theoretical_move_result.current_position = desired_position;
    theoretical_move_result.desired_position = desired_position-18;
    if (!theoretical_move_result.status) {return 0;}
    
    theoretical_move_result.current_position = desired_position;
    theoretical_move_result.desired_position = desired_position+14;
    check_move(pieces_array, &theoretical_move_result);
    if (!theoretical_move_result.status) {return 0;}
    
    theoretical_move_result.current_position = desired_position;
    theoretical_move_result.desired_position = desired_position-14;
    check_move(pieces_array, &theoretical_move_result);
    if (!theoretical_move_result.status) {return 0;}

    return 1;
}

/**
 * @brief Combines all the functions from this project into a single
 *         runnable function for playing the game.
 *
 * @param prompt_user     A pointer to the an external function that handles
 *                         the UI/frontend for prompting the user for moves
 *                         and displaying moves to them.
 *
 * @returns  void as it runs in an "infinite"(10,000 moves max) loop, and shouldn't
 *            ever return. May change in the future.
 */
void run(int (*prompt_user)(
                            char (*)[64], struct userChosenMove *,
                            char (*)[6])
                          ) 
{
    // 'l' = light (square the player can't move to)
    // ' ' = open square the player can move to
    char pieces_array[64] = {
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        'w', 'l', 'w', 'l', 'w', 'l', 'w', 'l',
        'l', 'w', 'l', 'w', 'l', 'w', 'l', 'w',
        ' ', 'l', ' ', 'l', ' ', 'l', ' ', 'l',
        'l', ' ', 'l', ' ', 'l', ' ', 'l', ' ',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l',
        'l', 'b', 'l', 'b', 'l', 'b', 'l', 'b',
        'b', 'l', 'b', 'l', 'b', 'l', 'b', 'l'
    };

    char players_move[6] = "black";

    // Declared outside of the loop for readability
    //
    int desired_minus_current;
    int absolute_desired_minus_current;
    struct userChosenMove user;
    struct checkedMoveResult move_result;

    int move_limit = 10000;
    while (move_limit--) {
        (*prompt_user)(&pieces_array, &user, &players_move);

        // Set the default move_result values
        move_result.status = 1; // Invalid Move
        move_result.current_position = user.current_position;
        move_result.desired_position = user.desired_position;
        move_result.jumpable_position = 0;

        check_move(&pieces_array, &move_result);

        // If the checked move above was valid, make the move
        if (!move_result.status) {
            make_move(
                &pieces_array, 
                move_result.current_position,
                move_result.jumpable_position,
                move_result.desired_position
            );
        }

        // Ensure the piece is only moving between a 19 and 6 index change
        desired_minus_current = user.desired_position-user.current_position;
        absolute_desired_minus_current = desired_minus_current;

        if (desired_minus_current < 0) {
            absolute_desired_minus_current *= -1;
        }

        // To be able to skip switching players
        //  1. The current player had to of just jumped an opponent's piece
        //  2. Their has to be a spot open for another jump as the next move
        if (absolute_desired_minus_current > 13 && !check_multi_hop(&pieces_array, user.desired_position)) {
        }
        // If the last move was successful
        else if (!move_result.status) {
            // switch players
            if (players_move[0] == 'b') {
                players_move[0] = 'w';
                players_move[1] = 'h';
                players_move[2] = 'i';
                players_move[3] = 't';
                players_move[4] = 'e';
            }
            else {
                players_move[0] = 'b';
                players_move[1] = 'l';
                players_move[2] = 'a';
                players_move[3] = 'c';
                players_move[4] = 'k';
            }
        }
    }
}
