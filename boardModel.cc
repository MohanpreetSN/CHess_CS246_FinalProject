#include "boardModel.h"

#include <iostream>

#include "exception.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

std::string BoardModel::posToString(const Position& pos) {
    std::string str;
    str.push_back('`' + (pos.getFile() + 1));
    str.push_back('0' + (pos.getRank() + 1));
    return str;
}

void BoardModel::movePiece(Position from, Position to) {
    if (pieces.find(from) == pieces.end()) {  // check 'from' square NOT empty
        // from postion does not have any piece to move
        // throw exception
        throw InvalidInputException("No Piece on initial position");
    }

    // check valid move : is 'to' in og piece's valid moves?
    pieces[from]->updateValidMoves(bst);
    bool causeCheck = isCheckOnMove(pieces[from]->getColour(), from, to);
    if (!pieces[from]->isValidMove(to) || causeCheck) {
        if (causeCheck)
            std::cout << "Your king is in check!" << std::endl;
        else if (handleCastlingMove(from, to))
            return;
        else
            moveHelp(from);
        throw InvalidInputException("This is an invalid move");
    }

    // info.type is none if moved to an empty square
    // Move curmove{fromPiece->getInfo(), from, to, pieces[to]->getInfo()};
    // previousMoves.push_back(curmove);

    pieces[to] = pieces[from];
    pieces.erase(from);
    pieces[to]->setPosition(to);  // This was causing the error

    bst.bstate[to.getRank()][to.getFile()] = bst.bstate[from.getRank()][from.getFile()];
    PieceInfo empty(PieceType::Empty, PieceColour::White);
    bst.bstate[from.getRank()][from.getFile()] = empty;

    if (pieces[to]->getType() == PieceType::Pawn) {
        if (to.getRank() == 0 || to.getRank() == 7) {
            handlePromotion(to);
        }
    }

    // at the end of moving, update gameState for next player/bot
    // ex. player1 moves, now player2 is in check
    PieceColour enemyCol =
        pieces[to]->getColour() == PieceColour::White ? PieceColour::Black : PieceColour::White;
    if (isCheck(enemyCol)) {
        if (isCheckMate(enemyCol)) {
            if (enemyCol == PieceColour::White)
                gstate = GameState::BlackWins;
            else
                gstate = GameState::WhiteWins;
        } else {
            if (enemyCol == PieceColour::White)
                gstate = GameState::WhiteInCheck;
            else
                gstate = GameState::BlackInCheck;
        }
    } else {
        gstate = GameState::Active;
    }

    notifyObservers();
}

void BoardModel::handlePromotion(Position to) {
    std::cout << "You can promote your pawn, type q for queen, b for bishop, " << std::endl;
    std::cout << "r for rook, n for knight." << std::endl;
    bool wrong = true;
    std::string choice;
    while (wrong) {
        std::cin >> choice;
        PieceColour pc = pieces[to]->getColour();
        if (choice == "q") {
            PieceInfo pi = PieceInfo(PieceType::Queen, pc);
            pieces[to] = std::make_shared<Queen>(pi, to);
            bst.bstate[to.getRank()][to.getFile()] = pi;
            wrong = false;
        } else if (choice == "r") {
            PieceInfo pi = PieceInfo(PieceType::Rook, pc);
            pieces[to] = std::make_shared<Queen>(pi, to);
            bst.bstate[to.getRank()][to.getFile()] = pi;
            wrong = false;
        } else if (choice == "b") {
            PieceInfo pi = PieceInfo(PieceType::Bishop, pc);
            pieces[to] = std::make_shared<Queen>(pi, to);
            bst.bstate[to.getRank()][to.getFile()] = pi;
            wrong = false;
        } else if (choice == "n") {
            PieceInfo pi = PieceInfo(PieceType::Knight, pc);
            pieces[to] = std::make_shared<Queen>(pi, to);
            bst.bstate[to.getRank()][to.getFile()] = pi;
            wrong = false;
        } else {
            std::cout << "Invalid choice, try again" << std::endl;
        }
    }
}

