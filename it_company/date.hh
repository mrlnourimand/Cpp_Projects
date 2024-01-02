/* Class Date
 * ----------
 * COMP.CS.110 FALL 2023
 * ----------
 * Class for describing a date.
 *
 * Note: Students need not change this class, but they can if necessary.
 * */
#ifndef DATE_HH
#define DATE_HH

#include <string>

class Date
{
public:
    /**
     * @brief Date default constructor
     * Uses the default value 0 for day, month, and year
     */
    Date();

    /**
     * @brief Date constructor
     * @param day
     * @param month
     * @param year
     * If any parameter is out of sensible limits, the value 1 is used instead
     */
    Date(unsigned int day, unsigned int month, unsigned int year);

    /**
     * @brief Date constructor
     * @param date_as_string
     * Assumes the parameter string to follow the format ddmmyyyy
     */
    Date(const std::string& date_as_str);

    /**
     * @brief Date destructor
     */
    ~Date();

    /**
     * @brief set
     * @param day
     * @param month
     * @param year
     */
    void set(unsigned int day, unsigned int month, unsigned int year);

    /**
     * @brief is_default
     * @return true if the date is a default one, otherwise returns false
     * Meant for checking if a project has ended or not, since the end date
     * will be set only as the project ends, before that it has a default value
     */
    bool is_default() const;

    /**
     * @brief advance
     * @param days
     * Advances the date with the amount of days,
     * can't be advanced by negative amounts
     */
    void advance(unsigned int days);

    /**
     * @brief print as dd.mm.yyyy
     */
    void print() const;

    /**
     * @brief comparison operators
     */
    bool operator==(const Date& rhs) const;
    bool operator<(const Date& rhs) const;

private:
    /**
     * @brief obvious attributes
     */
    unsigned int day_;
    unsigned int month_;
    unsigned int year_;

    /**
     * @brief is_leap_year
     * @return true if the date is a leap year, otherwise returns false
     */
    bool is_leap_year() const;

    /**
     * @brief str_to_date_int
     * @param date_part : either day, or month, or year
     * @return int value of date_part, an initial zero is dropped away,
     * if such exists
     */
    unsigned int str_to_date_int(const std::string& date_part) const;
};

#endif // DATE_HH
