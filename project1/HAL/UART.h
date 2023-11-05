/*
 * UART.h
 *
 *  Created on: Dec 31, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Ever since the new API update in mid 2019, the UART configuration struct's
// name changed from [eUSCI_UART_Config] to [eUSCI_UART_ConfigV1]. If your code
// does not compile, uncomment the alternate #define and comment the original
// one.
// -----------------------------------------------------------------------------
// #define UART_Config eUSCI_UART_Config
#define UART_Config eUSCI_UART_ConfigV1

// An enum outlining what baud rates the UART_construct() function can use in
// its initialization.
enum _UART_Baudrate
{
    BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, NUM_BAUD_CHOICES
};
typedef enum _UART_Baudrate UART_Baudrate;

// TODO: Write an overview explanation of what this UART struct does, and how it
//       interacts with the functions below. Consult <HAL/Button.h> and
//       <HAL/LED.h> for examples on how to do this.

/**=============================================================================
 * A simple UART object, implemented in the C object-oriented style. Use the
 * constructor [UART_construct()] to create an UART object. Afterwards, when
 * accessing each method of the UART object, pass in a pointer to the proper
 * UART as the first argument of the method.
 * =============================================================================
 */
struct _UART
{
    UART_Config config;

    uint32_t moduleInstance;
    uint32_t port;
    uint32_t pins;
};
typedef struct _UART UART;

// Initializes a UART connection.
UART UART_construct(UART_Baudrate baudChoice);

// TODO: Write a comment which explains what each of these functions does. In the
//       header, prefer explaining WHAT the function does, as opposed to HOW it is
//       implemented.

// Receives character from the serial terminal
char UART_getChar(UART* uart_p);
// Determines if the user has sent a UART data packet to the board
bool UART_hasChar(UART* uart_p);
// Check if the UART module can send the character
bool UART_canSend(UART* uart_p);
// Send the character
void UART_putChar(UART* uart_p, char c);

// Updates the UART baudrate to use the new baud choice.
void UART_updateBaud(UART* uart_p, UART_Baudrate baudChoice);

#endif /* HAL_UART_H_ */
