import time
import serial
import sys

# Get some config info from the user
def par(x):
    return {
        0 : serial.PARITY_NONE,
        '0' : serial.PARITY_ODD,
        'e' : serial.PARITY_EVEN,
        'm' : serial.PARITY_MARK,
        's' : serial.PARITY_SPACE,
    }.get(x, serial.PARITY_NONE)

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port = sys.argv[1],
    baudrate = sys.argv[2],
    parity = serial.PARITY_ODD,
    stopbits = serial.STOPBITS_TWO,
    bytesize = serial.SEVENBITS
)

ser.open()
ser.isOpen()

print 'Enter your commands below.\r\nInsert "exit" to leave the application.'

input=1
while 1 :
    # get keyboard input
    input = raw_input(">> ")
        # Python 3 users
        # input = input(">> ")
    if input == 'exit':
        ser.close()
        exit()
    else:
        # send the character to the device
        # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
        ser.write(input + '\r\n')
        out = ''
        # let's wait one second before reading output (let's give device time to answer)
        time.sleep(1)
        while ser.inWaiting() > 0:
            out += ser.read(1)
            
        if out != '':
            print ">>" + out
