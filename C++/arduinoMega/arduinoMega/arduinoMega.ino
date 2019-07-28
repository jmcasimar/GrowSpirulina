#include <OneWire.h>                
#include <DallasTemperature.h>
#include <Spirulina.h>

/***** Temperature Sensors (DS18B20)*****/
OneWire ourWire(2); // Define pin 2 as oneWire bus
DallasTemperature tempSensors(&ourWire); // Declare tempSensor object with oneWire bus
DeviceAddress address1 = {0x28, 0xFF, 0xB7, 0x7A, 0xB5, 0x16, 0x3, 0xC8}; // Direction sensor 1 (South)
DeviceAddress address2 = {0x28, 0xFF, 0x10, 0x14, 0xC0, 0x16, 0x5, 0x28}; // Direction sensor 2 (North)
DeviceAddress address3 = {0x28, 0xFF, 0xF5, 0x57, 0xC2, 0x16, 0x4, 0x59}; // Direction sensor 3 (Center)
// T1 (south), T2(North), T3(Center)
float temp1 = 0, temp2 = 0, temp3 = 0;
float offsetTemp1 = -1.1, offsetTemp2 = -0.94, offsetTemp3 = -0.82;

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
  //tempSensors.setWaitForConversion(true);
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
