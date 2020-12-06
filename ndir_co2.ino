
const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};
 
unsigned char dataRevice[9];
int temperature;
int CO2PPM;
 
void setup()
{
    Serial1.begin(9600);
    Serial.begin(9600);
    Serial2.begin(9600);
    Serial.println("get a 'g', begin to read from sensor!");
    Serial.println("********************************************************");
    Serial.println();
}
 
void loop()
{
    if(dataRecieve())
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("  CO2: ");
        Serial.println(CO2PPM);
         Serial2.print("Temperature: ");
        Serial2.print(temperature);
        Serial2.print("  CO2: ");
        Serial2.println(CO2PPM);
        //Serial.print(" |||||  ");
    }
   
    delay(60000);
}
 
bool dataRecieve(void)
{
    byte data[9];
    int i = 0;
 
    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        Serial1.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(Serial1.available())
    {
        while(Serial1.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = Serial1.read();
            }
        }
    }
 
   /* for(int j=0; j<9; j++)
    {
        Serial.print(data[j]);
        Serial.print(" ");
    }
    Serial.println("");
 */
    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }
 
    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;
 
    return true;
}
