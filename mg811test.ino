#include <MQ135.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <TimeLib.h>

#include <DS1307RTC.h>

#include <Wire.h>

#include "RTClib.h"

RTC_DS1307 rtc;
String DateString;
char commandChar;
char Date[15];
int t=0;
int j,k;
int result[12];
File myFile;
char b,a1,a2,a3,a4,a5;
int DATE_RTC,MONTH_RTC,YEAR_RTC,HOUR_RTC,MIN_RTC,SEC_RTC;

/************************Hardware Related Macros************************************/
#define         MG_PIN                       A1//efine which analog input channel you are going to use
#define         BOOL_PIN                     2
#define         DC_GAIN                      5   //define the DC gain of amplifier
 
/***********************Software Related Macros************************************/
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interval(in milisecond) between each samples in 
                                                     //normal operation
 
/**********************Application Related Macros**********************************/
//These two values differ from sensor to sensor. user should derermine this value.
#define         ZERO_POINT_VOLTAGE           3.01//define the output of the sensor in volts when the concentration of CO2 is 400PPM
#define         REACTION_VOLTGAE             0.7 //define the voltage drop of the sensor when move the sensor from air into 1000ppm CO2
 
/*****************************Globals***********************************************/
float CO2Curve[3]  =  {2.602,ZERO_POINT_VOLTAGE,(REACTION_VOLTGAE/(2.602-3))};   
                                                     //two points are taken from the curve. 
 int sensorValue;
  int sensorValue1;
 //with these two points, a line is formed which is
      int id1=1;                                               //"approximately equivalent" to the original curve.
  int maxValue=0;
int minValue=100;
SoftwareSerial bluetoothSerial(2,3);
 float volts;
 float volts1;//lope}; point1: (lg400, 0.324), point2: (lg4000, 0.280) 
char c[10]; 
int incomingByte=0; 
int iter = 0;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
//slope = ( reaction voltage ) / (log400 –log1000) 
float MGRead(int mg_pin)
{
    int i;
    float v=0;
 
    for (i=0;i<READ_SAMPLE_TIMES;i++) {
        v += analogRead(mg_pin);
        delay(READ_SAMPLE_INTERVAL);
    }
    v = (v/READ_SAMPLE_TIMES) *30/1024 ;     //change the factor for proper range
    return v;  
}

void setup()
{
   Serial.begin(9600);                              //UART setup, baudrate = 9600bps
    pinMode(BOOL_PIN, INPUT);  
//    pinMode(sensorPin, INPUT); //set pin to input
    digitalWrite(BOOL_PIN, HIGH);                    //turn on pullup resistors
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
  
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    bluetoothSerial.begin(9600);
 

    //This example uses HC-05 Bluetooth to communicate with an Android device.
    //Download bluetooth terminal from google play store, https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en
    //Pair and connect to 'HC-05', the default password for connection is '1234'.
    //You should see this message from your arduino on your android device
    Serial.println("Bluetooth On....");
  while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
Serial.print("Initializing SD card...");
if (!SD.begin(10)) {
Serial.println("initialization failed!");
while (1);
}
Serial.println("initialization done.");
// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
myFile = SD.open("test.txt", FILE_WRITE);
// if the file opened okay, write to it:
if (myFile) {
Serial.print("Writing to test.txt...");

Serial.println("done.");
} else {
// if the file didn't open, print an error:
Serial.println("error opening test.txt");
}

  // Serial.print("MG-811 Demostration\n");                
}
 
void loop()
{

 if(t==0){
  while(bluetoothSerial.available() > 0) {
     
    incomingByte = bluetoothSerial.read();
    if (isDigit(incomingByte)) {  // tests if myChar is a digit
    char Date =incomingByte;
    DateString += Date;
     //Serial.print(Date);    
   
    }
    /*else if (isPunct(incomingByte)){
     
      switch(id1){
        case 1: Serial.print(" : Date");
                 Serial.print("\n ");
                  ++id1;
                  break;
        case 2 :Serial.print(" : Month");
                 Serial.print("\n ");
                 ++id1;
                 break;
        case 3: Serial.print(" : Year");
                 
                Serial.print("\n ");
                  ++id1;
                  break;
        case 4: Serial.print(" : Hour");
                  
                  Serial.print("\n ");
                  ++id1;
                  break;
        case 5: Serial.print(" : Min");
                   
                  Serial.print("\n ");
                  ++id1;
                  break;
        case 6: Serial.print(" : Sec");
                a5 = Date;
                 Serial.print(a5);
                  Serial.print("\n ");
                  ++id1;
                  break;
        default:
                  id1++;
                  break;
        
      }
    }*/
   
      }
      if(DateString.length()==12){
        t=1;
         Serial.print("Date");
     Serial.println(DateString);
     for ( j=0;j<12;j++){
 
      result[j] =(DateString[j]-48);
     }
     Serial.print("results");
     for ( j=0;j<12;j++){
 
      Serial.print(result[j]);
      
     }
     DATE_RTC=result[0]*10+result[1];
     MONTH_RTC=result[2]*10+result[3];
     YEAR_RTC=(result[4]*10+result[5])+2000;
     HOUR_RTC=result[6]*10+result[7];
     MIN_RTC=result[8]*10+result[9];
     SEC_RTC=result[10]*10+ result[11];
     Serial.print("\n");
     Serial.print("DATE_RTC");
     Serial.print(DATE_RTC);
      Serial.print("\n");
     Serial.print("MONTH_RTC");
     Serial.print(MONTH_RTC);
      Serial.print("\n");
     Serial.print("YEAR_RTC");
     Serial.print(YEAR_RTC);
      Serial.print("\n");
     Serial.print("HOUR_RTC");
     Serial.print(HOUR_RTC);
      Serial.print("\n");
     Serial.print("MIN_RTC");
     Serial.print(MIN_RTC);
      Serial.print("\n");
     Serial.print("SEC_RTC");
     Serial.print(SEC_RTC);
     
     rtc.adjust(DateTime(YEAR_RTC,MONTH_RTC,DATE_RTC,HOUR_RTC,MIN_RTC,SEC_RTC)); 
     Serial.print("\n");
     
        }
       
     
 }
 
if (t==1){
  
   sensorValue=analogRead(A0);
  volts=MGRead(MG_PIN);
   
  sensorValue1=map(sensorValue,90,110,500,900);
  if (minValue>=sensorValue1)
{
  
  minValue=sensorValue1;
 
}
if (maxValue<=sensorValue1)
{
  maxValue=sensorValue1;
  
}
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
 Serial.print("Min val= ");
  Serial.print(minValue);
  Serial.print(",");

Serial.print("maxValue:");
  Serial.print(maxValue);
  volts1=map(volts,1.0,10.0,350,800);
 
  Serial.print(", sensor Value");
 Serial.print(sensorValue,DEC);
  Serial.print(" sensor Value");
  Serial.print(sensorValue1,DEC);
  //Serial.print(", volts");
  //Serial.println(volts);
    Serial.print(", volts");
  Serial.println(volts1);
  myFile.println("VOLTS");
  myFile.println(volts);
  myFile.println("SENSORVALUE");
  myFile.println(sensorValue);
  bluetoothSerial.println("volts:");
  bluetoothSerial.println(volts);
  bluetoothSerial.println("SensorValue");
  bluetoothSerial.println(sensorValue);
  delay(100);
     
  
}
 }
  
  


/*****************************  MGRead *********************************************
Input:   mg_pin - analog channel
Output:  output of SEN-000007
Remarks: This function reads the output of SEN-000007
************************************************************************************/ 
