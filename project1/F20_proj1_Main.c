/**
 * Starter code for Project 2. Good luck!
 */

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

extern const Graphics_Image title8BPP_UNCOMP;
extern const Graphics_Image gameOver8BPP_UNCOMP;

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
    // Stop Watchdog Timer
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct(&hal);

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
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
uint32_t CircularIncrement(uint32_t value, uint32_t maximum)
{
    return (value + 1) % maximum;
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

    app.state = EMPTY;
    app.timer = SWTimer_construct(EMPTY_WAIT);
    SWTimer_start(&app.timer);

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
    // control game screens
    switch (app->state)
    {
        case EMPTY:
            Application_handleEmpty(app, hal);
            break;

        case TITLE_SCREEN:
            Application_handleTitleScreen(app, hal);
            break;

        case GAME_SCREEN:
            Application_handleGameScreen(app, hal);
            break;

        case RESULT_SCREEN:
            Application_handleGameOverScreen(app, hal);
            break;

        default:
            break;
    }

}

/**
 * Restart/Update communications if either this is the first time the application is
   run or if Boosterpack S1 is pressed (which means a new baudrate is being set up)
 */
void Application_UARTCommunication(Application* app, HAL* hal)
{
    if (Button_isTapped(&hal->boosterpackS1) || app->firstCall) {
        Application_updateCommunications(app, hal);
    }

    // if BB2 is pressed, aborting the command
    if (Button_isTapped(&hal->boosterpackS2)) {
        app->isAborted = true;
        Application_UARTResponse(app, hal);
        GFX_print(&hal->gfx, "    ", 13, 7);
    }

    if (UART_hasChar(&hal->uart)) {
        // The character received from your serial terminal
        char rxChar = UART_getChar(&hal->uart);

        char txChar = Application_interpretIncomingChar(rxChar);

        // Only send a character if the UART module can send it
        if (UART_canSend(&hal->uart))
            UART_putChar(&hal->uart, txChar);

        // if "esc" is pressed, aborting the command
        if (rxChar == 27)
            app->isAborted = true;
        else
            // Interpret command operation
            Application_commandFSM(app, rxChar);

        // Response to the input command
        Application_UARTResponse(app, hal);

        // Echoing input on LCD
        if (app->isCommand)
            Application_echoInput(app, &hal->gfx, &txChar);
        else
            GFX_print(&hal->gfx, "    ", 13, 7);

    }

    // refresh interpretation
    app->isCommand = false;
    app->isAborted = false;

}

/**
 * Interprets the input command
 */
void Application_UARTResponse(Application* app, HAL* hal) {

    // The input is a movement command
    if (app->isMovement) {
        UART_putChar(&hal->uart, 'M');
        Application_updateLine(hal);
    }

    // The input is a feed command
    else if (app->isFeed) {
        UART_putChar(&hal->uart, 'F');
         Application_updateLine(hal);
    }

    // The input command is aborted
    else if (app->isAborted) {
        UART_putChar(&hal->uart, 'X');
        Application_updateLine(hal);
        app->input = SU;
    }

    // The input is not a command
    else if (!app->isCommand) {
        UART_putChar(&hal->uart, 'U');
        Application_updateLine(hal);
    }

}

/**
 * Start the next input with a new line
 */
void Application_updateLine(HAL* hal) {

    UART_putChar(&hal->uart, '\n');
    UART_putChar(&hal->uart, 13);

}

