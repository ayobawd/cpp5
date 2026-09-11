#include "Form.hpp"

// Default Constructor
Form::Form() : _name("Default Form"), _isSigned(false), _gradeToSign(150), _gradeToExecute(150) {}

// Parameterized Constructor
Form::Form(const std::string& name, int gradeToSign, int gradeToExecute)
    : _name(name), _isSigned(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute) {
    if (gradeToSign < 1 || gradeToExecute < 1)
        throw Form::GradeTooHighException();
    if (gradeToSign > 150 || gradeToExecute > 150)
        throw Form::GradeTooLowException();
}

// Copy Constructor
Form::Form(const Form& copy)
    : _name(copy._name), _isSigned(copy._isSigned),
      _gradeToSign(copy._gradeToSign), _gradeToExecute(copy._gradeToExecute) {}

// Copy Assignment Operator
Form& Form::operator=(const Form& assign) {
    if (this != &assign) {
        // const members (_name, _gradeToSign, _gradeToExecute) cannot be reassigned
        this->_isSigned = assign._isSigned;
    }
    return *this;
}

// Destructor
Form::~Form() {}

// Getters
const std::string& Form::getName() const {
    return this->_name;
}

bool Form::getIsSigned() const {
    return this->_isSigned;
}

int Form::getGradeToSign() const {
    return this->_gradeToSign;
}

int Form::getGradeToExecute() const {
    return this->_gradeToExecute;
}

// Member function beSigned
void Form::beSigned(const Bureaucrat& bureaucrat) {
    if (bureaucrat.getGrade() <= this->_gradeToSign) {
        this->_isSigned = true;
    } else {
        throw Form::GradeTooLowException();
    }
}

// Exception what() overrides
const char* Form::GradeTooHighException::what() const throw() {
    return "Form grade is too high! (Highest possible grade is 1)";
}

const char* Form::GradeTooLowException::what() const throw() {
    return "Form grade is too low! (Grade required to sign is higher than Bureaucrat grade)";
}

// Insertion Operator Overload
std::ostream& operator<<(std::ostream& os, const Form& form) {
    os << "Form: " << form.getName()
       << " | Signed: " << (form.getIsSigned() ? "Yes" : "No")
       << " | Sign Grade: " << form.getGradeToSign()
       << " | Exec Grade: " << form.getGradeToExecute();
    return os;
}
