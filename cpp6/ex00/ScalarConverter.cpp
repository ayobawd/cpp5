#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>
#include <climits>
#include <limits>

// Everything below is a free function with internal (file-only) linkage,
// kept out of the class on purpose: the subject says ScalarConverter must
// contain only ONE static method, "convert". These helpers are just the
// private machinery that method leans on.
namespace {

enum LiteralType { TYPE_CHAR, TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, TYPE_INVALID };

// NaN is the only value in IEEE-754 that is not equal to itself.
// That quirk is a portable way to test for it in plain C++98,
// without depending on the (C99/C++11-only) std::isnan.
bool isNan(double value) {
    return value != value;
}

bool isInf(double value) {
    return value == std::numeric_limits<double>::infinity()
        || value == -std::numeric_limits<double>::infinity();
}

// Figures out which of the 4 kinds of literal `literal` is, and, when it
// is numeric, stores its value in `value` (always as a double: a double
// can hold every int, float, char and the pseudo-literals without loss,
// so it is a convenient common currency between the 4 branches below).
LiteralType detectType(const std::string& literal, double& value) {
    // 1) The six pseudo-literals mentioned in the subject must be
    //    matched first and exactly, before any generic parsing.
    if (literal == "nanf") {
        value = std::numeric_limits<double>::quiet_NaN();
        return TYPE_FLOAT;
    }
    if (literal == "+inff") {
        value = std::numeric_limits<double>::infinity();
        return TYPE_FLOAT;
    }
    if (literal == "-inff") {
        value = -std::numeric_limits<double>::infinity();
        return TYPE_FLOAT;
    }
    if (literal == "nan") {
        value = std::numeric_limits<double>::quiet_NaN();
        return TYPE_DOUBLE;
    }
    if (literal == "+inf") {
        value = std::numeric_limits<double>::infinity();
        return TYPE_DOUBLE;
    }
    if (literal == "-inf") {
        value = -std::numeric_limits<double>::infinity();
        return TYPE_DOUBLE;
    }

    // 2) A char literal is exactly one character that is NOT a digit
    //    (a lone digit, like "0", is an int -- see the subject example).
    if (literal.length() == 1 && !std::isdigit(static_cast<unsigned char>(literal[0]))) {
        value = static_cast<double>(literal[0]);
        return TYPE_CHAR;
    }

    char* end;

    // 3) No '.' and no 'f' -> try to read it as a plain int.
    //    We parse it with strtod (not strtol) on purpose: a literal like
    //    "2147483648" is valid *decimal notation*, it is simply too big to
    //    fit an int. Parsing it as a double lets printInt() below say
    //    "impossible" for that one type while float/double still show the
    //    real value, instead of rejecting the whole literal.
    if (literal.find('.') == std::string::npos && literal.find('f') == std::string::npos) {
        double asDouble = std::strtod(literal.c_str(), &end);
        if (!literal.empty() && *end == '\0') {
            value = asDouble;
            return TYPE_INT;
        }
        return TYPE_INVALID;
    }

    // 4) Ends with 'f' -> float, e.g. "4.2f". The part before the 'f'
    //    must still contain a '.' and parse fully as a number.
    if (literal[literal.length() - 1] == 'f') {
        std::string body = literal.substr(0, literal.length() - 1);
        if (body.find('.') == std::string::npos)
            return TYPE_INVALID;
        double asDouble = std::strtod(body.c_str(), &end);
        if (!body.empty() && *end == '\0') {
            value = asDouble;
            return TYPE_FLOAT;
        }
        return TYPE_INVALID;
    }

    // 5) Whatever is left with a '.' in it -> double, e.g. "4.2".
    double asDouble = std::strtod(literal.c_str(), &end);
    if (!literal.empty() && *end == '\0') {
        value = asDouble;
        return TYPE_DOUBLE;
    }
    return TYPE_INVALID;
}

// std::cout prints 42.0 as "42", dropping the ".0". That is correct for
// a double variable, but wrong for a literal we want to *display* as a
// float/double: "42.0f" must keep looking like a floating literal, so we
// glue the ".0" back on whenever it got stripped.
std::string formatFloating(double value) {
    std::ostringstream oss;
    oss << value;
    std::string text = oss.str();
    bool looksInteger = text.find('.') == std::string::npos
        && text.find("inf") == std::string::npos
        && text.find("nan") == std::string::npos;
    if (looksInteger)
        text += ".0";
    return text;
}

void printChar(double value) {
    bool outOfRange = value < static_cast<double>(std::numeric_limits<char>::min())
        || value > static_cast<double>(std::numeric_limits<char>::max());
    if (isNan(value) || isInf(value) || outOfRange) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    char c = static_cast<char>(value);
    if (std::isprint(static_cast<unsigned char>(c)))
        std::cout << "char: '" << c << "'" << std::endl;
    else
        std::cout << "char: Non displayable" << std::endl;
}

void printInt(double value) {
    if (isNan(value) || isInf(value) || value < INT_MIN || value > INT_MAX)
        std::cout << "int: impossible" << std::endl;
    else
        std::cout << "int: " << static_cast<int>(value) << std::endl;
}

void printFloat(double value) {
    if (isNan(value))
        std::cout << "float: nanf" << std::endl;
    else if (isInf(value))
        std::cout << "float: " << (value > 0 ? "+inff" : "-inff") << std::endl;
    else
        std::cout << "float: " << formatFloating(static_cast<float>(value)) << "f" << std::endl;
}

void printDouble(double value) {
    if (isNan(value))
        std::cout << "double: nan" << std::endl;
    else if (isInf(value))
        std::cout << "double: " << (value > 0 ? "+inf" : "-inf") << std::endl;
    else
        std::cout << "double: " << formatFloating(value) << std::endl;
}

} // namespace

void ScalarConverter::convert(const std::string& literal) {
    double value = 0;
    LiteralType type = detectType(literal, value);

    if (type == TYPE_INVALID) {
        std::cout << "char: impossible" << std::endl;
        std::cout << "int: impossible" << std::endl;
        std::cout << "float: impossible" << std::endl;
        std::cout << "double: impossible" << std::endl;
        return;
    }

    // Whatever the literal's own type was, we always print ALL FOUR
    // conversions: that is the whole point of the exercise. Each cast
    // below is a static_cast, the module's chosen cast for this exercise,
    // because these are safe, related-numeric-type conversions known at
    // compile time -- exactly what static_cast is for.
    printChar(value);
    printInt(value);
    printFloat(value);
    printDouble(value);
}
