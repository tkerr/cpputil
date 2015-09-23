/******************************************************************************
 * aunit.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/22/2015 - Tom Kerr
 * Refactored.  Fail messages can now print source file line numbers.
 * Renamed most functions and added a corresponding macro to automatically
 * insert the line number.
 * Added TEST_INIT() and TEST_ASSERT_COUNT().
 *
 * 08/28/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino unit test helper functions.
 *
 * A set of functions used to facilitate unit testing on an Arduino.
 *
 * Designed to be lightweight due to the limited resources of some
 * Arduino platforms.  This is not a complete unit test framework.
 *
 * Function names are all caps, as this appears to be the tradition with
 * unit test frameworks.  It also makes the test functions more noticeable
 * in the code.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include <avr/pgmspace.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "aunit.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
static uint32_t sAssertCount = 0;  //!< Total assertion count


/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * TEST_INIT
 **************************************/ 
void TEST_INIT(void)
{
    sAssertCount = 0;
}


/**************************************
 * TEST_ASSERT_COUNT
 **************************************/ 
uint32_t TEST_ASSERT_COUNT(void)
{
    return sAssertCount;
}

    
/**************************************
 * TEST_ASSERT_L
 **************************************/
bool TEST_ASSERT_L(bool cond, uint16_t line)
{
    sAssertCount++;
    
    // Print pass/fail message.
    if (cond)
        Serial.println(F("PASS"));
    else
    {
        Serial.print(F("LINE "));
        Serial.print(line);
        Serial.println(F(" FAIL"));
    }
    return cond;
}


/**************************************
 * TEST_ASSERT_PASS
 **************************************/
bool TEST_ASSERT_PASS(bool cond)
{
    sAssertCount++;
     
    // Print test pass message only.
    if (cond)
        Serial.println(F("PASS"));
    return cond;
}


/**************************************
 * TEST_ASSERT_FAIL_L
 **************************************/
bool TEST_ASSERT_FAIL_L(bool cond, uint16_t line)
{
    sAssertCount++;
     
    // Print fail message only.
    if (!cond)
    {
        Serial.print(F("LINE "));
        Serial.print(line);
        Serial.println(F(" FAIL"));
    }
    return cond;
}


/**************************************
 * TEST_ASSERT_FAIL1_L
 **************************************/
bool TEST_ASSERT_FAIL1_L(bool cond, int16_t i, uint16_t line)
{
    sAssertCount++;
     
    // Print fail message only + loop variables.
    if (!cond)
    {
        Serial.print(F("LINE "));
        Serial.print(line);
        Serial.print(F(" FAIL ("));
        Serial.print(i);
        Serial.println(F(")"));
    }
    return cond;
}


/**************************************
 * TEST_ASSERT_FAIL2_L
 **************************************/
bool TEST_ASSERT_FAIL2_L(bool cond, int16_t i, int16_t j, uint16_t line)
{
    sAssertCount++;
     
    // Print fail message only + loop variables.
    if (!cond)
    {
        Serial.print(F("LINE "));
        Serial.print(line);
        Serial.print(F(" FAIL ("));
        Serial.print(i);
        Serial.print(',');
        Serial.print(j);
        Serial.println(F(")"));
    }
    return cond;
}


/**************************************
 * TEST_WAIT
 **************************************/
void TEST_WAIT()
{
    Serial.print(F("Press a key to start: "));
    while (Serial.available() < 1) {}
    Serial.read();
    Serial.println();
}


/**************************************
 * TEST_NUMBER
 **************************************/
void TEST_NUMBER(uint16_t num)
{
    Serial.print(num);
    Serial.print(F(": "));
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/


/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/
 

// End of file.
