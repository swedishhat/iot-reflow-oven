
/*
Copyright (c) 2013, Colin-N. Brosseau
Modifications by Patrick Lloyd, 2015
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "globals.h"

// Used for processing user unput on UART
static char uartData[CMD_LENGTH]  = {0};
static uint8_t dataCount = 0;


static int16_t cmd_parseAssignment (char input[CMD_LENGTH])
{
    // If set is true, assume a '=' in cmd and send to cmd_parseAssignment
    // This relies heavily on properly formatted data w/ very little robustness
    char *pch;
    char cmdValue[CMD_LENGTH];

    // Find the position the equals sign is
    // in the string, keep a pointer to it
    pch = strchr(input, '=');

    // Copy everything after that point into
    // the buffer variable
    strcpy(cmdValue, pch+1);

    // Now turn this value into an integer and
    // return it to the caller.
    return atoi(cmdValue);
}

static Command cmd_processCommand(void)
{
    // Temp array to work with
    char tmpData[8] = {0};

    // Put it on the stack!!
    Command cc;
    cc.hasValue = FALSE;
    cc.value = 0;
    cc.ucmd = NO_COMMAND;

    // Copy the contents of uartData into cmdData; clear uartData for reuse
    memcpy(tmpData, uartData, 8);
    memset(uartData, 0, 8);

    // Test input strings for command keywords
    if(strcasestr(_command_in, "gtc") != NULL)
        //cmd_printValue("TC Temp", tempSense->extTemp, ID_OFF);
        cc.ucmd = GET_TC_TEMP;

    if(strcasestr(_command_in, "grj") != NULL)
        //cmd_printValue("RJ Temp", tempSense->intTemp, ID_OFF);
        cc.ucmd = GET_RJ_TEMP;

    if(strcasestr(_command_in, "gst") != NULL)
        cc.ucmd = GET_MAX_STATUS;

    if(strcasestr(_command_in, "sgp") != NULL)
        cc.ucmd = GET_GAIN_P;

    if(strcasestr(_command_in, "sgi") != NULL)
        cc.ucmd = GET_GAIN_I;

    if(strcasestr(_command_in, "sgd") != NULL)
        cc.ucmd = GET_GAIN_D;

    if(strcasestr(_command_in, "sop") != NULL)
        cc.hasValue = TRUE;
        cc.ucmd = SET_POWER;
        cc.value = cmd_parseAssignment(tmpData);


        //if(strcasestr(_command_in, "p") != NULL)
        //{
        //    if(tmpcmd >= 0 && tmpcmd <= 100)
        //        oven_setDutyCycle(tmpcmd);
        //}
    if(strcasestr(_command_in, "sgp") != NULL)
    if(strcasestr(_command_in, "sgi") != NULL)
    if(strcasestr(_command_in, "sgd") != NULL)


    GET_GAIN_P,
    GET_GAIN_I,
    GET_GAIN_D,

    SET_OVEN_POWER,
    SET_GAIN_P,
    SET_GAIN_I,
    SET_GAIN_D

    return cc
}

void cmd_printValue (char *id, int value, uint8_t printID)
{
    char buffer[8];
    itoa(value, buffer, 10);

    if(printID)
    {
        uart0_puts(id);
        uart0_puts_P(" = ");
    }

    uart0_puts(buffer);
    uart0_puts_P(RETURN_NEWLINE);
}

void cmd_uartOK(void)
{
  uart0_puts_P("OK");
  uart0_puts_P(RETURN_NEWLINE);
}

UARTCommand cmd_processUART(void)
{
    /* Get received character from ringbuffer
    * uart0_getc() returns in the lower byte the received character and
    * in the higher byte (bitmask) the last receive error
    * UART_NO_DATA is returned when no data is available.   */
    uint16_t c = uart0_getc();

    UARTCommand cmd = NO_COMMAND;

    if ( !(c & UART_NO_DATA) )
    {
        /* New data available from UART. Check for Frame or Overrun error */
        if ( c & UART_FRAME_ERROR )
            /* Framing Error detected, i.e no stop bit detected */
            uart0_puts_P("UART Frame Error: ");

        if ( c & UART_OVERRUN_ERROR )
            /* Overrun, a character already present in the UART UDR register was
            * not read by the interrupt handler before the next character arrived,
            * one or more received characters have been dropped */
            uart0_puts_P("UART Overrun Error: ");

        if ( c & UART_BUFFER_OVERFLOW )
            /* We are not reading the receive buffer fast enough,
            * one or more received character have been dropped  */
            uart0_puts_P("Buffer overflow error: ");


        // Add char to input buffer
        uartData[dataCount] = c;

        // Return is signal for end of command input
        if (UARTdata_in[UARTdata_count] == CHAR_RETURN)
        {
            // Reset to 0, ready to go again
            data_count = 0;
            //uart0_puts(RETURN_NEWLINE);

            cmd = cmd_processCommand();
            //cmd_uartOK();
        }
        else _data_count++;

        //uart0_putc( (unsigned char)c );
    }
    return cmd;
}
