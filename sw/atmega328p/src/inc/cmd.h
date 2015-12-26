#ifndef _CMD_PARSE_H_
#define _CMD_PARSE_H_

// Commands
typedef enum
{
    NO_COMMAND,
    GET_TC_TEMP,    // gtc
    GET_RJ_TEMP,    // grj
    GET_MAX_STATUS, // gst

    GET_GAIN_P,     // ggp
    GET_GAIN_I,     // ggi
    GET_GAIN_D,     // ggd

    SET_OVEN_POWER, // sop=###
    SET_GAIN_P,     // sgp=###
    SET_GAIN_I,     // sgi=###
    SET_GAIN_D      // sgd=###
} UARTCommand;

typedef struct CtrlCmd
{
    uint8_t hasValue;
    UARTCommand ucmd;
    int16_t value;
} CtrlCmd;


#define CHAR_NEWLINE    '\n'
#define CHAR_RETURN     '\r'
#define RETURN_NEWLINE  "\r\n"

#define ID_OFF  0
#define ID_ON   1

#define CMD_LENGTH 16

extern unsigned char _data_count;
extern unsigned char _data_in[8];
extern char _command_in[8];

// Commands......
//extern int _getTempCmd;
//extern int _getIntTempCmd;
//extern int _ovenPowerCmd;
//extern int _fadeDelayCmd;

// Eventually add "save to EEPROM" commands

extern int cmd_parseAssignment (char input[16]);
extern void cmd_copyCommand (void);
extern void cmd_processCommand(max31855 *tempSense);
void cmd_printValue (char *id, int value, uint8_t printID);
extern void cmd_processUART(max31855 *tempSense);
extern void cmd_uartOK(void);

#endif
