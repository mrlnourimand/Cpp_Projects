/* Class Company
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a company. Only a single object created from this
 * class. Implements all commands in IT company program.
 *
 * Note: Students need change this class to implement commands missing
 * in the template code.
 * */
#ifndef COMPANY_HH
#define COMPANY_HH

#include "employee.hh"
#include "project.hh"
#include "date.hh"
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

/**
 * @brief error and information outputs
 */
const std::string ALREADY_EXISTS = "Error: Already exists: ";
const std::string NOT_NUMERIC = "Error: Wrong type of parameters.";
const std::string CANT_FIND = "Error: Can't find anything matching: ";
const std::string CANT_ASSIGN = "Error: Can't assign: ";
const std::string EMPLOYEE_RECRUITED = "A new employee has been recruited.";
const std::string EMPLOYEE_LEFT = "Employee left company.";
const std::string PROJECT_CREATED = "A new project has been created.";
const std::string PROJECT_CLOSED = "Project closed.";
const std::string SKILL_ADDED = "Skill added for: ";
const std::string REQUIREMENT_ADDED = "Requirement added for: ";
const std::string STAFF_ASSIGNED = "Staff assigned for: ";

using Params = const std::vector<std::string>&;

class Company
{
public:
    /**
     * @brief Company default constructor
     */
    Company();

    /**
     * @brief Company destructor
     */
    ~Company();

    /**
     * @brief set_date
     * @param params, vector containing three numeric strings: dd, mm, yyyy
     * Sets a new value for the current date
     */
    void set_date(Params params);

    /**
     * @brief advance_date
     * @param params, vector containing one numeric string: the amount of days
     * Advances the current date with the given number of days
     */
    void advance_date(Params params);

    /**
     * @brief recruit
     * @param params, vector containing id string
     * Recruits a new employee (creates a new employee object)
     */
    void recruit(Params params);

    /**
     * @brief leave
     * @param params, vector containing id string
     * Removes the employee (id) from the company (but not from their projects)
     */
    void leave(Params params);

    /**
     * @brief add_skill
     * @param params, vector containing two strings: employee id and skill
     * Adds a skill for the employee, if the employee already has the skill,
     * nothing happens
     */
    void add_skill(Params params);

    /**
     * @brief print_current_staff
     * @param params, an empty vector of strings
     * Prints the current staff in the company
     */
    void print_current_staff(Params);

    /**
     * @brief create_project
     * @param params, vector containing id string
     * Creates a new project (object)
     */
    void create_project(Params params);

    /**
     * @brief close_project
     * @param params, vector containing id string
     * Closes a project, i.e. sets today as its closing date
     */
    void close_project(Params params);

    /**
     * @brief print_projects
     * @param params, an empty vector of strings
     * Prints all created projects in the order they have been created,
     * from each project prints id and start date,
     * from a closed project prints also end date
     */
    void print_projects(Params);

    /**
     * @brief add_requirement
     * @param params, vector containing two strings: project id and requirement
     * Add a skill for a project, if the project already has the requirement,
     * nothing happens.
     * If the new requirement makes an employee working in the project
     * unqualified, the employee is removed from the project
     */
    void add_requirement(Params params);

    /**
     * @brief assign
     * @param params, vector containing two strings: employee id and project id
     * Assigns an employee for a project, if the project already has the
     * employee assigned, nothing happens
     */
    void assign(Params params);

    /**
     * @brief print_project_info
     * @param params, vector containing id string
     * Prints a project: start date, requirements, and staff,
     * from a closed project prints also end date
     */
    void print_project_info(Params params);

    /**
     * @brief print_employee_info
     * @param params, vector containing id string
     * Prints an employee: skills and project, both current and closed ones
     */
    void print_employee_info(Params params);

    /**
     * @brief print_active_staff
     * @param params, an empty vector of strings
     * Prints active staff, i.e. employees that are working or have worked
     * in a project
     */
    void print_active_staff(Params);

private:
    /**
     * Obvious containers
     */
    std::map<std::string, Employee*> current_staff_ = {};
    std::map<std::string, Employee*> all_staff_ = {};

    // More attributes and private methods

    // a vector of all perojects in the company
    std::vector<Project*> projects_ = {};
};

#endif // COMPANY_HH
