#include <Seeed_BMP280.h>
#include <UARTClass.h>
#include <Wire.h>
#include <LGPS.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>
#include "DHT.h"
#include "MutichannelGasSensor.h"
#include "Seeed_BMP280.h"
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <math.h>
#define WIFI_AP "Jonathan Phone"
#define WIFI_PASSWORD "airquality"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define URL "jpapps.co.uk"

float alt_ground = 0;
float pm25pcs2ugm3 (float concentration_pcs);
int pm25ugm32aqi (float concentration_ugm3);
// Data 
#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card
String filename = "";
int data1 = 0;

String UTC = "";
String LatitudeD = "";
String LongitudeD = "";
String Satellites = "";
String AMSL = "";
String Temperature = "";
String Pressure = "";
String AGL = "";
String NH3D = "";
String COD = "";
String NO2D = "";
String C3H8D = "";
String C4H10D = "";
String CH4D = "";
String H2D = "";
String C2H5OHD = "";
String MQ2D = "";
String MQ5D = "";
String AQID = "";

bool wifi = false;
bool gprs = false;
bool first_transmit = true;
bool internet_able = true;
int retry_count = 10;

// Gas Sensors
#define mq2_pin A0
#define mq5_pin A2


// Temperature & Humidity
#define DHTPIN A2
#define DHTTYPE DHT22

// Transmit
#define WIFI_PIN 2
#define GPRS_PIN 3

// LEDs
#define PRE_HEAT 4
#define FILES 5
#define INTERNET 6
#define GPS_LIGHT 7

DHT dht(DHTPIN, DHTTYPE); //Set Temperature & Humidity Function Parameters

BMP280 bmp280; //Set Barometer Function Parameters

unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 2000;//sampe 30s&nbsp;;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float concentration_pcs = 0;
float concentration_ugm3 = 0;
int aqi =0;

// GPS Setup
gpsSentenceInfoStruct info;
char buff[256];
static unsigned char getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
      j++;
     if(j == num)
      return i + 1; 
  }
  return 0; 
}
static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}
static double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  return rev; 
}

String parseGPGGA(const char* GPGGAstr)
{
  String data = "";
  double latitude;
  double longitude;
  double alt;
  int tmp, hour, minute, second, num ;
  if(GPGGAstr[0] == '$')
  {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');
    
    sprintf(buff, "%02d:%02d:%02d", hour, minute, second);
    //Serial.println(buff);
    data = String(buff);
    data += ",";
    UTC = String(buff);
    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp])/100.0;
    int latitude_int=floor(latitude);
    double latitude_decimal=(latitude-latitude_int)*100.0/60.0;
    latitude=latitude_int+latitude_decimal;
    LatitudeD = String(latitude);
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp])/100.0;
    int longitude_int=floor(longitude);
    double longitude_decimal=(longitude-longitude_int)*100.0/60.0;
    longitude=(-1*(longitude_int+longitude_decimal));
    LongitudeD = String(longitude);
    sprintf(buff,"%10.6f,%10.6f",latitude,longitude);
   // Serial.println(buff); 
    data += String(buff);
    data += ",";
    tmp = getComma(7, GPGGAstr);
    num = getIntNumber(&GPGGAstr[tmp]);
    if (num == 0){
      digitalWrite(GPS_LIGHT,LOW);
    } else {digitalWrite(GPS_LIGHT,HIGH);}
    Satellites = String(num);    
    sprintf(buff,"%d",num);
    Serial.print("GPS Altitude = "); 
    data += String(buff);
    data += ",";
    AMSL = "";
    for (int i = 48; i < 52; i++)
    {
      if (GPGGAstr[i] != ','){
      data += String(GPGGAstr[i]);
      AMSL += String(GPGGAstr[i]);
      Serial.print(GPGGAstr[i]);
      }
    }
  }
  return data;
}

void Transmit()
{
  if (digitalRead(WIFI_PIN) == false && wifi == false && retry_count == 10)
    {
      LWiFi.begin();
      wifi = true;
      Serial.println("Connecting to AP");
      while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
      {
        delay(1000);
      }
      if (LWiFi.RSSI() != 0)
      {
        internet_able = true;
      }
      if (first_transmit == true)
      {
        Serial.println("deleting to server");
        save_value(false);
        first_transmit = false;
      } else {
        Serial.println("saving new to server");
        save_value(true);
      }
    }
    if (digitalRead(WIFI_PIN) == true && wifi == true){
     retry_count = 10;
     LWiFi.disconnect();
     wifi = false;
    }
    if (digitalRead(WIFI_PIN) == false && wifi == true && internet_able == true){
      save_value(true);
      Serial.print("Wifi Strength =");
      Serial.println(LWiFi.RSSI());
    }
    if (internet_able == false && retry_count < 11)
    {
      Serial.print("Retry Count =");
      Serial.println(retry_count);
      retry_count++;
    }
}

