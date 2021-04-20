#include <SoftwareSerial.h>


#include <TH02_dev.h>
#include "Arduino.h"
#include "Wire.h" 
#include "MutichannelGasSensor.h"
#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

#include <DS1307RTC.h>
#include "RTClib.h"

RTC_DS1307 rtc;
#define TH02_EN 1
#define LENG 31   //0x42 + 31 bytes equal to 32 bytes
unsigned char buf[LENG];

int PM01Value=0;          //define PM1.0 value of the air detedtor module
int PM2_5Value=0;         //define PM2.5 value of the air detedtor module
int PM10Value=0;         //define PM10 value of the air detedtor module

///16-tx  17-rx 
const int pin_scl= 2;
const int pin_sda =3;
File myFile;
char c[10]; 
int sensorValue;
const int ChipSelect = 53;
int incomingByte=0; 
String DateString;
char commandchar;
   float temper;
 float humidity;
char Date[15];
int t=0;
int j,k;
int result[12];
float d,d1,d2;
int DATE_RTC,MONTH_RTC,YEAR_RTC,HOUR_RTC,MIN_RTC,SEC_RTC;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
/************************Hardware Related Macros************************************/
#define         MG_PIN                       (A8)     //define which analog input channel you are going to use
#define         BOOL_PIN                     (2)
#define         DC_GAIN                      (8.5)   //define the DC gain of amplifier

/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in
                                                     //normal operation

/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           (0.0625) //define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             (0.50) //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2

/*****************************Globals************************************************/
float           CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};
                                                     //two points are taken from the curve.
                                                     //with these two points, a line is formed which is
                                                     //"approximately equivalent" to the original curve.
                                                     //data format:{ x, y, slope}; point1: (lg400, 0.324), point2: (lg4000, 0.280)
                                                     //slope = ( reaction voltage ) / (log400 –log1000)
void setup()
{  
  Serial.begin(9600); 
   pinMode(BOOL_PIN, INPUT);                        //set pin to input
   digitalWrite(BOOL_PIN, HIGH);   
   Serial1.begin(9600);
   Serial1.setTimeout(1500);
   Serial2.begin(9600);
   
   Serial.println("start");
  
   while (! rtc.begin()) 
  {
    Serial.print("couldn't find RTC");
   // while (1);
  }
   if (! rtc.isrunning()) 
  {
    Serial.print("RTC is NOT running!");
  }
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  while(!Serial){
    ;
  }
   Serial.println("Bluetooth on.......");
 
  // start serial for output
//  Serial.println("****TH02_dev demo by seeed studio****\n");
  
  /* Power up,delay 150ms,until voltage is stable */
  delay(150);
  /* Reset HP20x_dev */
  //TH02.begin();
  delay(100);
  
  /* Determine TH02_dev is available or not */
  Serial.println("TH02_dev is available.\n");
 TH02.begin(pin_scl,pin_sda);
 /* gas.begin(0x04);//the default I2d address of the slave is 0x04
   gas.powerOn();
*/pinMode(53, OUTPUT);
Serial.println("Initializing SD Card...");
if (!SD.begin(53)) {
Serial.println("initialization failed!");
while (1);
}
Serial.println("initialization done.");

File myFile = SD.open("Book1.csv", FILE_WRITE);
 

  // if the file is available, write to it:
  myFile.println("Date,Time,PM1.0.PM2.5,PM10.0,NO2,CO,HUMIDITY,TEMPERATURE,MG811,MQ135");

myFile.close();
   
   // ope.beginn the file. note that only one file dan be open at a time,
// so you have to close this one before opening another.
// if the file opened okay, write to it:
if (myFile) {
Serial.print("Writing to test.txt...");
 
  myFile.close();
  Serial.print("done");

} else {
// if the file didn't open, print an error:
Serial.println("error opening test.txt");
   
   
}
} 

