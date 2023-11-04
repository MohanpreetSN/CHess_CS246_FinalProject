#ifndef _QUEEN_H
#define _QUEEN_H
#include "../piece.h"

class Queen : public Piece {
private:
  void checkDiagonalMoves(const BoardState&, int x, int y);
public:
Queen(PieceInfo pinfo, Position pos);
  void updateValidMoves(const BoardState&) override;
};


#endif
