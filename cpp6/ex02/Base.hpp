#ifndef BASE_HPP
#define BASE_HPP

// The subject only asks for a public virtual destructor. The destructor
// MUST be virtual: without it, dynamic_cast would still work on the
// pointer overload, but deleting an A/B/C object through a Base* would
// be undefined behaviour (only Base's destructor would run).
class Base {
public:
    virtual ~Base();
};

#endif
