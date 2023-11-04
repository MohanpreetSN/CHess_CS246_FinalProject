// Rook.h
#ifndef _ROOK_H
#define _ROOK_H
#include "../piece.h"
class Board; class Position;

class Rook : public Piece {
  void checkAcrossMoves(const BoardState&, int x, int y);
public:
  Rook(PieceInfo pinfo, Position pos);
  void updateValidMoves(const BoardState&) override;
};

#endif
