// Move.h
#ifndef _MOVE_H
#define _MOVE_H
#include "pieceInfo.h"
#include "position.h"

struct Move {
    PieceInfo movedPiece;
    Position initialPos;
    Position finalPos;
    PieceInfo attackedPiece;
    
    Move(PieceInfo movedPiece, Position initialPos, Position finalPos, PieceInfo attackedPiece);
};

#endif
