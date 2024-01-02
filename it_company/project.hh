/* Class Project
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a project in an IT company.
 *
 * Note: Students need to implement almost all of this class by themselves.
 * */
#ifndef PROJECT_HH
#define PROJECT_HH

#include "employee.hh"
#include "date.hh"
#include "utils.hh"
#include <string>
#include <map>

const std::string NOT_QUALIFIED = "Not qualified any more: ";

class Project
{
public:
    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as string ddmmyyyy)
     */
    Project(const std::string& id, const std::string& start);

    /**
     * @brief Project constructor
     * @param : id
     * @param : start (given as Date object)
     */
    Project(const std::string& id, const Date& start);

    /**
     * @brief Project destructor
     */
    ~Project();

    // More public methods

    /**
     * @brief getId
     * to get the id of the project
     */
    std::string getId();

    /**
     * @brief setEndDate
     * @param : endDate
     * set the end date of the project
     */
    void setEndDate(Date endDate);

    /**
     * @brief printProject
     * print id and date(s) of the project
     */
    void printProject();

    /**
     * @brief insertRequirement
     * @param : requirement (string)
     * populate the requirement set. check for disqualified staff.
     *  if any, return a set of their emplyee_id.
     */
    std::set< std::string > insertRequirement(const std::string requirement);

    /**
     * @brief getIsClosed
     * @return bool
     * return true if the project is closed, otherwise return false
     */
    bool getIsClosed();

    /**
     * @brief isStaff
     * @param emply_id (string containing id)
     * @return bool
     * return true if an employee is staff member of this project,
     * otherwise false.
     */
    bool isStaff(std::string emply_id);

    /**
     * @brief getRequirement
     * @return set<string> (a set containing requirements of the project)
     * get requirement set.
     */
    std::set < std::string > getRequirement();

    /**
     * @brief insertStaff
     * @param emply_id (string containing emplyee id)
     * @param Employee* (a pointer to the Emplyee instance as an
     *       all_staff_ member of the company)
     * insert staff to a project
     */
    void insertStaff(std::string emply_id, Employee* emply);

    /**
     * @brief getStaff
     * @return map<id, Emplyee*> a map of this project's staff.
     * get staff map
     */
    std::map < std::string, Employee* > getStaff();

private:
    /**
     * @brief obvious attributes
     */
    std::string id_; // Can be a name or any other identifier
    Date start_;
    Date end_;
    bool is_closed_ = false ;

    // More attributes and private methods
    std::map< std::string, Employee* > staff_ = {};
    std::set < std::string > requirements_ = {} ;
};

#endif // PROJECT_HH
