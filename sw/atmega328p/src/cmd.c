/*
Copyright (c) 2013, Colin-N. Brosseau
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

unsigned char _data_count = 0;
int _variable_A = 23; //user modifiable variable
int _variable_goto = 12; //user modifiable variable
unsigned char _data_in[8] = {0};
char _command_in[8] = {0};


int cmd_parseAssignment (char input[16])
{
    char *pch;
    char cmdValue[16];

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

void cmd_copyCommand (void)
{
    // Copy the contents of data_in into command_in
    memcpy(_command_in, _data_in, 8);

    // Now clear _data_in, the UART can reuse it now
    memset(_data_in, 0, 8);
}

void cmd_processCommand(max31855 *tempSense)
{
    if(strcasestr(_command_in, "g") != NULL)
    {
        if(strcasestr(_command_in, "t") != NULL)
        {
            if(strcasestr(_command_in, "e") != NULL)
                cmd_printValue("TC Temp", tempSense->extTemp, ID_OFF);

            if(strcasestr(_command_in, "i") != NULL)
                cmd_printValue("RJ Temp", tempSense->intTemp, ID_OFF);
        }

        if(strcasestr(_command_in, "s") != NULL)
        {
            uart0_puts(max31855_statusString(tempSense->status));
            uart0_puts(RETURN_NEWLINE);
        }
    }

    if(strcasestr(_command_in, "s") != NULL)
    {
        // If set is true, assume a '=' in cmd and send to cmd_parseAssignment
        int tmpcmd = cmd_parseAssignment(_command_in);

        // if(strcasestr(_command_in, "f") != NULL)
        // {
        //
        //     if (tmpcmd > 0)
        //         oven_fade(tmpcmd);
        // }

        if(strcasestr(_command_in, "p") != NULL)
        {
            if(tmpcmd >= 0 && tmpcmd <= 100)
                oven_setDutyCycle(tmpcmd);
        }
    }
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

void cmd_processUART(max31855 *tempSense)
{
    /* Get received character from ringbuffer
    * uart0_getc() returns in the lower byte the received character and
    * in the higher byte (bitmask) the last receive error
    * UART_NO_DATA is returned when no data is available.   */
    unsigned int c = uart0_getc();

    if ( !(c & UART_NO_DATA) )
    {
        // new data available from UART check for Frame or Overrun error
        if ( c & UART_FRAME_ERROR ) {
            /* Framing Error detected, i.e no stop bit detected */
            uart0_puts_P("UART Frame Error: ");
        }
        if ( c & UART_OVERRUN_ERROR ) {
            /* Overrun, a character already present in the UART UDR register was
            * not read by the interrupt handler before the next character arrived,
            * one or more received characters have been dropped */
            uart0_puts_P("UART Overrun Error: ");
        }
        if ( c & UART_BUFFER_OVERFLOW ) {
            /* We are not reading the receive buffer fast enough,
            * one or more received character have been dropped  */
            uart0_puts_P("Buffer overflow error: ");
        }

        // Add char to input buffer
        _data_in[_data_count] = c;

        // Return is signal for end of command input
        if (_data_in[_data_count] == CHAR_RETURN)
        {
            // Reset to 0, ready to go again
            _data_count = 0;
            uart0_puts(RETURN_NEWLINE);

            cmd_copyCommand();
            cmd_processCommand(tempSense);
            cmd_uartOK();
        }
        else _data_count++;

        uart0_putc( (unsigned char)c );
    }
}
