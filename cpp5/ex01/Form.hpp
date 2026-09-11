#ifndef FORM_HPP
#define FORM_HPP

#include <iostream>
#include <string>
#include <exception>
#include "Bureaucrat.hpp"

class Form {
private:
    const std::string _name;
    bool              _isSigned;
    const int         _gradeToSign;
    const int         _gradeToExecute;

public:
    // Orthodox Canonical Form
    Form();
    Form(const std::string& name, int gradeToSign, int gradeToExecute);
    Form(const Form& copy);
    Form& operator=(const Form& assign);
    ~Form();

    // Getters
    const std::string& getName() const;
    bool               getIsSigned() const;
    int                getGradeToSign() const;
    int                getGradeToExecute() const;

    // Member function
    void beSigned(const Bureaucrat& bureaucrat);

    // Exception Classes
    class GradeTooHighException : public std::exception {
    public:
        virtual const char* what() const throw();
    };

    class GradeTooLowException : public std::exception {
    public:
        virtual const char* what() const throw();
    };
};

// Insertion operator overload
std::ostream& operator<<(std::ostream& os, const Form& form);

#endif
