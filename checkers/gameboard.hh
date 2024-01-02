/* Program author:
* Name: Maral Nourimand
* E-Mail: maral.nourimand@tuni.fi
* */

#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH
#include<vector>
#include<string>

// Pelilaudan koko
// Size of the gameboard
const int SIZE = 8;

// Vasemmanpuoleisimman sarakkeen koko, tarvitaan pelilaudan tulostamisessa
// Width of the left-most column, needed in printing the gameboard
const unsigned int LEFT_COLUMN_WIDTH = 5;




// GameBoard-luokka
// GameBoard class
class GameBoard
{
// we define using Board for simplicity.
using Board = std::vector<std::vector<char>>;

public:
    // Constructor:
    GameBoard(char c);

    // Tulostaa pelilaudan (kunhan olet viimeistellyt metodin).
    // Prints the gameboard.
    void print() const;

    // Checks whether the movement is valid based on the user input. Returns True/False.
    bool checkMove(const std::string& input);

    // Counts the number of pieces still exist on the board and returns it.
    int pieceCounter();

private:
    // Tulostaa annetun pituisen rivin annettua tulostumerkkiä.
    // Prints a line with the given length and fill character.
    void print_line(unsigned int length, char fill_character) const;

    // Initializes the board before the game starts.
    void init_board();

    // Checks if the given coordinate is empty(no piece exists).
    bool isEmpty(int x, int y);

    // Moves the piece from the start point to the destination, skipping the middle piece.
    void move(int& startX, int& startY, int& destX, int& destY);

    Board state_;
    char c_;
};

#endif // GAMEBOARD_HH
