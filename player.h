#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <stdlib.h>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    std::vector<Move*> getLegalMoves(Board *cBoard, Side side);
    Move *pickFirstAvailableMove();
    Move *randomMove(std::vector<Move*> moveList);
    Move *spaceValueHeuristic(std::vector<Move*> moveList);
    bool isEdge(Move *currentMove);
    bool isCorner(Move *currentMove);
    bool isAdjCorner(Move *currentMove);
    bool isCaddy(Move *currentMove);
    double miniMax(Board *currBoard, int depth, Move* bestMove, bool maxPlayer, Side side);
    double piecesScore(Board *daBoard, Side side);
    double mobilityScore(Board *daBoard, Side side);
    double earlyMidGameScore(Board *daBoard, Side side);
    double lateGameScore(Board *daBoard, Side side);
    double cornerScore(Board *daBoard);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side mySide;
    Side oppoSide;
    Board *gBoard;
    int count;

};

#endif
