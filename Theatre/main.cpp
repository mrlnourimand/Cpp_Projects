/* Theatre
 *
 * Desc:
 *   This program reads the play collection data of different theatres from
 *   a file at startup, stores them in a suitable data structure and gives
 *   the user the opportunity to search the data structure in question. The
 *   user can give 6 different commands {theatres, plays,
 *   theatres_of_play <play>, plays_in_theatre <theatre>, plays_in_town <town>,
 *   players_in_play <play> [theatre]} then related info will be print out.
 *
 * Program author
 * Name: Maral Nourimand
 * E-Mail: maral.nourimand@tuni.fi
 *
 * Notes about the program and it's implementation (if any):
 *
 * */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// define data structures
struct Play{
    string name;
    string alias = "";
    bool has_alias = false;
    string theatre;
    string performer;
    int free_seats;

    // Overloading the less-than and == operator for custom comparison in set
    bool operator<(const Play &other) const {
        return name < other.name;
    }
    bool operator==(const std::string &name) const {
            return this->name == name;
    }
};

struct Theatre{
    string city;
    string name;
    vector<Play> plays;
};


// Fields in the input file
const int NUMBER_OF_FIELDS = 5;

// Command prompt
const string PROMPT = "the> ";

// Error and other messages
const string EMPTY_FIELD = "Error: empty field in line ";
const string FILE_ERROR = "Error: input file cannot be opened";
const string WRONG_PARAMETERS = "Error: wrong number of parameters";
const string THEATRE_NOT_FOUND = "Error: unknown theatre";
const string PLAY_NOT_FOUND = "Error: unknown play";
const string PLAYER_NOT_FOUND = "Error: unknown player";
const string TOWN_NOT_FOUND = "Error: unknown town";
const string COMMAND_NOT_FOUND = "Error: unknown command";
const string NOT_AVAILABLE = "No plays available";

// Casual split function, if delim char is between "'s, ignores it.
vector<string> split(const string& str, char delim)
{
    vector<string> result = {""};
    bool inside_quotation = false;
    for(char current_char : str)
    {
        if(current_char == '"')
        {
            inside_quotation = not inside_quotation;
        }
        else if(current_char == delim and not inside_quotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(current_char);
        }
    }
    return result;
}


// check the command that the user enters if it is among the 6 proper commands.
// It returns true if the command is properly entered.
bool checkCommand(string &command, vector<string> &com_parts){
    string::size_type delim1 = command.find(' ');
    if (delim1 != string::npos){
        string cmd1 = command.substr(0, delim1);
        //cout << cmd1 << endl;
        com_parts.push_back(cmd1);
        string cmd2 = command.substr(delim1+1);
        //cout << cmd2 << endl;
        com_parts.push_back(cmd2);
    }
    else{
        com_parts.push_back(command);
    }

    vector<string>::size_type size = com_parts.size();
    if (!(com_parts.at(0) == "theatres" or com_parts.at(0) == "plays" or
        com_parts.at(0) == "theatres_of_play" or com_parts.at(0) == "plays_in_theatre"
        or com_parts.at(0) == "plays_in_town" or com_parts.at(0) == "players_in_play"
        or com_parts.at(0) == "quit")){
        cout << COMMAND_NOT_FOUND << endl;
        return false;
    }
    if(size != 1 and (com_parts.at(0) == "theatres" or com_parts.at(0) == "plays"
                      or com_parts.at(0) == "quit")){
        cout << WRONG_PARAMETERS << endl;
        return false;
    }

    return true;
}

// check if the play's name has an alias
bool hasAlias(string &play, string &alias){
    vector<string> parts = split(play, '/');
    if (parts.size() == 2){
        play = parts.at(0);
        alias = parts.at(1);
        return true;
    }
    return false;
}


// check if the vecor contains any empty string, if so return true.
bool hasEmpty(vector<string> parts){
    for (string &p:parts){
        //boost::trim_right(p); //to trim the right whitespace
        //boost::trim_left(p);
        // count how many spaces are in the p
        string::size_type num_spaces = count(p.begin(),p.end(),' ');
        if (p.empty() or num_spaces == p.size()){
            return true;
        }
    }
    return false;
}

// read file function. If the file successfully opened, returns true.
// while reading, it load the data structure.
// format of each line: <town>;<theatre>;<play>;<performer>;<number_of_free_seats>
bool readFile(map<string, Theatre> &theatres){
    string file_name;
    cout << "Input file: ";
    getline(cin, file_name);

    // open file
    ifstream file(file_name);
    if(file){
        string line;
        int line_counter =0 ;
        while(getline(file, line)){
            // split the line into pieces
            vector<string> parts = split(line, ';');
            ++line_counter;

            // check if the line has no error.
            if(parts.size() != NUMBER_OF_FIELDS or hasEmpty(parts)){
                cout << EMPTY_FIELD << line_counter << endl;
                return false;
            }

            // if no error in line
            string town = parts.at(0);
            string theatre_name = parts.at(1);
            string play = parts.at(2);
            string performer = parts.at(3);
            string seats = parts.at(4);

            // populate the map. first check if it's a new key.
            if (theatres.find(theatre_name) == theatres.end()){
                Theatre t = Theatre{town, theatre_name, {}};
                theatres.insert({theatre_name, t});
            }
            // populate Play
            Play p = Play();
            p.performer = performer;
            p.theatre = theatre_name;
            p.name = play;
            p.has_alias = hasAlias(p.name, p.alias);
            p.free_seats = (seats == "none") ? 0 : stoi(seats);
            // add p to the vector of plays for each theatre name
            theatres.at(theatre_name).plays.push_back(p);

//            for(auto play: theatres.at(theatre_name).plays){
//                if(p.name == play.name and p.free_seats < play.free_seats){
//                    play.free_seats = p.free_seats;
//                }
//            }
        }
        file.close();
        return true;
    }
    cout << FILE_ERROR << endl;
    return false;
}

