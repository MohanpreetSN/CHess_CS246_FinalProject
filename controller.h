#ifndef _CONTROLLER_H
#define _CONTROLLER_H
#include <memory>
#include <utility>

#include "boardModel.h"
#include "boardState.h"
#include "pieceInfo.h"

class TextDisplay;
class GraphicsDisplay;

enum PlayerType { human = 0, comLev1, comLev2, comLev3, comLev4 };
enum ControllerState { IDEAL = 0, InGame, InSetupMode };

/**
 * Controller class contorls the chess game and communicates with
 * client to simulate game of chess
 */
class Controller {
private:
    bool graphicsMode;
    int seed;
    int boardSize;
    PlayerType player1Type;
    PlayerType player2Type;
    PieceColour whosTurn;
    ControllerState state;
    float whitePoints;
    float blackPoints;
    bool usingCustomBoard;

private:
    std::unique_ptr<TextDisplay> textDisplay;
    std::unique_ptr<GraphicsDisplay> graphicsDisplay;
    std::unique_ptr<BoardModel> board;

public:
    Controller(bool graphicsMode, int seed, int boardSize = 8);
    ~Controller();

    void startNewGame(PlayerType player1, PlayerType player2);
    /**
     * @pre: 1 <= iniPos.first, iniPos.second <= BoardSize
     *       1 <= finalPos.first, finalPos.second <= BoardSize
     */
    void handleMove(std::pair<int, int> iniPos, std::pair<int, int> finalPos);
    void makeBotMove();
    bool isWhitesTurn() const;
    /**
     * perform clean up after game is over
     * setup controller for next game
     */
    void quitCurrentGame(bool resigned = false);
    float getWhitePoints() const;
    float getBlackPoints() const;
    void helpMove(std::pair<int, int> pos);
    PlayerType getPlayer1Type() const;
    PlayerType getPlayer2Type() const;
private:
    void switchTurn();
    

    // Datafields and methods for set up mode
private:
    BoardState setupboardState;
    PieceColour currentColour;  // set up mode

public:
    // setup mode
    void enterSetupMode();
    void exitSetupMode();
    /**
     *  Calling placePiece, removePiece, switchPieceColour when not
     *  set up mode throws InvalidInputException
     * @pre: 1 <= pos.first <= BoardSize
     *       1 <= pos.second <= BoardSize
     */
    void placePiece(std::pair<int, int> pos, PieceType piece);
    void removePiece(std::pair<int, int> pos);
    void switchPieceColour(PieceColour colour = PieceColour::White);

private:
    void setDefaultBoardConfiguration();
    void clearBoard();
    void displayTextView();
    std::vector<PieceInfo> generateBackRank(PieceColour colour);
    std::vector<PieceInfo> generatePawnRank(PieceColour colour);
    std::vector<PieceInfo> generateEmptyRank();
};

#endif