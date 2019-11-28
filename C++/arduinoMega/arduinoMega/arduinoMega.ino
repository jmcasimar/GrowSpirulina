#include <OneWire.h>                
#include <DallasTemperature.h>
#include <Spirulina.h>

/***** Temperature Sensors (DS18B20)*****/
OneWire ourWire(2); // Define pin 2 as oneWire bus
DallasTemperature tempSensors1(&ourWire); // Declare tempSensor object with oneWire bus
DeviceAddress address1 = {0x28, 0xFF, 0x10, 0x14, 0xC0, 0x16, 0x5, 0x28}; // Direction sensor 2 (North)

OneWire otherWire(4); // Define pin 4 as oneWire bus
DallasTemperature tempSensors2(&ourWire); // Declare tempSensor object with oneWire bus

float temp1 = 0; // T1(Bioreactor1)
float offsetTemp1 = -0.94; 
float temp2 = 0; // T2(Bioreactor2)
float offsetTemp2 = -0.94; 
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
byte dayBegin = 7;
byte nightBegin = 19;

/***** Serial Communication Variables *****/
const byte CMDBUFFER_SIZE = 32;
String inputString = "";
bool inputString_complete = false;

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("Setting up device..."));
  tempSensors1.begin();   // Init Temp Sensors
  tempSensors2.begin();   // Init Temp Sensors
  //tempSensors.setWaitForConversion(true);
  Serial.println(F("Temperature Sensores started correctly"));
  setupActuators(); // Init Actuators
  Serial.println(F("Device Ready"));
}

void loop() {
  aPumpTest.run(isItDay); // Turn on/off airPump
  aPumpBioreactor.run(isItDay); // Turn on/off airPump1
  hResistor.run(isItDay, temp1); // Turn on/off heatResistor
  getTemp(10); // 10 Attemps
  //waterLevel_control(); // Level water control whit solenoid and level sensor
}
