/**
 * @author Aaron Berk
 * 
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * CMPS03 digital compass module.
 *
 * Datasheet:
 *
 * http://www.robot-electronics.co.uk/htm/cmps3tech.htm
 */

/**
 * Includes
 */
#include "CMPS03.h"

CMPS03::CMPS03(PinName sda, PinName scl, int address) {

    i2c = new I2C(sda, scl);
    //Compass designed to work at 100KHz. See datasheet for details.
    i2c->frequency(100000);
    i2cAddress = address;

}

char CMPS03::readSoftwareRevision(void){

    char registerNumber   = SOFTWARE_REVISION_REG;
    char registerContents = 0;

    //First, send the number of register we wish to read,
    //in this case, command register, number 0.
    i2c->write(i2cAddress, &registerNumber, 1);
    
    //Now, read one byte, which will be the contents of the command register.
    i2c->read(i2cAddress, &registerContents, 1);
    
    return registerContents;
    
}

int CMPS03::readBearing(void){

    char registerNumber = COMPASS_BEARING_WORD_REG;
    char registerContents[2] = {0x00, 0x00};
    
    //First, send the number of register we wish to read,
    //in this case, register numbers 2, 3, which hold the
    //compass bearing as a 16-bit word.
    i2c->write(i2cAddress, &registerNumber, 1);
    
    //Now read two bytes which will be the contents of
    //these registers.
    i2c->read(i2cAddress, registerContents, 2);
    
    //Register 2 [read first], was the high byte, followed by
    //register 3 [read second], which was the low byte.
    int bearing = ((int)registerContents[0] << 8) | ((int)registerContents[1]);
    
    return bearing;
    
}
