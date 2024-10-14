#include <stdio.h>
#include "Board.h"
#include "AI.h"
#include "raylib.h"

#define DEFAULT_WIDTH 700
#define DEFAULT_HEIGHT 700


#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

#define BUTTON_BUFFER 75
#define BUTTON_SIZE_X 400
#define BUTTON_SIZE_Y 110


#define BUTTON_FONT_SIZE 30
#define BUTTON_SPACING 7


#define GAME_OVER_FONT_SIZE 50
#define GAME_OVER_SPACING 10

#define NEW_GAME_SIZE_X 400
#define NEW_GAME_SIZE_Y 120

#define WINNER_BUFFER 120
#define WINNER_FONT_SIZE 100


typedef enum Scene {MENU = 0, GAME, GAME_OVER} Scene;
typedef enum GameType {ONE_PLAYER=0, TWO_PLAYER} GameType;


Scene currScene = MENU;
GameType currGame;
BoardDisplay bd = {.checker_buffer_x = 40, .checker_buffer_y = 40};
Board board = {.board = 0, .filled = 0, .moves = 0};
Rectangle screen = {
        .x=0,
        .y=0,
        .height=DEFAULT_HEIGHT,
        .width=DEFAULT_WIDTH
    };

int monitor_width;
int monitor_height;
int nodes = 0;


Rectangle button1 = {0};
Rectangle button2 = {0};

Rectangle newGame = {0};
// 0 = red, 1 = yellow
int turn = 0;
// 0 = red, 1 = yellow, -1 = draw
int winner = -1;


