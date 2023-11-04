#ifndef _PIECE_INFO_H
#define _PIECE_INFO_H
#include <vector>
enum class PieceColour { Black = 0, White };
enum class PieceType { King = 0, Queen, Bishop, Rook, Knight, Pawn, Empty };

struct PieceInfo {
    PieceType type;
    PieceColour colour;
    PieceInfo(PieceType type, PieceColour colour) : type{type}, colour{colour} {}
    PieceInfo(const PieceInfo & other) : type{other.type}, colour{other.colour}{}
    PieceInfo() : type{PieceType::Empty}, colour{PieceColour::White} {}
};

#endif
