const byte flotador = 23;

void setup() {
  Serial.begin(115200);
  pinMode(flotador,INPUT_PULLUP);
  
}

void loop() {
  if(digitalRead(flotador)==LOW){
    Serial.println("Activado");
  }
  else if(digitalRead(flotador)==HIGH){
    Serial.println("Desactivado");
  }
  delay(2000);
}
