#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
    std::cout << "=== TEST 1: Form Creation & Display ===" << std::endl;
    try {
        Form taxForm("Tax Return", 50, 25);
        std::cout << taxForm << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 2: Invalid Form Grades ===" << std::endl;
    try {
        Form invalidFormHigh("Top Secret", 0, 50);
    } catch (std::exception& e) {
        std::cout << "High grade exception caught: " << e.what() << std::endl;
    }

    try {
        Form invalidFormLow("Toilet Paper Requisition", 151, 50);
    } catch (std::exception& e) {
        std::cout << "Low grade exception caught: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 3: Successful Form Signing ===" << std::endl;
    try {
        Bureaucrat manager("Alice (Manager)", 40);
        Form budgetApproval("Budget Approval 2026", 50, 30);

        std::cout << manager << std::endl;
        std::cout << budgetApproval << std::endl;

        std::cout << "Manager attempting to sign budget approval..." << std::endl;
        manager.signForm(budgetApproval);
        std::cout << budgetApproval << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 4: Unsuccessful Form Signing (Grade Too Low) ===" << std::endl;
    try {
        Bureaucrat intern("Bob (Intern)", 120);
        Form launchNuclear("Launch Nuke Form", 5, 1);

        std::cout << intern << std::endl;
        std::cout << launchNuclear << std::endl;

        std::cout << "Intern attempting to sign nuclear launch form..." << std::endl;
        intern.signForm(launchNuclear);
        std::cout << launchNuclear << std::endl;
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
