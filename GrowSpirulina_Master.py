import os
import sys
import serial
import select
from time import sleep
import colored_traceback
from datetime import datetime
sys.path.insert(0, './src/')
from logger import logger
from asciiART import asciiArt

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

# Init Arduino object, Serial Port and Baud Rate
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=0)
arduino.close()

# Define functions

def GetLine(Block=False): # Get an Input Line
  if Block or select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], []):
      return input()

def mainClose(): # When program is finishing
    log.logger.warning("Closing devices")
    arduino.close()
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
        arduino.open()
        log.logger.info("Devices ready")
    else:
        x=1
        log.logger.warning("Permission to start GrowGreens refused")
else: x = 0

try:
    while x==0:
        sleep(0.2)
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

        if(CmD=="On" or CmD=="on" or CmD=="ON"): arduino.write(bytes("1\n",'utf-8'))
        elif(CmD=="Off" or CmD=="off" or CmD=="OFF"): arduino.write(bytes("2\n",'utf-8'))
        elif(CmD=="heatOff" or CmD=="heatoff"): arduino.write(bytes("3\n",'utf-8'))
        elif(CmD=="heatOn" or CmD=="heaton"): arduino.write(bytes("4\n",'utf-8'))
        elif(CmD=="wmOn" or CmD=="wmon"): arduino.write(bytes("5\n",'utf-8'))
        elif(CmD=="wmOff" or CmD=="wmoff"): arduino.write(bytes("6\n",'utf-8'))
        elif(CmD=="airOn2" or CmD=="airon2"): arduino.write(bytes("a\n",'utf-8'))
        elif(CmD=="airOn3" or CmD=="airon3"): arduino.write(bytes("b\n",'utf-8'))
        elif(CmD=="airOff2" or CmD=="airoff2"): arduino.write(bytes("c\n",'utf-8'))
        elif(CmD=="airOff3" or CmD=="airoff3"): arduino.write(bytes("d\n",'utf-8'))
        elif(CmD=="airEnable2" or CmD=="airenable2"): arduino.write(bytes("e\n",'utf-8'))
        elif(CmD=="airEnable3" or CmD=="airenable3"): arduino.write(bytes("f\n",'utf-8'))

        while arduino.inWaiting()>0:
            line = str(arduino.readline(), "utf-8")[0:-1]
            log.logger_arduino.info(line)

    if(x==1): mainClose()

except Exception as e:
    log.logger.critical("Exception Raised", exc_info=True)
    raise e

finally: mainClose()
