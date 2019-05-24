import time
from datetime import datetime
import serial

# Init Arduino object, Serial Port and Baud Rate
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
arduino.close()

day = 0
hour = (datetime.now).hour
minute = (datetime.now).minute

print("Setting Devices...")
arduino.open()
print("Devices ready")
print("")

start = input("Do you want to continue? y/n \n")
if(start == "s" or start =="S" or start == "y" or start == "Y"):
    x=0
    print("\nStarting program...")
else: x=1

while x==0:
    now = datetime.now()
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

        arduino.write(bytes("0"+str_hour+str_minute+"\r\n", 'utf-8'))

    while arduino.inWaiting()>0:
        line = arduino.readline()
        print("\nArduino sent: {}".format(str(line,'utf-8')))

if(x==1):
    print("Closing devices")
    arduino.close()
    print("Program finished")
