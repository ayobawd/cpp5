# C++ - Module 05: Repetition and Exceptions

## Chapter I: Introduction

C++ is a general-purpose programming language created by Bjarne Stroustrup as an extension of the C programming language, or "C with Classes" (source: Wikipedia).

The goal of these modules is to introduce you to Object-Oriented Programming.

This will be the starting point of your C++ journey. Many languages are recommended for learning OOP. We have chosen C++ since it is derived from your old friend, C.

Because this is a complex language, and in order to keep things simple, your code will comply with the C++98 standard.

We are aware that modern C++ is significantly different in many aspects. So, if you want to become a proficient C++ developer, it is up to you to go further after the 42 Common Core.

---

## Chapter II: General Rules

### Compiling
- Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.
- Your code should still compile if you add the flag `-std=c++98`.

### Formatting and Naming Conventions
- The exercise directories will be named this way: `ex00`, `ex01`, ..., `exn`.
- Name your files, classes, functions, member functions, and attributes as required in the guidelines.
- Write class names in **UpperCamelCase** format. Files containing class code will always be named according to the class name. For instance: `ClassName.hpp`/`ClassName.h`, `ClassName.cpp`, or `ClassName.tpp`.
- If you have a header file containing the definition of a class `BrickWall`, its name will be `BrickWall.hpp`.
- Unless specified otherwise, every output message must end with a newline character and be displayed to the standard output.
- Goodbye Norminette! No coding style is enforced in the C++ modules. Write clean and readable code.

### Allowed / Forbidden
- You are allowed to use almost everything from the standard library.
- External libraries, C++11 (and derived forms), and Boost libraries are **forbidden**.
- The following functions are forbidden: `*printf()`, `*alloc()`, and `free()`. If you use them, your grade will be 0.
- Unless explicitly stated otherwise, `using namespace <ns_name>` and `friend` keywords are **forbidden** (penalty: -42).
- You are allowed to use the STL only in Modules 08 and 09. No Containers (`vector`, `list`, `map`, etc.) and no Algorithms (anything requiring `<algorithm>`) until then (penalty: -42).

### Design Requirements
- Avoid memory leaks when using `new`.
- From Module 02 to Module 09, your classes must be designed in the **Orthodox Canonical Form** (OCF), except when explicitly stated otherwise.
- Any function implementation put in a header file (except for function templates) means 0 to the exercise.
- Header files must be usable independently with include guards to prevent double inclusion.
- Makefile rules follow standard C conventions (`all`, `clean`, `fclean`, `re`).

---

## Exercise 00: Mommy, when I grow up, I want to be a bureaucrat!

- **Directory:** `ex00/`
- **Files to Submit:** `Makefile`, `main.cpp`, `Bureaucrat.{h, hpp}`, `Bureaucrat.cpp`
- **Forbidden:** None

> Exception classes do not have to be designed in Orthodox Canonical Form. However, every other class must follow it.

### Specification:
- **Bureaucrat Attributes:**
  - Constant `name` (`std::string`)
  - `grade` (`int`, 1 = highest, 150 = lowest)
- Instantiating with invalid grade must throw `Bureaucrat::GradeTooHighException` or `Bureaucrat::GradeTooLowException`.
- **Getters:** `getName()`, `getGrade()`.
- **Member Functions:** `incrementGrade()`, `decrementGrade()`. (Incrementing grade 3 gives grade 2; throw exceptions if grade goes out of range).
- Thrown exceptions must be catchable using `try` and `catch` blocks (`std::exception & e`).
- **Insertion Operator Overload (`<<`):** `<name>, bureaucrat grade <grade>.`

---

## Exercise 01: Form up, maggots!

- **Directory:** `ex01/`
- **Files to Submit:** Files from previous exercise + `Form.{h, hpp}`, `Form.cpp`
- **Forbidden:** None

### Specification:
- **Form Attributes (all private):**
  - Constant `name` (`std::string`)
  - `isSigned` (`bool`, initialized to `false`)
  - Constant `gradeToSign` (`int`)
  - Constant `gradeToExecute` (`int`)
- Form grades follow Bureaucrat rules (1-150). Throws `Form::GradeTooHighException` or `Form::GradeTooLowException`.
- **Getters:** for all attributes.
- **Insertion Operator Overload (`<<`):** prints all form information.
- **Member Function `beSigned(Bureaucrat const &)`:** signs the form if bureaucrat's grade <= `gradeToSign`. Otherwise throws `Form::GradeTooLowException`.
- **Bureaucrat Member Function `signForm(Form &)`:** calls `beSigned()`.
  - On success: `<bureaucrat> signed <form>`
  - On failure: `<bureaucrat> couldn’t sign <form> because <reason>.`

---

## Exercise 02: No, you need form 28B, not 28C...

- **Directory:** `ex02/`
- **Files to Submit:** `Makefile`, `main.cpp`, `Bureaucrat.[{h, hpp},cpp]`, `AForm.[{h, hpp},cpp]`, `ShrubberyCreationForm.[{h, hpp},cpp]`, `RobotomyRequestForm.[{h, hpp},cpp]`, `PresidentialPardonForm.[{h, hpp},cpp]`
- **Forbidden:** None

### Specification:
- Base class `Form` becomes abstract class `AForm`. Attributes remain private in base class.
- **Concrete Classes:**
  1. `ShrubberyCreationForm` (Required grades: sign 145, exec 137): Creates a file `<target>_shrubbery` in the working directory and writes ASCII trees inside it.
  2. `RobotomyRequestForm` (Required grades: sign 72, exec 45): Makes drilling noises, 50% chance of successfully robotomizing `<target>`.
  3. `PresidentialPardonForm` (Required grades: sign 25, exec 5): Informs that `<target>` has been pardoned by Zaphod Beeblebrox.
- All take 1 parameter in constructor: `target` (e.g. `"home"`).
- **Member Function `execute(Bureaucrat const & executor) const`:** Checks that form is signed and executor grade is high enough, otherwise throws an exception.
- **Bureaucrat Member Function `executeForm(AForm const & form)`:**
  - On success: `<bureaucrat> executed <form>`
  - On failure: prints explicit error message.

---

## Exercise 03: At least this beats coffee-making

- **Directory:** `ex03/`
- **Files to Submit:** Files from previous exercises + `Intern.{h, hpp}`, `Intern.cpp`
- **Forbidden:** None

### Specification:
- Class `Intern`: has no name, grade, or unique characteristics.
- **Member Function `makeForm(std::string const & formName, std::string const & target)`:**
  - Returns a pointer to an `AForm` object corresponding to `formName` initialized with `target`.
  - Prints: `Intern creates <form>` on success.
  - Prints explicit error message if `formName` does not exist.
  - **Requirement:** Avoid unreadable/messy `if/else if/else` chains (use dispatch tables / function pointers).
