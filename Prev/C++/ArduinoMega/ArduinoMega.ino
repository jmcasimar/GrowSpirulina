// Pines utilizados
const byte pump = 13;

// Variables control día y noche
byte date_Hour;
byte date_Minute;
bool is_it_day = LOW;
const byte day_Begin = 7;
const byte night_Begin = 20;

// Variables control. Temporizadores
const unsigned long time_On_day = 468000000; // 13 horas
const unsigned long time_Off_day = 468000000; // 13 horas igual, para que no se apague durante el día
const unsigned long time_On_night = 300000; // 5 minutos
const unsigned long time_Off_night = 3600000; // 60 min
unsigned long time_On = time_On_day;
unsigned long time_Off = time_Off_day;
bool pump_State = LOW;
unsigned long renovate_Time;
unsigned long pump_Time;

// Comunicación serial
const byte CMDBUFFER_SIZE = 32;
String inputstring = "";
bool input_string_complete = false;
byte serial_Count;
byte serial_State;

void serialEvent() {                                  
  inputstring = Serial.readStringUntil(13);
  input_string_complete = true;

  if(input_string_complete==true){
    
    switch(inputstring.charAt(0)){
      case 'a': // Información Hora Actual
        serial_State=1;
        serial_Count=0;
        break;
    }
  }

  if(input_string_complete==true && serial_State==1){
    switch(serial_Count){
      case 0:
        serial_Count+=1;
        break;
      case 1:
        date_Hour=inputstring.toInt();
        serial_Count+=1;
        break;
      case 2:
        date_Minute=inputstring.toInt();
        serial_Count+=1;
        break;
      case 3:
        serial_Count+=1;
        break;
    }
  }   
}


void setup() {
  Serial.begin(115200);
  Serial.println("Preparando el sistema...");
  pinMode(pump,OUTPUT);
  digitalWrite(pump,!LOW);
  pump_Time = millis();
  renovate_Time = millis();
  Serial.println("Sistema preparado");
}

void loop() {
  // Renovar/Solicitar la información del tiempo cada minuto
  if((millis()-renovate_Time)>60000){
    Serial.println("Update");
    renovate_Time=millis();
  }

  // Día/Noche
  if ((date_Hour>=day_Begin) && (date_Hour<night_Begin) && is_it_day==LOW){     
    is_it_day = HIGH;
    time_On = time_On_day;
    time_Off = time_Off_day;
    Serial.print("Comienza el día. Son las "); Serial.println(date_Hour);
    // Serial.print(":"); Serial.println(date_Minute);
  } else if ( ((date_Hour>=night_Begin) || (date_Hour<day_Begin)) && is_it_day==HIGH){
    is_it_day = LOW;
    time_On = time_On_night;
    time_Off = time_Off_night;
    Serial.println("Comienza la noche. Son las "); Serial.println(date_Hour);
    // Serial.print(":"); Serial.println(date_Minute);
  }

  if(millis()-pump_Time>10 && millis()-pump_Time<time_On+10 && pump_State==LOW){
    pump_State = HIGH;
    digitalWrite(pump,!HIGH);
    Serial.println("Bomba encendida");
  }
  else if(millis()-pump_Time>time_On+10 && pump_State==HIGH){
    pump_State = LOW;
    digitalWrite(pump,!LOW);
    Serial.println("Bomba apagada");
  }
  if(millis()-pump_Time>time_Off){
    pump_Time = millis();
  }

}
