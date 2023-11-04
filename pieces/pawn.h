#ifndef _PAWN_H
#define _PAWN_H
#include "../piece.h"

class Pawn : public Piece {
  bool enPassantLastMove;
  bool possibleToCaptureENPassent;
public:
Pawn(PieceInfo pinfo, Position pos);
  void updateValidMoves(const BoardState&) override;
  bool didEnPassent() const;
  bool isPossibleEnPassentCapture() const;
  void setPosition(const Position&) override;
};


#endif
