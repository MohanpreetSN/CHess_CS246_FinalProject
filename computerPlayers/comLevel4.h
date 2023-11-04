#ifndef COMPUTER_LEVEL4_H
#define COMPUTER_LEVEL4_H

#include "../pieceInfo.h"

struct Move;
struct BoardModel;
class ComLevel4 {
    virtual Move calcMove(PieceColour colourToPlay, BoardModel& board) = 0;
};

#endif
