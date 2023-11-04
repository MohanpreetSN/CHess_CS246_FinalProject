#include "king.h"
#include "../boardState.h"

King::King(PieceInfo pinfo, Position pos) : Piece{pinfo, pos} {}

void King::checkMove(const BoardState & bs,int m, int n) {
    Position move{getPosition().getRank()+m, getPosition().getFile()+n};
    if (bs.isValidMove(move, info)) { 
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile())); 
        // ** these are not neccessarily valid captures (or even valid moves): board has to check whether 
        // these moves will cause check for our colour
        // it's the same for pieces other than king
        if(bs.isEnemyPiece(move, info)) captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    }
}

void King::updateValidMoves(const BoardState & bs) {
    validMoves.clear();
    captureMoves.clear();
    checkMove(bs, 1, 0);
    checkMove(bs, -1, 0);
    checkMove(bs, 0, 1);
    checkMove(bs, 0, -1);
    checkMove(bs, 1, 1);
    checkMove(bs, 1, -1);
    checkMove(bs, -1, 1);
    checkMove(bs, -1, 1);
}
