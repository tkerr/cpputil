/******************************************************************************
 * aunit.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 10/04/2015 - Tom Kerr
 * Added TEST_DONE().
 *
 * 09/22/2015 - Tom Kerr
 * Refactored.  Fail messages can now print source file line numbers.
 * Renamed most functions and added a corresponding macro to automatically
 * insert the line number.
 * Added TEST_INIT() and TEST_ASSERT_COUNT().
 *
 * 08/29/2015 - Tom Kerr
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

#ifndef _AUNIT_H
#define _AUNIT_H

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/


/******************************************************************************
 * Public definitions.
 ******************************************************************************/

/****
 * These macros are  used to automatically insert the line number into the
 * test failure message.
 */
 
/**
 * @brief
 * Test the boolean condition and print a pass/fail message. 
 * See TEST_ASSERT_L().
 */ 
#define TEST_ASSERT(a) TEST_ASSERT_L((a), __LINE__);

/**
 * @brief
 * Break out of a loop if the test assertion fails.
 * Print a fail message only; do not print a pass message.
 * See TEST_ASSERT_FAIL_L().
 */
#define TEST_ASSERT_BREAK(a) if (!TEST_ASSERT_FAIL_L((a), __LINE__)) break;

/**
 * @brief
 * Break out of a loop if the test assertion fails.
 * Print a fail message only; do not print a pass message.
 * Print one user-supplied numeric parameter in the fail message.
 * See TEST_ASSERT_FAIL1_L().
 */
#define TEST_ASSERT_BREAK1(a, b) if (!TEST_ASSERT_FAIL1_L((a), b, __LINE__)) break;

/**
 * @brief
 * Break out of a loop if the test assertion fails.
 * Print a fail message only; do not print a pass message.
 * Print two user-supplied numeric parameters in the fail message.
 * See TEST_ASSERT_FAIL2_L().
 */ 
#define TEST_ASSERT_BREAK2(a, b, c) if (!TEST_ASSERT_FAIL2_L((a), b, c, __LINE__)) break;

/**
 * @brief
 * Test the boolean condition and print a message only if the test fails.
 * See TEST_ASSERT_FAIL_L().
 */
#define TEST_ASSERT_FAIL(a) TEST_ASSERT_FAIL_L((a), __LINE__);

/**
 * @brief
 * Test the boolean condition and print a message only if the test fails.
 * Print one user-supplied numeric parameter in the fail message.
 * See TEST_ASSERT_FAIL1_L().
 */
#define TEST_ASSERT_FAIL1(a, b) TEST_ASSERT_FAIL1_L((a), b, __LINE__);

/**
 * @brief
 * Test the boolean condition and print a message only if the test fails.
 * Print two user-supplied numeric parameters in the fail message.
 * See TEST_ASSERT_FAIL2_L().
 */
#define TEST_ASSERT_FAIL2(a, b, c) TEST_ASSERT_FAIL2_L((a), b, c, __LINE__);

/**
 * @brief
 * Print the current file name.
 */
#define TEST_FILE() Serial.println(F(__FILE__));


/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**
 * @brief
 * Initialize the unit test environment. 
 *
 * Currently only needed to clear the test assertion count.  The test assertion
 * count can be read using TEST_ASSERT_COUNT().
 */ 
void TEST_INIT(void);


/**
 * @brief
 * Return the test assertion count.
 *
 * The test assertion count is incremented with each call to  TEST_ASSERT(),
 * TEST_ASSERT_PASS(), TEST_ASSERT_FAIL(),TEST_ASSERT_FAIL2(), and 
 * TEST_ASSERT_BREAK().  Use this function to read the total count.  
 * The count is reset by calling TEST_INIT().
 *
 * @return The total test assertion count since the last initialization.
 */ 
uint32_t TEST_ASSERT_COUNT(void);

 
/**
 * @brief
 * Test the boolean condition and print a pass/fail message. 
 *
 * @param cond The test assertion condition.
 *
 * @param line The line number of the failed test.  Call using the __LINE__ 
 * preprocessor directive for this parameter.  Use the TEST_ASSERT()
 * macro to automatically insert the line number.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_L(bool cond, uint16_t line);


/**
 * @brief
 * Test the boolean condition and print a message only if the test passes. 
 *
 * @param cond The test assertion condition.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_PASS(bool cond);


/**
 * @brief
 * Test the boolean condition and print a message only if the test fails. 
 *
 * @param cond The test assertion condition.
 *
 * @param line The line number of the failed test.  Call using the __LINE__ 
 * preprocessor directive for this parameter.  Use the TEST_ASSERT_FAIL()
 * macro to automatically insert the line number.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_FAIL_L(bool cond, uint16_t line);


/**
 * @brief
 * Test the boolean condition and print a message only if the test fails. 
 * Also prints one user-supplied numeric value in the test fail message.
 *
 * @param cond The test assertion condition.
 *
 * @param x A test variable printed if the test fails.
 *
 * @param line The line number of the failed test.  Call using the __LINE__ 
 * preprocessor directive for this parameter.  Use the TEST_ASSERT_FAIL1()
 * macro to automatically insert the line number.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_FAIL1_L(bool cond, int16_t x, uint16_t line);

/**
 * @brief
 * Test the boolean condition and print a message only if the test fails. 
 * Also prints two user-supplied numeric values in the test fail message.
 *
 * @param cond The test assertion condition.
 *
 * @param x A test variable printed if the test fails.
 *
 * @param y A test variable printed if the test fails.
 *
 * @param line The line number of the failed test.  Call using the __LINE__ 
 * preprocessor directive for this parameter.  Use the TEST_ASSERT_FAIL2()
 * macro to automatically insert the line number.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_FAIL2_L(bool cond, int16_t x, int16_t y, uint16_t line);


/**
 * @brief
 * Print a prompt string and wait for the user to press a key. 
 *
 * Used to prompt the user to start the test.  Waits forever
 * until a character is read over the serial port.
 */
void TEST_WAIT();


/**
 * @brief
 * Convenience function used to print a test number. 
 *
 * @param num The test number to print.
 */
void TEST_NUMBER(uint16_t num);


/**
 * @brief
 * Prints a TEST DONE message. 
 *
 * Intended for use by automated test tools so they can detect when a unit
 * test is complete.
 */
void TEST_DONE();


#endif // _AUNIT_H
