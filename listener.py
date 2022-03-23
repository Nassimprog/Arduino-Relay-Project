import serial
import time
    
with serial.Serial('COM10', 9600, timeout=10) as ser:
    #x = ser.read()          # read one 
    while True: # while not found device
        #s = ser.read(1024)        # read up to ten bytes (timeout)
        line = ser.readline()   # read a '\n' terminated line
        print(line)
        time.sleep(0.01)


