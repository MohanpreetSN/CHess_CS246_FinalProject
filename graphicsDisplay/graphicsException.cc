#include "graphicsException.h"

GraphicsException::GraphicsException(std::string s) : s{s} {}
std::string GraphicsException::what() { return s; }
