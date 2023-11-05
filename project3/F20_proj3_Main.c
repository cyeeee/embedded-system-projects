/**
 * Starter code for Project 3. Good luck!
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <HAL/ADC_HAL.h>

extern const Graphics_Image menuScreen8BPP_UNCOMP;
extern const Graphics_Image titleScreen8BPP_UNCOMP;
extern const Graphics_Image highScoreScreen8BPP_UNCOMP;
extern const Graphics_Image endScreen8BPP_UNCOMP;

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct(&hal);

    Application_showTitleScreen(&hal.gfx);

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        PollNonBlockingLED();

        // Main super-loop
        // call the main FSM function over and over.
        Application_loop(&app, &hal);
        HAL_refresh(&hal);
    }
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct(HAL* hal)
{
    Application app;

    app.state = TITLE_SCREEN;
    app.titleTimer = SWTimer_construct(TITLE_SCREEN_WAIT);
    SWTimer_start(&app.titleTimer);

    app.cursor = CURSOR_0;

    app.top1 = 0;
    app.top2 = 0;
    app.top3 = 0;

    app.colliSum = pow((PLAYER_RADIUS - ORBS_RADIUS), 2);
    app.colliDiff = pow((PLAYER_RADIUS + ORBS_RADIUS), 2);

    app.randomX2 = app.randomX1;
    app.randomX1 = hal->joystick.x % 10;
    app.randomY2 = app.randomY1;
    app.randomY1 = hal->joystick.y % 10;

    Application_initGameVariables(&app, hal);

    return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_loop(Application* app, HAL* hal)
{
    // control screens
    switch (app->state)
    {
    case TITLE_SCREEN:
        Application_handleTitleScreen(app, hal);
        break;

    case MENU_SCREEN:
        Application_handleMenuScreen(app, hal);
        break;

    case HowToPlay_SCREEN:
        Application_handleHowToPlayScreen(app, hal);
        break;

    case HighScore_SCREEN:
        Application_handleHighScoreScreen(app, hal);
        break;

    case GAME_SCREEN:
        Application_handleGameScreen(app, hal);
        break;

    case RESULT_SCREEN:
        Application_handleResultScreen(app, hal);
        break;

    default:
        break;
    }

}

/**
 * Shows Title screen
 */
void Application_showTitleScreen(GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "ECE 2564 Project 3   ", 0, 0);
    GFX_print(gfx, "---------------------", 1, 0);
    GFX_print(gfx, "Author:              ", 2, 0);
    GFX_print(gfx, "       Chenyi Wang   ", 3, 0);

    Graphics_drawImage(&gfx->context, &titleScreen8BPP_UNCOMP, 17, 55);

}

/**
 * Display the main menu screen
 */
void Application_handleTitleScreen(Application* app, HAL* hal)
{
    // When the SW timer expires, show instruction screen
    if (SWTimer_expired(&app->titleTimer))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);
    }
}

/**
 * Shows Main Menu screen
 */
void Application_showMenuScreen(Application* app, GFX* gfx)
{
    GFX_clear(gfx);

    GFX_print(gfx, "Main Menu            ", 0, 0);
    GFX_print(gfx, "---------------------", 1, 0);
    GFX_print(gfx, "   Play Game         ", 2, 0);
    GFX_print(gfx, "   How to Play       ", 3, 0);
    GFX_print(gfx, "   High Scores       ", 4, 0);

    // Draw the cursor
    GFX_print(gfx, " > ", 2 + app->cursor, 0);

    Graphics_drawImage(&gfx->context, &menuScreen8BPP_UNCOMP, 17, 55);
}

/**
 * Display the user selected screen
 */
