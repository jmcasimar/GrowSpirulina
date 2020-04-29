import os
import sys
import serial
import select
from time import sleep
import colored_traceback
from datetime import datetime
sys.path.insert(0, './src/')
from gui import GUI
from logger import logger
from asciiART import asciiArt
from serialCallback import serialController

# Colored traceback useful for raise exception with colors in terminal
colored_traceback.add_hook()

art = asciiArt()
print("\033[1;36;40m", end='')
print(" Welcome to GrowSpirulina ".center(80,'*'))
art.img_print('./img/GrowSpirulina1_Web.png')
print("\033[0;37;40m")

# Check if temp dir exists, if not then create it
if not os.path.exists('temp/'): os.makedirs('temp/')

# Charge logger parameters
log = logger()

# From Serial Callback
serialControl = serialController(log.logger, log.logger_arduino)

# Charge GUI parameters and connect logger and serialControl
gui = GUI(log.logger, serialControl)

# Define functions
def GetLine(Block=False): # Get an Input Line
  if Block or select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], []):
      return input()

def mainClose(): # When program is finishing
    log.logger.warning("Closing devices")
    serialControl.close()
    log.logger.warning("Program finished")

day = 0
hour = (datetime.now()).hour
minute = (datetime.now()).minute

ask = False
if(len(sys.argv)<=1): ask = True
elif(str(sys.argv[1]) != 'yes'): ask = True
else: ask = False

if(ask):
    print("\nWith this window you will be able to control your production,"+
          " of Spirulina, but you must be capable to operate this system or you can cause severe damage.\n")
    start = input("Do you want to continue? y/n \n")
    if(start == "s" or start =="S" or start == "y" or start == "Y"):
        x=0
        log.logger.info("Setting Devices...")
        serialControl.open()
        log.logger.info("Devices ready")
        gui.begin()
    else:
        x=1
        log.logger.warning("Permission to start GrowGreens refused")
else: x = 0

try:
    while x==0:
        sleep(0.2)
        if gui.isOpen: gui.run()
        now = datetime.now()

        if(day!=now.day):
            day = now.day

        if(minute!=now.minute):
            hour = now.hour
            minute = now.minute
            if(hour<10):
                str_hour = "0"+str(hour)
            else:
                str_hour = str(hour)
            if(minute<10):
                str_minute = "0"+str(minute)
            else:
                str_minute = str(minute)
            str_date = "0"+str_hour+str_minute+"\r\n"
            arduino.write(bytes(str_date, 'utf-8'))

        CmD = GetLine()

        if(CmD=="On" or CmD=="on" or CmD=="ON"): serialControl.write(serialControl.arduino, "1\n")
        elif(CmD=="Off" or CmD=="off" or CmD=="OFF"): serialControl.write(serialControl.arduino, "2\n")
        elif(CmD=="heatOff" or CmD=="heatoff"): serialControl.write(serialControl.arduino, "3\n")
        elif(CmD=="heatOn" or CmD=="heaton"): serialControl.write(serialControl.arduino, "4\n")
        elif(CmD=="wmOn" or CmD=="wmon"): serialControl.write(serialControl.arduino, "5\n")
        elif(CmD=="wmOff" or CmD=="wmoff"): serialControl.write(serialControl.arduino, "6\n")
        elif(CmD=="airOn2" or CmD=="airon2"): serialControl.write(serialControl.arduino, "a\n")
        elif(CmD=="airOn3" or CmD=="airon3"): serialControl.write(serialControl.arduino, "b\n")
        elif(CmD=="airOff2" or CmD=="airoff2"): serialControl.write(serialControl.arduino, "c\n")
        elif(CmD=="airOff3" or CmD=="airoff3"): serialControl.write(serialControl.arduino, "d\n")
        elif(CmD=="airEnable2" or CmD=="airenable2"): serialControl.write(serialControl.arduino, "e\n")
        elif(CmD=="airEnable3" or CmD=="airenable3"): serialControl.write(serialControl.arduino, "f\n")

        serialControl.loop()

    if(x==1): mainClose()

except Exception as e:
    log.logger.critical("Exception Raised", exc_info=True)
    raise e

finally: mainClose()
