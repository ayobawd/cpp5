#include "Bureaucrat.hpp"
#include <iostream>

int main() {
    std::cout << "=== TEST 1: Valid Bureaucrat Creation ===" << std::endl;
    try {
        Bureaucrat bob("Bob", 75);
        std::cout << bob << std::endl;
        
        std::cout << "Incrementing Bob's grade..." << std::endl;
        bob.incrementGrade();
        std::cout << bob << std::endl;

        std::cout << "Decrementing Bob's grade..." << std::endl;
        bob.decrementGrade();
        std::cout << bob << std::endl;
    } catch (std::exception& e) {
        std::cout << "Unexpected Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 2: Grade Too High at Construction ===" << std::endl;
    try {
        Bureaucrat alice("Alice", 0);
        std::cout << alice << std::endl;
    } catch (std::exception& e) {
        std::cout << "Caught Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 3: Grade Too Low at Construction ===" << std::endl;
    try {
        Bureaucrat charlie("Charlie", 151);
        std::cout << charlie << std::endl;
    } catch (std::exception& e) {
        std::cout << "Caught Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 4: Incrementing Grade 1 ===" << std::endl;
    try {
        Bureaucrat boss("Boss", 1);
        std::cout << boss << std::endl;
        std::cout << "Attempting to increment Boss grade..." << std::endl;
        boss.incrementGrade();
        std::cout << boss << std::endl;
    } catch (std::exception& e) {
        std::cout << "Caught Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== TEST 5: Decrementing Grade 150 ===" << std::endl;
    try {
        Bureaucrat intern("Intern", 150);
        std::cout << intern << std::endl;
        std::cout << "Attempting to decrement Intern grade..." << std::endl;
        intern.decrementGrade();
        std::cout << intern << std::endl;
    } catch (std::exception& e) {
        std::cout << "Caught Exception: " << e.what() << std::endl;
    }

    return 0;
}
