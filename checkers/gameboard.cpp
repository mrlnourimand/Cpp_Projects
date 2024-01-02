/* Program author:
* Name: Maral Nourimand
* Student number: 151749113
* UserID: txmano
* E-Mail: maral.nourimand@tuni.fi
* */


#include "gameboard.hh"
#include <iostream>
#include <sstream>

GameBoard::GameBoard(char c):
    c_(c)
{
    init_board();
}

void GameBoard::print() const
{
    // Tulostetaan yläreuna
    // Printing upper border
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '=');

    // Tulostetaan otsikkorivi
    // Printing title row
    std::cout << "|   | ";
    for(unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << i + 1 << " ";
    }
    std::cout << "|" << std::endl;

    // Tulostetaan viiva otsikkorivin alle
    // Printing line after the title row
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '-');

    // Tulostetaan pelilaudan varsinainen sisältö
    // Printing the actual content of the gameboard
    for(unsigned int i = 0; i < SIZE; ++i)
    {
        std::cout << "| " << i + 1 << " | ";
        for(unsigned int j = 0; j < SIZE; ++j)
        {
            // Tulosta tässä tyhjä tai pelilaudan alkio (i, j)
            // Print here empty or the element (i, j) in the gameboard
            std::cout << state_.at(i).at(j) << " ";
        }
        std::cout << "|" << std::endl;
    }

    // Tulostetaan alareuna
    // Printing lower border
    print_line(LEFT_COLUMN_WIDTH + 1 + 2 * SIZE + 1, '=');
}

bool GameBoard::checkMove(const std::string& input)
{
    int startX = 0;
    int startY = 0;
    int destX = 0;
    int destY = 0;
    std::string::size_type len = input.length();

    // to check the lenght of the input, no extra input is entered.
    if (len != 7)
    {
        std::cout<< "Invalid start/destination point." << std::endl;
        return false;
    }

    // Use stringstream to parse the input
    std::stringstream ss(input);
    ss >> startX >> startY >> destX >> destY ;


    // Check if parsing was successful and only digits were entered.
    if (ss.fail())
    {
        std::cout << "Invalid start/destination point." << std::endl;
        return false;
    }

    // Check if the given points are inside the game board
    if (startX < 1 || startX > SIZE || startY < 1 || startY > SIZE ||
    destX < 1 || destX > SIZE || destY < 1 || destY > SIZE)
    {
        std::cout << "Invalid start/destination point." << std::endl;
        return false;
    }

    // Check if the start point has a piece and the destination point is empty
    if ( !(!(isEmpty(startY - 1, startX - 1)) and isEmpty(destY-1, destX-1)) )
    {
        std::cout << "Cannot move from start point to destination point." << std::endl;
        return false;
    }

    // Check if the start and destination points are in the same diagonal with one piece in the middle
    int midX = (startX + destX) / 2;
    int midY = (startY + destY) / 2;

    if ((abs(startX - destX) == 2 and abs(startY - destY) == 2) and
        !(isEmpty(midY - 1, midX - 1)) )
    {
        // valid input, so move the pieces.
        move(startX, startY, destX, destY);
        return true;
    }
    //invalid move! because the start and destination points are not
    //in the same diagonal OR no piece is in the middle.
    else
    {
        std::cout << "Cannot move from start point to destination point." << std::endl;
        return false;
    }

}

int GameBoard::pieceCounter()
{
    int counter = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (state_.at(j).at(i) == c_)
            {
                // If there is a piece at this location, count it.
                counter++;
            }
        }
    }
    return counter;

}


void GameBoard::print_line(unsigned int length, char fill_character) const
{
    for(unsigned int i = 0; i < length; ++i)
    {
        std::cout << fill_character;
    }
    std::cout << std::endl;
}

void GameBoard::init_board()
{
    // fill board according to default mapping
    std::vector<char> row;

    // 1st row and 2nd row
    for(unsigned int i = 0; i < SIZE ; i++ )
    {
        row.push_back(c_);
    }
    state_.push_back(row);
    state_.push_back(row);

    // 3rd, 4th, 5th, 6th row.
    row.clear();
    for(unsigned int i = 0; i < SIZE ; i++ )
    {
        if(i == 0 || i == 1 || i == 6 || i == 7)
        {
            row.push_back(c_);
        }
        else
        {
            row.push_back(' ');
        }
    }
    for(unsigned int i = 3; i < 7 ; i++)
    {
        state_.push_back(row);
    }

    // 7th and 8th row.
    row.clear();
    for(unsigned int i = 0; i < SIZE ; i++ )
    {
        row.push_back(c_);
    }
    state_.push_back(row);
    state_.push_back(row);

}

bool GameBoard::isEmpty(int x, int y)
{
    if(state_.at(x).at(y) == c_)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void GameBoard::move(int& startX, int& startY, int& destX, int& destY)
{
    // define the middle location
    int midX = (startX + destX) / 2;
    int midY = (startY + destY) / 2;

    // Empty the start, fill the destination, skip the middle point.
    state_.at(startY-1).at(startX-1) = ' ';
    state_.at(destY-1).at(destX-1) = c_;
    state_.at(midY - 1).at(midX-1) = ' ';
}
