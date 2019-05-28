/***** isItDay *****/
// Control pumps
// Control LED´s
void is_it_day(byte hr, byte mn){
  if( hr >= dayBegin && hr < nightBegin && isItDay == LOW ){
    isItDay = HIGH;
    pumpOn = pumpOnDay;
    pumpOff = pumpOffDay;
    Serial.println(F("Starting day"));
  }
  else if( (hr >= 23 || hr < 1) && isItDay == LOW ){
    isItDay = HIGH;
    pumpOn = pumpOnDay;
    pumpOff = pumpOffDay;
    Serial.println(F("Starting day"));
    digitalWrite(led, !isItDay);
    Serial.println(F("Turning on LED´s"));
  }
  else if( hr >= 3 && hr < 5 && isItDay == LOW ){
    isItDay = HIGH;
    pumpOn = pumpOnNight;
    pumpOff = pumpOffNight;
    Serial.println(F("Starting day"));
    digitalWrite(led, !isItDay);
    Serial.println(F("Turning on LED´s"));
  }
  else if( (hr >= nightBegin || hr < dayBegin) && isItDay == HIGH ){
    isItDay = LOW;
    pumpOn = pumpOnNight;
    pumpOff = pumpOffNight;
    Serial.println(F("Starting night"));
    if(digitalRead(led)== !HIGH){
      digitalWrite(led, !LOW);
      Serial.println(F("Turning off LED´s"));
    }
  }
}

/***** Decide if the pump turn on/off *****/
void pumpRun(){
  if( millis()-pumpTime>=1 && millis()-pumpTime<pumpOn+1 && pumpState==LOW){
    pumpState = HIGH;
    digitalWrite(pump, !pumpState);
    digitalWrite(pump1, !pumpState);
    Serial.println(F("Pump Turned On"));
  }
  else if( millis()-pumpTime>=pumpOn+1 && pumpState==HIGH ){
    pumpState = LOW;
    digitalWrite(pump, !pumpState);
    digitalWrite(pump1, !pumpState);
    Serial.println(F("Pump Turned Off"));
  }
  if( millis()-pumpTime>pumpOff){
    pumpTime = millis();
  }
}

/***** Get Temperature Data *****/
void getTemp_controlHR(){
  float temp1=0, temp2=0, temp3=0;
  while(temp1<=0 || temp2<=0 || temp3<=0){
    tempSensors.requestTemperatures();
    temp1= tempSensors.getTempC(address1); // Get temperature from south sensor
    temp2= tempSensors.getTempC(address2); // Get temperature from north sensor
    temp3= tempSensors.getTempC(address3); // Get temperature from center sensor
    delay(5);
  }
  
  
  Serial.print(F("North Sensor Temperature = ")); Serial.print(temp2); Serial.println(F(" °C"));
  Serial.print(F("South Sensor Temperature = ")); Serial.print(temp1); Serial.println(F(" °C"));
  Serial.print(F("Center Sensor Temperature = ")); Serial.print(temp3); Serial.println(F(" °C"));
  
  if( (temp1>=maxTemp || temp2>=maxTemp || temp3>=maxTemp) && hR_State==HIGH){
    hR_State = LOW;
    digitalWrite(heatResistor, !hR_State);
    Serial.println(F("Temperature exceed max temp allowed. Turning off heaters"));
  }
  else if( temp1<(maxTemp-2) && temp2<(maxTemp-2) && temp3<(maxTemp-2) && hR_State==LOW){
    hR_State = HIGH;
    digitalWrite(heatResistor, !hR_State);
    Serial.println(F("Temperature getting down. Turning on heaters"));
  }  
}

/***** Control waveMaker *****/
void control_wM(byte hr, byte mn){
  // Turn on 10 minutes at 2, 9, 14, 19
  if( ((hr == 9 && mn <= wM_On) || (hr == 14 && mn <= wM_On) || (hr == 19 && mn <= wM_On) || (hr == 2 && mn <= wM_On) ) && wM_State == LOW ){
    wM_State = HIGH;  
    digitalWrite(waveMaker, !wM_State);  
    Serial.println(F("waveMaker turned on"));
  }
  else if(wM_State == HIGH){
    wM_State = LOW;
    digitalWrite(waveMaker, !wM_State);
    Serial.println(F("waveMaker turned off"));
  }
}

void waterLevel_control(){
  /*
  if(digitalRead(waterLevel)==HIGH){
    Serial.println("Water Level too high");
    delay(60000);
  }
  */
}
