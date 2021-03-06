#include <OneWire.h>
#include <DallasTemperature.h>
#include <Spirulina.h>

/***** Temperature Sensors (DS18B20)*****/
OneWire ourWire(2); // Define pin 2 as oneWire bus
DallasTemperature tempSensors(&ourWire); // Declare tempSensor object with oneWire bus
DeviceAddress address2 = {0x28, 0xFF, 0xF5, 0x57, 0xC2, 0x16, 0x4, 0x59}; // Direction sensor 2 (B-2)
DeviceAddress address3 = {0x28, 0xFF, 0x10, 0x14, 0xC0, 0x16, 0x5, 0x28}; // Direction sensor 3 (B-3)

float temp2 = 0; // T2(Bioreactor2)
float offsetTemp2 = +0.24;
float temp3 = 0; // T3(Bioreactor3)
float offsetTemp3 = +0.32;
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
bool wMaker_enable = true;

/*** airPump definitions ***/
airPump aPumpBioreactor2(24, "Bioreactor-2"); // Pin, Name
airPump aPumpBioreactor3(22, "Bioreactor-3"); // Pin, Name

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
byte dayBegin = 7;
byte nightBegin = 19;

/***** Auxiliar 5V output power supply check *****/
bool powerSupply = false;
byte powerCount = 0;
byte powerSupplyPin = 3;

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
  pinMode(powerSupplyPin, INPUT); // Start powerSupplyPin
  Serial.println(F("Device Ready"));
}

void loop() {
  aPumpBioreactor2.run(isItDay); // Turn on/off airPump2
  aPumpBioreactor3.run(isItDay); // Turn on/off airPump3
  hResistor.run(isItDay, temp3); // Turn on/off heatResistor
  getTemp(10); // 10 Attemps
  //waterLevel_control(); // Level water control whit solenoid and level sensor
}
