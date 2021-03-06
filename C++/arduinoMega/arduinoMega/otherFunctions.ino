/***** isItDay *****/
// Control pumps
void is_it_day(byte hr, byte mn){
  if( hr >= dayBegin && hr < nightBegin){
    if(isItDay == LOW){
      isItDay = HIGH;
      Serial.println(F("Starting day"));
    }
  }
  else if( (hr >= nightBegin || hr < dayBegin) ){
    if(isItDay == HIGH){
      isItDay = LOW;
      Serial.println(F("Starting night"));
    }
  }
}

/***** Exponential Filter *****/
float exponentialFilter(float alpha, float t, float t_1){
  if(isnan(alpha) || isnan(t) || isnan(t_1)){
      return t;
    }
    else{
      return (t*alpha+(1-alpha)*t_1);
    }
  }

/***** Get Temperature Data *****/
void getTemp(byte attempNum){
  if(millis()-tempTime>1000){
    tempTime = millis();
    int i=0;
    float t3=0, t2=0;

    while( (t3<=0 || t3>80 || t3==25.00 || t2<=0 || t2>80 || t2==25.00) && i<attempNum){
      tempSensors.requestTemperatures();

      if(t3<=0 || t3>80 || t3==25.00){
        t3= tempSensors.getTempC(address3); // Get temperature from B-3
      }
      if(t2<=0 || t2>80 || t2==25.00){
        t2= tempSensors.getTempC(address2); // Get temperature from B-2
      }
      delay(5);
      i++;
    }
    if(t3>0 && t3<80 && t3!=25.00){
      temp3 = exponentialFilter(0.2, (t3+offsetTemp3), temp3);
    }
    if(t2>0 && t2<80 && t2!=25.00){
      temp2 = exponentialFilter(0.2, (t2+offsetTemp2), temp2);
    }
  }
}

void waterLevel_control(){
  if(digitalRead(waterLevel)==HIGH && water_count_high <10 && water_state == LOW){
    water_count_high++;
    delay(500);
  }
  else if(digitalRead(waterLevel)==HIGH && water_count_high>=10 && water_state == LOW){
    water_state = HIGH;
    Serial.println(F("Water Level too high"));
    digitalWrite(solenoidValve, !LOW);
    digitalWrite(waterPump, !LOW);
    Serial.println(F("Turning Off Solenoid"));
    delay(1000);
  }
  else if(digitalRead(waterLevel)==LOW && water_count_low<10 && water_state == HIGH){
    water_count_low++;
    delay(500);
  }
  else if(digitalRead(waterLevel)==LOW && water_count_low>=10 && water_state == HIGH){
    water_state = LOW;
    Serial.println(F("Water Level is going down"));
    digitalWrite(solenoidValve, !HIGH);
    digitalWrite(waterPump, !HIGH);
    Serial.println(F("Turning On Solenoid"));
    delay(1000);
  }

  if(digitalRead(waterLevel)==LOW){water_count_high = 0;}
  else{water_count_low = 0;}

}