void setup() {
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(WIFI_PIN, INPUT_PULLUP);
    pinMode(GPRS_PIN, INPUT_PULLUP);
    pinMode(8, INPUT);
    pinMode(PRE_HEAT, OUTPUT);
    pinMode(FILES, OUTPUT);
    pinMode(INTERNET, OUTPUT);
    pinMode(GPS_LIGHT, OUTPUT);
    LTask.begin();
    
    //Serial.begin(9600);


    // make sure that the default chip select pin is set to
    // output, even if you don't use it:
    pinMode(10, OUTPUT);
    // see if the card is present and can be initialized:
    Drv.begin();
    //Serial.println("Card initialized.");
    LGPS.powerOn();
    dht.begin(); // Begin Temperature Service
    gas.begin(0x04);//the default I2C address of the gas slave is 0x04
    gas.powerOn();
    if(!bmp280.init()){
    Serial.println("Device error!");
    }
    delay(1000);
    starttime = millis();
    data_setup();
    Serial.print("Pressure = ");
    Serial.println(bmp280.getPressure());
    alt_ground = bmp280.calcAltitude(bmp280.getPressure());
    Serial.print("Alt Ground = ");
    Serial.println(alt_ground);
    light_test();
    for (int z=0;z<601;z++){
      digitalWrite(PRE_HEAT, HIGH);   // turn the LED on (HIGH is the voltage level)
      data(gps_run(),barometer(),multichannel_gas(),mq2(),mq5(),dust());
      Transmit();
   }
   digitalWrite(PRE_HEAT, LOW);
}

void light_test()
{
  for (int i=0;i<2;i++){
    digitalWrite(PRE_HEAT, HIGH);
    delay(500);
    digitalWrite(PRE_HEAT, LOW);
    digitalWrite(FILES, HIGH);
    delay(500);
    digitalWrite(FILES, LOW);
    digitalWrite(INTERNET, HIGH);
    delay(500);
    digitalWrite(INTERNET, LOW);
    digitalWrite(GPS_LIGHT, HIGH);
    delay(500);
    digitalWrite(GPS_LIGHT, LOW);
  }
}
void loop() {
   digitalWrite(FILES, LOW);
   data(gps_run(),barometer(),multichannel_gas(),mq2(),mq5(),dust());
   Transmit();
}

String multichannel_gas()
{
  float c;
  String data_combined = "";

    c = (gas.measure_NH3()*0.75);
    Serial.print("The concentration of NH3 is ");
    NH3D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0) Serial.print(c);
    else Serial.print("invalid");
    Serial.println(" mg/m3");

    c = (gas.measure_CO()*1.23);
    Serial.print("The concentration of CO is ");
    COD = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print(c);Serial.println(" mg/m3");} 
    else {Serial.print("invalid");}
    
    c = (gas.measure_NO2()*2.03);
    Serial.print("The concentration of NO2 is ");
    NO2D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print(c);Serial.println(" mg/m3");} 
    else {Serial.print("invalid");}

    c = gas.measure_C3H8();
    Serial.print("The concentration of C3H8 is ");
    C3H8D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print((c));Serial.println(" ppm");} 
    else {Serial.print("invalid");}

    c = gas.measure_C4H10();
    Serial.print("The concentration of C4H10 is ");
    C4H10D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print(c);Serial.println(" ppm");} 
    else {Serial.print("invalid");}

    c = (gas.measure_CH4()*0.70667);
    Serial.print("The concentration of CH4 is ");
    CH4D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print(c);Serial.println(" mg/m3");} 
    else {Serial.print("invalid");}

    c = (gas.measure_H2()*82.45);
    Serial.print("The concentration of H2 is ");
    H2D = String(c);
    data_combined += String(c);
    data_combined += ",";
    if(c>=0){ Serial.print(c);Serial.println(" ug/m3");} 
    else {Serial.print("invalid");}

    c = gas.measure_C2H5OH();
    Serial.print("The concentration of C2H5OH is ");
    C2H5OHD = String(c);
    data_combined += String(c);
    if(c>=0){ Serial.print(c);Serial.println(" ppm");} 
    else {Serial.print("invalid");}

    //delay(1000);
    Serial.println("...");

    return data_combined;
}

