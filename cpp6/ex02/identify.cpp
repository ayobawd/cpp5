#include "identify.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>
#include <iostream>

// Picks A, B or C at random and hands it back as a Base*. The caller only
// ever sees "a Base", the concrete type is hidden -- that is exactly the
// situation identify() below has to untangle at runtime.
Base* generate(void) {
    switch (std::rand() % 3) {
        case 0:
            return new A();
        case 1:
            return new B();
        default:
            return new C();
    }
}

// dynamic_cast<A*>(p) returns a real pointer if p truly points to an A
// (or something derived from A), and NULL otherwise. No header/RTTI
// dance needed for the pointer form: a failed cast simply gives NULL,
// which `if` checks directly.
void identify(Base* p) {
    if (dynamic_cast<A*>(p))
        std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p))
        std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p))
        std::cout << "C" << std::endl;
    else
        std::cout << "Unknown" << std::endl;
}

// A reference can never be NULL, so dynamic_cast<A&>(p) can't fail
// "silently" the way the pointer version does: on a mismatch it throws
// std::bad_cast instead. That type lives in <typeinfo>, which this
// exercise forbids us from including -- so we never name it. We just
// catch(...) ("catch anything") around each attempt, which needs no
// header at all, and move on to the next candidate type on failure.
void identify(Base& p) {
    try {
        (void)dynamic_cast<A&>(p);
        std::cout << "A" << std::endl;
        return;
    } catch (...) {}

    try {
        (void)dynamic_cast<B&>(p);
        std::cout << "B" << std::endl;
        return;
    } catch (...) {}

    try {
        (void)dynamic_cast<C&>(p);
        std::cout << "C" << std::endl;
        return;
    } catch (...) {}

    std::cout << "Unknown" << std::endl;
}
