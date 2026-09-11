#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

// uintptr_t is an unsigned integer type guaranteed to be wide enough to
// hold a memory address. It comes from the C header stdint.h (available
// even under -std=c++98, since it ships with the C library, not the C++
// standard library).
#include <stdint.h>
#include "Data.hpp"

// Same idea as ScalarConverter: Serializer only groups two static
// functions, so it must never be instantiated.
class Serializer {
private:
    Serializer();
    Serializer(const Serializer& other);
    Serializer& operator=(const Serializer& other);
    ~Serializer();

public:
    static uintptr_t serialize(Data* ptr);
    static Data*      deserialize(uintptr_t raw);
};

#endif
