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
 * 07/25/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Bcd.c module test program.
 *
 * Contains the setup() and loop() functions for the program.
 * Also contains all necessary test functions.
 *
 * Copy Bcd.c, Bcd.h and UnionTypeDefs.h into your sketch folder.  This sketch 
 * tests all functions in the Bcd.c module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Bcd.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define BCD_DIGITS 20
#define HEX_DIGITS 16


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/

 
/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define RUN_TEST(x) if (x) {Serial.write("PASS\r\n");} else {Serial.write("FAIL\r\n");}


/******************************************************************************
 * Local data.
 ******************************************************************************/
 

/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * Test_BCD_ByteToBcd
 **************************************/
bool Test_BCD_ByteToBcd(void)
{
    bool ok = true;
    Serial.write("Testing BCD_ByteToBcd() ");
    
    // Test all combinations from 00 to 99.
    for (uint8_t i=0; i < 10; i++)
    {
        for (uint8_t j=0; j < 10; j++)
        {
            uint8_t bin = (i * 10) + j;
            uint8_t bcd_exp = (i << 4) | j;
            uint8_t bcd_act = BCD_ByteToBcd(bin);
            if (bcd_act != bcd_exp)
            {
                ok = false;
                break;
            }
        }
        
        if (ok == false)
        {
            break;
        }
    }
    
    return ok;
}


/**************************************
 * Test_BCD_BcdToByte
 **************************************/
bool Test_BCD_BcdToByte(void)
{
    bool ok = true;
    Serial.write("Testing BCD_BcdToByte() ");
    
    // Test all combinations from 00 to 99.
    for (uint8_t i=0; i < 10; i++)
    {
        for (uint8_t j=0; j < 10; j++)
        {
            uint8_t bcd = (i << 4) | j;
            uint8_t bin_exp = (i * 10) + j;
            uint8_t bin_act = BCD_BcdToByte(bcd);
            if (bin_act != bin_exp)
            {
                ok = false;
                break;
            }
        }
        
        if (ok == false)
        {
            break;
        }
    }
    
    return ok;
}


/**************************************
 * Test_ByteToBcdToAscii
 **************************************/
bool Test_ByteToBcdToAscii(void)
{
    bool     ok = true;
    int      size = 0;
    uint64_t bin = 0;
    uint8_t  bcd[BCD_DIGITS+1];
    char     ascii[BCD_DIGITS+1];
    
    Serial.write("Testing BCD_BinaryToBcd -> BCD_BcdToAscii\r\n");
    Serial.write("[digit] [BCD array size] [ASCII result]\r\n");

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
        Serial.write(" ");
        Serial.print(size);
        Serial.write(" ");
        Serial.write(ascii);
        Serial.write("\r\n");
        
        power *= 10;
    }
    
    // Test maximum value.
    bin = 0xFFFFFFFFFFFFFFFFull;
    size = BCD_BinaryToBcd(bin, bcd);
    BCD_BcdToAscii(bcd, size, ascii);
    Serial.write("M ");
    Serial.print(size);
    Serial.write(" ");
    Serial.write(ascii);
    Serial.write("\r\n");
    
    return ok;  // Always returns true
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
    Serial.write("Press any key to start: ");
    while (Serial.available() < 1) {}
    Serial.read();
    Serial.write("\r\n");
    
    RUN_TEST(Test_BCD_ByteToBcd())
    RUN_TEST(Test_BCD_BcdToByte())
    RUN_TEST(Test_ByteToBcdToAscii())
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
