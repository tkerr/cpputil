/******************************************************************************
 * aunitTest.ino
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
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno aunit.cpp module test program.
 *
 * Contains the setup() and loop() functions for the program. 
 * This is the unit test program for the aunit test framework.
 *
 * Copy the following files into your sketch folder:
 * aunit.cpp
 * aunit.h
 * This sketch tests all functions in the aunit.cpp module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <limits.h>
#include <avr/wdt.h>
#include <Wire.h>
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
void setup(void)
{
    Serial.begin(9600);
}


/**************************************
 * loop
 **************************************/
void loop(void)
{
    uint16_t  i;
    bool cond = true;
    
    TEST_WAIT();
    TEST_FILE();
    
    // ************************************************************************
    // * Tests that should pass.
    // ************************************************************************
    
    TEST_NUMBER(1);
    Serial.println(F("These tests should pass."));
    
    // Test the basic test assertion.
    cond &= TEST_ASSERT(true);
    TEST_ASSERT(cond);
    cond &= TEST_ASSERT_L(true, __LINE__);
    TEST_ASSERT_L(cond, __LINE__);
    if (!cond) return;
    
    // Test the test pass assertion.
    cond = TEST_ASSERT_PASS(true);
    TEST_ASSERT(cond);
    if (!cond) return;
    
    // Test the test fail assertions.
    cond = TEST_ASSERT_FAIL(true);
    TEST_ASSERT(cond);
    cond &= TEST_ASSERT_FAIL1(true, 1);
    TEST_ASSERT(cond);
    cond &= TEST_ASSERT_FAIL2(true, 1, 2);
    TEST_ASSERT(cond);
    
    cond &= TEST_ASSERT_FAIL_L(true, __LINE__);
    TEST_ASSERT(cond);
    cond &= TEST_ASSERT_FAIL1_L(true, 1, __LINE__);
    TEST_ASSERT(cond);
    cond &= TEST_ASSERT_FAIL2_L(true, 1, 2, __LINE__);
    TEST_ASSERT(cond);
    if (!cond) return;
    
    // Test the TEST_BREAK assertions.
    // These must be tested within a loop.
    for (i = 0; i < 2; i++)
    {
        TEST_ASSERT_BREAK(true);
        TEST_ASSERT_BREAK1(true, i);
        TEST_ASSERT_BREAK2(true, i, 1);
    }
    
    // ************************************************************************
    // * Tests that should fail.
    // ************************************************************************
    
    TEST_NUMBER(2);
    Serial.println(F("These tests should fail."));
    
    // Test the basic test assertion.
    cond  = TEST_ASSERT(false);
    cond |= TEST_ASSERT(cond);
    cond |= TEST_ASSERT_L(false, __LINE__);
    cond |= TEST_ASSERT_L(cond, __LINE__);
    if (cond) return;
    
    // Test the test pass assertion.
    cond = TEST_ASSERT_PASS(false);
    TEST_ASSERT(cond);
    if (cond) return;
    
    // Test the test fail assertions.
    cond = TEST_ASSERT_FAIL(false);
    TEST_ASSERT(cond);
    cond |= TEST_ASSERT_FAIL1(false, 1);
    TEST_ASSERT(cond);
    cond |= TEST_ASSERT_FAIL2(false, 1, 2);
    TEST_ASSERT(cond);
    
    cond |= TEST_ASSERT_FAIL_L(false, __LINE__);
    TEST_ASSERT(cond);
    cond |= TEST_ASSERT_FAIL1_L(false, 1, __LINE__);
    TEST_ASSERT(cond);
    cond |= TEST_ASSERT_FAIL2_L(false, 1, 2, __LINE__);
    TEST_ASSERT(cond);
    if (cond) return;
    
    // Test the TEST_BREAK assertions.
    // These must be tested within a loop.
    for (i = 0; i < 2; i++)
    {
        TEST_ASSERT_BREAK(false);
        TEST_ASSERT_PASS(true);    // If we see a PASS, then we've actually failed.
    }
    
    for (i = 0; i < 2; i++)
    {
        TEST_ASSERT_BREAK1(false, i);
        TEST_ASSERT_PASS(true);    // If we see a PASS, then we've actually failed.
    }
    
    for (i = 0; i < 2; i++)
    {
        TEST_ASSERT_BREAK2(false, i, 1);
        TEST_ASSERT_PASS(true);    // If we see a PASS, then we've actually failed.
    }
    
    // ************************************************************************
    // * Tests that should pass.
    // ************************************************************************
    
    TEST_NUMBER(3);
    Serial.println(F("These tests should pass."));
    
    // Test the accumulated test assertion count.
    TEST_INIT();
    for (i = 0; i <= 1000; i++)
    {
        cond = (TEST_ASSERT_COUNT() == i * 3);
        TEST_ASSERT_BREAK(cond);
        TEST_ASSERT_BREAK1(cond, i);
        TEST_ASSERT_BREAK2(cond, i, 1);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) return;
    
    i = 0;
    TEST_INIT();
    // Each ASSERT should increment the count by one.
    TEST_ASSERT(TEST_ASSERT_COUNT() == i++);
    TEST_ASSERT_L((TEST_ASSERT_COUNT() == i++), __LINE__);
    
    TEST_ASSERT_PASS(TEST_ASSERT_COUNT() == i++);
    
    TEST_ASSERT_FAIL((TEST_ASSERT_COUNT() == i++));
    TEST_ASSERT_FAIL1((TEST_ASSERT_COUNT() == i++), 1);
    TEST_ASSERT_FAIL2((TEST_ASSERT_COUNT() == i++), 1, 2);
    TEST_ASSERT_FAIL_L((TEST_ASSERT_COUNT() == i++), __LINE__);
    TEST_ASSERT_FAIL1_L((TEST_ASSERT_COUNT() == i++), 1, __LINE__);
    TEST_ASSERT_FAIL2_L((TEST_ASSERT_COUNT() == i++), 1, 2, __LINE__);
}
    
/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
