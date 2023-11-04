#ifndef BSTATE_H
#define BSTATE_H
#include <vector>

#include "pieceInfo.h"
#include "position.h"

struct BoardState {
    std::vector<std::vector<PieceInfo>> bstate;
    bool isValidMove(Position pos, PieceInfo pinfo) const;
    bool isSquareEmpty(const Position & pos) const;
    bool isEnemyPiece(const Position & pos, const PieceInfo &info) const;
    PieceInfo infoAtPosition(const Position & pos) const;
    BoardState & operator=(const BoardState & other);
};

#endif
