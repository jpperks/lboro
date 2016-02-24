#include "mbed.h"
#include "SerialGPS.h"
#include "MSCFileSystem.h"
#include "C12832_lcd.h"

C12832_LCD lcd;
SerialGPS gps(p9, p10, 4800);
MSCFileSystem msc("msc");

DigitalOut myled(LED1);

int main() {
    
    int i=0;
    FILE *fp = fopen( "/msc/msctest.csv", "w");
    if ( fp == NULL )
    {
    error("Could not open file for write\n");
    }
    fprintf(fp,"Time, Latitude, Longitude\n");
    
    while (1) {
        lcd.cls();
        lcd.locate(0,0);
        lcd.printf("Waiting...");
        if (gps.sample()) {
            myled = myled ? 0 : 1;
            lcd.cls();
            lcd.locate(3,3);
            lcd.printf("Position Locked");
            float longitude = gps.longitude - 0.663;
            fprintf(fp,"%d, %f, %f\n", i, gps.latitude, longitude);
            i++;
            wait(1);
        }
         if (i == 20)
        {
            fclose(fp);
            break;  
        }
    }
}