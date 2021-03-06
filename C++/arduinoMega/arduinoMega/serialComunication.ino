/*****   Lecture on serial Events   *****/
/*
 * The structure of the commands is specified on each function
 *
 * Notes: Values of 1, 2, 3, 4, 5, 7, 9, etc... are char type, and its value is assigned according to the ASCII Table starting at char(48)="0"
 * For further information see ASCII Table
*/
const char zero_char = char(48);
const char a_char = char(97);

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
         /*
         if(dateMinute==0){
          aPumpTest.resetTime();
          aPumpBioreactor.resetTime();

          if(aPumpTest.isEnable()==LOW){
            aPumpTest.enable(HIGH);
          }
          if(aPumpBioreactor.isEnable()==LOW){
            aPumpBioreactor.enable(HIGH);
          }
         }
         */
         Serial.print(F("Recieved Hour Data ")); Serial.print(newHour); Serial.print(F(":")); Serial.print(newMinute); Serial.println(F(" hrs"));
         is_it_day(dateHour, dateMinute);
         if(wMaker_enable){ wMaker.run(dateHour, dateMinute); }
         led.run(dateHour);
         bool auxRead = digitalRead(powerSupplyPin);
         if (auxRead!=powerSupply || auxRead==LOW){
          if(powerCount<2) powerCount++;
          else {
            powerCount = 0;
            if (auxRead!=powerSupply) powerSupply = !powerSupply;
            if (powerSupply) Serial.println(F("info,Power Supply is on"));
            else Serial.println(F("warning,Power Supply is off"));
          }
         }
       }
       else{Serial.println(F("Time Format Incorrect."));}
       Serial.print(F("Bioreactor-2 Temperature = ")); Serial.print(temp2); Serial.println(F(" °C"));
       Serial.print(F("Bioreactor-3 Temperature = ")); Serial.print(temp3); Serial.println(F(" °C"));
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

    else if(inputString.charAt(0)==zero_char+3){ // disabled heatResistor --> '3'
      hResistor.turnOff();
      hResistor.enable(LOW);
      Serial.println(F("Disabled Heat Resistor"));
    }

    else if(inputString.charAt(0)==zero_char+4){ // enabled heatResistor --> '4'
      hResistor.enable(HIGH);
      Serial.println(F("Enabled Heat Resistor"));
    }

    else if(inputString.charAt(0)==zero_char+5){ // turnOn waveMakers --> '5'
      wMaker.turnOn();
      wMaker_enable = false;
    }

    else if(inputString.charAt(0)==zero_char+6){ // turnOff waveMakers --> '6'
      wMaker.turnOff();
      wMaker_enable = true;
    }


    else if(inputString.charAt(0)==a_char){ // turnOn airPumps2 --> 'a'
      aPumpBioreactor2.enable(false);
      aPumpBioreactor2.turnOn();
    }

    else if(inputString.charAt(0)==a_char+1){ // turnOn airPumps3 --> 'b'
      aPumpBioreactor3.enable(false);
      aPumpBioreactor3.turnOn();
    }

    else if(inputString.charAt(0)==a_char+2){ // turnOff airPumps2 --> 'c'
      aPumpBioreactor2.enable(false);
      aPumpBioreactor2.turnOff();
    }

    else if(inputString.charAt(0)==a_char+3){ // turnOff airPumps3 --> 'd'
      aPumpBioreactor3.enable(false);
      aPumpBioreactor3.turnOff();
    }

    else if(inputString.charAt(0)==a_char+4){ // enable airPumps2 --> 'e'
      aPumpBioreactor2.enable(true);
    }

    else if(inputString.charAt(0)==a_char+5){ // enable airPumps3 --> 'f'
      aPumpBioreactor3.enable(true);
    }
  }

  inputString_complete = false;
}