String barometer()
{
  String data_combined = "";
  float pressure;
  
  //get and print temperatures
  data_combined = String(bmp280.getTemperature());
  Temperature = String(bmp280.getTemperature());
  data_combined += ",";
  //Serial.print("Temperature: ");
  //Serial.print(bmp280.getTemperature());
  //Serial.println("C"); // The unit for  Celsius because original arduino don't support speical symbols
  
  //get and print atmospheric pressure data
  pressure = bmp280.getPressure();
  data_combined += String((bmp280.getPressure()/100));
  Pressure = String((bmp280.getPressure()/100));
  data_combined += ",";
  //Serial.print("Pressure: ");
  //Serial.print(bmp280.getPressure()/100);
  //Serial.println("hPa");
  
  //get and print altitude data
  data_combined += String(alt_ground-(bmp280.calcAltitude(pressure)));
  AGL = String(alt_ground-(bmp280.calcAltitude(pressure)));
  Serial.print("Altitude: ");
  Serial.print(alt_ground-(bmp280.calcAltitude(pressure)));
  Serial.println("m");
  
  //delay(1000);
  return data_combined;
}

String mq2()
{   
    String data = "";
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(A1);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL      
    ratio = RS_gas/0.71;  // ratio = RS/R0
    //Serial.print("MQ2 Ratio (9.8 = clean): ");
    //Serial.println(ratio);
    data = String(ratio);
    MQ2D = String(ratio);
    return data;
}

String mq5()
{
    String data = "";
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(A0);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL
    ratio = RS_gas/1.71;  // ratio = RS/R0
    //Serial.print("MQ5 Ratio (6.5 = clean: ");
    //Serial.println(ratio);
    data = String(ratio);
    MQ5D = String(ratio);
    return data;
}

String dust()
{
  String data = "0,0";
  duration = pulseIn(8, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  pinMode(10, OUTPUT);
  if ((millis()-starttime) >= sampletime_ms){//if the sampel time = = 30s
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0);
    concentration_pcs = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    concentration_ugm3 = pm25pcs2ugm3(concentration_pcs);
    aqi = pm25ugm32aqi(concentration_ugm3);
 
//    Serial.print(concentration_pcs);
//    Serial.print("pcs/0.01cf  ");
    //Serial.print(concentration_ugm3);
    //Serial.print("ug/m3  ");
    //Serial.print(aqi);
    //Serial.println("AQI");
    data = String(concentration_pcs);
    data += ",";
    data += String(concentration_ugm3);
    data += ",";
    data += String(aqi); 
    AQID = String(aqi);
    lowpulseoccupancy = 0;
    starttime = millis();
  }
  return data;
}

/* convert pcs/0.01cf to μg/m3 */
float pm25pcs2ugm3 (float concentration_pcs)
{
  
  double pi = 3.14159;
  double density = 1.65 * pow (10, 12);
  double r25 = 0.44 * pow (10, -6);
  double vol25 = (4/3) * pi * pow (r25, 3);
  double mass25 = density * vol25;
  double K = 3531.5;
  return ((concentration_pcs) * K * mass25);
}

#define AQI_LEVELS 7

static struct pm25aqi {
    float clow;
    float chigh;
    int llow;
    int lhigh;
} pm25aqi[] = {
  {0.0,    12.0,   0, 50},
  {12.1,   35.4,  51, 100},
  {35.5,   55.4, 101, 150},
  {55.5,  150.4, 151, 200},
  {150.5, 250.4, 201, 300},
  {250.5, 350.4, 301, 350},
  {350.5, 500.4, 401, 500},
};

/* calculate AQI (Air Quality Index) based on μg/m3 concentration */
int pm25ugm32aqi (float concentration_ugm3)
{
  int i;

  for (i = 0; i < AQI_LEVELS; i++) {
    if (concentration_ugm3 >= pm25aqi[i].clow &&
        concentration_ugm3 <= pm25aqi[i].chigh) {
      return ((pm25aqi[i].lhigh - pm25aqi[i].llow) /
          (pm25aqi[i].chigh - pm25aqi[i].clow)) *
              (concentration_ugm3 - pm25aqi[i].clow) + pm25aqi[i].llow;
    }
  }
  return 0;
}

