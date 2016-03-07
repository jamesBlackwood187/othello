#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    inBoard = new Board();
    mySide = side;
    if (mySide == BLACK) {
    	oppoSide = WHITE;
    }
    else {
        oppoSide = BLACK;
    }
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    inBoard->doMove(opponentsMove, oppoSide);
    std::vector<Move> legalMoves = getLegalMoves(mySide);
    if(legalMoves.size() == 0) { // check if no legal moves
    	return NULL; // pass
    }
    else {
        Move *chosenMove = this->pickRandomMove(legalMoves);
        inBoard->doMove(chosenMove, mySide);
        return chosenMove;
    }
}

/*
 * Gets a vector of all legal moves for the given board configuration
 */
std::vector<Move> Player::getLegalMoves(Side side) {
    std::vector<Move> moveList;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *cMove = new Move(i,j);
            if (inBoard->checkMove(cMove, side) == true) {
                moveList.push_back(*cMove);
            }
            delete cMove;
        }
    }
    return moveList;
}



Move *Player::pickRandomMove(std::vector<Move> legalMoves) {
	int randomIndex = rand() % legalMoves.size();
	Move *myMove= &legalMoves[randomIndex];
	return myMove;
} 