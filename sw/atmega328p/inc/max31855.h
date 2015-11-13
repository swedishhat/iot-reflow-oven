/*** max31855.h ***/

#ifndef _MAX31855_H_
#define _MAX31855_H_

/*** GPIO Definitions ***/
#define MAX31855_CS             PB2
#define MAX31855_CS_PORT        PORTB
#define MAX31855_CS_DDR         DDRB
#define MAX31855_CS_PIN         PINB

#define MAX31855_MOSI           PB3
#define MAX31855_MOSI_PORT      PORTB
#define MAX31855_MOSI_DDR       DDRB
#define MAX31855_MOSI_PIN       PINB

#define MAX31855_MISO           PB4
#define MAX31855_MISO_PORT      PORTB
#define MAX31855_MISO_DDR       DDRB
#define MAX31855_MISO_PIN       PINB

#define MAX31855_SCK            PB5
#define MAX31855_SCK_PORT       PORTB
#define MAX31855_SCK_DDR        DDRB
#define MAX31855_SCK_PIN        PINB

/*** Constants ***/
// Status Definitions
#define UNKNOWN                 0
#define OK                      1
#define SCV_FAULT               2
#define SCG_FAULT               3
#define OC_FAULT                4

#define DEFAULT_POLL_INTERVAL   1000    // Read temp probe every second

/*** MAX31855 Device Struct ***/
typedef struct max31855
{
    int16_t     extTemp;        // 14-bit TC temp
    int16_t     intTemp;        // 12-bit internal temp
    uint8_t     status;         // Status flags
    uint32_t    lastTempTime;   // "Timestamp"
    uint32_t    pollInterval;   // Refresh rate of sensor
} max31855;


/*** Temperature Sensor "Object" Constructor ***/
max31855 *max31855_setup(void);

/*** Temperature Sensor Printing Function ***/
void max31855_print(max31855 *tempSense);

/*** Read and Update Temperature Sensor Function ***/
bool max31855_readTempDone(max31855 *tempSense);

/*** Status Message Helper Function ***/
const char *max31855_statusString(uint8_t status);
#endif
