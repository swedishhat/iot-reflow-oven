import sys
import glob
import serial

def get_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def port_mon(port):
    """ Opens serial port monitor

        :raises 
        :returns
    """
    thing = 0
    return thing

if __name__ == '__main__':
    port_list = get_ports()
    if len(port_list) > 0:
        # Just dump some status info about the ports. Make the user pick one.
        print('{} serial port{} detected. Please select port number to continue:'
            .format(len(port_list), '' if len(port_list)==1 else 's'))
        print('=================================================================')

        for port in range(len(port_list)):
            print('{} : {}'.format(port + 1, port_list[port]))
        
        # Select serial port and open port monitor
        while True:
            selected = int(input('--> '))
            if selected - 1 not in range(len(port_list)):
                print('{} not an option'.format(selected))
            else:
                print('You selected {}. Cool.'.format(port_list[selected - 1]))
                port_mon(port_list[selected - 1])
                break
        
    else:
        print('No ports detected. Please check your stuff and try again.')
