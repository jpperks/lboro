#include <Wire.h>
#include <LGPS.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>
#include <LTask.h>
#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <math.h>
#define WIFI_AP "Jonathan Phone"
#define WIFI_PASSWORD "airquality"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define URL "jpapps.co.uk"

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

#define Drv LFlash          // use Internal 10M Flash
String filename = "";
int data1 = 0;

bool wifi = false;
bool gprs = false;
bool first_transmit = true;
bool internet_able = true;
int retry_count = 10;

String UTC = "";
String LatitudeD = "";
String LongitudeD = "";
String Satellites = "";
String AMSL = "";
String Wind = "";

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
      //Serial.print(GPGGAstr[i]);
      }
    }
    AMSL += "0";
  }
  return data;
}

void setup() {
  // initialize serial communications at 9600 bps:
  //Serial.begin(9600);
  LTask.begin();
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  Drv.begin();
  LGPS.powerOn();
  data_setup();
}

void Transmit()
{
  if (wifi == false && retry_count == 10)
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
    if (wifi == true && internet_able == true){
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

void loop() {
  data(gps_run(),Anemometer());
  Transmit();          
}

String Anemometer()
{
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  float voltage = sensorValue * (5.0 / 1024.0);
  float wind = (voltage - 0.39)*32.4/(2.00 - 0.4);

  if (wind < 0.00)
  {
    wind = 0.00;
  }
  Serial.print("Wind Speed = ");
  Serial.print(wind);
  Serial.println("m/s");
  Wind = String(wind);
  delay(2);
  return String(wind);
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
  String dataString = "UTC,Latitude,Longitude,Satellites,Altitude(AMSL),Wind(m/s)";

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


void data(String gps,String anemometer)
{
  char filechar[50];
  filename.toCharArray(filechar, 50);
  if (data1 != 0)
  {
    String dataString = gps;
    dataString += ",";
    dataString += anemometer;
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    LFile dataFile = Drv.open(filechar, FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      Serial.println("Writing to file...");
      dataFile.println(dataString);
      dataFile.close();
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
  Serial.println(Wind);
  LWiFiClient client;
  if (del == true)
  {
    if(client.connect(URL, 80))
    {
      String str = "GET /record_anemometer.php?";
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
          str += "&wind=";
          str += Wind;
          Serial.println(str);
          client.print(str);
          client.println(" HTTP/1.1");
          client.print("Host: ");
          client.println(URL);
          client.println("Connection: close");
          client.println();
          Serial.println("Data Sent to JPAPPS");
    }
    else {Serial.println("Unable to connect");internet_able = false;retry_count = 0;wifi = false;LWiFi.disconnect();}
  } else {
    if(client.connect(URL, 80))
    {
      String str = "GET /record_delete_ane.php?true=1";
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
