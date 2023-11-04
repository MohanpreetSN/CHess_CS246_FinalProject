#ifndef COMPUTER_LEVEL2_H
#define COMPUTER_LEVEL2_H

#include "../pieceInfo.h"

struct Move;
struct BoardModel;
class ComLevel2 {
    virtual Move calcMove(PieceColour colourToPlay, BoardModel& board) = 0;
};

#endif
