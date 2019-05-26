import time
from datetime import datetime
import serial

# Init Arduino object, serial port and Baud Rate
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=1) # Linux
#arduino = serial.Serial('/dev/MegaControl', 115200, timeout=1) # Linux
arduino.close()

day = 0
hour = (datetime.now()).hour

print("Preparando dispositivos...")
arduino.open()
print("Dispositivos preparados")
print("\n\n***** Bienvenido a GrownSpirulina *****\n\nMediante esta ventana usted podrá monitorear y controlar su producción,"+
      " de Spirulina para ello debe haber acreditado satisfactoriamente su capacitación. En caso de no haber acreditado su capacitación"+
      " por favor comúniqueselo a su encargado directo, ya que puede causar daños irreversibles.\n")
start = input("¿Desea continuar? s/n \n")
if(start=="s" or start=="S" or start=="y" or start=="Y"):
    x=0
    print("\nComenzando programa...")
    
else:
    x=1

while x==0:
    now = datetime.now()
    
    while arduino.inWaiting()>0: # Analizar si hay datos entrantes en espera

        line=arduino.readline()  # Leer datos del serial en este caso linea compleata, con read() se puede asignar una cantidad de caracteres

        if str(line,'utf-8')=='Update\r\n':
            print("\nMicrocontrolador pregunta qué hora es\n")
            print("Enviando variables a microcontrolador:")
            arduino.write(bytes("a\r\n",'utf-8'))
            arduino.write(bytes(str(now.hour)+"\r\n",'utf-8'))
            arduino.write(bytes(str(now.minute)+"\r\n",'utf-8'))
            print("{0}:{1}\n".format(int(now.hour),int(now.minute)))
            continue

        elif(str(line,'utf-8')!='Update\r\n'):
            print("\nMicrocontrolador envió: {}".format(str(line,'utf-8')))


if(x==1):
    print("Cerrando dispósitivos")
    arduino.close() # Cerrar arduino
    print("Programa terminado")