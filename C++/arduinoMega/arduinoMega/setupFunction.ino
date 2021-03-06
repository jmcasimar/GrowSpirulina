void setupActuators(){
  pinMode(solenoidValve, OUTPUT);
  pinMode(waterPump, OUTPUT);
  pinMode(waterLevel, INPUT_PULLUP);

  digitalWrite(solenoidValve, !LOW);
  digitalWrite(waterPump, !LOW);


  led.begin(7, 8, 18, 19); // Parameters: On1, Off1, On2, Off2 [hours]
  wMaker.begin(2, 9, 14, 19); // Parameters: On1, On2, On3, On4 [hours]
  aPumpBioreactor2.begin(20, 10, 5, 55); // Parameters: OnDay, OffDay, OnNight, OffNight [minutes]
  aPumpBioreactor3.begin(20, 10, 5, 55); // Parameters: OnDay, OffDay, OnNight, OffNight [minutes]
  hResistor.begin(23.0, 21.0, 21.0, 19.0); // Parameters: tempMaxDay, tempMinDay, tempMaxNight, tempMinNight;
  tempTime = millis();
}
