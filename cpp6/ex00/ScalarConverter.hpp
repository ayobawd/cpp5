#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>

// ScalarConverter has no data to store: it only groups the "convert"
// function together. So nobody should ever be able to write
// `ScalarConverter sc;` — that is why every special member function
// below is private. They are declared but never defined on purpose:
// since the class itself never calls them either, they are never
// needed, so the linker never complains about the missing body.
class ScalarConverter {
private:
    ScalarConverter();
    ScalarConverter(const ScalarConverter& other);
    ScalarConverter& operator=(const ScalarConverter& other);
    ~ScalarConverter();

public:
    static void convert(const std::string& literal);
};

#endif
