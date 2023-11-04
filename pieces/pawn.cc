#include "pawn.h"

#include <cmath>

#include "../boardState.h"
#include "../position.h"
Pawn::Pawn(PieceInfo pinfo, Position pos)
    : Piece{pinfo, pos}, enPassantLastMove{false}, possibleToCaptureENPassent{false} {}

bool Pawn::didEnPassent() const { return enPassantLastMove; }

bool Pawn::isPossibleEnPassentCapture() const { return possibleToCaptureENPassent; }

void Pawn::setPosition(const Position& pos) {
    if (enPassantLastMove) enPassantLastMove = false;
    if (std::abs(position.getRank() - pos.getRank()) == 2) {
        enPassantLastMove = true;
    }
    Piece::setPosition(pos);
}
void Pawn::updateValidMoves(const BoardState& board) {
    validMoves.clear();
    captureMoves.clear();
    possibleToCaptureENPassent = false;
    Position move{0, 0};
    int x = getPosition().getRank();
    int y = getPosition().getFile();  // * switch
    int dir = info.colour == PieceColour::Black ? -1 : 1;  // move back if black

    // en passant ONLY if *attacked* pawn has moved once AND moved 2 squares forward

    move.setPosition(x + dir, y + 1);
    if (board.isValidMove(move, info) && board.isEnemyPiece(move, info) &&
        board.infoAtPosition(Position{x, y + 1}).type == PieceType::Pawn) {
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
        captureMoves.push_back(std::make_shared<Position>(move.getRank() - dir, move.getFile()));
        possibleToCaptureENPassent = true;
    }
    move.setPosition(x + dir, y - 1);
    if (board.isValidMove(move, info) && board.isEnemyPiece(move, info) &&
        board.infoAtPosition(Position{x, y + 1}).type == PieceType::Pawn) {
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
        captureMoves.push_back(std::make_shared<Position>(move.getRank() - dir, move.getFile()));
        possibleToCaptureENPassent = true;
    }

    // capture ONLY if there exists ENEMY piece 1 to diagonal (can't be empty)
    move.setPosition(x + dir, y + 1);
    if (board.isValidMove(move, info) && board.isEnemyPiece(move, info)) {
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
        captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    }
    move.setPosition(x + dir, y - 1);
    if (board.isValidMove(move, info) && board.isEnemyPiece(move, info)) {
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
        captureMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    }

    // move forward 1 if square EMPTY (can't capture)
    move.setPosition(x + dir, y);
    if (!board.isValidMove(move, info) || !board.isSquareEmpty(move)) return;
    validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
    // move forward x2 ONLY if moved == false and EMPTY (can't capture)
    move.setPosition(x + 2 * dir, y);
    if (!moved && board.isValidMove(move, info) && board.isSquareEmpty(move))
        validMoves.push_back(std::make_shared<Position>(move.getRank(), move.getFile()));
}
