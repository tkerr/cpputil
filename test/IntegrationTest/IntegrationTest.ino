/******************************************************************************
 * IntegrationTest.ino
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
 * 11/23/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Integration.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, Integration.c and Integration.h into 
 * your sketch folder.  This sketch tests all functions in the Integration.c module.
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
#include "Integration.h"
 
 
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
 
// Implements one cycle of the function y = 500 * sin(2*pi*x/256)
// Function is symmetrical: F(x+128) = -F(x)
static const int sSineTable[256] =
{
   0,   12,   25,   37,   49,   61,   73,   85,   98,  110,  121,  133,  145,  157,  168,  180, 
 191,  203,  214,  225,  236,  246,  257,  267,  278,  288,  298,  308,  317,  327,  336,  345,
 354,  362,  370,  379,  387,  394,  402,  409,  416,  422,  429,  435,  441,  447,  452,  457,  
 462,  466,  471,  475,  478,  482,  485,  488,  490,  493,  495,  496,  498,  499,  499,  500,  
 500,  500,  499,  499,  498,  496,  495,  493,  490,  488,  485,  482,  478,  475,  471,  466,  
 462,  457,  452,  447,  441,  435,  429,  422,  416,  409,  402,  394,  387,  379,  370,  362,  
 354,  345,  336,  327,  317,  308,  298,  288,  278,  267,  257,  246,  236,  225,  214,  203,  
 191,  180,  168,  157,  145,  133,  121,  110,  98,    85,   73,   61,   49,   37,   25,   12,
   0,  -12,  -25,  -37,  -49,  -61,  -73,  -85,  -98, -110, -121, -133, -145, -157, -168, -180,
-191, -203, -214, -225, -236, -246, -257, -267, -278, -288, -298, -308, -317, -327, -336, -345, 
-354, -362, -370, -379, -387, -394, -402, -409, -416, -422, -429, -435, -441, -447, -452, -457, 
-462, -466, -471, -475, -478, -482, -485, -488, -490, -493, -495, -496, -498, -499, -499, -500, 
-500, -500, -499, -499, -498, -496, -495, -493, -490, -488, -485, -482, -478, -475, -471, -466, 
-462, -457, -452, -447, -441, -435, -429, -422, -416, -409, -402, -394, -387, -379, -370, -362, 
-354, -345, -336, -327, -317, -308, -298, -288, -278, -267, -257, -246, -236, -225, -214, -203, 
-191, -180, -168, -157, -145, -133, -121, -110,  -98,  -85,  -73,  -61,  -49,  -37,  -25,  -12,
};


/******************************************************************************
 * Public functions.
 ******************************************************************************/

 
/**************************************
 * linearFn
 **************************************/
integ_data_t linearFn(integ_data_t x)
{
    return 2 * x + 10;
}


/**************************************
 * sineFn
 **************************************/
