#ifndef DATA_HPP
#define DATA_HPP

#include <string>

// The subject just asks for a "non-empty" struct (i.e. one that actually
// holds data members) so that serialize()/deserialize() have something
// real to point at. A struct with public members is enough here: there
// is no invariant to protect, so we don't need private fields + getters.
struct Data {
    std::string label;
    int         value;
};

#endif