void Application_handleMenuScreen(Application* app, HAL* hal)
{
    // If joystick is tapped either up or down, update the cursor
    // "wrap-around"
    if (Joystick_isTappedUp(&hal->joystick)) {
        app->cursor = (Cursor) (((int) app->cursor + 2) % CURSOR_NUM);
        Application_updateMenuScreen(app, &hal->gfx);
    }
    else if (Joystick_isTappedDown(&hal->joystick)) {
        app->cursor = (Cursor) (((int) app->cursor + 1) % CURSOR_NUM);
        Application_updateMenuScreen(app, &hal->gfx);
    }

    // If JSB is pressed, transit to the user selected state
    if (Button_isTapped(&hal->boosterpackJS))
    {
        switch (app->cursor)
        {
        case CURSOR_0: // Game
            app->state = GAME_SCREEN;
            Application_showGameScreen(app, &hal->gfx, hal);
            break;

        case CURSOR_1: // How to Play
            app->state = HowToPlay_SCREEN;
            Application_showHowToPlayScreen(&hal->gfx);
            break;

        case CURSOR_2: // High Scores
            app->state = HighScore_SCREEN;
            Application_showHighScoreScreen(app, &hal->gfx);
            break;

        default:
            break;
        }
    }
}

/**
 * A helper function which updates the main menu screen by redrawing only the
 * positions where the cursor could possibly be updated.
 */
void Application_updateMenuScreen(Application* app, GFX* gfx)
{
    // Clear the cursors from any previous game screen
    GFX_print(gfx, "   ", 2, 0);
    GFX_print(gfx, "   ", 3, 0);
    GFX_print(gfx, "   ", 4, 0);

    // Draw the cursor
    GFX_print(gfx, " > ", 2 + app->cursor, 0);
}

/**
 * Shows How To Play screen
 */
void Application_showHowToPlayScreen(GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "How to Play           ", 0, 0);
    GFX_print(gfx, "----------------------", 1, 0);
    GFX_print(gfx, "Tile the board to     ", 2, 0);
    GFX_print(gfx, "move and collect orbs ", 3, 0);

    GFX_setForeground(gfx, GREEN);
    GFX_print(gfx, "   GREEN - \"point\"  ", 5, 0);
    GFX_resetColors(gfx);
    GFX_print(gfx, "   (increases life)   ", 6, 0);

    GFX_setForeground(gfx, YELLOW);
    GFX_print(gfx, "  YELLOW - \"hazard\" ", 8, 0);
    GFX_resetColors(gfx);
    GFX_print(gfx, "   (decreases life)   ", 9, 0);

    GFX_setForeground(gfx, RED);
    GFX_print(gfx, "    RED  - \"danger\" ", 11, 0);
    GFX_resetColors(gfx);
    GFX_print(gfx, "      (game over)     ", 12, 0);

    GFX_print(gfx, "GOOD LUCK!            ", 14, 0);
    GFX_print(gfx, "Press JS to return... ", 15, 0);

}

/**
 * Go back the main menu screen when JS is pressed
 */
void Application_handleHowToPlayScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);

    }
}

/**
 * A helper function which resets all the game variables to their unselected
 * states and resets the cursor position.
 */
void Application_initGameVariables(Application* app, HAL* hal)
{
    app->counter = 0;

    app->score = 0;     // initial score = 0
    app->life = 3;      // initial life = 3
    app->gameover = false;

    app->currentX = CENTER_X;
    app->currentY = CENTER_Y;   // initial position is center
    app->previousX = NULL;
    app->previousY = NULL;

    app->sizeX = X_ARR_CAP - 1;
    app->sizeY = Y_ARR_CAP - 1;

    Application_initArray(app->arrX, app->sizeX, FIELD_X_MIN);
    Application_initArray(app->arrY, app->sizeY, FIELD_Y_MIN);

    app->orbsNum = 0;
    app->orbsCurrent = app->orbsNum;

    app->isColli = false;

}

/*
 * A helper function which initialize x and y arrays
 */
void Application_initArray(int arr[], int size, int min)
{
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = i + min;
    }
}

