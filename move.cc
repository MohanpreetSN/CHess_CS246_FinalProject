#include "move.h"

Move::Move(PieceInfo movedPiece, Position initialPos, Position finalPos, PieceInfo attackedPiece):
            movedPiece{movedPiece}, initialPos{initialPos}, 
            finalPos{finalPos}, attackedPiece{attackedPiece} {}
