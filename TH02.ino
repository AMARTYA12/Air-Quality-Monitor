/*
 * Demo name   : TH02_dev demo 
 * Usage       : DIGITAL I2C HUMIDITY AND TEMPERATURE SENSOR 
 * Author      : Oliver Wang from Seeed Studio
 * Version     : V0.1
*/

#include <TH02_dev.h>
#include "Arduino.h"
#include "Wire.h" 
#define TH02_EN 1 
const int pin_scl= 2;
const int pin_sda =3;
void setup()
{  
  Serial.begin(9600);        // start serial for output
  
  Serial.println("****TH02_dev demo by seeed studio****\n");
  /* Power up,delay 150ms,until voltage is stable */
  delay(150);
  /* Reset HP20x_dev */
  //TH02.begin();
  delay(100);
  
  /* Determine TH02_dev is available or not */
  Serial.println("TH02_dev is available.\n");
  TH02.begin(pin_scl,pin_sda);  
}
 

void loop()
{
   float temper = TH02.ReadTemperature(); 
   Serial.println("Temperature: ");   
   Serial.print(temper);
   Serial.println("C\r\n");
   
   float humidity = TH02.ReadHumidity();
   Serial.println("Humidity: ");
   Serial.print(humidity);
   Serial.println("%\r\n");
   delay(1000);
}
 
