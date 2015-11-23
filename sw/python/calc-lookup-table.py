import numpy as np
import matplotlib.pyplot as plt

# Define area under sine curve from t1 to t2
def sin_area(f, a, t1, t2):
    cos_t2 = -np.cos(2 * np.pi * f * t2)
    cos_t1 = -np.cos(2 * np.pi * f * t1)    
    return a * (cos_t2 - cos_t1) / (2 * np.pi * f)

# System parameters
f = 1 / 360
a = 1.0
t = np.linspace(0, 180, 1000)   # More points = same LUT with prettier graphs

percent_mapper = t * 100 / 180  # Maps a phase ange / 180 to a power / 100 

total_area = sin_area(f, a, t[0], t[-1])    # Area under half a period of sine

# Determine area of each slice from t[0] to t
areas = sin_area(f, a, t[0], t)

percent_pow = 100 * (1 - (total_area - areas) / total_area)
int_percent_pow = np.around(percent_pow)
errors = np.absolute(percent_pow - int_percent_pow)

# LUT to hold all of our remapped values
power_lut = []

# Iterate over list of unique values in int_percent_pow
for integer in set(int_percent_pow):
    # This could maybe be improved with better list comprehensions:
    # https://docs.python.org/3/tutorial/datastructures.html#tut-listcomps    
    
    # Get list of indices corresponding to each integer in int_percent_pow
    int_indices = ([i for i, x in enumerate(int_percent_pow) if x == integer])
    
    # Use those indecies to define a sublist of errors defined by int_indices
    e = errors[int_indices[0]:int_indices[-1]]
    
    # Find index of min error in e; add it back to int_indices for global index
    pmap_index = int_indices[0] + np.argmin(e)

    # Fill up our lovely, lovely lookup table    
    power_lut.append([pmap_index, np.around(percent_mapper[pmap_index])])

# Cast the percent_mapper values to ints for easy copypasta
power_lut = [(x[0], int(x[1])) for x in power_lut]

# Print the LUT out to admire it and copypasta into your code
print('Oh boy! Just look at that LUT!')
print([x[1] for x in power_lut])

# Put everything onto a plot
plt.figure()
plt.plot(t, percent_pow, \
        'b-', linewidth = 2, label = '% Power')
plt.plot(t, percent_mapper, \
        'r-', label = '% Mapped to')
plt.plot(t[[x[0] for x in power_lut]], [x[1] for x in power_lut], \
        'k.', label = 'LUT Values')
plt.legend(loc = 'best')
plt.show()
