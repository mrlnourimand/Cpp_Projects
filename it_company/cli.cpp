#include "cli.hh"
#include "utils.hh"
#include <fstream>
#include <sstream>


Cli::Cli(Company* company, const std::string& prompt):
    company_(company), prompt_(prompt), can_start(company != nullptr)
{
}

bool Cli::exec()
{
    if( not can_start )
    {
        std::cout << UNINITIALIZED << std::endl;
        return false;
    }
    std::vector<std::string> input;
    std::string cmd;
    std::cout << prompt_;
    std::getline(std::cin, cmd);
    input = Utils::split(cmd, ' ');
    if( input.empty() or Utils::is_empty(input.at(0)) )
    {
        return true;
    }
    cmd = input.front();
    pop_front(input);
    Cmd* func = find_command(cmd);
    if( func == nullptr )
    {
        std::cout << UNKNOWN_CMD << std::endl;
        return true;
    }

    // The number of parameters of commands Quit and Help does not matter
    if( func->name == "Quit" )
    {
        return false;
    }

    if( func->name == "Help" )
    {
        print_help(input);
        return true;
    }

    if( func->params.size() != input.size() )
    {
        std::cout << WRONG_PARAMETERS << std::endl;
        return true;
    }

    if( func->name == "Read" )
    {
        if( not read_from_file(input.at(0)) )
        {
            std::cout << FILE_READING_ERROR << std::endl;
            return false;
        }
        else
        {
            std::cout << FILE_READING_OK << input.at(0) << std::endl;
        }
        return true;
    }

    // Call to member func ptr: (OBJ ->* FUNC_PTR)(PARAMS)
    (company_->*(func->func_ptr))(input);
    return true;
}

void Cli::pop_front(std::vector<std::string> &vec)
{
    std::vector<std::string> result;
    for( unsigned int i = 1; i < vec.size(); ++i )
    {
        if( not Utils::is_empty(vec.at(i)) )
        {
            result.push_back(vec.at(i));
        }
    }
    vec = result;
}

Cmd *Cli::find_command(const std::string& cmd)
{
    std::string upper_cmd = cmd;
    for( unsigned int i = 0; i < cmd.size(); ++i )
    {
        upper_cmd.at(i) = std::toupper(cmd.at(i));
    }
    for( auto command = cmds_.begin(); command != cmds_.end(); ++command )
    {
        for( auto alias : command->aliases )
        {
            if ( alias == upper_cmd )
            {
                return &(*command);
            }
        }
    }
    return nullptr;
}

void Cli::print_help(const std::vector<std::string>& params)
{
    if( params.size() != 0 )
    {
        print_cmd_info(find_command(params.at(0)), true);
        return;
    }
    for( Cmd cmd : cmds_ )
    {
        print_cmd_info(&cmd);
    }
}

void Cli::print_cmd_info(Cmd* cmd, bool longer) const
{
    std::cout << cmd->name << " : " ;
    for( auto alias : cmd->aliases )
    {
        std::cout << alias << " ";
    }
    std::cout << std::endl;
    if( longer )
    {
        std::cout << "Params: " << std::endl;
        if( cmd->params.size() == 0 )
        {
            std::cout << "None." << std::endl;
        }
        else
        {
            for( auto param : cmd->params )
            {
                std::cout << param << std::endl;
            }
        }
    }
}

bool Cli::read_from_file(const std::string &filename)
{
    std::ifstream inputfile(filename);
    if( not inputfile )
    {
        return false;
    }

    // Redirect the file to cin and save the old cin
    auto cinbuf = std::cin.rdbuf(inputfile.rdbuf());
    std::ostringstream unwanted_output;
    auto coutbuf = std::cout.rdbuf(unwanted_output.rdbuf());

    while( exec() ){}

    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    inputfile.close();

    return true;
}