/**
 * Shows Game screen
 */
void Application_showGameScreen(Application* app, GFX* gfx, HAL* hal)
{
    GFX_clear(gfx);

    GFX_print(gfx, "Play the Game         ", 0, 0);
    GFX_print(gfx, "----------------------", 1, 0);
    GFX_print(gfx, "Score: 00    Life: 03 ", 2, 0);

    // draw border
    GFX_drawRectangle(gfx, X_MIN, Y_MIN, X_MAX, Y_MAX);

    // Drawing a circle representing the player
    GFX_setForeground(gfx, PLAYER);
    GFX_drawSolidCircle(gfx, CENTER_X, CENTER_Y, PLAYER_RADIUS);
    GFX_resetColors(gfx);

}

/*
 * Generates random numbers for x and y
 */
void Application_generateRand(Application* app, HAL* hal)
{
    app->randomX2 = app->randomX1;
    app->randomX1 = hal->joystick.x % 10;
    app->randomY2 = app->randomY1;
    app->randomY1 = hal->joystick.y % 10;

    int rX = app->randomX2 * 10 + app->randomX1;
    int rY = app->randomY2 * 10 + app->randomY1;
    if (rX < app->sizeX && rY < app->sizeY){
        app->randomX = rX;
        app->randomY = rY;
    }
}

/**
 * Moves the player around the playing field by using the accelerometer
 */
void Application_handleGameScreen(Application* app, HAL* hal)
{
    app->previousX = app->currentX;
    app->previousY = app->currentY;

    Application_generateRand(app, hal);       // generate random values for x and y

    Application_checkCollision(app, &hal->gfx, app->arrOrbX, app->arrOrbY, app->orbsCurrent);

    if (app->isColli) {
        app->isColli = false;                                               // refresh collision status
        Application_updateCollision(app, hal);                              // update score and life values
        GFX_removeSolidCircle(&hal->gfx, app->x1, app->y1, ORBS_RADIUS);    // Clear the collided orb
        Application_updateScore(app, &hal->gfx);
        Application_updateLife(app, &hal->gfx);
    }

    // When the SW timer for frame delay (40 ms) expires, updates the player's position
    if (SWTimer_expired(&app->frameDelay)) {

        Application_updatePlayerPos(app, &hal->gfx, hal);

        if ((app->orbsNum < ORB_ARR_CAP - 1)) {
            app->counter++;
            if (app->counter == 25) {   // spawn an orb approximatly every two seconds
                app->counter = 0;
                Application_spawnOrbs(app, &hal->gfx, hal);
            }
        }

        app->frameDelay = SWTimer_construct(FRAME_DELAY);
        SWTimer_start(&app->frameDelay);
    }

    // If life decreases to 0 or red orb collided, game over
    if (app->gameover) {
        Application_rankScores(app);
        app->state = RESULT_SCREEN;
        Application_showResultScreen(app, &hal->gfx);
    }
}

/*
 * Update the position of the player based on the change of x, y axes of accel
 */
void Application_updatePlayerPos(Application* app, GFX* gfx, HAL* hal)
{
    // Clear the Player from any previous game screen
    GFX_removeSolidCircle(gfx, app->previousX, app->previousY, PLAYER_RADIUS);

    // update x position, and check if the player is still in the border
    if (Accel_isLeft(&hal->accel) && (app->previousX > FIELD_X_MIN)) {
        if (hal->accel.x < L_TH)
            app->currentX -= MOVE2;
        else
            app->currentX -= MOVE1;
    }
    else if (Accel_isRight(&hal->accel) && (app->previousX < FIELD_X_MAX)) {
        if (hal->accel.x > R_TH)
            app->currentX += MOVE2;
        else
            app->currentX += MOVE1;
    }

    // update y position
    if (Accel_isUp(&hal->accel) && (app->previousY > FIELD_Y_MIN)) {
        if (hal->accel.y > U_TH)
            app->currentY -= MOVE2;
        else
            app->currentY -= MOVE1;
    }
    else if (Accel_isDown(&hal->accel) && (app->previousY < FIELD_Y_MAX)) {
        if (hal->accel.y < D_TH)
            app->currentY += MOVE2;
        else
            app->currentY += MOVE1;
    }

    // Draw the Explorer
    GFX_setForeground(gfx, PLAYER);
    GFX_drawSolidCircle(gfx, app->currentX, app->currentY, PLAYER_RADIUS);
    GFX_resetColors(gfx);
}