/**
 * Updates which LEDs are lit and what baud rate the UART module communicates
 * with, based on what the application's baud choice is at the time this
 * function is called.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_updateCommunications(Application* app, HAL* hal)
{
    // When this application first loops, the proper LEDs aren't lit. The
    // firstCall flag is used to ensure that the
    if (app->firstCall) {
        app->firstCall = false;
    }

    // When Boosterpack S1 is tapped, circularly increment which baud rate is used.
    else {
        uint32_t newBaudNumber = CircularIncrement((uint32_t) app->baudChoice, NUM_BAUD_CHOICES);
        app->baudChoice = (UART_Baudrate) newBaudNumber;
    }

    // Start/update the baud rate according to the one set above.
    UART_updateBaud(&hal->uart, app->baudChoice);

    // Turn off all LEDs
    LED_turnOff(&hal->boosterpackRed);
    LED_turnOff(&hal->boosterpackGreen);
    LED_turnOff(&hal->boosterpackBlue);

    // Turn on all appropriate LEDs according to the tasks below.
    switch (app->baudChoice)
    {
        // When the baud rate is 9600, turn on Boosterpack LED Red
        case BAUD_9600:
            LED_turnOn(&hal->boosterpackRed);
            break;

        // When the baud rate is 19200, turn on Boosterpack LED Green
        case BAUD_19200:
            LED_turnOn(&hal->boosterpackGreen);
            break;

        // When the baud rate is 38400, turn on Boosterpack LED Blue
        case BAUD_38400:
            LED_turnOn(&hal->boosterpackBlue);
            break;

        // When the baud rate is 57600, turn on all Boosterpack LEDs (illuminates white)
        case BAUD_57600:
            LED_turnOn(&hal->boosterpackBlue);
            LED_turnOn(&hal->boosterpackGreen);
            LED_turnOn(&hal->boosterpackRed);
            break;

        // In the default case, this program will do nothing.
        default:
            break;
    }
}

/**
 * Interprets a character which was incoming and returns
 * that character (except "Enter" and "Esc").
 *
 * @param rxChar: Input character
 * @return :  Output character
 */
char Application_interpretIncomingChar(char rxChar)
{
    // The character to return back to sender. By default, we assume the letter
    // to send back is an 'O' (assume the character is an "other" character)
    char txChar = 'O';

    // If the key pressed was "enter" or "esc", return nothing
    if (rxChar == 13 || rxChar == 27) {
        txChar = NULL;
    }

    // If the character entered was a number, letter, special symbol,
    // or blank space, return what it is
    if (rxChar >= 32 && rxChar <= 126) {
        txChar = rxChar;
    }

    return (txChar);
}

/**
 * Interprets if the input is a movement command or feed command
 */
void Application_commandFSM(Application* app, char input) {

    switch (app->input)
    {
        case SU:
            if (input == 'm') {
                app->input = Sm;
                app->isCommand = true;
            }
            else if (input == 'f') {
                app->input = FEED;
                app->isCommand = true;
            }
            else
                app->input = SU;
            break;

        case Sm:
            if (input == 'v') {
                app->input = Smv;
                app->isCommand = true;
            }
            else
                app->input = SU;
            break;

        case Smv:
            if (input == ' ') {
                app->input = Smv_;
                app->isCommand = true;
            }
            else
                app->input = SU;
            break;

        case Smv_:
            if (input == 'l' || input == 'r' || input == 'u' || input == 'd') {
                app->input = Smv_p;
                app->isCommand = true;
                Application_interpretDirection(app, input);
            }
            else
                app->input = SU;
            break;

        case Smv_p:
            if (input == 13) {
                app->input = M_ENTER;
                app->isMovement = true;
                app->input = SU;
            }
            else
                app->input = SU;
            break;

        case FEED:
            if (input == 13) {
                app->input = F_ENTER;
                app->isFeed = true;
                app->input = SU;
                if (app->engery < E_5)
                    app->engery = (EnergyState) (((int) app->engery + 1));
            }
            else
                app->input = SU;
            break;

        default:
            break;
    }
}

/**
 * Interprets the input movement command direction
 */
void Application_interpretDirection(Application* app, char input) {

    if (input == 'l')
        app->isLeft = true;
    else
        app->isLeft = false;

    if (input == 'r')
        app->isRight = true;
    else
        app->isRight = false;

    if (input == 'u')
        app->isUp = true;
    else
        app->isUp = false;

    if (input == 'd')
        app->isDown = true;
    else
        app->isDown = false;

}

/**
 * Light up an LED when feeding happens at this point
 */
