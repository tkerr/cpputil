/******************************************************************************
 * aunit.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
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

/**
 * @brief
 * Break out of a loop if the test assertion fails.
 * Print a fail message only; do not print a pass message.
 */
#define TEST_ASSERT_BREAK(a, b, c) if (!TEST_ASSERT_FAIL((a), b, c)) break;



/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**
 * @brief
 * Test the boolean condition and print a pass/fail message. 
 *
 * @param cond The test assertion condition.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT(bool cond);


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
 * Intended to be used within loop constucts.  Two additional parameters are
 * passed by the caller and printed if the test fails.  The intention is to
 * help provide traceablilty to the test case that failed.
 *
 * @param cond The test assertion condition.
 *
 * @param x A test variable printed if the test fails.
 *
 * @param y A test variable printed if the test fails.
 *
 * @return The assertion result.
 */
bool TEST_ASSERT_FAIL(bool cond, int16_t x, int16_t y);


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


#endif // _AUNIT_H
