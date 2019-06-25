void setupActuators(){
  pinMode(solenoidValve, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(waterLevel, INPUT_PULLUP);
  
  digitalWrite(solenoidValve, !LOW);
  digitalWrite(waterPump, !LOW);


  led.begin(5, 7, 21, 23); // Parameters: On1, Off1, On2, Off2 [hours]
  wMaker.begin(2, 9, 14, 19); // Parameters: On1, On2, On3, On4 [hours]
  aPump.begin(20, 10, 5, 55); // Parameters: OnDay, OffDay, OnNight, OffNight [minutes]
  aPump1.begin(20, 10, 5, 55); // Parameters: OnDay, OffDay, OnNight, OffNight [minutes]
  hResistor.begin(33.5, 33.0, 30.0, 28.0); // Parameters: tempMaxDay, tempMinDay, tempMaxNight, tempMinNight;
  tempTime = millis();  
}
