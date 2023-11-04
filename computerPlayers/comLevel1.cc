#include "comLevel1.h"
#include "../move.h"
#include <stdlib.h>

Move ComLevel1::calcMove(PieceColour colourToPlay,  BoardModel& board) {
    std::vector<std::shared_ptr<Position>> froms = board.piecesOfColor(colourToPlay);
    int index = rand() % froms.size();
    Position from = *froms[index].get();
    std::vector<std::shared_ptr<Position>> tos = board.getPieces().at(from)->getValidMoves();
    while (tos.size() == 0) {
        index = rand() % froms.size();
        from = *froms[index].get();
        tos = board.getPieces().at(from)->getValidMoves();
    }
    int ind = rand() % tos.size();
    Position to =  *tos[ind].get();
    Move m{board.getBoardstate().infoAtPosition(from), from, to, board.getBoardstate().infoAtPosition(to) };
    return m;
}
