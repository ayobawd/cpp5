#include "Serializer.hpp"

// A pointer and a uintptr_t are the exact same bits, just labeled with
// two different types (one is "an address of a Data", the other is
// "a plain number"). reinterpret_cast is the tool made for exactly that:
// reinterpreting the same bit pattern as a different, unrelated type,
// with no conversion of the underlying value. That is why this exercise
// uses reinterpret_cast, unlike ex00's static_cast (which converts
// between related *numeric* types, actually changing the value).
uintptr_t Serializer::serialize(Data* ptr) {
    return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serializer::deserialize(uintptr_t raw) {
    return reinterpret_cast<Data*>(raw);
}
