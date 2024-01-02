/* IT company
 *
 * Desc:
 * The program consists of classes Company, Project, Employee, Date, and Cli,
 * as well as the module (namespace) Utils.
 * The objects (instances) of the class Employee describe staff in a company.
 * Employees can be recruited, and they can resign. Employees can be identified
 * by their names, or any string.
 * In addition, employees have skills required in projects.
 *
 * The class Project describes a single project. Each project can have several
 * employees, and each employee can work in several projects. A project has at
 * least a start date, and closed projects have also an end date. Moreover,
 * a project can have requirements, i.e. employees working in the project must
 * have certain skills.
 *
 * The class Cli (command line interpreter) manages the commands used in the
 * program. The module Utils provides general functions. They, for example,
 * check if the user gave a correct amount and type of parameters for each
 * command.
 *
 * The class Company manages the program in whole, and only one instance is
 * created from it. A company has staff and projects.

 * Program author
 * Name: Maral Nourimand
 * E-Mail: maral.nourimand@tuni.fi
 *
 * Notes about the program and it's implementation (if any):
 *
 * */

#include "cli.hh"
#include "company.hh"

const std::string PROMPT = "IT> ";

int main()
{
    Company* company = new Company();
    Cli cli(company, PROMPT);
    while( cli.exec() ){}

    delete company;
    return EXIT_SUCCESS;
}