void draw()
{


    switch(currScene)
    {  
        case MENU:
            ClearBackground(BLUE);

            DrawRectangleRec(button1, GRAY);
            DrawRectangleRec(button2, GRAY);
        
        
            
            Vector2 textSize = MeasureTextEx(GetFontDefault(),"One Player", (BUTTON_FONT_SIZE*(float)button1.width)/BUTTON_SIZE_X, (BUTTON_SPACING*(float)button1.width)/BUTTON_SIZE_X);
            Vector2 pos;
            pos.x = button1.x + (button1.width - textSize.x)/2;
            pos.y = button1.y + (button2.height - textSize.y)/2;
            DrawTextEx(GetFontDefault(), "One Player", pos,
            (BUTTON_FONT_SIZE*(float)button1.width)/BUTTON_SIZE_X, (BUTTON_SPACING*(float)button1.width)/BUTTON_SIZE_X, BLACK);
            textSize = MeasureTextEx(GetFontDefault(),"Two Player", (BUTTON_FONT_SIZE*(float)button1.width)/BUTTON_SIZE_X, (BUTTON_SPACING*(float)button1.width)/BUTTON_SIZE_X);
            pos.x = button2.x + (button2.width - textSize.x)/2;
            pos.y = button2.y + (button2.height - textSize.y)/2;
            DrawTextEx(GetFontDefault(), "Two Player", pos,
            (BUTTON_FONT_SIZE*(float)button2.width)/BUTTON_SIZE_X, (BUTTON_SPACING*(float)button2.width)/BUTTON_SIZE_X, BLACK);

            break;
        case GAME:
            ClearBackground(BLUE);

            BoardDisplay_display(&bd, &board, turn);

            break;
        case GAME_OVER:
            ClearBackground(BLUE);
            BoardDisplay_display(&bd, &board, turn);
            DrawRectangleRec(newGame, GRAY);
            textSize = MeasureTextEx(GetFontDefault(),"New Game", (GAME_OVER_FONT_SIZE*(float)newGame.width)/NEW_GAME_SIZE_X, (GAME_OVER_SPACING*(float)newGame.width)/NEW_GAME_SIZE_X);
            pos.x = newGame.x + (newGame.width - textSize.x)/2;
            pos.y = newGame.y + (newGame.height - textSize.y)/2;
            DrawTextEx(GetFontDefault(),"New Game", pos, (GAME_OVER_FONT_SIZE*(float)newGame.width)/NEW_GAME_SIZE_X, (GAME_OVER_SPACING*(float)newGame.width)/NEW_GAME_SIZE_X, BLACK);

            switch(winner)
            {
                case 1:
                    textSize = MeasureTextEx(GetFontDefault(),"Yellow Wins", (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width);
                    pos.x = (screen.width-textSize.x)/2;
                    pos.y = WINNER_BUFFER*screen.height/monitor_height;
                    DrawTextEx(GetFontDefault(),"Yellow Wins",pos, (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width, YELLOW);
                    break;
                case 0:
                    textSize = MeasureTextEx(GetFontDefault(),"Red Wins", (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width);
                    pos.x = (screen.width-textSize.x)/2;
                    pos.y = WINNER_BUFFER*screen.height/monitor_height;
                    DrawTextEx(GetFontDefault(),"Red Wins",pos, (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width, RED);
                    break;
                case -1:
                    textSize = MeasureTextEx(GetFontDefault(),"Draw", (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width);
                    pos.x = (screen.width-textSize.x)/2;
                    pos.y = WINNER_BUFFER*screen.height/monitor_height;
                    DrawTextEx(GetFontDefault(),"Draw",pos, (WINNER_FONT_SIZE*(float)screen.width)/monitor_width, (GAME_OVER_SPACING*(float)screen.width)/monitor_width, GRAY);
                    break;
            }
            break;

    }
  

}

void updateButtons()
{
    int buffer = BUTTON_BUFFER*screen.height/monitor_height;
    int width = BUTTON_SIZE_X*screen.width/monitor_width;
    int height = BUTTON_SIZE_Y*screen.height/monitor_height;
    
    button1.x = (screen.width - width)/2;
    button2.x = button1.x;
    button1.width = width;
    button2.width = width;

    button1.y = (screen.height-buffer)/2-height;
    button2.y = (screen.height + buffer)/2;
    button1.height = height;
    button2.height = height;


}


void updateGameOverUI()
{
    int width = (NEW_GAME_SIZE_X*screen.width)/monitor_width;
    int height = (NEW_GAME_SIZE_Y*screen.height)/monitor_height;

    newGame.x = (screen.width - width)/2;
    newGame.y = (screen.height - height)/2;
    newGame.width = width;
    newGame.height = height;



}



int main()
{




    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Connect 4");
    

    monitor_width = GetMonitorWidth(0);
    monitor_height = GetMonitorHeight(0);

    Vector2 mousePos;
    int mouseDown = 0;
    
    SetTargetFPS(60);
    
    while(!WindowShouldClose())
    {
        // update
        screen.width = GetScreenWidth();
        screen.height = GetScreenHeight();
        mousePos = GetMousePosition();
        switch(currScene)
        { 
            case MENU:
                updateButtons();
                if(CheckCollisionPointRec(mousePos, button1))
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        mouseDown = 1;
                    }
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if(mouseDown)
                        {
                            currGame = ONE_PLAYER;
                            currScene = GAME;
                        }
                        mouseDown = 0;
                    }
                }
                if(CheckCollisionPointRec(mousePos, button2))
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        mouseDown = 1;
                    }
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if(mouseDown)
                        {
                            currGame = TWO_PLAYER;
                            currScene = GAME;
                        }
                        mouseDown = 0;
                    }
                }
                break;
            case GAME:
                int move = -1;
                BoardDisplay_update(&bd, screen.width, screen.height, monitor_width, monitor_height);
                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    mouseDown = 1;
                }
                if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                {
                    if(mouseDown)
                    {
                        for(int i = 0; i < BOARD_WIDTH; i++)
                        {
                            if(BoardDisplay_getCoord(&bd, 0, i).x - bd.checker_size_x/2 <= mousePos.x && BoardDisplay_getCoord(&bd, 0, i).x + bd.checker_size_x/2 >= mousePos.x)
                            {
                                move = i;
                            }
                        }
                    }
                    mouseDown = 0;
                }

                
                switch(currGame)
                {
                    case ONE_PLAYER:
                        if(turn == 1)
                        {
                            if(move == -1 || !Board_legalMove(&board, move))
                            {
                                break;
                            }
                            Board_playMove(&board, move);
                            turn = !turn;
                            if(Board_win(&board))
                            {
                                currScene = GAME_OVER;
                                winner = !turn;
                                break;
                            }
                            if(board.moves == BOARD_HEIGHT *BOARD_WIDTH)
                            {
                                currScene = GAME_OVER;
                                winner = -1;
                                break;

                            }
                            move = -1;

                            break;
                        }
                        else
                        {
                            
                            move = solve(board, 20, -BOARD_WIDTH*BOARD_HEIGHT/2, BOARD_WIDTH*BOARD_HEIGHT/2);
                            printf("nodes explored: %d \n", nodes);
                            nodes = 0;
                            Board_playMove(&board, move);
                            turn = !turn;
                            if(Board_win(&board))
                            {
                                currScene = GAME_OVER;
                                winner = !turn;
                                break;
                            }
                            if(board.moves == BOARD_WIDTH*BOARD_HEIGHT)
                            {
                                currScene = GAME_OVER;
                                winner = -1;
                                break;
                            }
                            move = -1;
                        }
                        break;
                    case TWO_PLAYER:
                        if(move == -1 || !Board_legalMove(&board, move))
                        {
                            break;
                        }
                        Board_playMove(&board, move);
                        turn = !turn;
                        if(Board_win(&board))
                        {
                            currScene = GAME_OVER;
                            winner = !turn;
                            break;
                        }
                        if(board.filled == FULL_BOARD)
                        {
                            currScene = GAME_OVER;
                            winner = -1;
                            break;
                        }
                        move = -1;

                        

                        break;
                }
                

                break;
            case GAME_OVER:
                BoardDisplay_update(&bd, screen.width, screen.height, monitor_width, monitor_height);
                updateGameOverUI();

                if(CheckCollisionPointRec(mousePos, newGame))
                {
                    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                    {
                        mouseDown = 1;
                    }
                    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if(mouseDown)
                        {
                            currScene = MENU;
                            board.filled = 0;
                            board.board = 0;
                            board.moves = 0;
                            turn = 0;
                            winner = -1;                            
                        }
                        mouseDown = 0;
                    }
                }
                break;
        }


        // Draw
        BeginDrawing();
        
        draw();

        EndDrawing();

    }






    CloseWindow();
}


