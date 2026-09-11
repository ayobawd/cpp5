#include "Bureaucrat.hpp"

// Default Constructor
// Initializes _name to "Default" and _grade to 150 (lowest rank)
Bureaucrat::Bureaucrat() : _name("Default"), _grade(150) {}

// Parameterized Constructor
// Checks if grade is valid (between 1 and 150 inclusive)
Bureaucrat::Bureaucrat(const std::string& name, int grade) : _name(name) {
    if (grade < 1)
        throw Bureaucrat::GradeTooHighException();
    if (grade > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade = grade;
}

// Copy Constructor
// Copies name and grade from copy object
Bureaucrat::Bureaucrat(const Bureaucrat& copy) : _name(copy._name), _grade(copy._grade) {}

// Copy Assignment Operator
// Note: _name is const, so C++ does not allow reassigning _name. Only _grade can be reassigned.
Bureaucrat& Bureaucrat::operator=(const Bureaucrat& assign) {
    if (this != &assign) {
        this->_grade = assign._grade;
    }
    return *this;
}

// Destructor
Bureaucrat::~Bureaucrat() {}

// Getters
const std::string& Bureaucrat::getName() const {
    return this->_name;
}

int Bureaucrat::getGrade() const {
    return this->_grade;
}

// Increment grade: higher rank means SMALLER number (e.g. 3 -> 2)
void Bureaucrat::incrementGrade() {
    if (this->_grade - 1 < 1)
        throw Bureaucrat::GradeTooHighException();
    this->_grade--;
}

// Decrement grade: lower rank means LARGER number (e.g. 2 -> 3)
void Bureaucrat::decrementGrade() {
    if (this->_grade + 1 > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade++;
}

// Exception message for GradeTooHighException
const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "Grade is too high! (Highest possible grade is 1)";
}

// Exception message for GradeTooLowException
const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "Grade is too low! (Lowest possible grade is 150)";
}

// Insertion operator overload
// Output format: <name>, bureaucrat grade <grade>.
std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat) {
    os << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade() << ".";
    return os;
}
