#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <string>
class InvalidInputException {
    const std::string s;

public:
    InvalidInputException(std::string);
    std::string what();
};

#endif
