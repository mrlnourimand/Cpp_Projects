#include "project.hh"
#include <iostream>

Project::Project(const std::string& id, const std::string& start):
    id_(id), start_(start)
{
}

Project::Project(const std::string& id, const Date &start):
    id_(id), start_(start)
{
}

Project::~Project()
{
    //std::cout << "Project " << id_ << " destructed." << std::endl;
}

std::string Project::getId()
{
    return id_;
}

void Project::setEndDate(Date endDate)
{
    end_ = endDate;
    is_closed_ = true;
}

void Project::printProject()
{
    std::cout << id_ << " : " ;
    start_.print();
    std::cout << " - " ;
    if (is_closed_){
        end_.print();
    }
    std::cout << std::endl;
}

std::set <std::string> Project::insertRequirement(const std::string requirement)
{
    std::set < std::string > disqualified = {};
    // check if disqualify the staff
    if (requirements_.empty() and !(staff_.empty())){
        for(auto it = staff_.begin(); it != staff_.end();){
            // if staff does not have the skill remove it from map
            if( it->second->has_skill(requirement) == false ){
                disqualified.insert(it->first);
                it = staff_.erase(it);
            }
            else{
                ++it;  // go to check the next staff
            }
        }
    }

    requirements_.insert(requirement);  // insert the requirement
    return disqualified;
}

bool Project::getIsClosed()
{
    return is_closed_;
}

bool Project::isStaff(std::string emply_id)
{
    std::map<std::string, Employee*>::const_iterator
            iter = staff_.find(emply_id);

    if (iter == staff_.end()){
        return false;
    }
    return true;
}

std::set<std::string> Project::getRequirement()
{
    return requirements_;
}

void Project::insertStaff(std::string emply_id, Employee* emply)
{
    //Employee* new_staff = new Employee(emply_id);
    staff_.insert({emply_id, emply});
}

std::map<std::string, Employee*> Project::getStaff()
{
    return staff_;
}