void BoardModel::init(const BoardState& bs) {
    // TODO:: check that bs is valid
    // the board contains exactly one white
    // king and exactly one black king; that no pawns are on the first or
    // last row of the board; and that neither king is in check.
    // if that is not true raise an exception, so controller can deal with it

    int len = static_cast<int>(bs.bstate.size());
    for (int x = 0; x < len; x++) {
        for (int y = 0; y < len; y++) {
            if (bs.bstate[x][y].type == PieceType::Empty) {
                continue;
            } else {
                Position apos{x, y};
                PieceType apt = bs.bstate[x][y].type;
                if (apt == PieceType::Bishop) {
                    auto apiece = std::make_shared<Bishop>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                } else if (apt == PieceType::Pawn) {
                    auto apiece = std::make_shared<Pawn>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                } else if (apt == PieceType::Rook) {
                    auto apiece = std::make_shared<Rook>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                } else if (apt == PieceType::King) {
                    auto apiece = std::make_shared<King>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                } else if (apt == PieceType::Queen) {
                    auto apiece = std::make_shared<Queen>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                } else if (apt == PieceType::Knight) {
                    auto apiece = std::make_shared<Knight>(bs.bstate[x][y], apos);
                    pieces[apos] = apiece;
                }
            }
        }
    }
    bst = bs;
    // check that inital board is valid
    // else throw exception
    validateNewBoard();
}

std::vector<std::shared_ptr<Position>> BoardModel::piecesOfColor(PieceColour pc) {
    std::vector<std::shared_ptr<Position>> posns;
    for (auto x : pieces) {
        if (x.second->getColour() == pc) {
            auto p = std::make_shared<Position>(x.first.getRank(), x.first.getFile());
            posns.push_back(p);
        }
    }
    return posns;
}

std::map<Position, std::shared_ptr<Piece>> BoardModel::getPieces() const { return pieces; }

Position BoardModel::getKingPos(const PieceColour& col) const {
    // find position of king of colour col
    for (auto pieceM : pieces) {
        if (pieceM.second == NULL) continue;
        if (pieceM.second->getType() == PieceType::King && pieceM.second->getColour() == col) {
            return pieceM.first;
        }
    }
    // should not happen
    throw InvalidInputException("King not found");
}

// note : isCheck can be expensive depending on validMoves size
bool BoardModel::isCheck(PieceColour col) {
    /*
        check if the king's current position is 'in sight' of an attacking piece
    */
    Position kingPos = getKingPos(col);
    // check valid moves for ALL pieces of opposite colour
    for (auto pieceM : pieces) {
        // if not col, then must be enemy colour (cannot be none)
        if (pieceM.second->getColour() != col) {
            pieceM.second->updateValidMoves(bst);
            std::vector<std::shared_ptr<Position>> vMoves = pieceM.second->getValidMoves();
            for (auto pos : vMoves) {
                if (*pos.get() == kingPos) return true;
            }
        }
    }
    return false;
}

