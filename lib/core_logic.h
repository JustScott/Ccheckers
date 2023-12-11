/**
 * userChosenMove  A struct that contains variables for the players chosen 
 *                  pieces current index and desired index
 *
 */
struct userChosenMove {
    int current_position;
    int desired_position;
};

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
void run(int (*prompt_user)(char (*)[64], struct userChosenMove *, char (*)[6]));
