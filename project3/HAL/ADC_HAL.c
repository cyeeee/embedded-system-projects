/*
 * ADC_HAL.c
 *
 *  Created on: Oct 18, 2018
 *      Author: Leyla
 */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <HAL/ADC_HAL.h>

/*
 * Joystick portion
 */
Joystick Joystick_consturct() {

    Joystick joystick;

    initJoyStick();

    joystick.jsState = MIDDLE_J;

    return joystick;
}

// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    // This sets the conversion clock to 3MHz
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 up to ADC_MEM1. Each conversion will
    // thus use two channels.
    // 2 for joystick and 3 for accel
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // TODO: which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    ADC14_configureConversionMemory(ADC_MEM1,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A9,                 // joystick y
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);




}

void Joystick_refresh(Joystick *joystick_p) {

    // ADC runs in continuous mode, we just read the conversion buffers
    joystick_p->x = ADC14_getResult(ADC_MEM0);

    // Read the Y channel
    joystick_p->y = ADC14_getResult(ADC_MEM1);

    joystick_p->isTappedUp = false;
    joystick_p->isTappedDown = false;

    Joystick_moveCursor(joystick_p);

}

void Joystick_moveCursor(Joystick *joystick_p)
{
    if (joystick_p->jsState == MIDDLE_J)
    {
        if (joystick_p->y > UP_THRESHOLD)
        {
            joystick_p->isTappedUp = true;
            joystick_p->jsState = UP_J;
        }
        else if (joystick_p->y < DOWN_THRESHOLD)
        {
            joystick_p->isTappedDown = true;
            joystick_p->jsState = DOWN_J;
        }
    }
    else
    {
        if ((joystick_p->y < UP_THRESHOLD) && (joystick_p->y > DOWN_THRESHOLD))
            joystick_p->jsState = MIDDLE_J;
    }
}

bool Joystick_isTappedUp(Joystick *joystick_p)
{
    return (joystick_p->isTappedUp);
}

bool Joystick_isTappedDown(Joystick *joystick_p)
{
    return (joystick_p->isTappedDown);
}


/*
 * Accelerometer portion
 */
Accel Accel_consturct() {

    Accel accel;

    initAccel();

    accel.LeftRightState = MID_LR;
    accel.UpDownState = MID_UD;

    return accel;
}

// Interfacing the Accelerometer with ADC (making the proper connections in software)
void initAccel() {

    // x-axis of accel
    ADC14_configureConversionMemory(ADC_MEM2,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A14,
                                  ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN1,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    // y-axis of accel
    ADC14_configureConversionMemory(ADC_MEM3,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A13,
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    // z-axis of accel
    ADC14_configureConversionMemory(ADC_MEM4,
                                   ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                   ADC_INPUT_A11,
                                   ADC_NONDIFFERENTIAL_INPUTS);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


}

void Accel_refresh(Accel *accel)
{
    // updating the raw data
    // ADC runs in continuous mode, we just read the conversion buffers
    // Read the X axis
    accel->x = ADC14_getResult(ADC_MEM2);

    // Read the Y axis
    accel->y = ADC14_getResult(ADC_MEM3);

    // Read the Z axis
    accel->z = ADC14_getResult(ADC_MEM4);

    // update the states
    accel->LeftRightState = MID_LR;
    accel->UpDownState = MID_UD;
    accel->isLeft = false;
    accel->isRight = false;
    accel->isUp = false;
    accel->isDown = false;

    Accel_interpretDirection(accel);
}

void Accel_interpretDirection(Accel *accel)
{
    if (accel->LeftRightState == MID_LR)
    {
        if (accel->x < LEFT_TH)
        {
            accel->isLeft = true;
            accel->LeftRightState = LEFT_A;
        }
        else if (accel->x > RIGHT_TH)
        {
            accel->isRight = true;
            accel->LeftRightState = RIGHT_A;
        }
    }
    else
    {
        if ((accel->x > LEFT_TH) && (accel->x < RIGHT_TH))
            accel->LeftRightState = MID_LR;
    }

    if (accel->UpDownState == MID_UD)
    {
        if (accel->y > UP_TH)
        {
            accel->isUp = true;
            accel->UpDownState = UP_A;
        }
        else if (accel->y < DOWN_TH)
        {
            accel->isDown = true;
            accel->UpDownState = DOWN_A;
        }
    }
    else
    {
        if ((accel->y < UP_TH) && (accel->y > DOWN_TH))
            accel->UpDownState = MID_UD;
    }

}

bool Accel_isLeft(Accel *accel)
{
    return (accel->isLeft);
}

bool Accel_isRight(Accel *accel)
{
    return (accel->isRight);
}

bool Accel_isUp(Accel *accel)
{
    return (accel->isUp);
}

bool Accel_isDown(Accel *accel)
{
    return (accel->isDown);
}

