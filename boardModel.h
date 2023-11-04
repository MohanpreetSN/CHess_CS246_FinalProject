#ifndef BOARD_MODEL_H
#define BOARD_MODEL_H
#include <bits/stdc++.h>

#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "boardState.h"
#include "move.h"
#include "subject.h"
class Piece;

enum class GameState { Active, BlackWins, WhiteWins, BlackInCheck, WhiteInCheck, StaleMate };

class BoardModel : public Subject {
    std::map<Position, std::shared_ptr<Piece>> pieces;
    // BoardState bst; bst is in Subject
    // std::vector<Move> previousMoves;
    GameState gstate;
    void handlePromotion(Position to);
    static std::string posToString(const Position& pos); 
    std::vector<Position> getInBetweenPositions(Position, Position);

    /**
     * check if move is castling move or not
     * if move is not castling move, it does not change state
     * if move is valid castling move, perfrom castling
     * if move is invalid castling move, throw exception
     * must call after performing that king is not in check 
     * returns true if the method has performed castling(modified boardState)
     */
    bool handleCastlingMove(const Position&, const Position&);
    

private:
    /**
     * check that the boardState contains exactly one white
     * king and exactly one black king; that no pawns are on the first or
     * last row of the board; and that neither king is in check.
     * if that is not true raise an exception, so controller can deal with it
     * @pre: pieces are correctly initialized
     */
    void validateNewBoard();
    int pieceToValue(PieceType pt);
    /**
     *  helper for castling function
     *  if all in between pieces in row are empty returns true
     *  also checks if castling causes check 
     *  @param: postions are not same and in same row
     *  first pos param is postion of king
     */
    bool isInBetweenPiecesEmpty(const Position&, const Position&, PieceColour) const;
    bool isDanger(Position & pos,PieceColour myc);

public:
    void movePiece(Position from, Position to);
    void init(const BoardState& bs);
    BoardState getBoardstate() const;
    GameState getGameState() const;

    std::vector<std::shared_ptr<Position>> piecesOfColor(PieceColour pc);

    std::map<Position, std::shared_ptr<Piece>> getPieces() const;
    Position getKingPos(const PieceColour&) const;

    bool isCheck(PieceColour col);
    bool isCheckOnMove(PieceColour col, const Position&, const Position&) const;
    bool isCheckMate(PieceColour col);
    
    void calcMove1(PieceColour colourToPlay);
    void calcMove2(PieceColour colourToPlay);
    void calcMove3(PieceColour colourToPlay);
    void calcMove4(PieceColour colourToPlay);
    void moveHelp(Position from);

    ~BoardModel();
    friend class ComputerPlayer;
    // void Undo();
};

#endif
