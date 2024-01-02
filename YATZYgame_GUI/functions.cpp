#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
//#include <iostream>
#include <sstream>    // ostringstream type
#include <random>     // random numbers of roll_dices function
#include <ctime>      // initialization of the random number generator

using namespace std;

enum Result{NOTHING, PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FULL_HOUSE,
            FOUR_OF_A_KIND, YATZY};

const map<Result, string> RESULT_TEXTS =
{ { NOTHING,         "nothing"},
  { PAIR,            "pair"},
  { TWO_PAIRS,       "two pairs"},
  { THREE_OF_A_KIND, "three of a kind"},
  { STRAIGHT,        "straight"},
  { FULL_HOUSE,      "full house"},
  { FOUR_OF_A_KIND,  "four of a kind"},
  { YATZY,           "yatzy"} };


//-----------------------------------------------------------------------------
// Private functions of the module

namespace
{

//-----------------------------------------------------------------------------
// Functions recognizing results

bool is_yatzy(const map<int, int>& statistics)
{
    if ( statistics.size() == 1 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_four_of_a_kind(const map<int, int>& statistics)
{
    if ( statistics.size() == 2 )
    {
        int either_count = statistics.begin()->second;
        if ( either_count == 1 or either_count == 4 )
        {
            return true;
        }
    }
    return false;
}

bool is_full_house(const map<int, int>& statistics)
{
    if ( statistics.size() == 2 and not is_four_of_a_kind(statistics) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_straight(const map<int, int>& statistics)
{
    if ( statistics.size() == 5
         and (statistics.find(1) == statistics.end()
              or statistics.find(6) == statistics.end()) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_three_of_a_kind(const map<int, int>& statistics)
{
    if ( statistics.size() == 3 )
    {
        for ( auto infopair : statistics )
        {
            if ( infopair.second == 3 )
            {
                return true;
            }
        }
    }
    return false;
}

bool is_two_pairs(const map<int, int>& statistics)
{
    if ( statistics.size() == 3 and not is_three_of_a_kind(statistics) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_pair(const map<int, int>& statistics)
{
    if ( statistics.size() == 4 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_six(const map<int, int>& statistics)
{
    if ( statistics.size() == 5 and not is_straight(statistics) )
    {
        return true;
    }
    else
    {
        return false;
    }
}


//-----------------------------------------------------------------------------
// Functions giving a textual representation for the result

string format_result_yatzy(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    outputstream << RESULT_TEXTS.at(YATZY) << " ("
                 << statistics.begin()->second << ")";
    return outputstream.str();
}

string format_result_four_of_a_kind(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    int what_four = 0;
    int what_one = 0;

    for ( auto pair : statistics )
    {
        if ( pair.second == 4 )
        {
            what_four = pair.first;
        }
        else
        {
            what_one = pair.first;
        }
    }

    outputstream << RESULT_TEXTS.at(FOUR_OF_A_KIND) << " ("
                 << what_four << "), in addition "
                 << what_one;
    return outputstream.str();
}

string format_result_full_house(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    int what_three = 0;
    int what_two = 0;

    for ( auto pair : statistics )
    {
        if ( pair.second == 3 )
        {
            what_three = pair.first;
        }
        else
        {
            what_two = pair.first;
        }
    }

    outputstream << RESULT_TEXTS.at(FULL_HOUSE) << " ("
                 << what_three << ", " << what_two << ")";
    return outputstream.str();
}

string format_result_straight(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    outputstream << "straight (";
    if ( statistics.find(1) == statistics.end() )
    {
        outputstream << 6;
    }
    else
    {
        outputstream << 1;
    }
    outputstream << ")";
    return outputstream.str();
}

string format_result_three_of_a_kind(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    int what_three = 0;
    vector<int> what_one;

    for ( auto pair : statistics )
    {
        if ( pair.second == 3 )
        {
            what_three = pair.first;
        }
        else
        {
            what_one.push_back(pair.first);
        }
    }

    sort(what_one.begin(), what_one.end());
    if(what_one.size() < 2)
    {
        return "error: format_result_three_of_a_kind";
    }
    outputstream << RESULT_TEXTS.at(THREE_OF_A_KIND) << " ("
                 << what_three << "), in addition "
                 << what_one.at(1) << " and " << what_one.at(0);
    return outputstream.str();
}

string format_result_two_pairs(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    vector<int> what_two;
    int what_one = 0;

    for ( auto pair : statistics )
    {
        if ( pair.second == 1 )
        {
            what_one = pair.first;
        }
        else
        {
            what_two.push_back(pair.first);
        }
    }

    if(what_two.size() < 2)
    {
        return "error: format_result_two_pairs";
    }
    sort(what_two.begin(), what_two.end());
    outputstream << RESULT_TEXTS.at(TWO_PAIRS) << " ("
                 << what_two.at(1) << ", "
                 << what_two.at(0) << "), in addition " << what_one;
    return outputstream.str();
}

string format_result_pair(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    int what_two = 0;
    vector<int> what_one;;

    for ( auto pair : statistics )
    {
        if ( pair.second == 2 )
        {
            what_two = pair.first;
        }
        else
        {
            what_one.push_back(pair.first);
        }
    }

    if(what_one.size() < 3)
    {
        return "error: format_result_pair";
    }
    sort(what_one.begin(), what_one.end());
    outputstream << RESULT_TEXTS.at(PAIR) << " ("
                 << what_two << "), in addition "
                 << what_one.at(2) << ", "
                 << what_one.at(1) << ", "
                 << what_one.at(0);
    return outputstream.str();
}

string format_result_six(const map<int, int>& statistics)
{
    ostringstream outputstream{""};
    vector<int> point_values;

    for ( auto pair : statistics )
    {
        point_values.push_back(pair.first);
    }

    // call of function sort not needed, why?
    // sort(pointValues.begin(), pointValues.end());

    if(point_values.size() < 5)
    {
        return "error: format_result_six";
    }

    outputstream << RESULT_TEXTS.at(NOTHING) << " (" << point_values.at(4)
                 << "), in addition ";
    point_values.pop_back();
    reverse(point_values.begin(), point_values.end());
    for ( int point_value : point_values )
    {
        outputstream << point_value << ", ";
    }
    return outputstream.str();
}

} // end of namespace


//-----------------------------------------------------------------------------
// Public functions

int roll_dice()
{
    static unsigned int seed = static_cast<unsigned int>(time(0));
    static default_random_engine engine{seed};
    static uniform_int_distribution<int> generator{1, 6};

    return generator(engine);
}

int construe_result(const vector<int>& point_values, string& result_text)
{
    // The purpose of this function is to form and return textual construction
    // for the values in <<point_values>> vector.
    // Format could be e.g.:
    //   5 5 5 5 5  -> "yatzy (5)"
    //   3 2 3 3 3  -> "four of a kind (3), in addition 2"
    //   5 1 1 5 5  -> "full house (5, 1)"
    //   4 2 6 5 3  -> "straight (6)"
    //   4 6 4 4 2  -> "three of a kind (4), in addition 6 ja 2",
    //   5 1 1 2 5  -> "two pairs (5, 1), in addition 2"
    //   3 1 6 4 3  -> "pair (3), in addition 6, 4, 1"
    //   2 6 1 4 5  -> "nothing (6), in addition 5, 4, 2, 1"
    // or something similar.

    if(point_values.empty())
    {
        result_text = RESULT_TEXTS.at(NOTHING);
        return NOTHING;
    }

    map<int, int> point_value_statistics;
    for ( int point_value : point_values )
    {
        if ( point_value_statistics.find(point_value)
             == point_value_statistics.end() )
        {
            point_value_statistics.insert( { point_value, 1 } );
        }
        else
        {
            ++(point_value_statistics.at(point_value));
        }
    }

    if ( is_yatzy(point_value_statistics) )
    {
        result_text = format_result_yatzy(point_value_statistics);
        return YATZY;
    }
    else if ( is_four_of_a_kind(point_value_statistics) )
    {
        result_text = format_result_four_of_a_kind(point_value_statistics);
        return FOUR_OF_A_KIND;
    }
    else if ( is_full_house(point_value_statistics) )
    {
        result_text = format_result_full_house(point_value_statistics);
        return FULL_HOUSE;
    }
    else if ( is_straight(point_value_statistics) )
    {
        result_text = format_result_straight(point_value_statistics);
        return STRAIGHT;
    }
    else if ( is_three_of_a_kind(point_value_statistics) )
    {
        result_text = format_result_three_of_a_kind(point_value_statistics);
        return THREE_OF_A_KIND;
    }
    else if ( is_two_pairs(point_value_statistics) )
    {
        result_text = format_result_two_pairs(point_value_statistics);
        return TWO_PAIRS;
    }
    else if ( is_pair(point_value_statistics) )
    {
        result_text = format_result_pair(point_value_statistics);
        return PAIR;
    }
    else if ( is_six(point_value_statistics) )
    {
        result_text = format_result_six(point_value_statistics);
        return NOTHING;
    }
    else
    {
        result_text = "This is not happening...";
        return NOTHING;
    }
}

string decide_winner(const vector<vector<int>>& point_values)
{
    map<int, vector<unsigned int>> results;
    for(unsigned int i = 0; i < point_values.size(); ++i)
    {
        string dummy = ""; // needed only in the call below
        int rank = construe_result(point_values.at(i), dummy);
        if(results.find(rank) == results.end() )
        {
            results.insert({rank, {i + 1}});
        }
        else
        {
            results.at(rank).push_back(i + 1);
        }
    }

    if(results.empty())
    {
        return "No players";
    }

    ostringstream outputstream{""};
    auto iter = results.end();
    --iter; // pointing now to the last (i.e. best) element
    if(iter->second.size() == 1)
    {
        outputstream << "Player " << iter->second.at(0) << " is the winner!";
    }
    else
    {
        outputstream << "Result is tie between players " << iter->second.at(0);
        for(unsigned int i = 1; i < iter->second.size(); ++i)
        {
            outputstream << ", " << iter->second.at(i);
        }
    }
    outputstream << " ---> "
                 << RESULT_TEXTS.at(static_cast<Result>(iter->first));
    return outputstream.str();
}
