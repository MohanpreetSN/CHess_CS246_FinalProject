#include "rook.h"
#include "../position.h"
#include "../boardState.h"

Rook::Rook(PieceInfo pinfo, Position pos) : Piece{pinfo, pos} {}

void Rook::checkAcrossMoves(const BoardState& board, int dx, int dy) {
  Position move{getPosition().getRank()+dx, getPosition().getFile()+dy};

  while(board.isValidMove(move, info)) {
    validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    if(board.isEnemyPiece(move, info)) {
      captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
      break; // can't jump over enemy piece
    }
    move.setPosition(move.getRank()+dx, move.getFile()+dy);
  }
}

void Rook::updateValidMoves(const BoardState& board) {
  validMoves.clear(); // ** add to superclass ?
  captureMoves.clear();
  // move down
  checkAcrossMoves(board, 1, 0);
  // up
  checkAcrossMoves(board, -1, 0);
  // right 
  checkAcrossMoves(board, 0, 1);
  // left 
  checkAcrossMoves(board, 0, -1);
}
