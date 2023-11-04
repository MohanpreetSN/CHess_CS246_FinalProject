#include "boardState.h"

bool BoardState::isValidMove(Position pos, PieceInfo pinfo) const {
    PieceColour colr = pinfo.colour;
    int x = pos.getRank();
    int y = pos.getFile();
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }
    if (isSquareEmpty(pos)) {
        return true;
    }
    if (bstate[x][y].colour == colr) {
        return false;
    }
    return true;
}

bool BoardState::isSquareEmpty(const Position& pos) const {
    int x = pos.getRank();
    int y = pos.getFile();
    return bstate[x][y].type == PieceType::Empty;
}

bool BoardState::isEnemyPiece(const Position& pos, const PieceInfo& info) const {
    int x = pos.getRank();
    int y = pos.getFile();
    return bstate[x][y].type != PieceType::Empty && bstate[x][y].colour != info.colour;
}

PieceInfo BoardState::infoAtPosition(const Position & pos) const {
    return bstate[pos.getRank()][pos.getFile()];
}

BoardState& BoardState::operator=(const BoardState& other) {
    bstate.clear();
    if (this != &other) {
        for (size_t i = 0; i < other.bstate.size(); i++) {
            std::vector<PieceInfo> rank;
            for (size_t j = 0; j < other.bstate.size(); j++) {
                rank.emplace_back(other.bstate[i][j].type, other.bstate[i][j].colour);
            }
            this->bstate.emplace_back(rank);
        }
    }
    return *this;
}
