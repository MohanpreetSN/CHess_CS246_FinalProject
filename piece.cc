#include "piece.h"
#include <iostream>

Piece::Piece(PieceInfo pinfo, Position pos)
    : info{pinfo}, position{pos}, moved{false}, alive{true} {}

const std::vector<std::shared_ptr<Position>>& Piece::getValidMoves() const { return validMoves; }
const std::vector<std::shared_ptr<Position>>& Piece::getValidCaptureMoves() const { return captureMoves; }

bool Piece::isValidMove(const Position& finalPos) const {
    // ** here, might help to convert valid moves to map (can still iterate if necessary)
    // ** also, for later bots, have a priority queue for each of the valid moves ?
    //std::cout << "checking if can move to (" << finalPos.getRank() << ", "  << finalPos.getFile() << std::endl;
    for (auto move : validMoves) {
        //std::cout << "checking valid move (" << move->getRank() << ", "  << move->getFile() << std::endl;
        if (*move.get() == finalPos) return true;
    }
    return false;
} 

void Piece::setPosition(const Position& pos) { moved = true; position.setPosition(pos.getRank(), pos.getFile()); }

Position Piece::getPosition() const { return position; }

bool Piece::isAlive() const { return alive; }

void Piece::setAlive(const bool& alive) { this->alive = alive; }

bool Piece::hasMoved() const { return moved; }

PieceColour Piece::getColour() const { return info.colour; }
PieceType Piece::getType() const { return info.type; }
PieceInfo Piece::getInfo() const { return info; }
