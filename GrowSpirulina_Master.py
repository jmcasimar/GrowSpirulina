import time
from datetime import datetime
import serial
import os
import select
import sys

# Get an Input Line
def GetLine(Block=False):
  if Block or select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], []):
      return input()
    
# Init Arduino object, Serial Port and Baud Rate
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
arduino.close()

day = 0
hour = (datetime.now()).hour
minute = (datetime.now()).minute

print("Setting Devices...")
arduino.open()
print("Devices ready")
print("\n***** Welcome to Grow Spirulina *****\n\nWith this window you will be able to control your production,"+
      " of Spirulina, but you must be capable to operate this system or you can cause severe damage.\n")
start = input("Do you want to continue? y/n \n")
if(start == "s" or start =="S" or start == "y" or start == "Y"):
    x=0
    print("\nStarting program...")
else: x=1

while x==0:
    now = datetime.now()
    
    if(day!=now.day):
        day = now.day
        filelog = 'log/arduino-{0}-{1}-{2}.log'.format(now.day,now.month,now.year)
        
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
    
    if(CmD=="On" or CmD=="on" or CmD=="ON"):
        arduino.write(bytes("1\n",'utf-8'))
    elif(CmD=="Off" or CmD=="off" or CmD=="OFF"):
        arduino.write(bytes("2\n",'utf-8'))
        
    while arduino.inWaiting()>0:
        line = arduino.readline()
        printLine = "Arduino sent: " + str(line,'utf-8')
        print(printLine)
        if(os.path.isfile(filelog)):
            f = open(filelog, "a+")
        else:
            f = open(filelog, "w+")
        f.write("{0}:{1}:{2} ".format(now.hour,now.minute,now.second) + printLine)
        f.close()

if(x==1):
    print("Closing devices")
    arduino.close()
    print("Program finished")
