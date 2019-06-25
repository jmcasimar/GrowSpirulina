/*****   Lecture on serial Events   *****/
/*
 * The structure of the commands is specified on each function
 * 
 * Notes: Values of 1, 2, 3, 4, 5, 7, 9, etc... are char type, and its value is assigned according to the ASCII Table starting at char(48)="0"
 * For further information see ASCII Table
*/
const char zero_char = char(48);

void serialEvent(){
  inputString = Serial.readStringUntil(13);
  inputString_complete = true;

  if(inputString_complete){
    if(inputString.charAt(0)==zero_char){ // get hour/minute Function --> '0'
       String newHour = inputString.substring(1,3); // Form = 10Int(2)Int(2)
       String newMinute = inputString.substring(3,5);
       int nHour = newHour.toInt();
       int nMinute = newMinute.toInt();
       
       if(nHour>=0 && nHour<24 && nMinute>=0 && nMinute<60){
         if(dateHour!=nHour){
           dateHour = nHour;
         }
         dateMinute = nMinute;
         
         if(dateMinute==0){
          if(aPump.isEnable()==LOW){
            aPump.enable(HIGH);  
          }
          if(aPump1.isEnable()==LOW){
            aPump1.enable(HIGH);  
          }
         }
         
         Serial.print(F("Recieved Hour Data ")); Serial.print(newHour); Serial.print(F(":")); Serial.print(newMinute); Serial.println(F(" hrs")); 
         is_it_day(dateHour, dateMinute);
         wMaker.run(dateHour, dateMinute);
         led.run(dateHour);
       }
       else{Serial.println(F("Time Format Incorrect."));}
       Serial.print(F("North Sensor Temperature = ")); Serial.print(temp2); Serial.println(F(" °C"));
       Serial.print(F("South Sensor Temperature = ")); Serial.print(temp1); Serial.println(F(" °C"));
       Serial.print(F("Center Sensor Temperature = ")); Serial.print(temp3); Serial.println(F(" °C"));
    }
    
    else if(inputString.charAt(0)==zero_char+1){ // turnOn solenoid --> '1'
      Serial.println(F("Solenoid turn On"));
      digitalWrite(solenoidValve, !HIGH);
      digitalWrite(waterPump, !HIGH);
    }

    else if(inputString.charAt(0)==zero_char+2){ // turnOff solenoid --> '2'
      Serial.println(F("Solenoid turn Off"));
      digitalWrite(solenoidValve, !LOW);
      digitalWrite(waterPump, !LOW);
    }
  }
  
  inputString_complete = false;
}