void Application_energyIndicator(Application* app, HAL* hal) {

    // Turn off all LEDs
    LED_turnOff(&hal->launchpadLED2Red);
    LED_turnOff(&hal->launchpadLED2Green);
    LED_turnOff(&hal->launchpadLED2Blue);

    switch (app->engery) {
        // When the energy state is 1, turn on Launchpad RGB LED2 (LL2) Red
        case E_1:
            LED_turnOn(&hal->launchpadLED2Red);
            break;

        // When the energy state is 2, turn on LL2 Yellow
        case E_2:
            LED_turnOn(&hal->launchpadLED2Red);
            LED_turnOn(&hal->launchpadLED2Green);
            break;

        // When the energy state is 3, turn on LL2 Green
        case E_3:
            LED_turnOn(&hal->launchpadLED2Green);
            break;

        // When the energy state is 4, turn on LL2 Blue
        case E_4:
            LED_turnOn(&hal->launchpadLED2Blue);
            break;

        // When the energy state is 5, turn on all LL2 (illuminates white)
        case E_5:
            LED_turnOn(&hal->launchpadLED2Blue);
            LED_turnOn(&hal->launchpadLED2Green);
            LED_turnOn(&hal->launchpadLED2Red);
            break;

        // In the default case, this program will do nothing.
        default:
            break;
    }
}

/**
 * Display title screens
 */
void Application_handleEmpty(Application* app, HAL* hal) {
    // When the SW timer expires, show instruction screen
    if (SWTimer_expired(&app->timer))
    {
        app->state = TITLE_SCREEN;
        Application_showTitleScreen(app, hal, &hal->gfx);
    }
}

/**
 * Shows title screen
 */
void Application_showTitleScreen(Application* app, HAL* hal, GFX* gfx) {

    GFX_clear(gfx);

    GFX_print(gfx, "ECE 2564: Project 1   ", 0, 0);
    GFX_print(gfx, " - The Story          ", 2, 0);
    GFX_print(gfx, "   Of An Explorer -   ", 3, 0);

    Graphics_drawImage(&gfx->context, &title8BPP_UNCOMP, 45, 40);

    GFX_print(gfx, "--------------------- ", 10, 0);
    GFX_print(gfx, "Designed By:          ", 11, 0);
    GFX_print(gfx, "    Chenyi Wang       ", 12, 0);
    GFX_print(gfx, "--------------------- ", 13, 0);
    GFX_print(gfx, "Press any key on      ", 14, 0);
    GFX_print(gfx, "Mobaxterm to start!   ", 15, 0);

    Application_UARTTitle(hal);     // prompts on serial terminal

}

/**
 * Callback function for when the game is in the TITLE_SCREEN state. Used
 * to break down the main Application_loop() function into smaller
 * sub-functions.
 */
void Application_handleTitleScreen(Application* app, HAL* hal)
{
    // Transition to start the game when any key from a serial terminal is pressed
    if (UART_hasChar(&hal->uart)){
        // Update internal logical state
        app->state = GAME_SCREEN;

        // prompt that the game is started
        Application_UARTBegin(hal);

        // Display the next state's screen to the user
        Application_showGameScreen(app, hal, &hal->gfx);
    }
}

/**
 * A helper function which resets all the game variables to their not updated
 * states and resets all interprets
 */
void Application_initGameVariables(Application* app, HAL* hal) {

    // Initialize local application state variables
    app->baudChoice = BAUD_9600;
    app->firstCall = true;
    app->input = SU;
    app->direction = CENTER;     // Inital position of the Explorer is at the middle
    app->engery = E_1;           // Initial State of Energy: 1
    app->move = Move_0;          // Initial State of Move: 0

    // reset all interprets
    app->isCommand = false;
    app->isMovement = false;
    app->isFeed = false;
    app->isAborted = false;
    app->levelUp = false;
    app->energyDown = false;

    app->currentX = CENTER_X;
    app->currentY = CENTER_Y;   // initial position is center
    app->previousX = NULL;
    app->previousY = NULL;


    app->energyValue = 1;       // initial Energy is 1
    app->experienceValue = 0;   // initial Experience is 0
    app->levelValue = 0;        // initial Level is 0
    app->nextValue = 1;         // initial To Next is 1
    app->moveValue = 0;         // initial Move is 0;
}

/**
 * Show game screen
 */