integ_data_t sineFn(integ_data_t x)
{
    return sSineTable[x & 0xFF];
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
    ICX_T icx;  // Integration context structure used for testing.
    
    integ_data_t x;
    integ_data_t y;
    integ_data_t start;
    integ_data_t end;
    integ_result_t r;
    integ_result_t expected;
    bool cond;
    
    TEST_INIT();
    TEST_WAIT();
    TEST_FILE();
    
    /****
     * Test 1 - Rectangle method
     * Integrate a rectangle of width 100 and height 10.
     * Result should be exactly 1000.
     */
    TEST_NUMBER(1);
    start = 0;
    end = 100;
    y = 10;
    expected = 1000;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_rect(&icx, y);
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_rect(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 2 - Trapezoidal method
     * Integrate a rectangle of width 100 and height 10.
     * Coordinates are (11, 10) to (111, 10)
     * Result should be exactly 1000.
     */
    TEST_NUMBER(2);
    start = 0;
    end = 100;
    y = 10;
    expected = 1000;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_trap(&icx, y);
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_trap(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 3 - Simpson's Rule
     * Integrate a rectangle of width 100 and height 10.
     * Coordinates are (11, 10) to (111, 10)
     * Result should be exactly 1000.
     */
    TEST_NUMBER(3);
    start = 0;
    end = 100;
    y = 10;
    expected = 1000;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_simp(&icx, y);
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_simp(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 4 - Rectangle method
     * Integrate linear equation y = 2x + 10 from 0 to 100.
     * Actual result is 11,000.
     * Result using rectangle method is 10,900.
     */
    TEST_NUMBER(4);
    start = 0;
    end = 100;
    expected = 10900;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_rect(&icx, linearFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_rect(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 5 - Trapezoidal method
     * Integrate linear equation y = 2x + 10 from 0 to 100.
     * Actual result is 11,000.
     * Result using trapezoidal method is 11,000.
     */
    TEST_NUMBER(5);
    start = 0;
    end = 100;
    expected = 11000;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_trap(&icx, linearFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_trap(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 6 - Simpson's Rule
     * Integrate linear equation y = 2x + 10 from 0 to 100.
     * Actual result is 11,000.
     * Result using trapezoidal method is 11,000.
     */
    TEST_NUMBER(6);
    start = 0;
    end = 100;
    expected = 11000;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_simp(&icx, linearFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_simp(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 7 - Rectangle method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 256 (0 to 2*pi).
     * Actual result is 0.
     */
    TEST_NUMBER(7);
    start = 0;
    end = 256;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_rect(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_rect(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 8 - Trapezoidal method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 256 (0 to 2*pi).
     * Actual result is 0.
     */
    TEST_NUMBER(8);
    start = 0;
    end = 256;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_trap(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_trap(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
        
    /****
     * Test 9 - Simpson's Rule
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 256 (0 to 2*pi).
     * Actual result is 0.
     */
    TEST_NUMBER(9);
    start = 0;
    end = 256;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_simp(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_simp(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 10 - Rectangle method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 128 (0 to pi).
     * Actual result is 500 * 2 * (256/(2*pi)) = 40743.
     * Result using rectangle method is 40746, which corresponds 
     * to about 0.007% overall error.
     */
    TEST_NUMBER(10);
    start = 0;
    end = 128;
    expected = 40746;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_rect(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_rect(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 11 - Trapezoidal method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 128 (0 to pi).
     * Actual result is 500 * 2 * (256/(2*pi)) = 40743.
     * Result using trapezoidal method is 40746, which corresponds 
     * to about 0.007% overall error.
     */
    TEST_NUMBER(11);
    start = 0;
    end = 128;
    expected = 40746;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_trap(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_trap(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 12 - Simpson's Rule
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 128 (0 to pi).
     * Actual result is 500 * 2 * (256/(2*pi)) = 40743.
     * Result using Simpson's Rule is 40748, which corresponds 
     * to about 0.012% overall error.
     */
    TEST_NUMBER(12);
    start = 0;
    end = 128;
    expected = 40748;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_simp(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_simp(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 13 - Rectangle method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 2560 (10 cycles).
     * Actual result is 0.
     */
    TEST_NUMBER(13);
    start = 0;
    end = 256 * 10;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_rect(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_rect(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 14 - Trapezoidal method
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 2560 (10 cycles).
     * Actual result is 0.
     */
    TEST_NUMBER(14);
    start = 0;
    end = 256 * 10;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_trap(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_trap(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 15 - Simpson's Rule
     * Integrate sinusoid y = 500*sin(2*pi*x/256) from 0 to 2560 (10 cycles).
     * Actual result is 0.
     */
    TEST_NUMBER(15);
    start = 0;
    end = 256 * 10;
    expected = 0;
    integrate_start(&icx);
    for (x = start; x <= end; x++)
    {
        integrate_add_simp(&icx, sineFn(x));
        cond = (integrate_num_samples(&icx) == (x - start + 1u));
        TEST_ASSERT_BREAK1(cond, x);
    }
    r = integrate_result_simp(&icx);
    cond = (r == expected);
    TEST_ASSERT(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    /****
     * Test 16 - Rectangle method, trapezoidal method, and Simpson's Rule
     * Integrate sinusoid y = 500*sin(2*pi*x/256) using each method
     * Perform multiple iterations, starting from a different point each time 
     * to simulate phase delay.
     * Actual result is always 0.
     */
    TEST_NUMBER(16);
    expected = 0;
    ICX_T icx1, icx2, icx3;
    for (start = 0; start < 256; start++)
    {
        end = start + 256;
        integrate_start(&icx1);
        integrate_start(&icx2);
        integrate_start(&icx3);
        for (x = start; x <= end; x++)
        {
            integrate_add_rect(&icx1, sineFn(x));
            integrate_add_trap(&icx2, sineFn(x));
            integrate_add_simp(&icx3, sineFn(x));
        }
        r = integrate_result_rect(&icx1);
        cond = (r == expected);
        TEST_ASSERT_BREAK2(cond, 1, start);
        r = integrate_result_trap(&icx2);
        cond = (r == expected);
        TEST_ASSERT_BREAK2(cond, 2, start);
        r = integrate_result_simp(&icx3);
        cond = (r == expected);
        TEST_ASSERT_BREAK2(cond, 3, start);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond)
    {
        Serial.print("r = ");
        Serial.println(r);
    }
    
    Serial.print("Assert count: ");
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
