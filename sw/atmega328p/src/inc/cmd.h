#ifndef _CMD_PARSE_H_
#define _CMD_PARSE_H_

#define CHAR_NEWLINE    '\n'
#define CHAR_RETURN     '\r'
#define RETURN_NEWLINE  "\r\n"

#define ID_OFF  0
#define ID_ON   1

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