void Application_showGameScreen(Application* app, HAL* hal, GFX* gfx)
{
    // Clear the screen from any old text state
    GFX_clear(gfx);

    // Display the text
    GFX_print(gfx, "Exp:       Energy:   ", 0, 0);
    GFX_print(gfx, "Level:    To Next:   ", 1, 0);
    GFX_print(gfx, "Input:               ", 13, 0);
    GFX_print(gfx, "             Move:   ", 14, 0);

    // Draw rectangle border
    GFX_drawRectangle(gfx, X_MIN, Y_MIN, X_MAX, Y_MAX);

    // Drawing a circle for the explorer
    GFX_drawSolidCircle(gfx, CENTER_X, CENTER_Y, EXPLORER_RADIUS);

    // Printing all the explorer states with initial values
    GFX_print(gfx, "00", 0, 5);     // Experience = 0
    GFX_print(gfx, "1", 0, 19);     // Energy = 1
    GFX_print(gfx, "00", 1, 7);     // Level = 0
    GFX_print(gfx, "01", 1, 19);    // To Next = 1
    GFX_print(gfx, "0", 14, 19);    // Move = 0

    // Lights up LL2
    Application_energyIndicator(app, hal);

}

/**
 * Move the Explorer
 * There has a 3x3 grid of position
 * The Explorer will not walk into the borders
 */
void Application_handleGameScreen(Application* app, HAL* hal)
{
    // communicate with the serial terminal
    Application_UARTCommunication(app, hal);

    // update the position of the Explorer
    if (app->isMovement){
        // refresh the movement interpretation
        app->isMovement = false;

        app->previousX = app->currentX;
        app->previousY = app->currentY;

        // update the screen with the possible movement of Explorer and new states
        Application_updateGameScreen(app, &hal->gfx);
    }

    // updates to energy states
    if (app->isFeed)
    {
        app->isFeed = false;                                  // refresh the feed interpretation
        app->energyValue = (int) (app->engery);
        Application_updateEnergy(app, hal, &hal->gfx);        // update the screen with new energy state
    }
    if (app->energyDown)
    {
        app->energyDown = false;                              // refresh the interpretation
        app->engery = (EnergyState) (((int) app->engery - 1));
        app->energyValue -= 1;                                // decrease the Energy
        Application_updateEnergy(app, hal, &hal->gfx);        // update the screen with new energy state
    }

    // move to Game Over state if Energy reaches 0
    if (app->engery == E_0) {
        // change game state to Game Over
        app->state = RESULT_SCREEN;
        // Display the next state's screen to the user
        Application_showGameOverScreen(app, &hal->gfx);
        // prompts that the game is over on serial terminal
        Application_UARTOver(hal);
    }

}

/**
 * Helper functions which updates the main game screen by redrawing only the
 * positions where the Explorer could possibly be updated.
 */
void Application_updateGameScreen(Application* app, GFX* gfx) {

    // Clear the Explorer from any previous game screen
    GFX_removeSolidCircle(gfx, app->previousX, app->previousY, EXPLORER_RADIUS);

    // Update the direction states
    Application_updateDirectionState(app);

    // updates the current position
    Application_updateCoordinates(app);

    // Draw the Explorer
    GFX_drawSolidCircle(gfx, app->currentX, app->currentY, EXPLORER_RADIUS);

    // updates the move states
    Application_updateMove(app, gfx);

    // updates the value of Level
    Application_updateLevelState(app, gfx);

    // updates the value of To Next
    Application_updateNextState(app, gfx);

    // updates the value of Experience
    char expString[2] = "";
    expString[1] = (app->experienceValue % 10) + '0';
    expString[0] = ((app->experienceValue / 10) % 10) + '0';
    GFX_print(gfx, "  ", 0, 5);           // Clear the previous Experience value
    GFX_print(gfx, expString, 0, 5);      // Print the current Experience value

}

/**
 * Determine the direction from the command operation
 */
void Application_updateDirectionState(Application* app) {

    if (app->isLeft)
        app->direction = MOVE_L;
    else if (app->isRight)
        app->direction = MOVE_R;
    else if (app->isUp)
        app->direction = MOVE_U;
    else if (app->isDown)
        app->direction = MOVE_D;
}

/** update the x, y coordinates of the Explorer
 */
