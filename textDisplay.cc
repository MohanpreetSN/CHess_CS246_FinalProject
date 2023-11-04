#include "textDisplay.h"

#include <ctype.h>

#include <iostream>

#include "pieceInfo.h"
using namespace std;
TextDisplay::TextDisplay(int boardSize) : boardSize{boardSize} {
     for (int i = 0; i < boardSize; i++) {
        if (i % 2 == 0) 
            theDisplay.emplace_back(" _ _ _ _");
         else 
            theDisplay.emplace_back("_ _ _ _ ");
         }
}

void TextDisplay::notify(const BoardState &bs) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            switch (bs.bstate[i][j].type) {
                case PieceType::Bishop:
                    theDisplay[i][j] = 'B';
                    break;
                case PieceType::King:
                    theDisplay[i][j] = 'K';
                    break;
                case PieceType::Knight:
                    theDisplay[i][j] = 'N';
                    break;
                case PieceType::Queen:
                    theDisplay[i][j] = 'Q';
                    break;
                case PieceType::Pawn:
                    theDisplay[i][j] = 'P';
                    break;
                case PieceType::Rook:
                    theDisplay[i][j] = 'R';
                    break;
                default:
                    theDisplay[i][j] = (i + j) % 2 == 0 ? ' ' : '-';
            }

            if (bs.bstate[i][j].colour == PieceColour::Black)
                theDisplay[i][j] = tolower(theDisplay[i][j]);
        }
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    int x = td.boardSize;
    for (auto it = td.theDisplay.rbegin(); it != td.theDisplay.rend(); ++it) {
        std::cout << x << " ";
        for (const auto &square : *it) {
            out << square;
        }
        x--;
        out << endl;
    }
    std::cout << std::endl;
    std::cout << "  abcdefgh";
    return out;
}