String gps_run()
{
  String data = "";
  //Serial.println("LGPS loop"); 
  LGPS.getData(&info);
  Serial.println((char*)info.GPGGA); 
  delay(1000);
  data = parseGPGGA((const char*)info.GPGGA);
  return data;
}

void data_setup()
{
  int file = 0;
  char filechar[50];
  String dataString = "UTC,Latitude,Longitude,Satellites,Altitude(AMSL),Temperature(C),Pressure(hPa),Altitude(AGL),NH3(ug/m3),CO(mg/m3),NO2(mg/m3),C3H8(ppm),C4H10(ppm),CH4(mg/m3),H2(ug/m3),C2H5OH(ppm),MQ2,MQ5,Particles(0.01cf),Particles(ugm3),PM2.5 AQI";

  while(1) {
    filename = "datalog";
    filename += String(file);
    filename += ".csv";
    filename.toCharArray(filechar, 50);
    //Serial.println("In loop");
    if (Drv.exists(filechar)) {
      file++;
      } else {break;}
  }

  Serial.println(filename);
  LFile dataFile = Drv.open(filechar, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    //Serial.println("error opening datalog.csv");
  }
}


void data(String gps,String barometer,String multichannel,String mq2,String mq5,String dust)
{
  char filechar[50];
  filename.toCharArray(filechar, 50);
  if (data1 != 0)
  {
    String dataString = gps;
    dataString += ",";
    dataString += barometer;
    dataString += ",";
    dataString += multichannel;
    dataString += ",";
    dataString += mq2;
    dataString += ",";
    dataString += mq5;
    dataString += ",";
    dataString += dust;
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    LFile dataFile = Drv.open(filechar, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      Serial.println("Writing to file...");
      dataFile.println(dataString);
      dataFile.close();
      digitalWrite(FILES, HIGH);
      delay(100);
      digitalWrite(FILES, LOW);
      // print to the serial port too:
      //Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    //  else {
    //    //Serial.println("error opening datalog.csv");
    //  }
   } else {
    data1++;
   }
  
}

void save_value(bool del){

  Serial.println("Sending value");
  Serial.println(UTC);
  LWiFiClient client;

  if (AQID == "")
  {
    AQID = "0";
  }
  
  if (del == true)
  {
    if(client.connect(URL, 80))
    {
      String str = "GET /record.php?";
          str += "time=";
          str += UTC;
          str += "&lat=";
          str += LatitudeD;
          str += "&lng=";
          str += LongitudeD;
          str += "&sats=";
          str += Satellites;
          str += "&amsl=";
          str += AMSL;
          str += "&temp=";
          str += Temperature;
          str += "&press=";
          str += Pressure;
          str += "&agl=";
          str += AGL;
          str += "&nh3=";
          str += NH3D;
          str += "&co=";
          str += COD;
          str += "&no2=";
          str += NO2D;
          str += "&c3h8=";
          str += C3H8D;
          str += "&c4h10=";
          str += C4H10D;
          str += "&ch4=";
          str += CH4D;
          str += "&h2=";
          str += H2D;
          str += "&c2h5oh=";
          str += C2H5OHD;
          str += "&mq2=";
          str += MQ2D;
          str += "&mq5=";
          str += MQ5D;
          str += "&aqi=";
          str += AQID;
          Serial.println(str);
          client.print(str);
          client.println(" HTTP/1.1");
          client.print("Host: ");
          client.println(URL);
          client.println("Connection: close");
          client.println();
          Serial.println("Data Sent to JPAPPS");
          digitalWrite(INTERNET, HIGH);
          delay(100);
          digitalWrite(INTERNET, LOW);
    }
    else {Serial.println("Unable to connect");internet_able = false;retry_count = 0;wifi = false;LWiFi.disconnect();}
  } else {
    if(client.connect(URL, 80))
    {
      String str = "GET /record_delete.php?true=1";
          client.print(str);
          client.println(" HTTP/1.1");
          client.print("Host: ");
          client.println(URL);
          client.println("Connection: close");
          client.println();
          Serial.println("Data Deleted to JPAPPS");
    }
    else {Serial.println("Unable to connect");}
  }
  //client.stop();
}
