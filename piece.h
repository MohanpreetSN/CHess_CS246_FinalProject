// Piece.h
#ifndef _PIECE_H
#define _PIECE_H
#include <memory>
#include <vector>

#include "pieceInfo.h"
#include "position.h"

class BoardState;

class Piece {
protected:
    PieceInfo info;
    Position position;
    bool moved;
    bool alive;
   

protected:
    std::vector<std::shared_ptr<Position>> validMoves;
    std::vector<std::shared_ptr<Position>> captureMoves;

public:
    // methods
    Piece(PieceInfo pinfo, Position pos);
    virtual void updateValidMoves(const BoardState&) = 0;
    const std::vector<std::shared_ptr<Position>>& getValidMoves() const;
    const std::vector<std::shared_ptr<Position>>& getValidCaptureMoves() const;
    bool isValidMove(const Position&) const;

    virtual void setPosition(const Position&);
    Position getPosition() const;
    bool isAlive() const;
    void setAlive(const bool&);
    bool hasMoved() const;
    PieceColour getColour() const;
    PieceType getType() const;
    PieceInfo getInfo() const;
};

#endif
