#include "date.hh"
#include "utils.hh"
#include <iostream>

// Number of days in months
unsigned int const MONTH_SIZES[12] = { 31, 28, 31, 30, 31, 30,
                                       31, 31, 30, 31, 30, 31 };

Date::Date():
    day_(0), month_(0), year_(0)
{
}

Date::Date(unsigned int day, unsigned int month, unsigned int year)
{
    set(day, month, year);
}

Date::Date(const std::string& data_as_str)
{
    std::string day = data_as_str.substr(0, 2);
    std::string month = data_as_str.substr(2, 2);
    std::string year = data_as_str.substr(4, 4);
    day_ = str_to_date_int(day);
    month_ = str_to_date_int(month);
    year_ = str_to_date_int(year);
}

Date::~Date()
{
}

void Date::set(unsigned int day, unsigned int month, unsigned int year)
{
    day_ = day;
    month_ = month;
    year_ = year;

    if( month_ > 12 or month_ < 1 )
    {
        month_ = 1;
    }
    if( day_ > MONTH_SIZES[month_ - 1]
         or ( month_ == 2 and is_leap_year()
             and day > MONTH_SIZES[month - 1] + 1 ) )
    {
        day_ = 1;
    }
}

bool Date::is_default() const
{
    return day_ == 0 and month_ == 0 and year_ == 0;
}

void Date::advance(unsigned int days)
{
    day_ += days;
    while( day_ > MONTH_SIZES[month_ - 1] )
    {
        if( month_ == 2 and day_ == 29 and is_leap_year() )
        {
            return;
        }
        day_ -= MONTH_SIZES[month_ - 1];
        if ( month_ == 2 and is_leap_year() )
        {
            --day_;
        }
        ++month_;

        if ( month_ > 12 )
        {
            month_ -= 12;
            ++year_;
        }
    }
}

void Date::print() const
{
    std::cout << day_ << ".";
    std::cout << month_ << ".";
    std::cout << year_;
}

bool Date::operator==(const Date &rhs) const
{
    return day_ == rhs.day_ and month_ == rhs.month_ and year_ == rhs.year_ ;
}

bool Date::operator<(const Date &rhs) const
{
    return (year_ * 10000 + month_ * 100 + day_ ) <
           (rhs.year_ * 10000 + rhs.month_ * 100 + rhs.day_);
}

bool Date::is_leap_year() const
{
    return (year_ % 4 == 0) and ((year_ % 100 != 0) or (year_ % 400 == 0));
}

unsigned int Date::str_to_date_int(const std::string& date_part) const
{
    if( date_part.at(0) == '0' )
    {
        return std::stoi(date_part.substr(1, 1));
    }
    else
    {
        return std::stoi(date_part);
    }
}

