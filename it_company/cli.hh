/* Class Cli
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a command line interpreter. Preserves all commands
 * used in IT company program. Implements the exec method that calls an
 * appropriate method based on user-given command through a function pointer.
 *
 * Note: Students shouldn't need to make changes to this class.
 * */
#ifndef CLI_HH
#define CLI_HH

#include "company.hh"
#include <string>
#include <iostream>
#include <vector>

/**
 * @brief declares a type of function called
 */
using MemberFunc = void (Company::*)(const std::vector<std::string>& params);

/**
 * @brief struct that 'defines' a command
 */
struct Cmd
{
    std::vector<std::string> aliases;
    std::string name;
    std::vector<std::string> params;
    MemberFunc func_ptr;
};

/**
 * @brief error strings
 */
const std::string UNINITIALIZED = "Error: Object uninitialized. Stopping.";
const std::string WRONG_PARAMETERS = "Error: Wrong amount of parameters.";
const std::string UNKNOWN_CMD = "Error: Unknown commands given.";
const std::string FILE_READING_ERROR = "Error: Can't read given file.";
const std::string FILE_READING_OK = "Input read from file: ";

class Cli
{
public:
    /**
     * @brief Cli constructor
     * @param pointer to the company obj the functions are called to
     * @param prompt that is printed before taking in user input
     */
    Cli(Company* company, const std::string& prompt);

    /**
     * @brief exec the cli
     * @return false if execution should end, true if it should continue
     */
    bool exec();

private:
    /**
     * @brief pop_front
     * @param vec
     * vector utility func that erases the first element of given vector
     *
     * @note All iterators to the container are invalitaded
     */
    void pop_front(std::vector<std::string>& vec);

    /**
     * @brief find_command
     * @param cmd
     * @return Cmd pointer if one with given alias is found
     */
    Cmd* find_command(const std::string& cmd);

    /**
     * @brief print_help
     * @param params
     * Generate and print a help, either generic ( no params ) or
     * specific ( func name as param )
     */
    void print_help(const std::vector<std::string>& params);

    /**
     * @brief print_cmd_info
     * @param cmd
     * @param longer
     * Print info of a single cmd,
     * if longer == true, print brief and params
     */
    void print_cmd_info(Cmd *cmd, bool longer = false) const;

    /**
     * @brief read_from_file
     * @param filename
     * @return false if file could not be read, true otherwise
     *
     * @note will remove informative output, so even cmds are read, they may
     * not have worked
     */
    bool read_from_file(const std::string& filename);

    Company* company_;
    std::string prompt_;
    bool can_start;

    /**
     * @brief vector storing all cmd info.
     * Exceptionally the lines below may exceed 80 characters,
     * but otherwise the text would be less readable
     */
    std::vector<Cmd> cmds_ =
    {
        {{"QUIT", "Q"}, "Quit",{},nullptr},
        {{"HELP", "H"},"Help",{"command"},nullptr},
        {{"READ_FROM", "RF"}, "Read", {"filename"},nullptr},
        {{"SET_DATE", "SD"},"Set date",{"day","month","year"},&Company::set_date},
        {{"ADVANCE_DATE", "AD"},"Advance date",{"amount"},&Company::advance_date},
        {{"RECRUIT", "R"},"Recruit staff",{"employee id"},&Company::recruit},
        {{"LEAVE", "L"},"Resign employee from company",{"employee id"},&Company::leave},
        {{"ADD_SKILL", "AS"},"Add skill for an employee",{"employee id","skill name"},&Company::add_skill},
        {{"PRINT_CURRENT_STAFF", "PCS"},"Print current staff",{},&Company::print_current_staff},
        {{"CREATE_PROJECT", "CR"},"Create a new project", {"project id"},&Company::create_project},
        {{"CLOSE_PROJECT", "CL"},"Close a project", {"project id"},&Company::close_project},
        {{"PRINT_PROJECTS", "PP"},"Print all projects", {},&Company::print_projects},
        {{"ADD_REQUIREMENT", "AR"},"Add requirement for a project",{"project id","skill name"},&Company::add_requirement},
        {{"ASSIGN", "A"},"Assign employee for a project", {"employee id","project id"},&Company::assign},
        {{"PRINT_PROJECT_INFO", "PPI"},"Print project's info",{"project id"},&Company::print_project_info},
        {{"PRINT_EMPLOYEE_INFO", "PEI"},"Print employee's info",{"employee id"},&Company::print_employee_info},
        {{"PRINT_ACTIVE_STAFF", "PAS"},"Print active staff",{},&Company::print_active_staff},
    };
};

#endif // CLI_HH
