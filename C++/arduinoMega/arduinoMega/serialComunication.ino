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
       
       getTemp_controlHR(); // measure Temp and control heatResitor
       
       if(newHour.toInt()>=0 && newHour.toInt()<24 && newMinute.toInt()>=0 && newMinute.toInt()<60){
         dateHour = newHour.toInt();
         dateMinute = newMinute.toInt();
         Serial.print(F("Recieved Hour Data ")); Serial.print(newHour); Serial.print(F(":")); Serial.print(newMinute); Serial.println(F(" hrs")); 
         is_it_day(dateHour, dateMinute);
         control_wM(dateHour, dateMinute);
       }
       else{Serial.println(F("Time Format Incorrect."));}
    }
  }
  
  inputString_complete = false;
}
