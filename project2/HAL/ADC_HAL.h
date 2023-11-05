/*
 * ADC_HAL.h
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */

#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define LEFT_THRESHOLD  5000
#define RIGHT_THRESHOLD 11000
#define UP_THRESHOLD    11000
#define DOWN_THRESHOLD  5000

enum _JoystickState
{
    MIDDLE, UP, DOWN
};
typedef enum _JoystickState JoystickState;

struct _Joystick
{
    int x;  // read x - horizontal reading of the joystick
    int y;  // read y - vertick reading of the joystick

    JoystickState jsState;

    bool isTappedUp;
    bool isTappedDown;

};
typedef struct _Joystick Joystick;

void initADC();
void startADC();
void initJoyStick();

Joystick Joystick_consturct();

void Joystick_refresh(Joystick *);

void Joystick_moveCursor(Joystick *);

bool Joystick_isTappedUp(Joystick *);
bool Joystick_isTappedDown(Joystick *);

#endif /* ADC_HAL_H_ */
