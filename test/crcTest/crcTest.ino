/******************************************************************************
 * crcTest.ino
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Modification History:
 *
 * 10/07/2015 - Tom Kerr
 * Added support for automated unit testing over a serial port.
 *
 * 08/30/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno crc.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 *    aunit.cpp, aunit.h, hex.c, hex.h, UnionTypeDefs.h, crc.c, crc.h
 * This sketch tests all functions in the crc.c module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "aunit.h"
#include "crc.h"
#include "hex.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/


/******************************************************************************
 * Public functions.
 ******************************************************************************/

/**************************************
 * PrintCrc
 **************************************/
void PrintCrc(crc_t crc)
{
    char  crcHex[sizeof(crc_t)*2+1];
    #if defined(CRC32)
        HEX_Uint32ToHex(crc, crcHex);
    #else
        HEX_Uint16ToHex(crc, crcHex);
    #endif
    Serial.println(crcHex);
}


/**************************************
 * setup
 **************************************/
void setup(void)
{
    Serial.begin(9600);
}


/**************************************
 * loop
 **************************************/
void loop(void)
{   
    uint8_t  testVector[] = "123456789";
    uint8_t  longMessage[256];
    crc_t    crc;
    uint32_t start;
    uint32_t stop;

    TEST_WAIT();
    TEST_FILE();
    
    TEST_NUMBER(1);
    Serial.println(F(CRC_NAME));
    Serial.println((char*)testVector);
    start = millis();
    crc = crcSlow(testVector, (uint16_t)strlen((char*)testVector));
    stop = millis();
    PrintCrc(crc);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    TEST_ASSERT(crc == CHECK_VALUE);
    
    TEST_NUMBER(2);
    Serial.println(F(CRC_NAME));
    Serial.println((char*)testVector);
    crcInit();
    start = millis();
    crc = crcFast(testVector, (uint16_t)strlen((char*)testVector));
    stop = millis();
    PrintCrc(crc);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    TEST_ASSERT(crc == CHECK_VALUE);
    
    TEST_NUMBER(3);
    Serial.println(F(CRC_NAME));
    Serial.println(F("256 byte message"));
    for (uint16_t i = 0; i < 256; i++)
    {
        longMessage[i] = (uint8_t)i;
    }
    start = millis();
    crc = crcSlow(longMessage, 256);
    stop = millis();
    PrintCrc(crc);
    Serial.print(F("Slow: "));
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    start = millis();
    crc = crcFast(longMessage, 256);
    stop = millis();
    PrintCrc(crc);
    Serial.print(F("Fast: "));
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    TEST_DONE();
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