void Application_updateCoordinates(Application* app) {

    switch (app->direction){
    case MOVE_L:
        if (app->currentX > (CENTER_X - HORIZONTAL))
            app->currentX -= HORIZONTAL;
        break;
    case MOVE_R:
        if (app->currentX < (CENTER_X + HORIZONTAL))
            app->currentX += HORIZONTAL;
        break;
    case MOVE_U:
        if (app->currentY > (CENTER_Y - VERTICAL))
            app->currentY -= VERTICAL;
        break;
    case MOVE_D:
        if (app->currentY < (CENTER_Y + VERTICAL))
            app->currentY += VERTICAL;
        break;
    default:
        break;
    }
}

/**
 * A helper function which updates the main game screen by redrawing only the
 * positions where the Level value could possibly be updated.
 */
void Application_updateLevelState(Application* app, GFX* gfx)
{
    if ((app->experienceValue == app->nextValue)) {
        app->experienceValue = 0;
        app->levelValue += 1;
        app->levelUp = true;
        char levelString[2] = "";
        levelString[1] = (app->levelValue % 10) + '0';
        levelString[0] = ((app->levelValue / 10) % 10) + '0';
        GFX_print(gfx, "  ", 1, 7);               // Clear the previous Level value
        GFX_print(gfx, levelString, 1, 7);        // Print the current Level value
        // determine the To Next value
        app->nextValue = ((2 * app->levelValue) + 1);
    }

}

/**
 * A helper function which updates the main game screen by redrawing only the
 * positions where the To Next value could possibly be updated.
 */
void Application_updateNextState(Application* app, GFX* gfx)
{
    if (app->levelUp){
        app->levelUp = false;       // refresh the interpretation of level up
        // update To Next value
        char nextString[2] = "";
        nextString[1] = (app->nextValue % 10) + '0';
        nextString[0] = ((app->nextValue / 10) % 10) + '0';
        GFX_print(gfx, "  ", 1, 19);             // Clear the previous To Next value
        GFX_print(gfx, nextString, 1, 19);       // Print the current To Next value
    }
}

/**
 * A helper function which updates the main game screen by redrawing only the
 * positions where the move states could possibly be updated.
 */
void Application_updateMove(Application* app, GFX* gfx) {

    static MoveState currentMove = Move_0;

    switch (currentMove)
    {
    case Move_0:
        if ((app->currentX != app->previousX) || (app->currentY != app->previousY)) {
            currentMove = Move_1;
            app->moveValue = 1;
            app->experienceValue += 1;
        }
        break;
    case Move_1:
        if (((app->currentX != app->previousX) || (app->currentY != app->previousY)) || app->levelUp) {
            currentMove = Move_0;
            app->energyDown = true;
            app->moveValue = 0;
            app->experienceValue += 1;
        }
        break;
    }
    GFX_print(gfx, " ", 14, 19);             // Clear the previous Move
    char moveString[1] = "";
    moveString[0] = (app->moveValue % 10) + '0';
    GFX_print(gfx, moveString, 14, 19);      // Print the current Move

}

/**
 * A helper function which updates the main game screen by redrawing only the
 * positions where the energy states could possibly be updated.
 */
void Application_updateEnergy(Application* app, HAL* hal, GFX* gfx)
{

    char energyString[2] = "";
    energyString[0] = (app->energyValue % 10) + '0';
    GFX_print(gfx, " ", 0, 19);                // Clear the previous Energy level
    GFX_print(gfx, energyString, 0, 19);       // Print the current Energy level
    Application_energyIndicator(app, hal);     // Light up an LED when feeding happens at this point.

}

/**
 * Shows Game Over screen
 */
void Application_showGameOverScreen(Application* app, GFX* gfx)
{
    GFX_clear(gfx);
    GFX_print(gfx, "YOU ARE DEAD          ", 0, 0);
    GFX_print(gfx, "GAME OVER             ", 2, 0);

    Graphics_drawImage(&gfx->context, &gameOver8BPP_UNCOMP, 45, 30);

    GFX_print(gfx, "Highest Level:        ", 10, 0);

    char levelString[2] = "";
    levelString[1] = (app->levelValue % 10) + '0';
    levelString[0] = ((app->levelValue / 10) % 10) + '0';
    // Print the current Level value
    GFX_print(gfx, levelString, 10, 15);

    GFX_print(gfx, "--------------------- ", 12, 0);
    GFX_print(gfx, "Press any key to      ", 13, 0);
    GFX_print(gfx, "play gain!            ", 14, 0);
}

