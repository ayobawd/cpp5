# C++ - Module 06: C++ casts

## Chapter I: Introduction

C++ is a general-purpose programming language created by Bjarne Stroustrup as an extension of the C programming language, often referred to as "C with Classes" (source: Wikipedia).

The goal of these modules is to introduce you to Object-Oriented Programming.

This will be the starting point of your C++ journey. Many languages are recommended for learning OOP, but we chose C++ since it is derived from your old friend, C. As C++ is a complex language, your code will adhere to the C++98 standard to keep things simple.

We acknowledge that modern C++ differs significantly in many aspects. If you want to become a proficient C++ developer, it will be up to you to explore further beyond the 42 Common Core!

---

## Chapter II: General Rules

### Compiling
- Compile your code with `c++` and the flags `-Wall -Wextra -Werror`.
- Your code should still compile if you add the flag `-std=c++98`.

### Formatting and Naming Conventions
- The exercise directories will be named this way: `ex00`, `ex01`, ..., `exn`.
- Write class names in **UpperCamelCase** format. Files containing class code are named after the class: `ClassName.hpp`, `ClassName.cpp`.
- Every output message must end with a newline character and go to standard output.
- No coding style (Norminette) is enforced in the C++ modules — but code your peer evaluators can't understand is code they can't grade.

### Allowed / Forbidden
- You may use almost everything from the standard library. Prefer the C++ versions of C functions you already know.
- No other external library: **C++11 (and derived forms) and Boost are forbidden.**
- `*printf()`, `*alloc()` and `free()` are forbidden — using them means an automatic **0**.
- Unless stated otherwise, `using namespace <ns>` and `friend` are forbidden (penalty: **-42**).
- The STL (containers like `vector`/`list`/`map`, and anything from `<algorithm>`) is **only allowed starting Module 08** (penalty: **-42**).

### Design Requirements
- Avoid memory leaks whenever you use `new`.
- From Module 02 to Module 09, classes must be designed in the **Orthodox Canonical Form (OCF)**, unless explicitly stated otherwise.
- Any function body written in a header file (other than templates) means **0** for the exercise.
- Every header must be includable on its own (include guards, and all the dependencies it needs).

---

## Chapter III: Additional Rule (mandatory for the whole module)

> For each exercise, type conversion must be handled using a **specific type of casting**. Your choice will be reviewed during the defense.

This is the whole point of Module 06: C++ replaces the C-style cast `(type)value` — which silently does *whatever it takes* to convert, hiding your intent and your mistakes — with four explicit, purpose-built casts:

| Cast              | Used for                                                              | Used in     |
|-------------------|------------------------------------------------------------------------|-------------|
| `static_cast`     | Conversions between related types, known and checked at compile time (`int` ↔ `float`, base ↔ derived when you're sure) | `ex00` |
| `reinterpret_cast`| Reinterpreting the *same bits* as an unrelated type (pointer ↔ integer, pointer ↔ pointer) | `ex01` |
| `dynamic_cast`    | Safe downcasting through a polymorphic hierarchy, checked **at runtime** | `ex02` |
| `const_cast`      | Adding/removing `const` (not needed in this module)                    | — |

See `EXPLANATIONS.md` in this folder for a full walkthrough of *why* each exercise below uses the cast it uses.

---

## Chapter IV: AI Instructions (summary)

This module exists to build your own foundational reasoning, through genuine effort, repetition, and peer-learning — not to produce an answer for you to hand in unexplained. During the defense you must be able to explain **every** line and the cast choice behind it; during exams there is no AI to fall back on. Use this repository's code as a *worked, annotated reference* to study from — not as something to submit without understanding.

---

## Exercise 00: Conversion of scalar types

- **Directory:** `ex00/`
- **Files to Submit:** `Makefile`, `*.cpp`, `*.{h, hpp}`
- **Authorized:** any function that converts a string to an `int`, `float`, or `double` (helps, but isn't the whole job).

Write a class `ScalarConverter` with a single **static** method `convert(literal)`. It takes a string representation of a C++ literal (in its most common form) and prints its value converted to all four scalar types: `char`, `int`, `float`, `double`.

- The class stores nothing, so it must **not be instantiable**.
- Except for `char`, only decimal notation is used.
- Char examples: `c`, `a`, ... — non-displayable characters are not valid input; if a *conversion to* char isn't displayable, print an informative message instead.
- Int examples: `0`, `-42`, `42`...
- Float examples: `0.0f`, `-4.2f`, `4.2f`... plus the pseudo-literals `-inff`, `+inff`, `nanf`.
- Double examples: `0.0`, `-4.2`, `4.2`... plus the pseudo-literals `-inf`, `+inf`, `nan`.

If a conversion is nonsensical or overflows, say so explicitly instead of printing garbage.

```
./convert 0
char: Non displayable
int: 0
float: 0.0f
double: 0.0

./convert nan
char: impossible
int: impossible
float: nanf
double: nan

./convert 42.0f
char: '*'
int: 42
float: 42.0f
double: 42.0
```

---

## Exercise 01: Serialization

- **Directory:** `ex01/`
- **Files to Submit:** `Makefile`, `*.cpp`, `*.{h, hpp}`
- **Forbidden:** None

Implement a class `Serializer`, not instantiable by the user, with two **static** methods:

```cpp
uintptr_t serialize(Data* ptr);     // pointer -> integer
Data*     deserialize(uintptr_t raw); // integer -> pointer
```

Write a program that creates a non-empty `Data` struct, serializes its address, deserializes it back, and checks that the round-tripped pointer compares equal to the original. Turn in the `Data` struct's files too.

---

## Exercise 02: Identify real type

- **Directory:** `ex02/`
- **Files to Submit:** `Makefile`, `*.cpp`, `*.{h, hpp}`
- **Forbidden:** `std::typeinfo` (the `<typeinfo>` header)

Implement a class `Base` with only a **public virtual destructor**, and three empty classes `A`, `B`, `C` that publicly inherit from `Base`. These four classes are exempt from the Orthodox Canonical Form.

```cpp
Base* generate(void);       // randomly instantiates A, B or C, returned as Base*
void  identify(Base* p);    // prints "A", "B" or "C": the actual type behind p
void  identify(Base& p);    // same, but for a reference -- no pointer allowed inside
```

Including `<typeinfo>` is forbidden — even though `identify(Base&)` needs `dynamic_cast` on a reference, which throws on failure.

---

## Chapter VIII: Submission and Peer Evaluation

Submit to your Git repository as usual — only what's inside it is evaluated. During the defense, be ready for a short, timed modification request (a small behavior change, a few lines to add/rewrite) to prove you actually understand your own code, not just that it compiles.
