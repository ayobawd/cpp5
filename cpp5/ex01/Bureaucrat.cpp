#include "Bureaucrat.hpp"
#include "Form.hpp"

// Default Constructor
Bureaucrat::Bureaucrat() : _name("Default"), _grade(150) {}

// Parameterized Constructor
Bureaucrat::Bureaucrat(const std::string& name, int grade) : _name(name) {
    if (grade < 1)
        throw Bureaucrat::GradeTooHighException();
    if (grade > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade = grade;
}

// Copy Constructor
Bureaucrat::Bureaucrat(const Bureaucrat& copy) : _name(copy._name), _grade(copy._grade) {}

// Copy Assignment Operator
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

// Increment grade
void Bureaucrat::incrementGrade() {
    if (this->_grade - 1 < 1)
        throw Bureaucrat::GradeTooHighException();
    this->_grade--;
}

// Decrement grade
void Bureaucrat::decrementGrade() {
    if (this->_grade + 1 > 150)
        throw Bureaucrat::GradeTooLowException();
    this->_grade++;
}

// signForm function
void Bureaucrat::signForm(Form& form) {
    try {
        form.beSigned(*this);
        std::cout << this->_name << " signed " << form.getName() << std::endl;
    } catch (std::exception& e) {
        std::cout << this->_name << " couldn't sign " << form.getName()
                  << " because " << e.what() << std::endl;
    }
}

// Exception what() overrides
const char* Bureaucrat::GradeTooHighException::what() const throw() {
    return "Grade is too high! (Highest possible grade is 1)";
}

const char* Bureaucrat::GradeTooLowException::what() const throw() {
    return "Grade is too low! (Lowest possible grade is 150)";
}

// Insertion Operator Overload
std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat) {
    os << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade() << ".";
    return os;
}
