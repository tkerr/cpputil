/******************************************************************************
 * AdcSpeedTest.ino
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
 * 09/12/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Adc328.cpp module performance test program.
 *
 * This sketch performs a large number of timed ADC conversions using the 
 * Adc328 object.  The purpose is to measure the performance of the object.
 *
 * Contains the setup() and loop() functions for the program.
 *
 * Copy the following files into your sketch folder:
 *   + Adc328.cpp
 *   + Adc328.h
 *   + aunit.cpp
 *   + aunit.h 
 *
 * Performance data is printed to the serial port.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Adc328.h"
#include "aunit.h"


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
 * setup
 **************************************/ 
void setup()
{
	Serial.begin(9600);
    myAdc.begin(REF_1V1, CHANNEL_TMP);
    
}


/**************************************
 * loop
 **************************************/ 
void loop()
{
    volatile uint16_t val;
    uint32_t num_conversions = 65536;
    
    TEST_WAIT();
    TEST_FILE();
    
    // Perform a bunch of conversions.
    Serial.println(F("Converting, press a key to end test..."));
    uint32_t start = millis();
    for (uint32_t i = 0; i < num_conversions; i++)
    {
        val = myAdc.convert();
    }
    uint32_t end = millis();
    uint32_t delta = end - start;
    
    // Print the ADC result so that it isn't optimized out.
    Serial.print(F("ADC value: "));
    Serial.println(val);
    
    // Print test results.
    Serial.print(F("Num conversions: "));
    Serial.println(num_conversions);
    Serial.print(F("Total time (ms): "));
    Serial.println(delta);
    Serial.print(F("Average time (us): "));
    Serial.println(delta*1000/num_conversions);
    
    TEST_DONE();
}
