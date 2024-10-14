#ifndef AI_H
#define AI_H
#include "Board.h"



int negamax(Board currBoard, int depth, int alpha, int beta);
// seperate function from minimax as minimax returns optimal score, where we want optimal move
int solve(Board currBoard, int depth, int alpha, int beta);

#endif
