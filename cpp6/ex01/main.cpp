#include "Serializer.hpp"
#include <iostream>

int main() {
    Data original;
    original.label = "answer";
    original.value = 42;

    std::cout << "Original address : " << &original << std::endl;

    uintptr_t raw = Serializer::serialize(&original);
    std::cout << "Serialized value : " << raw << std::endl;

    Data* restored = Serializer::deserialize(raw);
    std::cout << "Restored address : " << restored << std::endl;

    if (&original == restored)
        std::cout << "OK: deserialize(serialize(ptr)) == ptr" << std::endl;
    else
        std::cout << "KO: pointers do not match" << std::endl;

    std::cout << "Restored data    : label=\"" << restored->label
               << "\" value=" << restored->value << std::endl;

    return 0;
}