void loop()
{
  
 if (t==0){
  if (Serial2.available()){
 
    incomingByte=Serial2.read();
    if (isDigit(incomingByte)){
      char Date = incomingByte;
      DateString += Date;
      
    }
    
      if(DateString.length()==12){
        t=1;
     Serial.print("Date:  ");
     Serial.println(DateString);
     Serial.print("\n");
     delay(100);
     for ( j=0;j<12;j++){
 
      result[j] =(DateString[j]-48);
     }
     Serial.print("results:  ");
     Serial.print("\n");
     for ( j=0;j<12;j++){
 
      Serial.print(result[j]);
      
     }
     delay(100);
     DATE_RTC=result[0]*10+result[1];
     MONTH_RTC=result[2]*10+result[3];
     YEAR_RTC=(result[4]*10+result[5])+2000;
     HOUR_RTC=result[6]*10+result[7];
     MIN_RTC=result[8]*10+result[9];
     SEC_RTC=result[10]*10+ result[11];
     Serial.print("\n");
     Serial.print("DATE_RTC: ");
     Serial.print(DATE_RTC);
      Serial.print("\n");
     Serial.print("MONTH_RTC: ");
     Serial.print(MONTH_RTC);
      Serial.print("\n");
     Serial.print("YEAR_RTC: ");
     Serial.print(YEAR_RTC);
      Serial.print("\n");
     Serial.print("HOUR_RTC: ");
     Serial.print(HOUR_RTC);
      Serial.print("\n");
     Serial.print("MIN_RTC: ");
     Serial.print(MIN_RTC);
      Serial.print("\n");
     Serial.print("SEC_RTC: ");
     Serial.print(SEC_RTC);
     
     rtc.adjust(DateTime(YEAR_RTC,MONTH_RTC,DATE_RTC,HOUR_RTC,MIN_RTC,SEC_RTC)); 
     Serial.print("\n");
     Serial.print("Date");
    Serial.print(" ");
    Serial.print("Time");
    Serial.print("  ");
    Serial.print("Temperature: ");   
   Serial.print("  ");
   Serial.print("Humidity: ");
   Serial.print(" CO : ");
   Serial.print(" ");
 
   Serial.print(" NO2 : ");
 Serial.print(" ");
   Serial.print("PM1.0: ");
    Serial.print("  ");
   Serial.print("PM2.5: ");
   Serial.print("  ");
   Serial.print("PM10: ");
    Serial.print("  ");
     Serial.print(" MG811 ");
     Serial.print("  ");
     Serial.print("MQ 135:");
     Serial.println();
     
   
    Serial2.print("Date");
   Serial2.print("  ");
   Serial2.print("Time");
   Serial2.print("  ");
   Serial2.print("PM1.0");
   Serial2.print("  ");
   Serial2.print("PM2.5");
   Serial2.print("  ");
   Serial2.print("PM10.0");
   Serial2.print("  ");
   Serial2.print("NO2");
   Serial2.print("  ");
   Serial2.print("CO");
   Serial2.print("  ");
   Serial2.print("HUM.");
   Serial2.print("  ");
   Serial2.print("TEMP.");
   Serial2.print(" ");
   Serial2.print("Mg811");
  Serial2.print(" ");
  Serial2.print("MQ135");
  Serial2.println();
  
        }
       
  }
 }
 if (t==1){
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
    Serial.print(now.year());
   Serial.print("   ");
    delay(1000);
   
   float temper = TH02.ReadTemperature();
   Serial.print(temper); 
    Serial.print("  ");
   float humidity = TH02.ReadHumidity();
   
   Serial.print(humidity);
   Serial.print("  ");
   /*d = gas.measure_NH3();
    Serial.print("NH3 : ");
    if(d>=0){
      Serial.print(d);
    }
    else
    {
      Serial.print("invalid");
    }
    */
    Serial.print("  ");
    d1 = gas.measure_CO();
        if(d1>=0){
      Serial.print(d1);
    }
    else
    {
      Serial.print("invalid");
    }
    Serial.print("  ");
    d2 = gas.measure_NO2();
    if(d2>=0){
      Serial.print(d2);
    }
    else
    {
      Serial.print("invalid");
    }
    Serial.print("  ");
    if(Serial1.find(0x42)){
    Serial1.readBytes(buf,LENG);

    if(buf[0] == 0x4d){
      if(checkValue(buf,LENG)){
        PM01Value=transmitPM01(buf); //count PM1.0 value of the air detector module
        PM2_5Value=transmitPM2_5(buf);//count PM2.5 value of the air detector module
        PM10Value=transmitPM10(buf); //count PM10 value of the air detector module
      }
    }
  }

  static unsigned long OledTimer=millis();
    if (millis() - OledTimer >=1000)
    {
      OledTimer=millis();

     
      Serial.print(PM01Value);
      Serial.print("  "); 
      
      Serial.print(PM2_5Value);
      Serial.print(" "); 
    
      Serial.print(PM10Value);
 
    }
    Serial.print("  ");
  int percentage;
    float volts;
    
    volts = MGRead(MG_PIN);
    //Serial.print( "SEN0159:" );
    //Serial.print(volts);
    //Serial.print( "V           " );

    percentage = MGGetPercentage(volts,CO2Curve);
   

  
        Serial.print(percentage);
        Serial.print(",");
  sensorValue=analogRead(A9);
 //  Serial.print("MQ135:");
   Serial.print(sensorValue);
 
  Serial.println();

   
 
   Serial2.print(now.day());
   Serial2.print("-");
   Serial2.print(now.month());
   Serial2.print("-");
   Serial2.print(now.year());
   Serial2.print("\t");
   Serial2.print(now.hour());
   Serial2.print(":");
   Serial2.print(now.minute());
   Serial2.print(":");
   Serial2.print(now.second());
   Serial2.print("\t");
   Serial2.print(PM01Value);
   Serial2.print("\t");
   Serial2.print(PM2_5Value);
   Serial2.print("\t");
   Serial2.print(PM10Value);
  Serial2.print("\t");
   Serial2.print(d2);
   Serial2.print("\t");
   Serial2.print(d1);
    Serial2.print("\t");
    Serial2.print(humidity);
   
   Serial2.print("\t");
   Serial2.print(temper);
   Serial2.print("\t");
   Serial2.print(percentage);
     Serial2.print("\t");
     Serial2.print(sensorValue); 
    Serial2.println();
  
   delay(100);

  // open the file. note that only one file can be open at a time
  // so you have to close this one before opening another.

  File myFile = SD.open("Book1.csv", FILE_WRITE);
 

  // if the file is available, write to it


  if (myFile) {
   myFile.print( now.day());
   myFile.print("-");
   myFile.print(now.month());
   myFile.print("-");
   myFile.print(now.year());
   myFile.print(",");
   myFile.print(now.hour());
   myFile.print(":");
   myFile.print(now.minute());
   myFile.print(":");
   myFile.print(now.second());
   myFile.print(",");
   myFile.print(PM01Value);
   myFile.print(",");
   myFile.print(PM2_5Value);
   myFile.print(",");
   myFile.print(PM10Value);
   myFile.print(",");
   myFile.print(d2);
   myFile.print(",");
   myFile.print(d1);
   myFile.print(",");
    myFile.print(temper);
    myFile.print(",");
    myFile.print(humidity);
    myFile.print(",");
    myFile.print(percentage);
    myFile.print(",");
    myFile.print(sensorValue);
    myFile.println();
    myFile.close();

    // print to the serial port too:

  

  } 

  // if the file isn't open, pop up an error:

  else {

    Serial.println("error opening text.txt");
  }

}

 delay(20000); 
}

