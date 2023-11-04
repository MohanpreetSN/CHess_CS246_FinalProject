#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "../pieceInfo.h"
#include "../position.h"


struct Move {
    PieceInfo movedPiece;
    Position initialPos;
    Position finalPos;
    PieceInfo attackedPiece;
    
    Move(PieceInfo movedPiece, Position initialPos, Position finalPos, PieceInfo attackedPiece);
};

struct BoardModel;
class ComputerPlayer {
    virtual Move calcMove(PieceColour colourToPlay, BoardModel& board) = 0;
};
#endif

