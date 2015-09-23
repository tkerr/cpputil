/******************************************************************************
 * SortTest.ino
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
 * 09/01/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno SelectionSort.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 *    aunit.cpp, aunit.h, SelectionSort.c, SelectionSort.h
 * This sketch tests all functions in the SelectionSort.c module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "aunit.h"
#include "SelectionSort.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define ARRAY_SIZE 100


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
 * compare_uint32
 **************************************/ 
int compare_uint32(const void* a, const void* b)
{
  if ( *(uint32_t*)a <  *(uint32_t*)b ) return -1;
  if ( *(uint32_t*)a == *(uint32_t*)b ) return 0;
  return 1;
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
    size_t i;
    bool pass;
    uint32_t start, end;
    
    TEST_WAIT();
    TEST_INIT();
    
    TEST_NUMBER(1);
    int8_t* pi8 = (int8_t*) malloc(ARRAY_SIZE * sizeof(int8_t));
    for (i = 0; i < ARRAY_SIZE; i++) pi8[i] = random(0, 0xFF);
    start = millis();
    selectsort_int8(pi8, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pi8[i] <= pi8[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pi8);
    
    TEST_NUMBER(2);
    int16_t* pi16 = (int16_t*) malloc(ARRAY_SIZE * sizeof(int16_t));
    for (i = 0; i < ARRAY_SIZE; i++) pi16[i] = random(0, 0xFFFF);  
    start = millis();    
    selectsort_int16(pi16, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pi16[i] <= pi16[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pi16);
        
    TEST_NUMBER(3);
    int32_t* pi32 = (int32_t*) malloc(ARRAY_SIZE * sizeof(int32_t));
    for (i = 0; i < ARRAY_SIZE; i++) pi32[i] = random(0, 0xFFFFFFFF);  
    start = millis();    
    selectsort_int32(pi32, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pi32[i] <= pi32[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pi32);
    
    TEST_NUMBER(4);
    uint8_t* pu8 = (uint8_t*) malloc(ARRAY_SIZE * sizeof(uint8_t));
    for (i = 0; i < ARRAY_SIZE; i++) pu8[i] = random(0, 0xFF);
    start = millis();
    selectsort_uint8(pu8, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pu8[i] <= pu8[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pu8);
    
    TEST_NUMBER(5);
    uint16_t* pu16 = (uint16_t*) malloc(ARRAY_SIZE * sizeof(uint16_t));
    for (i = 0; i < ARRAY_SIZE; i++) pu16[i] = random(0, 0xFFFF);  
    start = millis();    
    selectsort_uint16(pu16, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pu16[i] <= pu16[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pu16);
    
    TEST_NUMBER(6);
    uint32_t* pu32 = (uint32_t*) malloc(ARRAY_SIZE * sizeof(uint32_t));
    for (i = 0; i < ARRAY_SIZE; i++) pu32[i] = random(0, 0xFFFFFFFF);  
    start = millis();    
    selectsort_uint32(pu32, ARRAY_SIZE);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (pu32[i] <= pu32[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(pu32);
    
    // Test quicksort just to see the time difference.
    TEST_NUMBER(7);
    uint32_t* qs32 = (uint32_t*) malloc(ARRAY_SIZE * sizeof(uint32_t));
    for (i = 0; i < ARRAY_SIZE; i++) qs32[i] = random(0, 0xFFFFFFFF);  
    start = millis();    
    qsort(qs32, ARRAY_SIZE, sizeof(uint32_t), compare_uint32);
    end = millis();
    for (i = 0; i < ARRAY_SIZE-1; i++)
    {
        pass = (qs32[i] <= qs32[i+1]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    TEST_ASSERT_PASS(pass);
    Serial.print(end-start);
    Serial.println(F(" ms"));
    free(qs32);
    
    Serial.print("Test assertions: ");
    Serial.println(TEST_ASSERT_COUNT());
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
