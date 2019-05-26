
#define ESPERA_LECTURAS 1000 // tiempo en milisegundos entre lecturas de la intensidad de la luz
 
long cronometro_lecturas=0; // Variable auxiliar de tiempo
long tiempo_transcurrido; // Variable auxiliar de tiempo
unsigned int luminosidad; // Variable entera para medir la luminosidad
float coeficiente_porcentaje=100.0/1023.0; // El valor de la entrada analógica va de 0 a 1023 y se quiere convertir en un porcentaje de 0 a 100
 
void setup()
{
  Serial.begin(9600); //Se define la velocidad de comunicación con el puerto serial en 9600 bits/s
}
 
void loop()
{ // Condiciones de tiempo para realizar las lecturas del puerto establecido
  tiempo_transcurrido=millis()-cronometro_lecturas;
  if(tiempo_transcurrido>ESPERA_LECTURAS)
  {
    cronometro_lecturas=millis();
    luminosidad=analogRead(A0); // Se mide la caida de voltaje en la fotoresistencia y se lee con el puerto analógico A0

    // Impresión en pantalla de los resultados
    Serial.print("La luminosidad es del ");
    Serial.print(luminosidad*coeficiente_porcentaje);
    Serial.println("%");
  }
}
