/******************************************************************************
 * BcdTest.ino
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
 * 09/22/2015 - Tom Kerr
 * Use refactored aunit test functions.
 *
 * 08/29/2015 - Tom Kerr
 * Modified to use aunit test structure.
 * Moved some strings to PROGMEM.
 *
 * 07/25/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Bcd.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, UnionTypeDefs.h, Bcd.c, and Bcd.h into your sketch
 * folder.  This sketch tests all functions in the Bcd.c module.
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
#include "aunit.h"
#include "Bcd.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define BCD_DIGITS 20


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/

 
/******************************************************************************
 * Local definitions.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
 

/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * Test_BCD_ByteToBcd
 **************************************/
void Test_BCD_ByteToBcd(void)
{
    Serial.println(F("Testing BCD_ByteToBcd()"));
    bool pass = false;
    
    // Test all combinations from 00 to 99.
    for (uint8_t i=0; i < 10; i++)
    {
        for (uint8_t j=0; j < 10; j++)
        {
            uint8_t bin = (i * 10) + j;
            uint8_t bcd_exp = (i << 4) | j;
            uint8_t bcd_act = BCD_ByteToBcd(bin);
            pass = (bcd_act == bcd_exp);
            TEST_ASSERT_BREAK2(pass, i, j);
        }
        if (!pass) break;
    }
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_BCD_BcdToByte
 **************************************/
void Test_BCD_BcdToByte(void)
{
    Serial.println(F("Testing BCD_BcdToByte()"));
    bool pass = false;
    
    // Test all combinations from 00 to 99.
    for (uint8_t i=0; i < 10; i++)
    {
        for (uint8_t j=0; j < 10; j++)
        {
            uint8_t bcd = (i << 4) | j;
            uint8_t bin_exp = (i * 10) + j;
            uint8_t bin_act = BCD_BcdToByte(bcd);
            pass = (bin_act == bin_exp);
            TEST_ASSERT_BREAK2(pass, i, j);
        }
        if (!pass) break;
    }
    
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_ByteToBcdToAscii
 **************************************/
void Test_ByteToBcdToAscii(void)
{
    int      size = 0;
    uint64_t bin = 0;
    uint8_t  bcd[BCD_DIGITS+1];
    char     ascii[BCD_DIGITS+1];
    
    Serial.println(F("Testing BCD_BinaryToBcd -> BCD_BcdToAscii"));
    Serial.println(F("[digit] [BCD array size] [ASCII result]"));

    uint64_t power = 1;
    
    // Test a random digit in each digit location.
    // Note that this test does not automatically determine a pass/fail condition.
    // Results are printed, and must be inspected to determine pass/fail.
    for (int j=0; j < BCD_DIGITS-1; j++)
    {
        for (int i=0; i <= BCD_DIGITS; i++)
        {
            bcd[i] = 0;
            ascii[i] = 0;
        }
        
        uint8_t digit = (uint8_t)random(0, 10);
        bin += (digit * power);
        size = BCD_BinaryToBcd(bin, bcd);
        BCD_BcdToAscii(bcd, size, ascii);
        
        Serial.print(digit);
        Serial.print(" ");
        Serial.print(size);
        Serial.print(" ");
        Serial.println(ascii);
        
        power *= 10;
    }
    
    // Test maximum value.
    bin = 0xFFFFFFFFFFFFFFFFull;
    size = BCD_BinaryToBcd(bin, bcd);
    BCD_BcdToAscii(bcd, size, ascii);
    Serial.print("M ");
    Serial.print(size);
    Serial.print(" ");
    Serial.println(ascii);
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
    TEST_WAIT();
    
    Test_BCD_ByteToBcd();
    Test_BCD_BcdToByte();
    Test_ByteToBcdToAscii();
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