/*
 * Spawns orbs after 2 seconds
 */
void Application_spawnOrbs(Application* app, GFX* gfx, HAL* hal)
{
    // determine position of the orb spawning
    app->orbX = app->arrX[app->randomX];
    app->orbY = app->arrY[app->randomY];

    Application_updateArray(app->arrX, app->sizeX, app->randomX);
    app->sizeX -= 1;
    Application_updateArray(app->arrY, app->sizeY, app->randomY);
    app->sizeY -= 1;

    // store x and y values of the orb in arrays
    app->arrOrbX[app->orbsCurrent] = app->orbX;
    app->arrOrbY[app->orbsCurrent] = app->orbY;

    if (app->orbsNum % 3 == 0) {    // spawn green orb every 3 orbs
        GFX_setForeground(gfx, GREEN);
        app->orbs = POINT;
    }
    else if (app->orbsNum % 5 == 0) {   // spawn red orb every 5 orbs
        GFX_setForeground(gfx, RED);
        app->orbs = DANGER;
    }
    else {
        GFX_setForeground(gfx, YELLOW);
        app->orbs = HAZARD;
    }

    app->arrOrbs[app->orbsCurrent] = app->orbs;     // store type of the orb in an array
    app->orbsNum++;                             // increase number of orbs spawned
    app->orbsCurrent++;

    GFX_drawSolidCircle(gfx, app->orbX, app->orbY, ORBS_RADIUS);
    GFX_resetColors(gfx);

}

/*
 * A helper function which updates x and y arrays
 */
void Application_updateArray(int arr[], int size, int index)
{
    size -= 1;
    int j;
    for (j = index; j < size; j++) {
        arr[j] = arr[j + 1];    // remove an element from the array
    }
}

/*
 * Checks if there is a collision happened
 */
void Application_checkCollision(Application* app, GFX* gfx, int arrx[], int arry[], int size)
{
    int i;
    for (i = 0; i < size; i++) {
        app->x1 = arrx[i];
        app->y1 = arry[i];
        app->colliIntersect = pow((app->currentX - app->x1), 2) + pow((app->currentY - app->y1), 2);
        // Two circles intersect if, and only if, the distance between their centers
        // is between the sum and the difference of their radii.
        if ((app->colliIntersect >= app->colliSum) && (app->colliIntersect <= app->colliDiff)) {
            app->isColli = true;
            app->index = i;
            break;
        }
    }
}

/*
 * Removes collided orb and update score and life
 */
void Application_updateCollision(Application* app, HAL* hal)
{
    if (app->arrOrbs[app->index] == POINT) {    // if collides a green orb, increases life and score
        app->life += 1;
        app->score += 1;
    }
    else if (app->arrOrbs[app->index] == HAZARD) {  // if collides a yellow orb, decreases life
        app->life -= 1;
        if (app->life <= 0)
            app->gameover = true;
    }
    else if (app->arrOrbs[app->index] == DANGER)    // if collides a red orb, game over
        app->gameover = true;

    // removes the collided orb's information
    Application_updateArray(app->arrOrbX, app->orbsCurrent, app->index);
    Application_updateArray(app->arrOrbY, app->orbsCurrent, app->index);
    Application_updateArray(app->arrOrbs, app->orbsCurrent, app->index);
    app->orbsCurrent -= 1;
}

