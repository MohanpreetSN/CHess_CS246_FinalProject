#include "knight.h"
#include "../boardState.h"

Knight::Knight(PieceInfo pinfo, Position pos) : Piece{pinfo, pos} {}

void Knight::checkMove(const BoardState & bs,int m, int n) {
    Position move{getPosition().getRank()+m, getPosition().getFile()+n};
    if (bs.isValidMove(move, info)) { 
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile())); 
        if(bs.isEnemyPiece(move, info)) {
            captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
        }
    }
}

void Knight::updateValidMoves(const BoardState & bs) {
    validMoves.clear();
    captureMoves.clear();
    checkMove(bs, 2, 1);
    checkMove(bs, 2, -1);
    checkMove(bs, -2, 1);
    checkMove(bs, -2, -1);
    checkMove(bs, 1, 2);
    checkMove(bs, -1, 2);
    checkMove(bs, 1, -2);
    checkMove(bs, -1, -2);
}

