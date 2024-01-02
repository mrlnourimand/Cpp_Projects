/* Checkers (for one player)
 *
 * Desc:
 *   This program implements a checkers game for one player. The size of
 * the gameboard is 8 x 8, and it will filled with empty spaces and
 * game pieces (character given by the user).
 *   At first, the gameboard is filled with pieces, but there is an
 * empty area of size 4 x 4 in the middle of the gameboard. The aim is to
 * remove pieces such that only one of them is left. A piece can be moved
 * diagonally over another piece, whereupon the piece that was skipped
 * will be removed.
 *   On each round, the user is asked for two points (four coordinates):
 * the piece to be moved and its target position. The program checks if
 * if the move is possible.
 *   The program terminates when there is only one piece left or if the
 * user gives a quitting command ('q' or 'Q'). In such case the program
 * prints the number of moves made and that of pieces left.
 *
 * Program author:
 * Name: Maral Nourimand
 * Student number: 151749113
 * UserID: txmano
 * E-Mail: maral.nourimand@tuni.fi
 *
 * Notes about the program and it's implementation (if any):
 *  1. I changes SIZE type from unsigned int to int to avoid some of warnings while using abs().
 *  2. I used stringstream to parse the input of type string into 4 pieces(start and destination coordinates)
 * */

#include "gameboard.hh"
#include <iostream>
#include <string>

//using namespace std;


int main()
{    
    // get the piece character and print the initial game board.
    char piece;
    std::cout << "Enter piece character: ";
    std::cin >> piece;

    GameBoard game(piece);
    game.print();

    // initialize the numer of moves.
    int move_counter = 0;

    // get input form the user and handle the possible movemnet.
    while (true)
    {
            // Ask the user for input
            std::cout << "Enter start point (x, y) and destination point (x, y), or q to quit: ";
            std::string input ;
            std::getline(std::cin>>std::ws, input);

            // Check if the user wants to quit
            if (input == "q" || input == "Q")
            {
                std:: cout << move_counter << " move(s) made." << std::endl;
                std::cout << game.pieceCounter() << " piece(s) left." << std::endl;
                break;
            }

            // Check the user's move
            if (game.checkMove(input))
            {
                // Perform the move if it's valid.
                // Update the game board and the number of moves so far.
                game.print();
                move_counter++;
            }

            // the program terminates if there is only one piece left!
            if (game.pieceCounter() == 1)
            {
                break;
            }
      }

    return 0;
}