bool BoardModel::isInBetweenPiecesEmpty(const Position& p1, const Position& p2,
                                        PieceColour col) const {
    // @Pre: p1.getRank() = p2.getRank()
    int iniX = std::min(p1.getFile(), p2.getFile());
    int finX = std::min(p1.getFile(), p2.getFile());

    for (int i = iniX + 1; i <= finX; ++i) {
        if (bst.bstate[p1.getRank()][i].type != PieceType::Empty) return false;
        if (isCheckOnMove(col, p1, Position{p1.getRank(), i}))
            throw InvalidInputException("Castling Error: castling causes king in check");
    }
    return true;
}
bool BoardModel::handleCastlingMove(const Position& from, const Position& to) {
    std::string iniMove = posToString(from);
    std::string finalMove = posToString(to);
    PieceInfo iniPiece = bst.infoAtPosition(from);
    PieceInfo finalPiece = bst.infoAtPosition(to);
    // e1(8) g1(8) -> castle short
    // e1(8) c1(8) -> castle long
    bool castlingShort = true;
    // initial piece must be king
    if (iniPiece.type != PieceType::King) {
        return false;
    }

    // if king is in check
    if (isCheck(finalPiece.colour)) {
        throw InvalidInputException("Cannot Castle: King in check");
    }
    // check that rook is in the correct postion
    if (iniMove == "e1" && iniPiece.colour == PieceColour::White) {
        // move e1 g1 or move e1 c1
        if (finalMove == "g1") {
            if (bst.bstate[from.getRank()][7].type != PieceType::Rook ||
                bst.bstate[from.getRank()][7].colour != PieceColour::White)
                throw InvalidInputException("Cannot Castle: Rook is not at the correct Postion");

            if (pieces[Position{from.getRank(), 7}]->hasMoved()) {
                throw InvalidInputException("Cannot Castle: Rook is moved");
            }
            castlingShort = true;
        } else if (finalMove == "c1") {
            if (bst.bstate[from.getRank()][0].type != PieceType::Rook ||
                bst.bstate[from.getRank()][0].colour != PieceColour::White)
                throw InvalidInputException("Cannot Castle: Rook is not at the correct Postion");

            if (pieces[Position{from.getRank(), 0}]->hasMoved()) {
                throw InvalidInputException("Cannot Castle: Rook is moved");
            }

            castlingShort = false;
        } else {
            return false;
        }

    } else if (iniMove == "e8" && iniPiece.colour == PieceColour::Black) {
        // move e8 g8 or move e8 c8
        if (finalMove == "g8") {
            if (bst.bstate[from.getRank()][7].type != PieceType::Rook ||
                bst.bstate[from.getRank()][7].colour != PieceColour::Black)
                throw InvalidInputException("Cannot Castle: Rook is not at the correct Postion");

            if (pieces[Position{from.getRank(), 7}]->hasMoved()) {
                throw InvalidInputException("Cannot Castle: Rook is moved");
            }
            castlingShort = true;
        }
        if (finalMove == "c8") {
            if (bst.bstate[from.getRank()][0].type != PieceType::Rook ||
                bst.bstate[from.getRank()][0].colour != PieceColour::Black)
                throw InvalidInputException("Cannot Castle: Rook is not at the correct Postion");

            if (pieces[Position{from.getRank(), 0}]->hasMoved()) {
                throw InvalidInputException("Cannot Castle: Rook is moved");
            }
            castlingShort = false;
        } else {
            return false;
        }

    } else {
        return false;
    }

    // check that castling does not cause king to be in check
    if (pieces[from]->hasMoved()) throw InvalidInputException("Invalid Castling: king is moved");
    if (!isInBetweenPiecesEmpty(from, to, iniPiece.colour))
        throw InvalidInputException("Invalid Castling: pieces between king and rook");

    // correct castling move, so we move pieces
    Position rookIni;
    Position rookFinal;
    if (castlingShort) {
        rookIni.setPosition(from.getRank(), 7);
        rookFinal.setPosition(from.getRank(), 5);

    } else {
        rookIni.setPosition(from.getRank(), 0);
        rookFinal.setPosition(from.getRank(), 3);
    }
    // move king
    pieces[to] = pieces[from];
    pieces.erase(from);
    pieces[to]->setPosition(to);
    bst.bstate[to.getRank()][to.getFile()] = bst.bstate[from.getRank()][from.getFile()];
    PieceInfo empty(PieceType::Empty, PieceColour::White);
    bst.bstate[from.getRank()][from.getFile()] = empty;

    // move rook
    pieces[rookFinal] = pieces[rookIni];
    pieces.erase(rookIni);
    pieces[rookFinal]->setPosition(rookFinal);
    bst.bstate[rookFinal.getRank()][rookFinal.getFile()] =
        bst.bstate[rookIni.getRank()][rookIni.getFile()];
    bst.bstate[rookIni.getRank()][rookIni.getFile()] = empty;

    return true;
}

