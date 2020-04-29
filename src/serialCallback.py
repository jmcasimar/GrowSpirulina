#!/usr/bin/env python3

# Import directories
from serial import Serial
from time import time, sleep

class serialController:
    def __init__(self, logMaster, logArduino):
        # Define loggers
        self.logMaster = logMaster
        self.logArduino = logArduino

        # Define microcontrolers
        try:
            self.arduino = Serial('/dev/ttyACM0', 115200, timeout=0)
            self.arduino.dtr = False # Reset
            self.arduino.close()
            self.arduinoIsConnected = True
        except Exception as e:
            self.arduinoIsConnected = False
            raise Exception("Communication with arduino device cannot be stablished. [{}]".format(e))

    def open(self):
        if self.arduinoIsConnected and not self.arduino.is_open:
            self.arduino.open()
            sleep(0.33)
            self.arduino.reset_input_buffer()
            self.arduino.dtr = True

    def close(self):
        if self.arduinoIsConnected and self.arduino.is_open: self.arduino.close()

    def Msg2Log(self, logger, mssg):
        if(mssg.startswith("debug,")): logger.debug(mssg.split(",")[1])
        elif(mssg.startswith("info,")): logger.info(mssg.split(",")[1])
        elif(mssg.startswith("warning,")): logger.warning(mssg.split(",")[1])
        elif(mssg.startswith("error,")): logger.error(mssg.split(",")[1])
        elif(mssg.startswith("critical,")): logger.critical(mssg.split(",")[1])
        else: logger.info(mssg)

    def write(self, serialObject, mssg):
        if serialObject == self.arduino and self.arduinoIsConnected:
            serialObject.write(bytes(mssg, "utf-8"))
            serialObject.flush()
        else: self.logMaster.error("Cannot write to serial device. It is disconnected.")

    def loop(self):
        if self.arduinoIsConnected:
            # If bytes available in generalControl
            while self.arduino.in_waiting>0:
                line = str(self.arduino.readline(), "utf-8")[0:-1]
                self.Msg2Log(self.logArduino, line)
