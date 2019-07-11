#include <OneWire.h>                
#include <DallasTemperature.h>
 
OneWire ourWire(2);                //Se establece el pin 2  como bus OneWire
 
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor

DeviceAddress address1 = {0x28, 0x32, 0x58, 0x79, 0x97, 0x10, 0x3, 0x51}; // Direction sensor 1 (South)
DeviceAddress address2 = {0x28, 0xFF, 0x10, 0x14, 0xC0, 0x16, 0x5, 0x28}; // Direction sensor 2 (North)
DeviceAddress address3 = {0x28, 0xFF, 0xF5, 0x57, 0xC2, 0x16, 0x4, 0x59}; // Direction sensor 3 (Center)

void setup() {
delay(1000);
Serial.begin(9600);
sensors.begin();   //Se inicia el sensor
}
 
void loop() {
float temp1=0, temp2=0, temp3=0;
while(temp1<=0 || temp2<=0 || temp3<=0){
  sensors.requestTemperatures();   //envía el comando para obtener las temperaturas
  temp1= sensors.getTempC(address1);//Se obtiene la temperatura en °C del sensor 1
  temp2= sensors.getTempC(address2);//Se obtiene la temperatura en °C del sensor 2
  temp3= sensors.getTempC(address3);//Se obtiene la temperatura en °C del sensor 3
  delay(5);
}


Serial.print("Temperatura 1 = ");
Serial.print(temp1);
Serial.print(" C");
Serial.print("   Temperatura 2 = ");
Serial.print(temp2);
Serial.println(" C");
Serial.print("   Temperatura 3 = ");
Serial.print(temp3);
Serial.println(" C");
delay(2000);                
}
