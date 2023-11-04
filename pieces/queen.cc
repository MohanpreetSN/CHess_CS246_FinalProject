#include "queen.h"
#include "../position.h"
#include "../boardState.h"
#include <iostream>

Queen::Queen(PieceInfo pinfo, Position pos) : Piece{pinfo, pos} {}

void Queen::checkDiagonalMoves(const BoardState& board, int dx, int dy) {
  Position move{getPosition().getRank()+dx, getPosition().getFile()+dy};

  while(board.isValidMove(move, info)) {
    validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    //std::cout << "can move queen to x=" << move.getRank() << " y=" << move.getFile() << std::endl;
    if(board.isEnemyPiece(move, info)) {
      captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
      break; // can't jump over enemy piece
    }
    move.setPosition(move.getRank()+dx, move.getFile()+dy);
  }
}

void Queen::updateValidMoves(const BoardState& board) {
  validMoves.clear(); // ** add to superclass ?
  captureMoves.clear();
  // move up + right diagonally
  checkDiagonalMoves(board, 1, 1);
  // down + right
  checkDiagonalMoves(board, 1, -1);
  // down + left 
  checkDiagonalMoves(board, -1, -1);
  // up + left 
  checkDiagonalMoves(board, -1, 1);
  //now check across moves
  checkDiagonalMoves(board, 1, 0);
  checkDiagonalMoves(board, -1, 0);
  checkDiagonalMoves(board, 0, 1);
  checkDiagonalMoves(board, 0, -1);
}
