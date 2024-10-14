#include <stdio.h>
#include "AI.h"

// try better moves first for alpha beta pruning
// more central moves are usually better
static const int move_order[7] = {3,4,2,5,1,6,0};
extern int nodes;

// alpha is best score current play can get
// beta is best score opponent can get
int negamax(Board currBoard, int depth, int alpha, int beta)
{
    nodes++;
    if(depth == 0 || currBoard.moves == BOARD_HEIGHT*BOARD_WIDTH)
    {
        // draw
        return 0;
    }
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        if(Board_legalMove(&currBoard, i) && Board_isWinningMove(&currBoard, i))
        {
            return (BOARD_HEIGHT*BOARD_WIDTH+1 - currBoard.moves)/2;
        }
    }
    


    // we can't win on this turn so this is upper bound of score
    int max = (BOARD_HEIGHT*BOARD_WIDTH-1 - currBoard.moves)/2;
    if(beta > max)
    {
        beta = max;
        if(alpha >= beta)
        {
            return beta;
        }
    }
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        int move = move_order[i];
        if(!Board_legalMove(&currBoard, move))
        {
            continue;
        }
        Board newBoard = currBoard;
        Board_playMove(&newBoard, move);
        // score will be opposite of opponents score
        // alpha and beta will be switched, as turn changes
        int score = -negamax(newBoard, depth-1, -beta, -alpha);
        if(score >= beta)
        {
            return score;
        }
        if(score > alpha)
        {
            alpha = score;
        }
    }
    return alpha;
}

int solve(Board currBoard, int depth, int alpha, int beta)
{
    nodes++;
    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        if(Board_legalMove(&currBoard, i) && Board_isWinningMove(&currBoard, i))
        {
            return i;
        }
    }
    int bestMove = 0;
    int max = (BOARD_HEIGHT*BOARD_WIDTH-1 - currBoard.moves)/2;
    if(beta > max)
    {
        beta = max;
        if(alpha >= beta)
        {
            return beta;
        }
    }

    for(int i = 0; i < BOARD_WIDTH; i++)
    {
        int move = move_order[i];
        if(!Board_legalMove(&currBoard, move))
        {
            continue;
        }
        Board newBoard = currBoard;

        Board_playMove(&newBoard, move);
        // score will be opposite of opponents score
        // alpha and beta will be switched, as turn changes

        int score = -negamax(newBoard, depth-1, -beta, -alpha);
        if(score >= beta)
        {
            return move;
        }
        if(score > alpha)
        {
            alpha = score;
            bestMove = move;
        }
    }


    return bestMove;
}



