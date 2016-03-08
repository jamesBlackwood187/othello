#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
   
    gBoard = new Board();
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
	delete gBoard;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove( must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    gBoard->doBoardMove(opponentsMove, oppoSide);
    Move *chosenMove = NULL;
    int score = 0;

    if(!gBoard->hasMoves(mySide)) {
    	return chosenMove;
    }   

    std::vector<Move*> moveVec = getLegalMoves(gBoard, mySide);
    
    /* minimax testing algorithm */ 
    if (testingMinimax) {
        score = miniMax(gBoard, 2, NULL, true, mySide);
        return gBoard->bestMove;
    }
    else {
    	/**** Choose method below for heuristic+algo *****/
        
        /* Uncomment below to use simple algo that prioritized corners and edges */
        // chosenMove = spaceValueHeuristic(moveVec); 
        // return chosenMove;

        score = miniMax(gBoard, 3, NULL, true, mySide);
        fprintf(stderr, "Current projected score: %d\n", score);
        gBoard->doBoardMove(gBoard->bestMove, mySide);
        return gBoard->bestMove;
    }
}

/*
 * Gets a vector of all legal moves for the given board configuration for a given side
 */
std::vector<Move*> Player::getLegalMoves(Board *cBoard, Side side) {
    std::vector<Move*> moveList;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *cMove = new Move(i,j);
            if (cBoard->checkMove(cMove, side) == true) {
                moveList.push_back(cMove);
            }
        }
    }
    return moveList;
}


/* Picks the first available move */ 
Move *Player::pickFirstAvailableMove() {
	for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *cMove = new Move(i,j);
            if (gBoard->checkMove(cMove, mySide)) {
                return cMove;
            }
        }
    }
    return NULL;
} 


/* Picks a random move from a set of moves */ 
Move *Player::randomMove(std::vector<Move*> moveList) {
	if (moveList.size() == 0) {
		return NULL;
	}
	int randomIndex = rand() % moveList.size();
	Move *rMove = moveList[randomIndex];
	return rMove;
}


/* 
 * Simple AI to beat SimplePlayer. Looks at available moves and picks the best square.s
 *
 * We rank squares on the board according to:
 *
 * Corners have a score of 2
 * Edges have a score of 1
 * Edges adjacent to a corner have a score of -1
 * Diagonal to corners have score -2
 * All other squares have score 0
 * The function goes through the passed set of legal moves and simply
 * selects the square with the highest score.
 *
 * This heuristic can be used to beat Simple Player in most games.
 */
Move *Player::spaceValueHeuristic(std::vector<Move*> moveList) {
	int maxScore = -10;
	Move* bestMove = NULL;
    
    // Iterate through move list and evaluate a move score
	for(unsigned int i = 0; i < moveList.size(); i++) {
        Move *currMove = moveList[i];
        if(isEdge(currMove)) {// check is move is on an edge
        	if(isCorner(currMove)) { //additional check for corner move
                currMove->score = 2;
        	}
        	else if(isAdjCorner(currMove)) { // check if adjacent edge to corner
                currMove->score = -1;
        	}
        	else { // normal edge
                currMove->score = 1;
        	}
        }
        else if(isCaddy(currMove)) { // is diagonal to corner, aka worst square on the board
            currMove->score = -2;
        } 
        else { // square is "standard"
        	currMove->score = 0;
        }

        // check if this move is better than our current best move
        if (currMove->score > maxScore) {
        	bestMove = currMove;
        	maxScore = currMove->score;
        }
    }
    return bestMove;
}

/* Checks if a move is on an edge */
bool Player::isEdge(Move *currentMove) {
	int x = currentMove->x;
	int y = currentMove->y;
    return (x == 0) || (y == 0) || (x == 7) ||  (y == 7);
}

/* checks if a move is on a corner */
bool Player::isCorner(Move *currentMove) {
	int x = currentMove->x;
	int y = currentMove->y;
    return (x == 0 && y == 0) || (x == 7 && y == 0) || (x == 0 && y == 7) || (x == 7 && y == 7);
}

/* Checks if a move is directly adjacent to corner */ 
bool Player::isAdjCorner(Move *currentMove) {
	int x = currentMove->x;
	int y = currentMove->y;
    return (x == 0 && y == 1) || (x == 1 && y == 0) || (x == 0 && y == 6) || (x == 6 && y == 0) || 
           (x == 7 && y == 1) || (x == 1 && y == 7) || (x == 7 && y == 6) || (x == 6 && y == 7);
}

/* Checks if move is diagonal to a corner */
bool Player::isCaddy(Move *currentMove) {
	int x = currentMove->x;
	int y = currentMove->y;
    return (x == 1 && y == 1) || (x == 6 && y == 1) || (x == 1 && y == 6) || (x == 6 && y == 6);
}

/* 
 * Minimax algorithm.
 * Refered to https://en.wikipedia.org/wiki/Minimax for pseudocode
 * Implements a basic minimax algorithm to a desired depth.
 *
 */
int Player::miniMax(Board *currBoard, int depth, Move* bestMove, bool maxPlayer, Side side) {
    std::vector<Move*> moveSet = getLegalMoves(currBoard, side);
    
    // Check if depth is reached or we are at a terminal node
    if (depth == 0 || moveSet.size() == 0) {
    	return 4 * mobilityScore(currBoard, mySide) + 3 * piecesScore(currBoard, mySide); // scoring heuristic can be changed here
    }

    if(maxPlayer) { // maximizing player 
    	int bestVal = -1000;
    	// For each move in move set, make the move then evaluate outcome
        for(unsigned int j = 0; j < moveSet.size(); j++) {
        	Board *childBoard = currBoard->copy(); 
        	childBoard->doBoardMove(moveSet[j],mySide); // make current move
        	int val = miniMax(childBoard, depth - 1, bestMove, false, oppoSide);
        	// compare to current best node
        	if(val > bestVal) {
        		bestVal = val;
        		bestMove = moveSet[j];
        		currBoard->bestMove = bestMove; // set the "node's" best move
        	}
        	delete childBoard;
        }
        currBoard->bestMove = bestMove;
        return bestVal;
    }

    else { // minimizing player, similar approach as maximizing player
    	int bestVal = 1000;
        for(unsigned int j = 0; j < moveSet.size(); j++) {
        	Board *childBoard = currBoard->copy();
        	childBoard->doBoardMove(moveSet[j],oppoSide);
        	int val = miniMax(childBoard, depth - 1, bestMove, true, mySide);
        	if(val < bestVal) {
        		bestVal = val;
        		bestMove = moveSet[j];
        		currBoard->bestMove = bestMove;
        	}
        	delete childBoard;
        }
        return bestVal;
    }

}

/* Scoring heuristic based on difference in pieces on the board */
int Player::piecesScore(Board *daBoard, Side side) {
    if(side == BLACK) {
    	return daBoard->countBlack() - daBoard->countWhite();
    }
    else {
        return daBoard->countWhite() - daBoard->countBlack();
    }
}

/* Scoring heuristic based on number of available moves for each player */
int Player::mobilityScore(Board *daBoard, Side side) {
    	std::vector<Move *> myMoves = getLegalMoves(daBoard, mySide);
    	std::vector<Move *> oppoMoves = getLegalMoves(daBoard, oppoSide);
    	return (myMoves.size() - oppoMoves.size());
    
}