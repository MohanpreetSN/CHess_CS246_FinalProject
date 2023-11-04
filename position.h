#ifndef _POSITION_H
#define _POSITION_H

class Position {
private:
    int x, y;
public:
    Position(int x = 0, int y = 0);
    Position(const Position& other);

    // Methods
    void setPosition(int, int);
    int getRank() const;  // x or row
    int getFile() const;  // y or col
    Position& operator=(const Position&);
    bool operator<(const Position&) const;
    bool operator==(const Position&) const;
};

#endif
