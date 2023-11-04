#ifndef _GRAPHICS_DISPLAY_H
#define _GRAPHICS_DISPLAY_H

#include <memory>
#include <string>

#include "../observer.h"
#include "window.h"
class GraphicsDisplay : public Observer {
    std::unique_ptr<Xwindow> win;

public:
    GraphicsDisplay();
    void notify(const BoardState&) override;

private:
    /**
     * return imagePath given pieceinformaton
     * @pre: PieceInfo.type is not PieceType::Empty
     */
    const std::string& getImagePath(const PieceInfo&);

private:
    // image path
    std::string boardImg = "graphicsDisplay/img/Board.xpm";
    std::string wPawnImg = "graphicsDisplay/img/WhitePawn.xpm";
    std::string wRookImg = "graphicsDisplay/img/WhiteRook.xpm";
    std::string wBishopImg = "graphicsDisplay/img/WhiteBishop.xpm";
    std::string wKnightImg = "graphicsDisplay/img/WhiteKnight.xpm";
    std::string wKingImg = "graphicsDisplay/img/WhiteKing.xpm";
    std::string wQueenImg = "graphicsDisplay/img/WhiteQueen.xpm";

    std::string bPawnImg = "graphicsDisplay/img/BlackPawn.xpm";
    std::string bRookImg = "graphicsDisplay/img/BlackRook.xpm";
    std::string bBishopImg = "graphicsDisplay/img/BlackBishop.xpm";
    std::string bKnightImg = "graphicsDisplay/img/BlackKnight.xpm";
    std::string bKingImg = "graphicsDisplay/img/BlackKing.xpm";
    std::string bQueenImg = "graphicsDisplay/img/BlackQueen.xpm";
    // size of display window
    int boardWidth = 1006;
    int boardHeight = 1015;

    // variable used to place pieces
    int offsetX = 40;
    int offSetY = 10;
    // x offset for special pieces (Bishop, King)
    int offsetRK = -8;
    // x offset for special piece Queen
    int offsetQ = -12;
    int squareLength = 120;
};

#endif
