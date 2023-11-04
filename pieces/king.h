#ifndef KING_H
#define KING_H

#include "../piece.h"
class Board; class Position;

class King : public Piece {
    void checkMove(const BoardState & bs,int m, int n);
public:
    King(PieceInfo pinfo, Position pos);
    void updateValidMoves(const BoardState &bs) override;
};


#endif