/*
 * Update score on the screen
 */
void Application_updateScore(Application* app, GFX* gfx)
{
    GFX_print(gfx, "  ", 2, 7);
    char sString[2] = "";
    sString[1] = (app->score % 10) + '0';
    sString[0] = ((app->score / 10) % 10) + '0';
    GFX_print(gfx, sString, 2, 7);
}

/*
 * Update life state on the screen
 */
void Application_updateLife(Application* app, GFX* gfx)
{
    GFX_print(gfx, "  ", 2, 19);
    char lString[2] = "";
    lString[1] = (app->life % 10) + '0';
    lString[0] = ((app->life / 10) % 10) + '0';
    GFX_print(gfx, lString, 2, 19);
}

/**
 * Shows Result screen
 */
void Application_showResultScreen(Application* app, GFX* gfx)
{
    GFX_clear(gfx);

    GFX_print(gfx, "Game Over             ", 0, 0);
    GFX_print(gfx, "----------------------", 1, 0);

    GFX_print(gfx, "Your Score:  ", 3, 0);

    char scoreString[2] = "";
    scoreString[1] = (app->score % 10) + '0';
    scoreString[0] = ((app->score / 10) % 10) + '0';
    GFX_print(gfx, scoreString, 3, 12);      // Print the final score

    Graphics_drawImage(&gfx->context, &endScreen8BPP_UNCOMP, 17, 55);

    GFX_print(gfx, "Press JS to return... ", 15, 0);
}

/**
 * Go back the main menu screen when JSB is pressed
 */
void Application_handleResultScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);
        Application_initGameVariables(app, hal);
    }
}

/**
 * Compare the final score with top 3 scores
 */
void Application_rankScores(Application* app)
{
    if (app->score > app->top3) {
        if (app->score > app->top2) {
            if (app->score > app->top1) {
                app->top2 = app->top1;
                app->top1 = app->score;
            }
            else {
                app->top3 = app->top2;
                app->top2 = app->score;
            }
        }
        else {
            app->top3 = app->score;
        }
    }
}

/**
 * Shows High Score screen
 */
void Application_showHighScoreScreen(Application* app, GFX* gfx)
{

    GFX_clear(gfx);

    GFX_print(gfx, "High Scores           ", 0, 0);
    GFX_print(gfx, "----------------------", 1, 0);

    GFX_print(gfx, " 1: 00", 2, 0);
    GFX_print(gfx, " 2: 00", 3, 0);
    GFX_print(gfx, " 3: 00", 4, 0);

    Graphics_drawImage(&gfx->context, &highScoreScreen8BPP_UNCOMP, 17, 55);

    GFX_print(gfx, "Press JS to return... ", 15, 0);

    GFX_print(gfx, "  ", 2, 4);
    GFX_print(gfx, "  ", 3, 4);
    GFX_print(gfx, "  ", 4, 4);

    char top1String[2] = "";
    top1String[1] = (app->top1 % 10) + '0';
    top1String[0] = ((app->top1 / 10) % 10) + '0';
    GFX_print(gfx, top1String, 2, 4);      // Print the current score

    char top2String[2] = "";
    top2String[1] = (app->top2 % 10) + '0';
    top2String[0] = ((app->top2 / 10) % 10) + '0';
    GFX_print(gfx, top2String, 3, 4);

    char top3String[2] = "";
    top3String[1] = (app->top3 % 10) + '0';
    top3String[0] = ((app->top3 / 10) % 10) + '0';
    GFX_print(gfx, top3String, 4, 4);
}

/**
 * Go back the main menu screen when JSB is pressed
 */
void Application_handleHighScoreScreen(Application* app, HAL* hal)
{
    // When the joystick is tapped, show main menu screen
    if (Button_isTapped(&hal->boosterpackJS))
    {
        app->state = MENU_SCREEN;
        Application_showMenuScreen(app, &hal->gfx);
    }
}
