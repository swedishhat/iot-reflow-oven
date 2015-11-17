/*** max31855.c ***/

#include "globals.h"

/*** Temperature Sensor "Object" Constructor ***/
max31855 *max31855_setup(void)
{
    // Reserve some space and make sure that it's not null
    max31855 *tempSense = malloc(sizeof(max31855));
    assert(tempSense != NULL);

    // Initilaize struct
    tempSense->extTemp = 0;
    tempSense->intTemp = 0;
    tempSense->status = UNKNOWN;
    // Not sure why Andy Brown makes his last temp time start at 0xFFFFD8EF but
    // it works... Maybe it's to test timer0 wrap around / guarantee causality:
    // https://github.com/andysworkshop/awreflow2/blob/master/atmega8l/TemperatureSensor.h
    tempSense->lastTempTime = 0xFFFFFFFF - 10000;
    tempSense->pollInterval = DEFAULT_POLL_INTERVAL;

    // Set GPIO direction
    CONFIG_AS_OUTPUT(MAX31855_CS);
    CONFIG_AS_OUTPUT(MAX31855_MOSI);
    CONFIG_AS_OUTPUT(MAX31855_SCK);
    CONFIG_AS_INPUT(MAX31855_MISO);

    // Enable pullup on ~CS
    PULLUP_ON(MAX31855_CS);

    // Set outputs to default values
    SET_HIGH(MAX31855_CS);
    SET_LOW(MAX31855_MOSI);
    SET_LOW(MAX31855_SCK);

    // Enable SPI, Master, set clock rate fosc/4 (already default but we're
    // Paranoid Patricks over here and also like to make our code clear!)
    SPCR = (1 << SPE) | (1 << MSTR);
    SPCR &= ~((1 << SPR1) | (1 << SPR0)); // Not necessary............

    // Super speed 2x SPI clock powerup!
    SPSR |= (1 << SPI2X);

    return tempSense;
}

/*** Read and Update Temperature Sensor Function ***/
bool max31855_readTempDone(max31855 *tempSense)
{
    if(msTimer_hasTimedOut(tempSense->lastTempTime, tempSense->pollInterval))
    {
        uint8_t i;              // Loop index
        uint32_t rawBits = 0;   // Raw SPI bus bits

        // Bring ~CS low
        SET_LOW(MAX31855_CS);

        // clock 4 bytes from the SPI bus
        for(i = 0; i < 4; i++)
        {
            SPDR = 0;                           // start "transmitting" (actually just clocking)
            while(!(SPSR & (1 << SPIF)));       // wait until transfer ends

            rawBits <<= 8;                      // make space for the byte
            rawBits |= SPDR;                    // merge in the new byte
        }

        // restore CS high
        SET_HIGH(MAX31855_CS);

        // parse out the temp / error code from the raw bits. Are switch
        // statements bad? I dunno. Maybe. Who cares?
        uint8_t d = rawBits & 7;        // Are there any errors?
        if(!d)
        {
            tempSense->status = OK;
            // Only when tempterature is valid will it update temp. To get
            // Celcius integer, temp bits isolated with & bitmask, shifted
            // to right to align LSB (18 for extTemp, 4 for intTemp),
            // shifted to right again to get Celsius (extTemp = 0.25C per
            // bit >> 2; intTemp = 0.0625 C per bit >> 4)
            tempSense->extTemp = rawBits >> 20;
            tempSense->intTemp = (rawBits & 0x0000FFF0) >> 8;

            // Extend sign bit if negative value is read. In an oven. HA!
            if(tempSense->extTemp & 0x0800)
                tempSense->extTemp |= 0xF000;
            if(tempSense->intTemp & 0x0020)
                tempSense->intTemp |= 0xFFC0;
        }
        else
        {
            // Set temps to something obviously wrong
            tempSense->extTemp = -22222;
            tempSense->intTemp = -11111;

            // Which error code is it?
            switch(d)
            {
                case 1:
                    tempSense->status = OC_FAULT;
                    break;
                case 2:
                    tempSense->status = SCG_FAULT;
                    break;
                case 4:
                    tempSense->status = SCV_FAULT;
                    break;
                default:
                    tempSense->status = UNKNOWN;
                    break;
            }
        }

        // Update the timestamp and let the read loop unblock
        tempSense->lastTempTime = msTimer_millis();
        return true;
    }
    return false;
}

/*** Status Message Helper Function ***/
const char *max31855_statusString(uint8_t status)
{
    switch(status)
    {
        case UNKNOWN:
            return "UNKNOWN";
        case OK:
            return "OK!";
        case SCV_FAULT:
            return "SCV_FAULT";
        case SCG_FAULT:
            return "SCG_FAULT";
        case OC_FAULT:
            return "OC_FAULT";
    }
    return "Err";
}

/*** Temperature Sensor Printing Function ***/
void max31855_print(max31855 *tempSense)
{
    // int16_t max = 65535, + '\0'
    //char ibuffer[6] = {0};
    char buffer[6] = {0};

    uart0_puts("Status: ");
    uart0_puts(max31855_statusString(tempSense->status));
    uart0_puts("\r\n");
    
    uart0_puts("External Temp: ");
    uart0_puts(itoa(tempSense->extTemp, buffer, 10));
    uart0_puts("\r\n");

    uart0_puts("Internal Temp: ");
    uart0_puts(itoa(tempSense->intTemp, buffer, 10));
    uart0_puts("\r\n");
}
