#include "mbed.h"
#include "SerialGPS.h"
#include "MSCFileSystem.h"
#include "C12832_lcd.h"
#include "MMA7660.h"
#include "CMPS03.h"
 
CMPS03 compass(p9, p10, CMPS03_DEFAULT_I2C_ADDRESS);
C12832_LCD lcd;
SerialGPS gps(p13, p14, 4800);
MMA7660 MMA(p28, p27);
MSCFileSystem msc("msc");

DigitalOut myled(LED1);
DigitalOut myled2(LED2);

int main() {
    
    int i=0;
    FILE *fp = fopen( "/msc/msctest.csv", "w");
    if ( fp == NULL )
    {
    error("Could not open file for write\n");
    }
    fprintf(fp,"Time, Latitude, Longitude, Acc X, Acc Y, Acc Z,Bearing\n");
    
    if(MMA.testConnection())
    {
        myled2 = 1;
    }    
    
    while (1) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Waiting...");
        if (gps.sample()) {
            myled = myled ? 0 : 1;
            lcd.cls();
            lcd.locate(3,3);
            lcd.printf("Position Locked");
            lcd.locate(3,15);
            lcd.printf("%f", compass.readBearing() / 10.0);
            float longitude = gps.longitude + 0.6668;
            fprintf(fp,"%d, %f, %f, %.2f, %.2f, %.2f, %f\n", i, gps.latitude, longitude, MMA.x(), MMA.y(), MMA.z(), (compass.readBearing() / 10.0));
            i++;
            wait(1);
        }
         if (i == 30)
        {
            fclose(fp);
            break;  
        }
    }
}