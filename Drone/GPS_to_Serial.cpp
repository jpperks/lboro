#include "mbed.h"
#include "C12832_lcd.h"
#include "SerialGPS.h"

C12832_LCD lcd;
SerialGPS gps(p9, p10, 4800);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

int main()
{
    while(1)
        {
          lcd.printf("Waiting for Lock");
          if (gps.sample())
          {
            lcd.cls();
            led2 = 0;
            lcd.locate(0,0);
            lcd.printf("Position Locked");
            wait(2);
            lcd.cls();
            lcd.locate(0,2);
            lcd.printf("I'm at %f, %f\n", gps.longitude, gps.latitude);
            wait(8);
            lcd.cls();
          }
          else
          {
            led2=1;
            lcd.printf("Position cannot be locked");
            wait(1);
          }
        }
}
