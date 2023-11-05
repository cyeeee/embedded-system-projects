/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>
#include <HAL/Graphics.h>
#include <HAL/Timer.h>

#define EMPTY_WAIT      100  // 0.1 seconds
#define EXPLORER_RADIUS 5
#define CENTER_X        62
#define CENTER_Y        59
#define HORIZONTAL      22
#define VERTICAL        15

enum _GameState
{
    EMPTY, TITLE_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _InputState
{
    SU, Sm, Smv, Smv_, Smv_p, M_ENTER, FEED, F_ENTER
};
typedef enum _InputState InputState;

enum _EnergyState
{
    E_0, E_1, E_2, E_3, E_4, E_5
};
typedef enum _EnergyState EnergyState;

enum _Direction
{
    CENTER, MOVE_L, MOVE_R, MOVE_U, MOVE_D
};
typedef enum _Direction Direction;

enum _Move
{
    Move_0, Move_1
};
typedef enum _Move MoveState;

struct _Application
{

    UART_Baudrate baudChoice;
    bool firstCall;

    // General-purpose timer for when screens must disappear
    SWTimer timer;

    // Booleans to determine which command the user has entered
    bool isCommand;
    bool isMovement;
    bool isFeed;
    bool isAborted;

    // Booleans to determine the direction of movement
    bool isLeft;
    bool isRight;
    bool isUp;
    bool isDown;

    // Boolean to determine if the Explorer is leveled up
    bool levelUp;

    // Boolean to determine if the energy is used up
    bool energyDown;

    GameState state;        // Determines which screen is currently shown
    InputState input;       // determine the input to serial terminal
    EnergyState engery;     // ties Energy stat to feeding
    Direction direction;    // determine the command operation
    MoveState move;         // determine the number of moves

    // int's to store the state values
    int currentX;
    int currentY;
    int previousX;
    int previousY;
    int experienceValue;
    int levelValue;
    int nextValue;
    int moveValue;
    int energyValue;
};
typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct(HAL* hal);

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);

// Restart/Update UART communications
void Application_UARTCommunication(Application* app, HAL* hal);

// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app, HAL* hal);

// Interprets the input command
// The response from serial terminal to the input command
void Application_UARTResponse(Application* app, HAL* hal);

// Start with a new line
void Application_updateLine(HAL* hal);

// Interprets an incoming character and echoes back to terminal
// what character was received (except "Enter" and "Esc")
char Application_interpretIncomingChar(char);

// Interprets the input command operation
void Application_commandFSM(Application* app, char);

// Interprets the input movement command direction
void Application_interpretDirection(Application* app, char);

// Light up an LED when feeding happens at this point.
void Application_energyIndicator(Application* app, HAL* hal);

// Generic circular increment function
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);

// Drawing screens
void Application_showTitleScreen(Application* app, HAL* hal, GFX* gfx);
void Application_showGameScreen(Application* app, HAL* hal, GFX* gfx);
void Application_showGameOverScreen(Application* app, GFX* gfx);

// Handle each screens
void Application_handleEmpty(Application* app, HAL* hal);
void Application_handleTitleScreen(Application* app, HAL* hal);
void Application_handleGameScreen(Application* app, HAL* hal);
void Application_handleGameOverScreen(Application* app, HAL* hal);

// Helper functions which reduces the number of redraws required when moving the
// Explorer.
void Application_initGameVariables(Application* app, HAL* hal);         // initial all game variables
void Application_updateGameScreen(Application* app, GFX* gfx);          // update game screen with possible changes
void Application_updateDirectionState(Application* app);                // determine the current direction state
void Application_updateCoordinates(Application* app);                   // determine the current position
void Application_updateLevelState(Application* app, GFX* gfx);          // update the Level value
void Application_updateNextState(Application* app, GFX* gfx);           // update the To Next value
void Application_updateMove(Application* app, GFX* gfx);                // update the Move value
void Application_updateEnergy(Application* app, HAL* hal, GFX* gfx);    // update the Energy value

// Echoing Mobaxterm input on display
void Application_echoInput(Application* app, GFX* gfx, char*);

// Show instruction on serial terminal
void Application_UARTTitle(HAL* hal);           // title
void Application_UARTInsturction(HAL* hal);     // instruction
void Application_UARTBegin(HAL* hal);           // game begin
void Application_UARTOver(HAL* hal);            // game over
void Application_UARTDivider(HAL* hal);         // dividing line

#endif /* APPLICATION_H_ */
