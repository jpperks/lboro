#include "mbed.h"
#include "MSCFileSystem.h"
#include "GPS.h"
 
Serial pc(USBTX, USBRX);
GPS gps(p9, p10);

MSCFileSystem msc("msc"); // Mount flash drive under the name "msc"

int main() {
    
    int i=0;
    FILE *fp = fopen( "/msc/msctest.txt", "w");
    if ( fp == NULL )
    {
    error("Could not open file for write\n");
    }
    fprintf(fp,"Starting File...");
    while(1)
    {
        pc.printf("Going to search...\n");
        if(gps.sample()) {
        fprintf(fp,"I'm at %f, %f \r\n", gps.longitude, gps.latitude);
        pc.printf("I'm at %f, %f \r\n", gps.longitude, gps.latitude);
        i++;
        pc.printf("%d\n",i);
        } else {
        pc.printf("Oh Dear! No lock :(\n");
        }
        if (i == 60)
        {
            fclose(fp);
            break;  
        }
    }   
}
