#include <math.h>

#include <boost/algorithm/string.hpp>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "controller.h"
#include "exception.h"
#include "graphicsDisplay/graphicsException.h"
using namespace std;

PlayerType stringToPlayerType(const string &player) {
    if (player == "human") return PlayerType::human;
    if (player == "computer1") return PlayerType::comLev1;
    if (player == "computer2") return PlayerType::comLev2;
    if (player == "computer3") return PlayerType::comLev3;
    if (player == "computer4") return PlayerType::comLev4;

    throw InvalidInputException("Incorrect player type: " + player);
}

pair<int, int> stringToPos(const string &pos, int boardSize) {
    if (pos.length() != 2) throw InvalidInputException("Invalid position" + pos);

    int file = pos[0] - '`';
    int rank = pos[1] - '0';
    if (rank < 1 || rank > boardSize) throw InvalidInputException("Invalid rank: " + pos);
    if (file < 1 || file > boardSize) throw InvalidInputException("Invalid file: " + pos);
    return make_pair(rank, file);
}

PieceColour stringToColour(const string &colour) {
    if (colour == "white") return PieceColour::White;
    if (colour == "black") return PieceColour::Black;

    throw InvalidInputException("Invalid Colour: " + colour);
}

PieceType stringToPiece(const string &piece) {
    if (piece == "q") return PieceType::Queen;
    if (piece == "k") return PieceType::King;
    if (piece == "p") return PieceType::Pawn;
    if (piece == "n") return PieceType::Knight;
    if (piece == "r") return PieceType::Rook;
    if (piece == "b") return PieceType::Bishop;

    throw InvalidInputException("Invalid piece: " + piece);
}

int main(int argc, char **argv) {
    bool enableGraphics = true;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    int boardSize = 8;
    // interpt command line arguments
    for (int i = 0; i < argc; ++i) {
        if (string(argv[i]) == "-text") {
            enableGraphics = false;
        } else if (string(argv[i]) == "-seed" && ++i < argc) {
            try {
                seed = stoi(string(argv[i]));

            } catch (std::invalid_argument &e) {
                cerr << e.what() << std::endl;
                return 1;
            } catch (std::out_of_range &e) {
                cerr << e.what() << std::endl;
                return -1;
            }  // catch
        }  // end else if
    }  // end for

    auto chessController = make_unique<Controller>(enableGraphics, seed, boardSize);
    string cmd;
    string firstParam, secondParam;

    while (cin >> cmd) {
        try {
            boost::algorithm::to_lower(cmd);
            if (cmd == "game") {
                cin >> firstParam >> secondParam;
                boost::algorithm::to_lower(firstParam);
                boost::algorithm::to_lower(secondParam);
                chessController->startNewGame(stringToPlayerType(firstParam),
                                              stringToPlayerType(secondParam));
            } else if (cmd == "setup") {
                chessController->enterSetupMode();
            } else if (cmd == "move") {
                // white player is bot and it is their tuen
                if (chessController->isWhitesTurn() &&
                    chessController->getPlayer1Type() != PlayerType::human) {
                    chessController->makeBotMove();

                    // black player is bot and it is their tuen
                } else if (!chessController->isWhitesTurn() &&
                           chessController->getPlayer2Type() != PlayerType::human) {
                    chessController->makeBotMove();
                } else {
                    string place;
                    cin >> firstParam;
                    cin >> secondParam;
                    boost::algorithm::to_lower(firstParam);
                    boost::algorithm::to_lower(secondParam);
                    chessController->handleMove(stringToPos(firstParam, boardSize),
                                                stringToPos(secondParam, boardSize));
                }

            } else if (cmd == "resign") {
                if (chessController->isWhitesTurn())
                    cout << "White resigned the game" << endl;
                else
                    cout << "Black resigned the game" << endl;

                chessController->quitCurrentGame(/*resigned*/ true);
            } else if (cmd == "help") {
                cin >> firstParam;
                boost::algorithm::to_lower(firstParam);
                chessController->helpMove(stringToPos(firstParam, boardSize));
            }

            else if (cmd == "+") {
                cin >> firstParam;
                cin >> secondParam;
                boost::algorithm::to_lower(firstParam);
                boost::algorithm::to_lower(secondParam);

                chessController->placePiece(stringToPos(secondParam, boardSize),
                                            stringToPiece(firstParam));
            } else if (cmd == "-") {
                cin >> firstParam;
                boost::algorithm::to_lower(firstParam);

                chessController->removePiece(stringToPos(firstParam, boardSize));
            } else if (cmd == "=") {
                cin >> firstParam;
                boost::algorithm::to_lower(firstParam);

                chessController->switchPieceColour(stringToColour(firstParam));
            } else if (cmd == "done") {
                chessController->exitSetupMode();
            }

        } catch (InvalidInputException &e) {
            cerr << e.what() << endl;
        } catch (GraphicsException &e) {
            cerr << e.what() << endl;
        }
    }

    // print final score
    cout << "Final Score:" << endl;
    float wScore = chessController->getWhitePoints();
    float bScore = chessController->getBlackPoints();
    auto printScore = [](float score) {
        // if score is int
        if (ceil(score) == score)
            cout << (int)score << endl;
        else
            cout << fixed << showpoint << setprecision(2) << score << endl;
    };

    cout << "White: ";
    printScore(wScore);
    cout << "Black: ";
    printScore(bScore);

    return 0;
}
