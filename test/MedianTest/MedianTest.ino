/******************************************************************************
 * MedianTest.ino
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
 * 09/22/2015 - Tom Kerr
 * Use refactored aunit test functions.
 *
 * 08/30/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno MedianFilter.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 *    aunit.cpp, aunit.h, MedianFilter.c, MedianFilter.h
 * This sketch tests all functions in the MedianFilter.c module.
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
#include "MedianFilter.h"
 
 
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
static median_t sSortArray[MEDIAN_FILTER_SIZE];


/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * selectionSort
 **************************************/ 
median_t selectionSort(void)
{
    // Perform a selection sort on the array.
    // Worst case performance is O(n^2).
    // However, only one storage element is needed.
    for (uint8_t i = 0; i < MEDIAN_FILTER_SIZE - 1; i++)
    {
        for (uint8_t j = i + 1; j < MEDIAN_FILTER_SIZE; j++)
        {
            if (sSortArray[j] < sSortArray[i])
            {
                median_t temp = sSortArray[i];
                sSortArray[i] = sSortArray[j];
                sSortArray[j] = temp;
            }
        }
    }
    
    // Return the median value.
    return sSortArray[MEDIAN_FILTER_SIZE/2];
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
    TEST_INIT();
    TEST_FILE();
    
    median_t m;
    uint8_t count;
    bool pass;
    
    // Add a series of values in ascending order.
    TEST_NUMBER(1);
    MedianInit();
    count = 0;
    for (int i = 0; i < MEDIAN_FILTER_SIZE; i++)
    {
        pass = false;
        TEST_ASSERT_BREAK1(MedianAdd(i) == ++count, i);
        TEST_ASSERT_BREAK1(MedianCount() == count, i);
        pass = true;
    }
    TEST_ASSERT_PASS(pass);
    
    TEST_NUMBER(2);
    m = MedianValue();
    TEST_ASSERT(m == count/2);
    
    // Adding another value should not affect the count.
    TEST_NUMBER(3);
    MedianAdd(5);
    TEST_ASSERT(MedianCount() == count);
    
    // Add a series of values in descending order.
    TEST_NUMBER(4);
    MedianInit();
    count = 0;
    for (int i = MEDIAN_FILTER_SIZE-1; i >= 0; i--)
    {
        pass = false;
        TEST_ASSERT_BREAK1(MedianAdd(i) == ++count, i);
        TEST_ASSERT_BREAK1(MedianCount() == count, i);
        pass = true;
    }
    TEST_ASSERT_PASS(pass);
    
    TEST_NUMBER(5);
    m = MedianValue();
    TEST_ASSERT(m == count/2);
    
    // Make sure outliers are discarded.
    TEST_NUMBER(6);
    MedianInit();
    count = 0;
    uint8_t nOutliers = (MEDIAN_FILTER_SIZE / 4);        // 25% of the values are outliers
    uint8_t nValues   = MEDIAN_FILTER_SIZE - nOutliers;  // 75% of the values are 'normal'
    median_t value    = MEDIAN_VALUE_MAX / 128;          // The 'normal' value
    for (int i = 0; i < nOutliers; i++)
    {
        MedianAdd(MEDIAN_VALUE_MAX / 2);  // Add an outlier
    }
    for (int i = 0; i < nValues; i++)
    {
        MedianAdd(value);  // Add a 'normal' value
    }
    m = MedianValue();
    TEST_ASSERT(m == value);
    
    TEST_NUMBER(7);
    TEST_ASSERT(MedianCount() == MEDIAN_FILTER_SIZE);
    
    // Test some random numbers.
    TEST_NUMBER(8);
    median_t expected;
    median_t calc;
    for (int i = 0; i < 1000; i++)
    {
        MedianInit();
        for (int j = 0; j < MEDIAN_FILTER_SIZE; j++)
        {
            median_t v = random(0, MEDIAN_VALUE_MAX);
            sSortArray[j] = v;
            MedianAdd(v);
        }
        expected = MedianValue();
        calc = selectionSort();
        pass = (expected == calc);
        TEST_ASSERT_BREAK1(pass, i);
    }
    TEST_ASSERT_PASS(pass);
    
    Serial.print("Test assertions: ");
    Serial.println(TEST_ASSERT_COUNT());
    
    TEST_DONE();
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
