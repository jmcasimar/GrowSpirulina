#include <OneWire.h>                
#include <DallasTemperature.h>
#include <Spirulina.h>

/***** Temperature Sensors (DS18B20)*****/
OneWire ourWire(2); // Define pin 2 as oneWire bus
DallasTemperature tempSensors(&ourWire); // Declare tempSensor object with oneWire bus
DeviceAddress address1 = {0x28, 0x32, 0x58, 0x79, 0x97, 0x10, 0x3, 0x51}; // Direction sensor 1 (South)
DeviceAddress address2 = {0x28, 0xE3, 0x41, 0x79, 0x97, 0x9, 0x3, 0x73}; // Direction sensor 2 (North)
DeviceAddress address3 = {0x28, 0xFF, 0xF5, 0x57, 0xC2, 0x16, 0x4, 0x59}; // Direction sensor 3 (Center)
float temp1 = 0, temp2 = 0, temp3 = 0;
unsigned long tempTime;
/***** Water level sensor*****/
const byte waterLevel = 25;

/***** Define Actuators Pins *****/
const byte solenoidValve = 32;
const byte waterPump = 34;

/*** LED definition***/
SP_LED led(30); // Pin

/*** waveMaker definition ***/
waveMaker wMaker(28, 5); // Pin, timeOn [minutes]

/*** airPump definitions ***/
airPump aPumpTest(22, "Test"); // Pin, Name
airPump aPumpBioreactor(24, "Bioreactor"); // Pin, Name

/*** heatResistor definitions ***/
heatResistor hResistor(26); // Pin 

/***** Control water level variables *****/
int water_count_low = 0; // Filter variable
int water_count_high = 0; // Fitler variable
bool water_state = LOW;

/***** Control Day/Night Variables *****/
byte dateHour;
byte dateMinute;
bool isItDay = LOW;
byte dayBegin = 5;
byte nightBegin = 23;

/***** Serial Communication Variables *****/
const byte CMDBUFFER_SIZE = 32;
String inputString = "";
bool inputString_complete = false;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("Setting up device..."));
  tempSensors.begin();   // Init Temp Sensors
  Serial.println(F("Temperature Sensores started correctly"));
  setupActuators(); // Init Actuators
  Serial.println(F("Device Ready"));
}

void loop() {
  aPumpTest.run(isItDay); // Turn on/off airPump
  aPumpBioreactor.run(isItDay); // Turn on/off airPump1
  hResistor.run(isItDay, temp1, temp2, temp3); // Turn on/off heatResistor
  getTemp(10); // 10 Attemps
  //waterLevel_control(); // Level water control whit solenoid and level sensor
}
