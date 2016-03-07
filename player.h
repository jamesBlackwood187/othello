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
    std::vector<Move> getLegalMoves(Side side);
    Move *pickRandomMove(std::vector<Move> legalMoves);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side mySide;
    Side oppoSide;
    Board *inBoard;

};

#endif
