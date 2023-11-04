#ifndef _GRAPHICS_EXCEPTION_H_
#define _GRAPHICS_EXCEPTION_H_

#include <string>
class GraphicsException {
    const std::string s;

public:
    GraphicsException(std::string);
    std::string what();
};



#endif
