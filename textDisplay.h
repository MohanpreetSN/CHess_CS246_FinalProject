#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <iostream>
#include <string>

#include "observer.h"
class TextDisplay : public Observer {
    std::vector<std::string> theDisplay;
    int boardSize;

public:
    TextDisplay(int boardSize = 8);
    void notify(const BoardState &bs) override;
    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
