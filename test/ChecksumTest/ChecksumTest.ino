/******************************************************************************
 * ChecksumTest.ino
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
 * 09/16/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Checksum.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, UnionTypeDefs.h, Checksum.c and Checksum.h into your 
 * sketch folder.  This sketch tests all functions in the Checksum.c module.
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
#include "aunit.h"
#include "Checksum.h"
 
 
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
static char testData1[] = {
     1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    00};
    
static uint16_t testData2[] = {
    0x1234, 0x5678, 0x9ABC, 0xDEF0, 
    0xFEDC, 0xBA98, 0x7654, 0x3210, 
    0x1122, 0x3344, 0x5566, 0x7788, 
    0x99AA, 0xBBCC, 0xDDEE, 0xFF00,
    0x0000};

/******************************************************************************
 * Public functions.
 ******************************************************************************/


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
    char cksm1 = -120;
    char cksm2 = 47;
    uint16_t cksm3 = 0x780F;
    
    bool cond;
    
    TEST_WAIT();
    
    TEST_NUMBER(1);
    cond =  TEST_ASSERT_FAIL((checksum(testData1, 15) == cksm1));
    cond &= TEST_ASSERT_FAIL((checksum(testData1, 30) == cksm2));
    
    // Appending the checksum should result in a checksum of zero.
    testData1[30] = cksm2;
    cond &= TEST_ASSERT_FAIL((checksum(testData1, 31) == 0));
    TEST_ASSERT_PASS(cond);
    
    TEST_NUMBER(2);
    cond = TEST_ASSERT_FAIL((ipv4_checksum(testData2, 16*sizeof(uint16_t)) == cksm3));
    
    // Appending the checksum should result in a checksum of zero.
    testData2[16] = cksm3;
    cond &= TEST_ASSERT_FAIL((ipv4_checksum(testData2, 17*sizeof(uint16_t)) == 0));
    TEST_ASSERT_PASS(cond);
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
