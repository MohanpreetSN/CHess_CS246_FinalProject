#include "exception.h"

InvalidInputException::InvalidInputException(std::string s) : s{s} {}
std::string InvalidInputException::what() { return s; }