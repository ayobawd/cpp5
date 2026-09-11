#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <iostream>
#include <string>
#include <exception>

// Forward declaration of Form class to avoid circular include issues
class Form;

class Bureaucrat {
private:
    const std::string _name;
    int               _grade;

public:
    // Orthodox Canonical Form
    Bureaucrat();
    Bureaucrat(const std::string& name, int grade);
    Bureaucrat(const Bureaucrat& copy);
    Bureaucrat& operator=(const Bureaucrat& assign);
    ~Bureaucrat();

    // Getters
    const std::string& getName() const;
    int                getGrade() const;

    // Member functions
    void incrementGrade();
    void decrementGrade();
    void signForm(Form& form);

    // Nested Exception Classes
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
std::ostream& operator<<(std::ostream& os, const Bureaucrat& bureaucrat);

#endif
