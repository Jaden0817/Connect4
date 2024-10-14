
#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "raylib.h"



#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define FULL_SCREEN_BUFFER_X 60
#define FULL_SCREEN_BUFFER_Y 40


// various useful masks
#define COLUMN(col) (uint64_t)0xff << (col * 8)
#define FULL_BOARD (uint64_t)0b111111001111110011111100111111001111110011111100111111

typedef struct Board
{
    // The board is represented using two 64 bit integers called bitboards
    // a bit being on for board means there is a checker placed by the player who's turn it is
    // a bit being on for filled means there is a checker placed by any place there
    // to get the board for the other player, xor board with filled
    // Board layout
    // xx xx xx xx xx xx xx xx
    // xx xx xx xx xx xx xx xx
    // 05 11 17 23 29 35 41 xx
    // 04 10 16 22 28 34 40 xx
    // 03 09 15 21 27 33 39 xx
    // 02 08 14 20 26 32 38 xx
    // 01 07 13 19 25 31 37 xx
    // 00 06 12 18 24 30 36 xx
    // unused bits are set to zero so that wrap arounds aren't counted as wins
    uint64_t board;
    uint64_t filled;
    int moves;
} Board;


int Board_legalMove(Board* board, int col);
// turn is 0 for first player and 1 for second player
void Board_playMove(Board* board, int col);

// returns 1 if there is a connect 4 and 0 if not
int Board_win(Board* board);

int Board_isWinningMove(Board* board, int col);


typedef struct ScreenCoord
{
    int x;
    int y;
} ScreenCoord;


typedef struct BoardDisplay
{
    int checker_size_x;
    int checker_size_y;
    int checker_buffer_x;
    int checker_buffer_y;
    
} BoardDisplay;



void BoardDisplay_update(BoardDisplay* b, int screen_width, int screen_height, int monitor_width, int monitor_height);

// row and column are zero based
ScreenCoord BoardDisplay_getCoord(BoardDisplay* b, int row, int column);


void BoardDisplay_display(BoardDisplay* b, Board* position, int turn);


// prints the bit board as a board for debugging
void printBitBoard(uint64_t board);









uint64_t bitPos(int row, int col);

#endif