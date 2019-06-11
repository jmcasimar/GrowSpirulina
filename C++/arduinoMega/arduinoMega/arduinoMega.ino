#include <OneWire.h>                
#include <DallasTemperature.h>

/***** Temperature Sensors (DS18B20)*****/
OneWire ourWire(2); // Define pin 2 as oneWire bus
DallasTemperature tempSensors(&ourWire); // Declare tempSensor object with oneWire bus
DeviceAddress address1 = {0x28, 0x32, 0x58, 0x79, 0x97, 0x10, 0x3, 0x51}; // Direction sensor 1 (South)
DeviceAddress address2 = {0x28, 0xE3, 0x41, 0x79, 0x97, 0x9, 0x3, 0x73}; // Direction sensor 2 (North)
DeviceAddress address3 = {0x28, 0xFF, 0xF5, 0x57, 0xC2, 0x16, 0x4, 0x59}; // Direction sensor 3 (Center)

/***** Water level sensor*****/
const byte waterLevel = 25;

/***** Define Actuators Pins *****/
const byte pump = 22;
const byte pump1 = 24; // Replicate pump
const byte heatResistor = 26;
const byte waveMaker = 28;
const byte led = 30;
const byte solenoidValve = 32;
const byte waterPump = 34; // Check pin

/***** Control water level variables *****/
int water_count_low = 0; // Filter variable
int water_count_high = 0; // Fitler variable
bool water_state = LOW;

/***** Control Day/Night Variables *****/
byte dateHour;
byte dateMinute;
bool isItDay = LOW;
byte dayBegin = 7;
byte nightBegin = 21;
unsigned long timeUpdate;

/***** Control Actuators Cycles Variables *****/
// This values are the default parameters on serial monitor you can change it
unsigned long pumpOnDay = 46800000; // 13 hours
unsigned long pumpOffDay = 1; // 1 ms
unsigned long pumpOnNight = 300000; // 5 minutes
unsigned long pumpOffNight = 3600000; // 60 minutes
unsigned long pumpOn = pumpOnDay; // By default day
unsigned long pumpOff = pumpOffDay; // By default day
unsigned long pumpTime;
bool pumpState = LOW;

bool wM_State = LOW;
byte wM_On = 5; // 5 minutes 

/***** Temperature Control Variables *****/
float maxTemp = 35.00; // 35°C
bool hR_State = LOW;

/***** Serial Communication Variables *****/
const byte CMDBUFFER_SIZE = 32;
String inputString = "";
bool inputString_complete = false;

void setup() {
  Serial.begin(115200);
  delay(5000);
  Serial.println(F("Setting up device..."));
  tempSensors.begin();   // Init Temp Sensors
  Serial.println(F("Temperature Sensores started correctly"));
  setupActuators(); // Init Actuators
  Serial.println(F("Actuators started correctly"));
  Serial.println(F("Device Ready"));
}

void loop() {
  pumpRun(); // Turn on/off pumps
  //waterLevel_control(); // Level water control whit solenoid and level sensor
}
