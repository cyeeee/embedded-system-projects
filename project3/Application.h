/*
 * Application.h
 *
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define TITLE_SCREEN_WAIT   3000  // 3 seconds
#define FRAME_DELAY         80    // 80 milliseconds

#define CURSOR_NUM      3       // number of cursors

// center of the board
#define CENTER_X        63
#define CENTER_Y        75

// speed change thresholds
#define L_TH            6000
#define R_TH            9000
#define U_TH            9000
#define D_TH            6000

// moving rates
#define MOVE1           1
#define MOVE2           3

// arena constraints
#define FIELD_X_MIN     16
#define FIELD_Y_MIN     38
#define FIELD_X_MAX     111
#define FIELD_Y_MAX     111

#define PLAYER_RADIUS   7       // radius of the player
#define ORBS_RADIUS     5       // radius of orbs

#define X_ARR_CAP       97
#define Y_ARR_CAP       75
#define ORB_ARR_CAP     75      // capacitance of arrays

enum _GameState
{
    TITLE_SCREEN, MENU_SCREEN, HowToPlay_SCREEN, HighScore_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2
};
typedef enum _Cursor Cursor;

enum _Orbs
{
    POINT, HAZARD, DANGER
};
typedef enum _Orbs Orbs;

struct _Application
{

    SWTimer titleTimer;     // General-purpose timer for when screens must disappear
    GameState state;        // Determines which screen is currently shown
    Cursor cursor;          // Cursor tracker variable
    SWTimer frameDelay;     // timer for the delay time of changing frame
    Orbs orbs;              // types of orb

    // counter
    int counter;
    // game variables
    int score;
    int life;
    bool gameover;
    // player positions
    int currentX;
    int currentY;
    int previousX;
    int previousY;
    // arrays for possible x and y values
    int arrX[X_ARR_CAP];
    int arrY[Y_ARR_CAP];
    int sizeX;
    int sizeY;
    // orbs position
    int x1;
    int y1;
    int index;
    // arrays for displayed orbs
    int arrOrbs[ORB_ARR_CAP];
    // check collision
    int arrOrbX[ORB_ARR_CAP];
    int arrOrbY[ORB_ARR_CAP];
    int colliSum;
    int colliDiff;
    int colliIntersect;
    bool isColli;
    // orbs positions
    int orbX;
    int orbY;
    // number of orbs
    int orbsNum;
    int orbsCurrent;
    // store generated random numbers
    int randomX;
    int randomY;
    int randomX1;
    int randomY1;
    int randomX2;
    int randomY2;
    // top scores
    int top1;
    int top2;
    int top3;

};
typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct(HAL* hal);

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);

// Drawing screens
void Application_showTitleScreen(GFX* gfx);
void Application_showMenuScreen(Application* app, GFX* gfx);
void Application_showHowToPlayScreen(GFX* gfx);
void Application_showHighScoreScreen(Application* app, GFX* gfx);
void Application_showGameScreen(Application* app, GFX* gfx, HAL* hal);
void Application_showResultScreen(Application* app, GFX* gfx);

// Handle each screens
void Application_handleTitleScreen(Application* app, HAL* hal);
void Application_handleMenuScreen(Application* app, HAL* hal);
void Application_handleHowToPlayScreen(Application* app, HAL* hal);
void Application_handleHighScoreScreen(Application* app, HAL* hal);
void Application_handleGameScreen(Application* app, HAL* hal);
void Application_handleResultScreen(Application* app, HAL* hal);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app, HAL* hal);
void Application_initArray(int arr[], int size, int min);

// Helper functions
void Application_updateMenuScreen(Application* app, GFX* gfx);
void Application_updatePlayerPos(Application* app, GFX* gfx, HAL* hal);
void Application_spawnOrbs(Application* app, GFX* gfx, HAL* hal);
void Application_updateArray(int arr[], int size, int index);
void Application_checkCollision(Application* app, GFX* gfx, int arrx[], int arry[], int size);
void Application_updateCollision(Application* app, HAL* hal);
void Application_updateScore(Application* app, GFX* gfx);
void Application_updateLife(Application* app, GFX* gfx);
void Application_generateRand(Application* app, HAL* hal);
void Application_rankScores(Application* app);

#endif /* APPLICATION_H_ */
