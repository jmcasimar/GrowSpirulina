void setupActuators(){
  pinMode(pump, OUTPUT);
  pinMode(pump1, OUTPUT);
  pinMode(waveMaker, OUTPUT);
  pinMode(heatResistor, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(waterLevel, INPUT_PULLUP);
  
  digitalWrite(pump, !LOW);
  digitalWrite(pump1, !LOW);
  digitalWrite(waveMaker, !LOW);
  digitalWrite(heatResistor, !LOW);
  digitalWrite(led, !LOW);

  pumpTime = millis();
  timeUpdate = millis();
}