char checkValue(unsigned char *thebuf, char leng)
{
  char receiveflag=0;
  int receiveSum=0;

  for(int i=0; i<(leng-2); i++){
  receiveSum=receiveSum+thebuf[i];
  }
  receiveSum=receiveSum + 0x42;

  if(receiveSum == ((thebuf[leng-2]<<8)+thebuf[leng-1]))  //check the serial data
  {
   receiveSum = 0;
    receiveflag = 1;
  }
  return receiveflag;
}

int transmitPM01(unsigned char *thebuf)
{
  int PM01Val;
  PM01Val=((thebuf[3]<<8) + thebuf[4]); //count PM1.0 value of the air detector module
  return PM01Val;
}

//transmit PM Value to PC
int transmitPM2_5(unsigned char *thebuf)
{
  int PM2_5Val;
  PM2_5Val=((thebuf[5]<<8) + thebuf[6]);//count PM2.5 value of the air detector module
  return PM2_5Val;
  }

//transmit PM Value to PC
int transmitPM10(unsigned char *thebuf)
{
  int PM10Val;
  PM10Val=((thebuf[7]<<8) + thebuf[8]); //count PM10 value of the air detector module
  return PM10Val;
}
float MGRead(int mg_pin)
{
    int i;
    float v=0;

    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *5/1024 ;
    return v;
}
int  MGGetPercentage(float volts, float *pcurve)
{
   if ((volts/DC_GAIN )>=ZERO_POINT_VOLTAGE) {
       return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   } else {
      int x=((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0];
      
      return pow(10, ((volts/DC_GAIN)-pcurve[1])/pcurve[2]+pcurve[0]);
   }
}