/**
 * Restart the game
 */
void Application_handleGameOverScreen(Application* app, HAL* hal) {

    // Transition to restart the game when any key from a serial terminal is pressed
    if (UART_hasChar(&hal->uart))
    {
        // Update internal logical state
        app->state = GAME_SCREEN;

        // Reset all game variables
        Application_initGameVariables(app, hal);

        // prompt the game to start
        Application_UARTBegin(hal);

        // Display the next state's screen to the user
        Application_showGameScreen(app, hal, &hal->gfx);
    }

}

/**
 * Echoing MobaXterm input on Display
 */
void Application_echoInput(Application* app, GFX* gfx, char* in) {

    if ((app->input == Sm) || (app->input == FEED))
        GFX_print(gfx, in, 13, 7);
    else if (app->input == Smv)
        GFX_print(gfx, in, 13, 8);
    else if (app->input == Smv_)
        GFX_print(gfx, in, 13, 9);
    else if (app->input == Smv_p)
        GFX_print(gfx, in, 13, 10);
    else
        GFX_print(gfx, "    ", 13, 7);

}


/**
 * Show title and instruction on serial termnial before game start
 */
void Application_UARTTitle(HAL* hal) {

    Application_UARTDivider(hal);
    UART_putChar(&hal->uart, '-');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'T');
    UART_putChar(&hal->uart, 'H');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'S');
    UART_putChar(&hal->uart, 'T');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'R');
    UART_putChar(&hal->uart, 'Y');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'F');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'A');
    UART_putChar(&hal->uart, 'N');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, 'X');
    UART_putChar(&hal->uart, 'P');
    UART_putChar(&hal->uart, 'L');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'R');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, 'R');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, '-');
    Application_updateLine(hal);
    Application_UARTDivider(hal);
    Application_UARTInsturction(hal);

}

/**
 * Show instruction on serial termnial
 */
void Application_UARTInsturction(HAL* hal) {

    UART_putChar(&hal->uart, 'P');
    UART_putChar(&hal->uart, 'r');
    UART_putChar(&hal->uart, 'e');
    UART_putChar(&hal->uart, 's');
    UART_putChar(&hal->uart, 's');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'a');
    UART_putChar(&hal->uart, 'n');
    UART_putChar(&hal->uart, 'y');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'k');
    UART_putChar(&hal->uart, 'e');
    UART_putChar(&hal->uart, 'y');
    UART_putChar(&hal->uart, '.');
    UART_putChar(&hal->uart, '.');
    UART_putChar(&hal->uart, '.');

}

/**
 * Prompt the game to start
 */
void Application_UARTBegin(HAL* hal) {

    UART_putChar(&hal->uart, 'B');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, 'G');
    UART_putChar(&hal->uart, 'I');
    UART_putChar(&hal->uart, 'N');
    UART_putChar(&hal->uart, ',');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'G');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'D');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'L');
    UART_putChar(&hal->uart, 'U');
    UART_putChar(&hal->uart, 'C');
    UART_putChar(&hal->uart, 'K');
    UART_putChar(&hal->uart, '!');
    Application_updateLine(hal);
    Application_updateLine(hal);
}

/**
 * Show instruction on serial termnial when game start
 */
void Application_UARTOver(HAL* hal) {

    Application_updateLine(hal);
    Application_UARTDivider(hal);
    UART_putChar(&hal->uart, 'G');
    UART_putChar(&hal->uart, 'A');
    UART_putChar(&hal->uart, 'M');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, ' ');
    UART_putChar(&hal->uart, 'O');
    UART_putChar(&hal->uart, 'V');
    UART_putChar(&hal->uart, 'E');
    UART_putChar(&hal->uart, 'R');
    Application_updateLine(hal);
    Application_updateLine(hal);
    Application_UARTDivider(hal);
    Application_UARTInsturction(hal);
}

/**
 * Show dividing line between prompts
 */
void Application_UARTDivider(HAL* hal) {

    unsigned int i = 40;
    while (i > 0) {
        UART_putChar(&hal->uart, '-');
        i--;
    }
    Application_updateLine(hal);
}
