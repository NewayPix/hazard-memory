#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>
#include <string>

class Exception : public std::runtime_error {
public:
    Exception(const std::string& message) : std::runtime_error(message) {}
};

#endif // EXCEPTION_HPP
