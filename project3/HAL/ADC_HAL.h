/*
 * ADC_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// for joystick
#define UP_THRESHOLD    12000
#define DOWN_THRESHOLD  4000
// for accelerometer
#define LEFT_TH         7200
#define RIGHT_TH        7800
#define UP_TH           7800
#define DOWN_TH         7200

enum _JoystickState
{
    MIDDLE_J, UP_J, DOWN_J
};
typedef enum _JoystickState JoystickState;

enum _AccelLRState
{
    LEFT_A, MID_LR, RIGHT_A
};
typedef enum _AccelLRState AccelLRState;

enum _AccelUDState
{
    UP_A, MID_UD, DOWN_A
};
typedef enum _AccelUDState AccelUDState;

struct _Joystick
{
    int x;  // read x - horizontal reading of the joystick
    int y;  // read y - vertick reading of the joystick

    JoystickState jsState;

    bool isTappedUp;
    bool isTappedDown;

};
typedef struct _Joystick Joystick;


struct _Accel
{
    // raw data
    int x;  // x - axis
    int y;  // y - axis
    int z;  // z - axis

    // extras
    AccelLRState  LeftRightState;
    AccelUDState  UpDownState;

    bool isLeft;
    bool isRight;
    bool isUp;
    bool isDown;

};
typedef struct _Accel Accel;


void initADC();
void startADC();
void initJoyStick();
void initAccel();

Joystick Joystick_consturct();
void Joystick_refresh(Joystick *);

void Joystick_moveCursor(Joystick *);
bool Joystick_isTappedUp(Joystick *);
bool Joystick_isTappedDown(Joystick *);

Accel Accel_consturct();
void Accel_refresh(Accel *);

void Accel_interpretDirection(Accel *);
bool Accel_isLeft(Accel *);
bool Accel_isRight(Accel *);
bool Accel_isUp(Accel *);
bool Accel_isDown(Accel *);

#endif /* ADC_HAL_H_ */
