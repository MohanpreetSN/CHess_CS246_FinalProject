#include "controller.h"

#include <vector>

#include "boardModel.h"
#include "boardState.h"
#include "exception.h"
#include "graphicsDisplay/graphicsDisplay.h"
#include "iostream"
#include "textDisplay.h"
using namespace std;

Controller::Controller(bool graphicsMode, int seed, int boardSize)
    : graphicsMode{graphicsMode},
      seed{seed},
      boardSize{boardSize},
      whosTurn{PieceColour::White},
      state{ControllerState::IDEAL},
      whitePoints{0.0f},
      blackPoints{0.0f},
      usingCustomBoard{false},
      textDisplay{make_unique<TextDisplay>()},
      graphicsDisplay{graphicsMode ? make_unique<GraphicsDisplay>() : nullptr},
      board{make_unique<BoardModel>()},
      currentColour{PieceColour::White} {
    setDefaultBoardConfiguration();
}

Controller::~Controller() {}

void Controller::startNewGame(PlayerType player1, PlayerType player2) {
    if (state == ControllerState::InSetupMode)
        throw InvalidInputException("Cannnot start game in setup mode");
    if (state == ControllerState::InGame)
        throw InvalidInputException("Cannot start new game without finishing current game");

    player1Type = player1;
    player2Type = player2;
    state = ControllerState::InGame;

    // initialize board:
    // if we were using custom board, we
    // initialized to check validity of board config

    if (!usingCustomBoard) board->init(setupboardState);
    board->attach(textDisplay.get());
    if (graphicsMode) board->attach(graphicsDisplay.get());
    board->notifyObservers();
    // print text view
    displayTextView();
}

void Controller::handleMove(pair<int, int> iniPos, pair<int, int> finalPos) {
    if (state != ControllerState::InGame)
        throw InvalidInputException("Cannot move when players are not in game");
    PieceInfo fromInfo =
        board->getBoardstate().infoAtPosition(Position{iniPos.first - 1, iniPos.second - 1});
    
    // First sanity check that piece can be moved

    if (fromInfo.type == PieceType::Empty) throw InvalidInputException("No piece at this postion");
    if (whosTurn != fromInfo.colour) throw InvalidInputException("Cannot move opponent's piece");

    board->movePiece(Position{iniPos.first - 1, iniPos.second - 1},
                     Position{finalPos.first - 1, finalPos.second - 1});
    board->notifyObservers();
    displayTextView();

    switch (board->getGameState()) {
        case GameState::BlackInCheck:
            cout << "Black Player is in Check" << endl;
            break;
        case GameState::WhiteInCheck:
            cout << "White Player is in Check" << endl;
            break;
        case GameState::BlackWins:
            cout << "Black Player Wins!" << endl;
            blackPoints += 1;
            return quitCurrentGame();
            break;
        case GameState::WhiteWins:
            whitePoints += 1;
            cout << "White Player Wins!" << endl;
            return quitCurrentGame();
        case GameState::StaleMate:  // stalemate
            cout << "StaleMate!" << endl;
            blackPoints += 0.5f;
            whitePoints += 0.5f;
            return quitCurrentGame();
            break;
        case GameState::Active:
            break;
    }

    switchTurn();

    // TODO: we will call this->handleMove
    // need to integrate computerplayer into controller
    // they will return next move information
    // afterwards we just call this->handleMove

    // if (player2Type != PlayerType::human) {
    //     makeBotMove();
    // }
}

void Controller::makeBotMove() {
    std::cout << "computer turn " << std::endl;
    if (player1Type == PlayerType::comLev1) {
        board->calcMove1(whosTurn);
    } else if (player1Type == PlayerType::comLev2) {
        board->calcMove2(whosTurn);
    } else if (player1Type == PlayerType::comLev3) {
        board->calcMove3(whosTurn);
    } else if (player1Type == PlayerType::comLev4) {
        board->calcMove4(whosTurn);
    }
    else if (player2Type == PlayerType::comLev1) {
        board->calcMove1(whosTurn);
    } else if (player2Type == PlayerType::comLev2) {
        board->calcMove2(whosTurn);
    } else if (player2Type == PlayerType::comLev3) {
        board->calcMove3(whosTurn);
    } else if (player2Type == PlayerType::comLev4) {
        board->calcMove4(whosTurn);
    }
    board->notifyObservers();
    displayTextView();
    switch (board->getGameState()) {
        case GameState::BlackInCheck:
            cout << "Black Player is in Check" << endl;
            break;
        case GameState::WhiteInCheck:
            cout << "White Player is in Check" << endl;
            break;
        case GameState::BlackWins:
            cout << "Black Player Wins!" << endl;
            blackPoints += 1;
            return quitCurrentGame();
            break;
        case GameState::WhiteWins:
            whitePoints += 1;
            cout << "White Player Wins!" << endl;
            return quitCurrentGame();
        case GameState::StaleMate:  // stalemate
            cout << "StaleMate!" << endl;
            blackPoints += 0.5f;
            whitePoints += 0.5f;
            return quitCurrentGame();
            break;
        case GameState::Active:
            break;
    }
    switchTurn();
}

