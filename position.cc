#include "position.h"

Position::Position(int x, int y): x{x}, y{y} {}

Position::Position(const Position & other) : x{other.x}, y{other.y} {}

void Position::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

int Position::getRank() const { return x; }

int Position::getFile() const { return y; }

bool Position::operator<(const Position& rhs) const {
    return x < rhs.x || (x ==rhs.x && y < rhs.y);
}

bool Position::operator==(const Position& rhs) const { return x == rhs.x && y == rhs.y; }

Position& Position::operator=(const Position &other) {
    x = other.x;
    y = other.y;
    return *this;
}
