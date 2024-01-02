#ifndef GAMEENGINE_HH
#define GAMEENGINE_HH

#include <string>
#include <vector>

// Obvious constants
const int INITIAL_NUMBER_OF_ROLLS = 3;
const int NUMBER_OF_DICES = 5;

// Data of each player
struct Player
{
    unsigned int id_;
    unsigned int rolls_left_;
    std::vector<int> latest_point_values_;
    std::vector<int> best_point_values_;
    // New member to store the locked status of each dice
    std::vector<bool> locked_dices; // initially it contains false for each dice

};

class GameEngine
{
public:
    // Constructor
    GameEngine();

    // Destructor
    ~GameEngine();

    // Adds a new player
    void add_player(const Player player);

    // Prints guide text, telling which player is in turn and how many trials
    // they have left.
    void update_guide() const;

    // Rolls all dices, i.e. draws a new series of face numbers for the player
    // currently in turn. Moreover, reports the winner, if after the draw, all
    // players have used all their turns.
    void roll();

    // Gives turn for the next player having turns left, i.e. for the next
    // element in the players_ vector. After the last one, turn is given for
    // the second one (since the first one is NOBODY).
    bool give_turn();

    // Reports a winner based on the current situation and sets the game_over_
    // attribute as true.
    std::string report_winner();

    // Tells if the game is over, i.e. if all players have used all their
    // turns.
    bool is_game_over() const;

    uint getId();

    uint getRollsLeft();

    std::vector<int> getLatestpoints();

    // clear the current game and get ready to start a new round
    void gameReset();

    // to handle locking/unlocking of a dice
    void toggleLock(int diceIndex);

private:
    // Reports the status of the player currently in turn
    void report_player_status() const;

    // Updates best and latest points of the player in turn:
    // latest_point_values_ will always be new_points,
    // best_point_values_ will be new_points, if the last_mentioned is better.
    void update_points(const std::vector<int>& new_points);

    // Returns true if all turns of all players have been used,
    // otherwise returns false.
    bool all_turns_used() const;

    // Vector of all players
    std::vector<Player> players_;

    // Tells the player currently in turn (index of players_ vector)
    unsigned int game_turn_;

    // Tells if the game is over
    bool game_over_;
};

#endif // GAMEENGINE_HH