void BoardModel::calcMove1(PieceColour colourToPlay) {
    std::vector<std::shared_ptr<Position>> froms = piecesOfColor(colourToPlay);
    Position to, from;
    do {
        int index = rand() % froms.size();
        from = *froms[index].get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidMoves();
        while (tos.size() == 0) {
            index = rand() % froms.size();
            from = *froms[index].get();
            getPieces().at(from)->updateValidMoves(bst);
            tos = getPieces().at(from)->getValidMoves();
        }
        int ind = rand() % tos.size();
        to = *tos[ind].get();
    } while(isCheckOnMove(colourToPlay, from, to));
    // ** before moving, ensure move doesn't put computer into check, else exception
    movePiece(from, to);
}

void BoardModel::calcMove2(PieceColour colourToPlay) {
    std::vector<std::shared_ptr<Position>> froms = piecesOfColor(colourToPlay);
    PieceColour otherC =
        (colourToPlay == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    for (auto fr : froms) {  // Prefers moves which cause a check
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        for (auto t : tos) {
            Position to = *t.get();
            if (isCheckOnMove(otherC, from, to)) {
                movePiece(from, to);
                return;
            }
        }
    }
    for (auto fr : froms) {  // Prefers moves that capture
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        if (tos.size() == 0) continue;
        int ind = rand() % tos.size();
        Position to = *tos[ind].get();
        movePiece(from, to);
        return;
    }
    calcMove1(colourToPlay);
}

void BoardModel::calcMove3(PieceColour colourToPlay) {
    std::vector<std::shared_ptr<Position>> froms = piecesOfColor(colourToPlay);
    PieceColour otherC =
        (colourToPlay == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    for (auto fr : froms) {  // Prefers moves which cause a check
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        for (auto t : tos) {
            Position to = *t.get();
            if (isCheckOnMove(otherC, from, to) && !isDanger(to, colourToPlay)) {
                movePiece(from, to);
                return;
            }
        }
    }
    for (auto fr : froms) {  // Prefers moves that capture
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        if (tos.size() == 0) continue;
        int ind = rand() % tos.size();
        Position to = *tos[ind].get();
        if (!isDanger(to, colourToPlay)) {
            movePiece(from, to);
            return;
        }
    }
    calcMove2(colourToPlay);
}

void BoardModel::calcMove4(PieceColour colourToPlay) {
    std::vector<std::shared_ptr<Position>> froms = piecesOfColor(colourToPlay);
    PieceColour otherC =
        (colourToPlay == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    for (auto fr : froms) {  // Prefers moves which cause a check
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        for (auto t : tos) {
            Position to = *t.get();
            if (isCheckOnMove(otherC, from, to) && !isDanger(to, colourToPlay)) {
                movePiece(from, to);
                return;
            }
        }
    }
    Position afrom, ato;
    int attackValue = 0;
    for (auto fr : froms) {  // Prefers moves that capture
        Position from = *fr.get();
        getPieces().at(from)->updateValidMoves(bst);
        std::vector<std::shared_ptr<Position>> tos = getPieces().at(from)->getValidCaptureMoves();
        if (tos.size() == 0) continue;
        int ind = rand() % tos.size();
        Position to = *tos[ind].get();
        int a = pieceToValue(getPieces().at(to)->getType());
        if (attackValue == 0 && !isDanger(to, colourToPlay)) {
            attackValue = a;
            afrom = from;
            ato = to;
        } else if (attackValue < a && !isDanger(to, colourToPlay)) {
            attackValue = a;
            afrom = from;
            ato = to;
        }
    }
    if (attackValue == 0) {
        calcMove3(colourToPlay);
    } else {
        movePiece(afrom, ato);
    }
}

// note: this can be used for general moves, ex. does a certain bishop move cause check ?
bool BoardModel::isCheckOnMove(PieceColour col, const Position& p1, const Position& p2) const {
    // i assume that p1 -> is a valid move (comes from validMoves of piece)
    Position kingPos =
        bst.bstate[p1.getRank()][p1.getFile()].type == PieceType::King ? p2 : getKingPos(col);
    BoardState cpyState = bst;

    PieceInfo mPieceInfo = cpyState.bstate[p1.getRank()][p1.getFile()];
    cpyState.bstate[p1.getRank()][p1.getFile()] = PieceInfo();
    cpyState.bstate[p2.getRank()][p2.getFile()] = mPieceInfo;

    // check pieces of opposite colour don't have king in sight
    for (auto pieceM : pieces) {
        // if not col, then must be enemy colour (cannot be none)
        if (pieceM.second->getColour() != col) {
            pieceM.second->updateValidMoves(cpyState);
            std::vector<std::shared_ptr<Position>> vMoves = pieceM.second->getValidCaptureMoves();
            for (auto pos : vMoves) {
                if (*pos.get() == kingPos) return true;
            }
            // re update valid moves to prevent errs
            pieceM.second->updateValidMoves(bst);
        }
    }
    return false;
}

std::vector<Position> BoardModel::getInBetweenPositions(Position p1, Position p2) {
    int dx, dy;
    std::vector<Position> res;
    if (p1.getRank() > p2.getRank())
        dx = -1;  // go down from p1 to get to p2
    else
        dx = p1.getRank() == p2.getRank() ? 0 : 1;

    if (p1.getFile() > p2.getFile())
        dy = -1;  // go down from p1 to get to p2
    else
        dy = p1.getFile() == p2.getFile() ? 0 : 1;
    while (!(p1 == p2)) {
        p1.setPosition(p1.getRank() + dx, p1.getFile() + dy);
        res.push_back(p1);
    }
    return res;
}

bool BoardModel::isCheckMate(PieceColour col) {
    /*
        checkmate if king in check and
        * every possible king move is 'in sight' of an attacking piece or
        * there are no valid king moves
        + for the current check, none of our pieces can CAPTURE or BLOCK attacking piece
        OR  there are >1 attacking pieces

        implementation:
        1 make Move of king, try isCheck, then Undo()
        2 make boardState copy, change 2 positions needed to move king, pass to isValid moves
            ~ custom isCheck for move that hasn't been played
    */

    Position kingPos = getKingPos(col);
    auto king = pieces[kingPos];
    std::vector<Position> attackingPieces;
    // check how many pieces are attacking current king position (check)
    for (auto pieceM : pieces) {
        // if not col, then must be enemy colour (cannot be none)
        if (pieceM.second->getColour() != col) {
            pieceM.second->updateValidMoves(bst);
            std::vector<std::shared_ptr<Position>> vMoves = pieceM.second->getValidCaptureMoves();
            for (auto pos : vMoves) {
                if (*pos.get() == kingPos) attackingPieces.push_back(pieceM.second->getPosition());
            }
        }
    }
    if (attackingPieces.size() == 0) return false;  // not even in check
    king->updateValidMoves(bst);
    auto kValidMoves = king->getValidMoves();
    for (auto mv : kValidMoves) {
        if (!isCheckOnMove(col, kingPos, *mv.get())) {
            return false;
        }
    }  // at end of loop, we know there are no king moves that are not in check
    // we cannot move to other positions without check
    if (attackingPieces.size() > 1) return true;  // too many attacking pieces, checkmate

    // there is exactly 1 attacking piece attackingPiece[0]
    // can we either CAPTURE or BLOCK attacking piece ?
    std::vector<Position> posToBlockOrCapture = getInBetweenPositions(kingPos, attackingPieces[0]);
    for (auto pieceM : pieces) {
        if (pieceM.second->getColour() == col) {
            pieceM.second->updateValidMoves(bst);
            std::vector<std::shared_ptr<Position>> vMoves = pieceM.second->getValidMoves();
            // king is a bit different: we know already that there's no valid moves without check,
            // so ignore
            if (pieceM.second->getType() == PieceType::King) {
                continue;
            }
            for (auto pos : vMoves) {
                for (auto posBlockorCapture : posToBlockOrCapture) {
                    // found a piece with a BlockOrCapture validMove
                    if (*pos.get() == posBlockorCapture) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void BoardModel::moveHelp(Position from) {
    if (pieces.find(from) == pieces.end()) {
        std::cout << "No piece on that position, no possible moves!" << std::endl;
        return;
    }

    if (pieces[from]->getValidMoves().size() == 0) {
        std::cout << "No possible moves for this piece :(";
    }  // update valid move for en passent
    else if (pieces[from]->getType() == PieceType::Pawn) {
        auto pawnPiece = std::dynamic_pointer_cast<Pawn>(pieces[from]);

        for (size_t i = 0; i < pieces[from]->getValidMoves().size(); ++i) {
            auto move = (pieces[from]->getValidMoves())[i];
            if (*move.get() == Position{from.getRank(), from.getFile() + 1}) {
                auto oppPawn = std::dynamic_pointer_cast<Pawn>(pieces[*move.get()]);
                if (!oppPawn->didEnPassent())
                    continue;
            }
            if (*move.get() == Position{from.getRank(), from.getFile() - 1}) {
                auto oppPawn = std::dynamic_pointer_cast<Pawn>(pieces[*move.get()]);
                if (!oppPawn->didEnPassent())
                    continue;
            }
            std::cout << posToString(*move.get()) << "  ";
        }

    } else {
        pieces[from]->updateValidMoves(bst);
        std::cout << "The possible moves for this piece are ";
        for (auto x : pieces[from]->getValidMoves()) {
            std::cout << posToString(*x.get()) << "  ";
        }
        std::cout << std::endl;
    }
}

BoardState BoardModel::getBoardstate() const { return bst; }

GameState BoardModel::getGameState() const { return gstate; }

BoardModel::~BoardModel() {}

void BoardModel::validateNewBoard() {
    int numWhiteKings = 0;
    int numBlackKings = 0;
    for (size_t row = 0; row < bst.bstate.size(); ++row) {
        for (const auto& pInfo : bst.bstate[row]) {
            if ((row == 0 || row == bst.bstate.size() - 1) && pInfo.type == PieceType::Pawn) {
                throw InvalidInputException("pawns are on the first or last row of the board");
            }
            if (pInfo.type == PieceType::King) {
                if (pInfo.colour == PieceColour::White) {
                    // if we see king first time, numWhiteKings = 0
                    // in this case increase count
                    // else throw exception as we have already seen the king
                    numWhiteKings == 0
                        ? ++numWhiteKings
                        : throw InvalidInputException("Cannot have more than one White king");
                } else {
                    numBlackKings == 0
                        ? ++numBlackKings
                        : throw InvalidInputException("Cannot have more than one Black king");
                }
            }
        }
    }
    if (numWhiteKings == 0) throw InvalidInputException("Board is missing white king");
    if (numBlackKings == 0) throw InvalidInputException("Board is missing black king");

    if (isCheck(PieceColour::White) || isCheckMate(PieceColour::White))
        throw InvalidInputException("Invalid board: White is in check");
    if (isCheck(PieceColour::Black) || isCheckMate(PieceColour::Black))
        throw InvalidInputException("Invalid board: Black is in check");
}

int BoardModel::pieceToValue(PieceType pt) {
    if (pt == PieceType::King) {
        return 10;
    } else if (pt == PieceType::Queen) {
        return 8;
    } else if (pt == PieceType::Rook) {
        return 6;
    } else if (pt == PieceType::Bishop) {
        return 5;
    } else if (pt == PieceType::Knight) {
        return 4;
    } else if (pt == PieceType::King) {
        return 1;
    } else {
        return 0;
    }
}

bool BoardModel::isDanger(Position& pos, PieceColour myc) {
    PieceColour otherC = (myc == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    for (auto x : piecesOfColor(otherC)) {
        for (auto y : getPieces().at(*x.get())->getValidCaptureMoves()) {
            if (*y.get() == pos) {
                return true;
            }
        }
    }
    return false;
}
