#include <Time.h>
#include <TimeLib.h>

#include <DS1307RTC.h>

#include <Wire.h>

#include "RTClib.h"

RTC_DS1307 rtc;


char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void setup () 
{
  Serial.begin(9600);
  while(!Serial);

  
  while (! rtc.begin()) 
  {
    Serial.print("Couldn't find RTC");
   // while (1);
  }
   if (! rtc.isrunning()) 
  {
    Serial.print("RTC is NOT running!");
  }else {
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  
  }
  
  
}

void loop () 
{
    
 
   //auto update from computer time
    //rtc.adjust(DateTime(2019, 12, 31, 01, 47, ));// to set the time manualy 
    
    DateTime now = rtc.now();
    

    Serial.print(now.hour());
    Serial.print(':');
    Serial.print(now.minute());
    Serial.print(':');
    Serial.print(now.second());
    Serial.print("   ");

   
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(" ,");
     Serial.print(now.day());
    Serial.print('/');
    Serial.print(now.month());
    Serial.print('/');
    Serial.println(now.year());
    delay(1000);
}
