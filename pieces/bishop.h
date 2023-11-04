#ifndef _BISHOP_H
#define _BISHOP_H
#include "../piece.h"

class Bishop : public Piece {
private:
  void checkDiagonalMoves(const BoardState&, int x, int y);
public:
  Bishop(PieceInfo pinfo, Position pos);
  void updateValidMoves(const BoardState&) override;
};


#endif
