#include "company.hh"
#include "utils.hh"
#include <iostream>
#include <set>

Company::Company()
{
}

Company::~Company()
{
    //std::cout << "Company destructor" << std::endl;

    // TODO: Deallocate staff
    for (auto &s : all_staff_){
        delete s.second;
    }

    // TODO: Deallocate projects
    for (auto & p : projects_){
        delete p;
    }

}

void Company::set_date(Params params)
{
    std::string day = params.at(0);
    std::string month = params.at(1);
    std::string year = params.at(2);
    if( not Utils::is_numeric(day, false) or
        not Utils::is_numeric(month, false) or
        not Utils::is_numeric(year, false) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.set(stoi(day), stoi(month), stoi(year));
    std::cout << "Date has been set to ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::advance_date(Params params)
{
    std::string amount = params.at(0);
    if( not Utils::is_numeric(amount, true) )
    {
        std::cout << NOT_NUMERIC << std::endl;
        return;
    }
    Utils::today.advance(stoi(amount));
    std::cout << "New date is ";
    Utils::today.print();
    std::cout << std::endl;
}

void Company::recruit(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);

    if( iter != current_staff_.end() )
    {
        std::cout << ALREADY_EXISTS << employee_id << std::endl;
        return;
    }

    iter = all_staff_.find(employee_id);
    if( iter != all_staff_.end() )
    {
        current_staff_.insert(*iter);
        std::cout << EMPLOYEE_RECRUITED << std::endl;
        return;
    }

    Employee* new_employee = new Employee(employee_id);
    all_staff_.insert({employee_id, new_employee});
    current_staff_.insert({employee_id, new_employee});
    std::cout << EMPLOYEE_RECRUITED << std::endl;
}

void Company::leave(Params params)
{
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);
    if( iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    current_staff_.erase(iter); // Employee still stays in all_staff_
    std::cout << EMPLOYEE_LEFT << std::endl;
}

void Company::add_skill(Params params)
{
    std::string employee_id = params.at(0);
    std::string skill_name = params.at(1);

    std::map<std::string, Employee*>::const_iterator
            staff_iter = current_staff_.find(employee_id);
    if( staff_iter == current_staff_.end() )
    {
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    staff_iter->second->add_skill(skill_name);
    std::cout << SKILL_ADDED << employee_id << std::endl;
}

void Company::print_current_staff(Params)
{
    if( current_staff_.empty() )
    {
        std::cout << "None" << std::endl;
        return;
    }

    for( auto const &employee : current_staff_ )
    {
        employee.second->print_id("");
        std::cout << std::endl;
    }
}

void Company::create_project(Params params)
{
    // check if project already exists
    std::string id = params.at(0);
    std::vector<Project*>::iterator it = find_if(projects_.begin(),
                                       projects_.end(),
                                       [&id](auto a){return a->getId() == id;});
    if (it != projects_.end()){
        std::cout << ALREADY_EXISTS <<  id << std::endl ;
        //delete p;
        return;
    }

    // init new Project
    Project *p = new Project(id, Utils::today);

    // add to datastructure
    projects_.push_back(p);
    std::cout << PROJECT_CREATED << std::endl ;

}

void Company::close_project(Params params)
{
    std::string id = params.at(0);
    std::vector<Project*>::iterator it = find_if(projects_.begin(),
                                       projects_.end(),
                                       [&id](auto a){return a->getId() == id;});
    // can't find such a project
    if (it == projects_.end()){
        std::cout << CANT_FIND << id << std::endl;
        return;
    }

    // change the project state into "closed"
    (*it)->setEndDate(Utils::today);
    std::cout << PROJECT_CLOSED << std::endl;
}

void Company::print_projects(Params)
{
    // check if it's empty
    if (projects_.empty()){
        std::cout << "None" << std::endl ;
        return;
    }

    // loop through projects_ and print each
    for (const auto &p:projects_){
        p->printProject();
    }
}

void Company::add_requirement(Params params)
{
    // check if the project exists
    std::string id = params.at(0);
    std::vector<Project*>::iterator it = find_if(projects_.begin(),
                                       projects_.end(),
                                       [&id](auto a){return a->getId() == id;});
    // can't find such a project
    if (it == projects_.end()){
        std::cout << CANT_FIND << id << std::endl;
        return;
    }

    // check if the new requirement does not disqualify the currect staff of
    // the project:

    // if the project is closed can't add requirement
    if ((*it)->getIsClosed()){
        std::cout << CANT_FIND << id << std::endl ;
        return;
    }

    //populate the set in Project
    // if any disqualify exists print them out
    std::string req = params.at(1);
    std::set<std::string> dis_staff = (*it)->insertRequirement(req);
    if (!(dis_staff.empty())){
        std::cout << NOT_QUALIFIED ;
        auto iter = dis_staff.begin();
        while (true) {
            std::cout << *iter;
            ++iter;
            if (iter != dis_staff.end()) {
                std::cout << ", ";
            } else {
                break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << REQUIREMENT_ADDED << id << std::endl ;

}

void Company::assign(Params params)
{
    // check if the emplyee id exists
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = all_staff_.find(employee_id);

    if (iter == all_staff_.end()){
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // check if the project exists
    std::string id = params.at(1);
    std::vector<Project*>::iterator it = find_if(projects_.begin(),
                                       projects_.end(),
                                       [&id](auto a){return a->getId() == id;});
    // can't find such a project
    if (it == projects_.end()){
        std::cout << CANT_FIND << id << std::endl;
        return;
    }

    // check if assign is successful(project not closed)
    if ((*it)->getIsClosed()){
        std::cout << CANT_ASSIGN << employee_id << std::endl ;
        return;
    }

    // check if assign is successful(not re-assign)
    if ((*it)->isStaff(employee_id)){
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // check if assign is successful(emplyee has skill)
    // get requirements, loop requirements, check if emplyee has at least one
    // of them as skill.
    std::set < std::string > req = (*it)->getRequirement();
    bool hasMatchingSkill = false;
    for (const auto &r : req) {
        if (current_staff_[employee_id]->has_skill(r)) {
            hasMatchingSkill = true;
            break;  // Exit the loop as soon as a matching skill is found
        }
    }

    if (!hasMatchingSkill and !req.empty()) {
        std::cout << CANT_ASSIGN << employee_id << std::endl;
        return;
    }

    // no problem to add staff to project
    (*it)->insertStaff(employee_id, (*iter).second);
    std::cout << STAFF_ASSIGNED << id << std::endl;

}

void Company::print_project_info(Params params)
{
    // check if the project exists
    std::string id = params.at(0);
    std::vector<Project*>::iterator it = find_if(projects_.begin(),
                                       projects_.end(),
                                       [&id](auto a){return a->getId() == id;});
    // can't find such a project
    if (it == projects_.end()){
        std::cout << CANT_FIND << id << std::endl;
        return;
    }

    // print project
    (*it)->printProject();

    // Print requirements with comma separator
    std::set<std::string> req = (*it)->getRequirement();
    std::cout << "** Requirements: ";
    if (req.empty()) {
        std::cout << "None";
    } else {
        auto it = req.begin();
        while (true) {
            std::cout << *it;
            ++it;
            if (it != req.end()) {
                std::cout << ", ";
            } else {
                break;
            }
        }
    }
    std::cout << std::endl;

    // Print staff with comma separator
    std::map<std::string, Employee*> staff = (*it)->getStaff();
    std::cout << "** Staff: ";
    if (staff.empty()) {
        std::cout << "None";
    } else {
        auto it = staff.begin();
        while (true) {
            std::cout << it->first;
            ++it;
            if (it != staff.end()) {
                std::cout << ", ";
            } else {
                break;
            }
        }
    }
    std::cout << std::endl;

}

void Company::print_employee_info(Params params)
{
    // check if emplyee is in current_staff
    std::string employee_id = params.at(0);
    std::map<std::string, Employee*>::const_iterator
            iter = current_staff_.find(employee_id);

    if (iter == current_staff_.end()){
        std::cout << CANT_FIND << employee_id << std::endl;
        return;
    }

    // print skills from skill set of Employee
    (*iter).second->print_skills();

    // print projects of the emplyee, if any
    std::cout << "Projects: " ;
    bool no_project_for_emplyee = true;
    for (auto it = projects_.begin(); it != projects_.end(); ++it){
        if ((*it)->isStaff(employee_id)){
            if (no_project_for_emplyee){
                std::cout << std::endl ; // new line for the first project
            }
            std::cout << "** " ;
            (*it)->printProject();
            no_project_for_emplyee = false;
        }
    }
    if(no_project_for_emplyee){
        std::cout << "None" << std::endl;
    }
}

void Company::print_active_staff(Params)
{
    std::map< std::string, Employee* > p_staff;
    std::set < std::string > active_staff;
    // loop over each project in vector
    for(const auto &p : projects_){
        // retrieve its staff map
        p_staff = p->getStaff();

        // insert map staff key in a set
        for(auto iter = p_staff.begin(); iter != p_staff.end(); ++iter){
            active_staff.insert(iter->first);
        }
    }

    // check if active staff set is empty
    if (active_staff.empty()){
        std::cout << "None" << std::endl;
        return;
    }

    // print the set
    for(auto it = active_staff.begin(); it != active_staff.end(); ++it){
        std::cout << *it << std::endl ;
    }

}