bool Controller::isWhitesTurn() const { return whosTurn == PieceColour::White; }

void Controller::switchTurn() {
    whosTurn = whosTurn == PieceColour::White ? PieceColour::Black : PieceColour::White;
}

PlayerType Controller::getPlayer1Type() const { return player1Type; }
PlayerType Controller::getPlayer2Type() const { return player2Type; }

void Controller::quitCurrentGame(bool resigned) {
    if (resigned) {
        whosTurn == PieceColour::White ? blackPoints += 1 : whitePoints += 1;
    }
    state = ControllerState::IDEAL;
    board->detach(textDisplay.get());
    whosTurn = PieceColour::White;
    if (graphicsMode) board->detach(graphicsDisplay.get());
}

void Controller::enterSetupMode() {
    clearBoard();
    state = ControllerState::InSetupMode;
    currentColour = PieceColour::White;
    displayTextView();
    if (graphicsMode) graphicsDisplay->notify(setupboardState);
}

void Controller::exitSetupMode() {
    if (state != ControllerState::InSetupMode) throw InvalidInputException("Not in setup mode");
    bool wasBoardValid = true;
    try {
        board->init(setupboardState);
    } catch (InvalidInputException& e) {
        cerr << e.what() << endl;
        wasBoardValid = false;
    }
    // only change state of controller if board was valid
    // this allows user to correct invalid boardState
    if (wasBoardValid) {
        usingCustomBoard = true;
        state = ControllerState::IDEAL;
    }
}

void Controller::placePiece(pair<int, int> pos, PieceType piece) {
    if (state != ControllerState::InSetupMode) throw InvalidInputException("Not in setup mode");

    // register command
    setupboardState.bstate[pos.first - 1][pos.second - 1].type = piece;
    setupboardState.bstate[pos.first - 1][pos.second - 1].colour = currentColour;

    // update displays
    textDisplay->notify(setupboardState);
    displayTextView();
    if (graphicsMode) graphicsDisplay->notify(setupboardState);
}
void Controller::removePiece(pair<int, int> pos) {
    if (state != ControllerState::InSetupMode) throw InvalidInputException("Not in setup mode");

    // register command
    setupboardState.bstate[pos.first - 1][pos.second - 1].type = PieceType::Empty;

    // update displays
    textDisplay->notify(setupboardState);
    displayTextView();
    if (graphicsMode) graphicsDisplay->notify(setupboardState);
}
void Controller::switchPieceColour(PieceColour colour) {
    if (state != ControllerState::InSetupMode) throw InvalidInputException("Not in setup mode");
    currentColour = colour;
}

vector<PieceInfo> Controller::generateBackRank(PieceColour colour) {
    vector<PieceInfo> rank;
    rank.emplace_back(PieceType::Rook, colour);
    rank.emplace_back(PieceType::Knight, colour);
    rank.emplace_back(PieceType::Bishop, colour);
    rank.emplace_back(PieceType::Queen, colour);
    rank.emplace_back(PieceType::King, colour);
    rank.emplace_back(PieceType::Bishop, colour);
    rank.emplace_back(PieceType::Knight, colour);
    rank.emplace_back(PieceType::Rook, colour);
    return rank;
}

vector<PieceInfo> Controller::generatePawnRank(PieceColour colour) {
    vector<PieceInfo> rank;
    for (int i = 0; i < boardSize; ++i) {
        rank.emplace_back(PieceType::Pawn, colour);
    }
    return rank;
}

vector<PieceInfo> Controller::generateEmptyRank() {
    vector<PieceInfo> rank;
    for (int i = 0; i < boardSize; ++i) {
        // colour does not matter, if piece is Empty
        rank.emplace_back(PieceType::Empty, PieceColour::White);
    }
    return rank;
}

void Controller::helpMove(std::pair<int, int> pos) {
    if (state != ControllerState::InGame)
        throw InvalidInputException("Cannot get hints when players are not in game");
    board->moveHelp(Position{pos.first - 1, pos.second - 1});
}

void Controller::setDefaultBoardConfiguration() {
    setupboardState.bstate.clear();  // clear the previous state
    setupboardState.bstate.emplace_back(generateBackRank(PieceColour::White));  // first rank
    setupboardState.bstate.emplace_back(generatePawnRank(PieceColour::White));  // second rank
    setupboardState.bstate.emplace_back(generateEmptyRank());  // third rank
    setupboardState.bstate.emplace_back(generateEmptyRank());  // fourth rank
    setupboardState.bstate.emplace_back(generateEmptyRank());  // fifth rank
    setupboardState.bstate.emplace_back(generateEmptyRank());  // sixth rank
    setupboardState.bstate.emplace_back(generatePawnRank(PieceColour::Black));  // seventh rank
    setupboardState.bstate.emplace_back(generateBackRank(PieceColour::Black));  // eigth rank
}

void Controller::clearBoard() {
    for (auto& rank : setupboardState.bstate) {
        for (auto& square : rank) {
            square.type = PieceType::Empty;
        }
    }
}

void Controller::displayTextView() { cout << *textDisplay << endl; }

float Controller::getWhitePoints() const { return whitePoints; }
float Controller::getBlackPoints() const { return blackPoints; }
