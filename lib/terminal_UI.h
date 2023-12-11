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
int prompt_user(char (*pieces_array)[64], struct userChosenMove *user, char (*players_move)[6]);
