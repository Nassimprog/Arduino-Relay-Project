import serial
import time
import threading

import logging

# COM9 is Proxy
# COM10 is Reader
# COM11 is Mole


class SerialRelay:
    def __init__(self):
        self.mole = serial.Serial('COM11', 9600, timeout=10)        
        self.proxy = serial.Serial('COM9', 9600, timeout=10)
        #self.reader = serial.Serial('COM10', 9600, timeout=10)
        self.running = True
        self.log = logging.getLogger(__name__)


    def runReader(self):
        """
        Does whatever the reader does
        """
        self.log.info("Starting Reader")
        while self.running:
            line = self.reader.readline() 
            self.log.debug("READER: %s", line) 
            time.sleep(0.01)
           # self.proxy.write(line)

    def runMole(self):
        """
        Does whatever the mole Does
        """

        self.log.info("Starting Mole")
        while self.running:
            data = self.mole.readline()
            self.log.debug("MOLE: %s", data) 
            if b"MSG" in data:
                self.proxy.write(data)

    def runProxy(self):
        """
        Does whatever the Proxy doesdata
        """
        
        self.log.info("Starting Proxy")
        while self.running:
            #print("Run Reader")
            data = self.proxy.readline()
            self.log.debug("PROXY: %s", data)
            if b"MSG" in data:
                self.mole.write(data)


    def run(self):

        read = threading.Thread(target = self.runReader)
        # read.start()

        proxy = threading.Thread(target = self.runProxy)
        proxy.start()


        mole = threading.Thread(target = self.runMole)
        mole.start()


if __name__ == "__main__":
    logging.basicConfig(level = logging.DEBUG)

    relay = SerialRelay()
    #relay.runReader()
    relay.run()