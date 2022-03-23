import serial
import time

def serialReader():
    with serial.Serial('COM10', 9600, timeout=10) as read_ser:
        #x = read_ser.read()          # read one 
        while True: # while not found device
            #s = read_ser.read(1024)        # read up to ten bytes (timeout)
            line = read_ser.readline()   # read a '\n' terminated line
            print(line)
            time.sleep(0.01)
        #read_ser.close()

def serialWriter():
    with serial.Serial('COM9', 9600, timeout=10) as write_ser:
        #print("secret message")
        write_ser.write("SECRETPASSWORD".encode())
        write_ser.close()
        print("Success")

def main(): 
    serialWriter()
    #serialReader()

  
  
# Using the special variable  
# __name__ 
if __name__=="__main__": 
    main() 