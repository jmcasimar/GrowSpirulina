/***** isItDay *****/
// Control pumps
// Control LED´s
void is_it_day(byte hr, byte mn){
  if( hr >= dayBegin && hr < nightBegin){
    if(isItDay == LOW){
      isItDay = HIGH;
      pumpOn = pumpOnDay;
      pumpOff = pumpOffDay;
      maxTemp = maxTemp_day;
      minTemp = minTemp_day;
      Serial.println(F("Starting day"));
    }
  }
  else if( (hr >= 21 || hr < 23) ){
    if(isItDay == LOW){
      isItDay = HIGH;
      pumpOn = pumpOnDay;
      pumpOff = pumpOffDay;
      maxTemp = maxTemp_day;
      minTemp = minTemp_day;
      Serial.println(F("Starting day"));
      digitalWrite(led, !isItDay);
      Serial.println(F("Turning on LED´s"));  
    }
  }
  else if( hr >= 5 && hr < 7){
    if(isItDay == LOW){
      isItDay = HIGH;
      pumpOn = pumpOnDay;
      pumpOff = pumpOffDay;
      maxTemp = maxTemp_day;
      minTemp = minTemp_day;
      Serial.println(F("Starting day"));
      digitalWrite(led, !isItDay);
      Serial.println(F("Turning on LED´s"));
    }
  }
  else if( (hr >= nightBegin || hr < dayBegin) ){
    if(isItDay == HIGH){
      isItDay = LOW;
      pumpOn = pumpOnNight;
      pumpOff = pumpOffNight;
      maxTemp = maxTemp_night;
      minTemp = minTemp_night;
      Serial.println(F("Starting night"));
      if(digitalRead(led)== !HIGH){
        digitalWrite(led, !LOW);
        Serial.println(F("Turning off LED´s"));
      } 
    }
  }
}

/***** Decide if the pump turn on/off *****/
void pumpRun(){
  if(pumpEnable==HIGH){
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
}

/***** Get Temperature Data *****/
void getTemp_controlHR(byte attempNum){
  int i=0;
  float temp1=0, temp2=0, temp3=0;
  while( (temp1<=0 || temp2<=0 || temp3<=0 || temp1>80 || temp2>80 || temp3>80) && i<attempNum){
    tempSensors.requestTemperatures();
    if(temp1<=0 || temp1>80){
      temp1= tempSensors.getTempC(address1); // Get temperature from south sensor
    }
    if(temp2<=0 || temp2>80){
      temp2= tempSensors.getTempC(address2); // Get temperature from north sensor
    }
    if(temp3<=0 || temp3>80){
      temp3= tempSensors.getTempC(address3); // Get temperature from center sensor
    }
    delay(5);
    i++;
  }

  if(i<attempNum){
    Serial.print(F("North Sensor Temperature = ")); Serial.print(temp2); Serial.println(F(" °C"));
    Serial.print(F("South Sensor Temperature = ")); Serial.print(temp1); Serial.println(F(" °C"));
    Serial.print(F("Center Sensor Temperature = ")); Serial.print(temp3); Serial.println(F(" °C"));

    float meanTemp = (temp1+temp2+temp3)/3;
    
    if( (temp1>=maxTemp || temp2>=maxTemp || temp3>=maxTemp) && hR_State==HIGH){
      temp_counter++;
      if(temp_counter>=3){
        temp_counter = 0;
        hR_State = LOW;
        digitalWrite(heatResistor, !hR_State);
        Serial.println(F("Temperature exceed max temp allowed. Turning off heaters"));
      }
    }
    else if( meanTemp<minTemp && hR_State==LOW){
      temp_counter--;
      if(temp_counter<=-3){
        temp_counter = 0;
        hR_State = HIGH;
        digitalWrite(heatResistor, !hR_State);
        Serial.println(F("Temperature getting down. Turning on heaters"));
      }
    }
    else{
      temp_counter = 0;
    }
  }
  else if(i>=attempNum){
    Serial.println(F("Error temperature sensors not reporting data correctly"));
  }
  
}

/***** Control waveMaker *****/
void control_wM(byte hr, byte mn){
  // Turn on 10 minutes at 2:30, 9:30, 14:30, 19:30
  if( ((hr==9 && mn>=30 && mn<30+wM_On) || (hr==14 && mn>=30 && mn<30+wM_On) || (hr==19 && mn>=30 && mn<30+wM_On) || (hr==2 && mn>=30 && mn<30+wM_On) ) ){
    if(wM_State == LOW){
      wM_State = HIGH;  
      digitalWrite(waveMaker, !wM_State);  
      Serial.println(F("waveMaker turned on")); 
    }
  }
  else if(wM_State == HIGH){
    wM_State = LOW;
    digitalWrite(waveMaker, !wM_State);
    Serial.println(F("waveMaker turned off"));
  }
}

void waterLevel_control(){
  if(digitalRead(waterLevel)==HIGH && water_count_high <10 && water_state == LOW){
    water_count_high++;
    delay(500);
  }
  else if(digitalRead(waterLevel)==HIGH && water_count_high>=10 && water_state == LOW){
    water_state = HIGH;
    Serial.println("Water Level too high");
    digitalWrite(solenoidValve, !LOW);
    digitalWrite(waterPump, !LOW);
    Serial.println("Turning Off Solenoid");
    delay(1000);
  }
  else if(digitalRead(waterLevel)==LOW && water_count_low<10 && water_state == HIGH){
    water_count_low++;
    delay(500);
  }
  else if(digitalRead(waterLevel)==LOW && water_count_low>=10 && water_state == HIGH){
    water_state = LOW;
    Serial.println("Water Level is going down");
    digitalWrite(solenoidValve, !HIGH);
    digitalWrite(waterPump, !HIGH);
    Serial.println("Turning On Solenoid");
    delay(1000);
  }

  if(digitalRead(waterLevel)==LOW){water_count_high = 0;}
  else{water_count_low = 0;}
  
}
