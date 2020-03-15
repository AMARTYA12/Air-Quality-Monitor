
int sensorValue;
void setup()
{
    Serial.begin(9600);                              //UART setup, baudrate = 9600bps
  // Serial.print("MG-811 Demostration\n");                
}
 
void loop()
{
   sensorValue=analogRead(A0);
   Serial.print("sensorValue");
   Serial.print(sensorValue);
   Serial.print("\n");
   delay(1000);
}
