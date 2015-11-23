import time
import sys
import serial
import csv
from datetime import datetime

def countdown(t):
    while t:
        mins, secs = divmod(t, 60)
        timeformat = '{:02d}:{:02d}'.format(mins, secs)
        print(timeformat, end='\r')
        time.sleep(1)
        t -= 1

# Set up the port
ser = serial.Serial('/dev/ttyUSB0', 9600, timeout = 1)

oven_on = ("sp=100\r\n").encode('ascii')
oven_off = ("sp=0\r\n").encode('ascii')
get_tctemp = ("gte\r\n").encode('ascii')

ser.write(oven_off)

print('Measure step response of toaster')
print('Please ensure toaster is at ambient temp.')
print('Press ENTER to begin and <CTRL>+C to end.')
input('>>> ')

csv_file = open('stepresponse.csv', 'w', newline='')
writer = csv.writer(csv_file)

# Read some temps
print('Starting in')
countdown(5)

# Write some stuff to the oven
ser.write(oven_on)

try:
    while(True):
        ser.write(get_tctemp)
        tc_temp = (ser.readline()).decode('ascii')
        tc_temp = tc_temp.rstrip()
        
        print(tc_temp)
        writer.writerow([datetime.now(), tc_temp])
        
        time.sleep(0.01)

except KeyboardInterrupt:
    print("Interrupted. Closing CSV.")
    csv_file.close()
    ser.write(oven_off)


