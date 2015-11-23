import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from datetime import datetime, timedelta

# Define the exponential function we want to fit a curve to
def exponential(x, a, b, c):
    return a * np.exp(-b * x) + c

# Open CSV file in read+binary mode
#f = open(sys.argv[1], newline='')
f = open('natural-response.csv', newline='')

# Lists for CSV Datas
timestamp = []  
tempdata = []

# Read the CSV file and convert the data back into more useful types
try:
    reader = csv.reader(f)
    for row in reader:
        timestamp.append(datetime.strptime(row[0], '%Y-%m-%d %H:%M:%S.%f'))
        tempdata.append(int(row[1]))
finally:
    f.close()

# Determine from timestamps an array t with elements 
# equal to the time after beginning test
t = []
for entry in timestamp:
    delta = entry - timestamp[0]
    t.append(delta.seconds + delta.microseconds / 1000000)

# Numbered list from 0 to however big the array is.
# Not used but I think it's a cool expression
x = [i for i in range(len(tempdata))]

# Determine thermal time constant. During cooldown, find time to go from
tc = t[next(i[0] for i in enumerate(tempdata) if i[1] < 236)]

# Numpy stop being dumb and just let me use t as an argument!
popt, pcov = curve_fit(exponential, np.array(t), tempdata, p0 = (400, 1/tc, 27))
guess = exponential(np.array(t), *popt)
print('System parameters\n-----------------')
print('Estimated thermal time constant: {0} seconds'.format(tc))
print('Optimized thermal time constant: {0} seconds'.format(1/popt[1]))

print('Optimized values:')
print('y(0) = {0}, 1/TC = {1}, c = {2}'.format(*popt))

# Put everything onto a plot
plt.figure()
plt.plot(t, tempdata, 'b-', linewidth = 2, label="Original Sampled Data")
plt.plot(t, guess, 'r-', label="Fitted Curve")
plt.legend()
plt.show()