// print all theatres names in order
void printTheatres(map<string, Theatre> const &theatres){
    for (pair<const string, Theatre> const &t: theatres){
        cout << t.first << endl;
    }
}


// print a play name from struct Play. If the Play has an alias, print it with
// a given separator from the name.
void printPlays(const Play &p, string sep){
    cout << p.name;
    if(p.has_alias){
        cout << " " << sep << " " << p.alias;
    }
}

// Main function
int main()
{
    map<string, Theatre> theatres;

    // if file opening was unsuccessful
    if (!readFile(theatres)){
        return EXIT_FAILURE;
    }

    string command;

    while (true) {
        cout << PROMPT;
        getline(cin, command);

        vector<string> com_parts;

        if (!checkCommand(command, com_parts)){
            continue;
        }

        if (com_parts.at(0) == "quit"){
            return EXIT_SUCCESS;
        }

        else if(com_parts.at(0) == "theatres"){
            printTheatres(theatres);
            continue;
        }
        else if(com_parts.at(0) == "plays"){
            set<Play> plays_set;
            // populate a set of all Plays.
            for (const auto &t:theatres){
                for(const auto &play:t.second.plays){
                    plays_set.insert(play);
                }
            }
            for (auto &p:plays_set){
                printPlays(p, "***");
                cout << endl;
            }
        }

        else if (com_parts.at(0) == "theatres_of_play"){
            //check if there is a quotation mark.
            //If so, extract the inside string.
            string target = com_parts.at(1);
            if(com_parts.at(1).at(0) == '"'){
                target = com_parts.at(1).substr(1, (com_parts.at(1).length())-2);
            }
            // create a set of theatres regarding the input play.
            set<string> theatres_set;
            for (const auto &t:theatres){
                for(const auto &play:t.second.plays){
                    if(play.name == target || play.alias == target){
                        theatres_set.insert(t.first);
                    }
                }
            }
            if(theatres_set.empty()){
                cout << PLAY_NOT_FOUND << endl;
                continue;
            }
            for(auto &t:theatres_set){
                cout << t << endl;
            }

        }

        else if (com_parts.at(0) == "plays_in_theatre"){
            // remove the quotation mark
            string target = com_parts.at(1).substr(1, (com_parts.at(1).length())-2);
            if(theatres.find(target) == theatres.end()){
                cout << THEATRE_NOT_FOUND << endl;
                continue;
            }
            // create a set of Play regarding the input theatre.
            set<Play> subset;
            for(auto &p:theatres.at(target).plays){
                subset.insert(p);
            }
            for(auto &s:subset){
                cout << s.name << endl;
            }

        }

        else if(com_parts.at(0) == "plays_in_town"){
            // create a set of Play regarding the input town
            set<Play> playSeats;
            for (const auto &t : theatres) {
                if (t.second.city == com_parts.at(1)) {
                    // iterate reversly, because the last seat number is valid
                    for(auto it = t.second.plays.rbegin();
                        it != t.second.plays.rend();++it){
                        playSeats.insert(*it);
                    }
                }
            }
            if(playSeats.empty()){cout << TOWN_NOT_FOUND << endl; continue;}
            int counter = 0 ;
            for (auto &t:theatres){
                for (auto &p:playSeats){
                    if(p.free_seats > 0 and t.second.name == p.theatre){
                        cout << t.first << " : ";
                        printPlays(p, "---");
                        cout << " : " << p.free_seats << endl;
                        ++counter;
                    }
                }
            }

            if(counter == 0){
                cout << NOT_AVAILABLE << endl; // free_seats equal 0
            }
        }

        else if(com_parts.at(0) == "players_in_play"){
            string play = com_parts.at(1);
            string th = "";
            string::size_type delim1 = com_parts.at(1).find(' ');
            if(delim1 != string::npos){
                play = com_parts.at(1).substr(0, delim1);
                th = com_parts.at(1).substr(delim1+2);
                th = th.substr(0, th.length()-1);
                if(theatres.find(th) == theatres.end()){
                    cout << THEATRE_NOT_FOUND << endl;
                    continue;
                }
            }
            // create a set of pair<theatre, performer> regarding the input play
            set<pair<string, string>> performers;
            for(auto &t:theatres){
                for(auto &p:t.second.plays){
                    if((p.name == play or p.alias == play)and
                            (th == p.theatre or th.empty())){
                        performers.insert({p.theatre, p.performer});
                    }

                }
            }
            if(performers.empty()){
                cout << PLAY_NOT_FOUND << endl;
                continue;
            }
            for( auto &s:performers){
               cout << s.first << " : " << s.second << endl;
            }

        }


    }
    return EXIT_SUCCESS;
}
