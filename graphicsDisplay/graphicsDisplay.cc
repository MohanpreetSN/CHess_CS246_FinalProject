#include "graphicsDisplay.h"

#include <iostream>
GraphicsDisplay::GraphicsDisplay() { win = std::make_unique<Xwindow>(boardWidth, boardHeight); }

void GraphicsDisplay::notify(const BoardState& state) {
    win->drawXpmImage(0, 0, boardImg, false);
    int i = 0;
    for (auto it = state.bstate.rbegin(); it != state.bstate.rend(); it++) {
        for (size_t j = 0; j < it->size(); ++j) {
            PieceInfo pInfo = (*it)[j];
            if (pInfo.type == PieceType::Empty) continue;
            const std::string& imgPath = getImagePath(pInfo);

            int y = offSetY + i * squareLength;
            int x = offsetX + j * squareLength;
            if (pInfo.type == PieceType::King || pInfo.type == PieceType::Bishop) x += offsetRK;
            if (pInfo.type == PieceType::Queen) x += offsetQ;

            win->drawXpmImage(x, y, imgPath, /*transparent*/ true);
        }
        ++i;
    }
}

const std::string& GraphicsDisplay::getImagePath(const PieceInfo& piece) {
    switch (piece.type) {
        case PieceType::Bishop:
            return piece.colour == PieceColour::Black ? bBishopImg : wBishopImg;
        case PieceType::Rook:
            return piece.colour == PieceColour::Black ? bRookImg : wRookImg;
        case PieceType::Knight:
            return piece.colour == PieceColour::Black ? bKnightImg : wKnightImg;
        case PieceType::Queen:
            return piece.colour == PieceColour::Black ? bQueenImg : wQueenImg;
        case PieceType::King:
            return piece.colour == PieceColour::Black ? bKingImg : wKingImg;
        default: //PieceType::Pawn
            return piece.colour == PieceColour::Black ? bPawnImg : wPawnImg;
    }
}
