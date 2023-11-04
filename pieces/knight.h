#ifndef KNIGHT_H
#define KNIGHT_H

#include "../piece.h"
class Board; class Position;

class Knight : public Piece {
    void checkMove(const BoardState & bs,int m, int n);
public:
    Knight(PieceInfo pinfo, Position pos);
    void updateValidMoves(const BoardState & bs) override;
};


#endif
