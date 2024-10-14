#include <stdio.h>
#include "Board.h"



void BoardDisplay_update(BoardDisplay* b, int screen_width, int screen_height, int monitor_width, int monitor_height)
{
    

    b->checker_buffer_x = (screen_width*FULL_SCREEN_BUFFER_X)/monitor_width;
    b->checker_buffer_y = (screen_height*FULL_SCREEN_BUFFER_Y)/monitor_height;
    b->checker_size_x = (screen_width - b->checker_buffer_x*(BOARD_WIDTH+1))/BOARD_WIDTH;
    b->checker_size_y = (screen_height - b->checker_buffer_y*(BOARD_HEIGHT+1))/BOARD_HEIGHT;

}

ScreenCoord BoardDisplay_getCoord(BoardDisplay* b, int row, int column)
{
    ScreenCoord result = {
        .x = (column+1)*b->checker_buffer_x + column*b->checker_size_x+b->checker_size_x/2,
        .y = (row + 1)*b->checker_buffer_y + row*b->checker_size_y + b->checker_size_y/2
    };
    return result;
}

void BoardDisplay_display(BoardDisplay* b, Board* position, int turn)
{
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {              
            ScreenCoord pos = BoardDisplay_getCoord(b, i, j);
            if(position->board & bitPos(BOARD_HEIGHT-i-1, j))
            {
                DrawEllipse(pos.x, pos.y, b->checker_size_x/2, b->checker_size_y/2, turn ? YELLOW : RED);
            }
            else if(position->filled & bitPos(BOARD_HEIGHT-i-1, j))
            {
                DrawEllipse(pos.x, pos.y, b->checker_size_x/2, b->checker_size_y/2, turn ? RED : YELLOW);
            }
            else
            {
                
                DrawEllipse(pos.x, pos.y, b->checker_size_x/2, b->checker_size_y/2, WHITE);
            }
        }
    }

}

uint64_t bitPos(int row, int col)
{
    return ((uint64_t)1) << (col * 8 + row);
}

// if there is not a one at the top of the board, you can still fit a checker
// returns 1 if legal, 0 otherwise
int Board_legalMove(Board* board, int col)
{
    return !(board->filled & (((uint64_t)1 << (BOARD_HEIGHT-1)) << (col * 8)));
}



// in a valid position filled will all ones below where the checker should be placed
// adding a bit at the start of the column will then return a bit where the checker should be placed
// e.g

void Board_playMove(Board* board, int col)
{
    // after this move it will opposite player's turn
    board->board ^= board->filled;
    board->filled |= (board->filled + ((COLUMN(col) & ((uint64_t)1 << (8*col)))));
    board->moves++;
}



int Board_isWinningMove(Board* board, int col)
{
    Board b2 = *board;
    Board_playMove(&b2, col);
    if(Board_win(&b2))
    {
        return 1;
    }
    return 0;
}



// to check for horizontals (other aligments are similar) two steps are needed
// m = pos & (pos >> (8))
// this is the intersection between the pieces and the pieces shifted to the right by 1
// in other words, a one stays a one if it has a one to its left
// eg
// 0 1 1 1 1 0 0
// after applying operation
// 0 0 1 1 1 0 0
// m & (m >> (2 * 8))
// this is the interaction between the previous value and the previous value shifted to the right by 2
// in otherwords a one stays a one if it has a one two places to the left
// 0 0 0 0 1 0 0
// remember that each one is only a one if it had a one to left the in the original board
// in summary, if after both operations there is a one
// the bit to the left of the one in board must be a one due to operation 1
// the bit 2 places to the left of the one in both the board and the board after operation 1 must be a one due to operation 2
// the bit 3 places to the left of the one in the board also must be a one because the bit two places to the left in the board is a one after operation 1
// in total, there is a 1 at some place, a 1 to the left, a 1 to places to the left and 1 three places to the left, giving a connect 4
int Board_win(Board* board)
{
    // after placing a checker, board now represents the opponent so you must xor to get the bitboard of the player who placed the checker
    uint64_t pos = board->filled ^ board->board;

    // horizontal
    uint64_t m = pos & (pos >> (8));
    if(m & (m >> (2 * 8)))
    {
        return 1;
    }
    // vertical
    m = pos & (pos >> 1);
    if(m & (m >> 2))
    {
        return 1;
    }

    // upper right diagonal
    m = pos & (pos >> (8 + 1));
    if(m & (m >> (2 * 8 + 2)))
    {
        return 1;
    }

    // bottom right diagonal
    m = pos & (pos >> (8 - 1));
    if(m & (m & (m >> (2 * 8 - 2))))
    {
        return 1;
    }

    return 0;
}



void printBitBoard(uint64_t board)
{
    for(int i = 0; i < BOARD_HEIGHT; i++)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            if(board & bitPos(BOARD_HEIGHT - 1 - i, j))
            {
                printf("1 ");
            }
            else
            {
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}
