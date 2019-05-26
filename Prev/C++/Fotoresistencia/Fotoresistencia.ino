const long A = 1000;     //Resistencia en oscuridad en KΩ
const int B = 15;        //Resistencia a la luz (10 Lux) en KΩ
const int Rc = 10;       //Resistencia calibracion en KΩ
const int LDR = A0;   //Pin del LDR
 
int V;
int iluminacion;
 
void setup() 
{
   Serial.begin(115200);
}
 
void loop()
{
   V = analogRead(LDR);         
     iluminacion = ((long)V*A*10)/((long)B*Rc*(1024-V));   
   Serial.println(iluminacion);   
   delay(1000);
}
