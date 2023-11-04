#ifndef COMPUTER_LEVEL1_H
#define COMPUTER_LEVEL1_H

#include "../pieceInfo.h"
#include "computerPlayer.h"
#include "../move.h"
#include "../boardModel.h"


class BoardModel;
class ComLevel1 : public ComputerPlayer {
    Move calcMove(PieceColour colourToPlay,  BoardModel& board)override;
};

#endif
