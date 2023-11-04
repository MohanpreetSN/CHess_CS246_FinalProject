#ifndef COMPUTER_LEVEL3_H
#define COMPUTER_LEVEL3_H

#include "../pieceInfo.h"

struct Move;
struct BoardModel;
class ComLevel3 {
    virtual Move calcMove(PieceColour colourToPlay, BoardModel& board) = 0;
};

#endif
