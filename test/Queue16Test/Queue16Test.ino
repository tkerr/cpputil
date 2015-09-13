/******************************************************************************
 * Queue16Test.ino
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
 * 09/13/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Queue16.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, Queue16.c and Queue16.h into your sketch folder.  
 * This sketch tests all functions in the Queue16.c module.
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
#include "Queue16.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define QUEUE_SIZE 17u  //!< Use an odd size queue for testing


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
static uint16_t queueArray[QUEUE_SIZE];
static QUEUE16  testQueue;
 

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
    bool cond;
    uint16_t i, j, k;
    const uint16_t halfFull = QUEUE_SIZE / 2;

    TEST_WAIT();
    
    // Queue with overwrite disabled.
    QUEUE16_Define(&testQueue, queueArray, QUEUE_SIZE, 0);
    
    // Basic housekeeping tests.
    TEST_NUMBER(1);
    QUEUE16_Clear(&testQueue);
    TEST_ASSERT(QUEUE16_Count(&testQueue) == 0);
    TEST_ASSERT(QUEUE16_Available(&testQueue) == QUEUE_SIZE);
    
    // Basic enqueueing tests.
    TEST_NUMBER(2);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 1);
        cond = (QUEUE16_Count(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 3);
    }
    TEST_ASSERT_PASS(cond);
    
    // Adding to a full queue should fail.
    TEST_NUMBER(3);
    TEST_ASSERT(QUEUE16_Count(&testQueue) == QUEUE_SIZE);
    TEST_ASSERT(QUEUE16_Available(&testQueue) == 0);
    TEST_ASSERT(QUEUE16_Enqueue(&testQueue, i) == 0);
    TEST_ASSERT(QUEUE16_Count(&testQueue) == QUEUE_SIZE);
    TEST_ASSERT(QUEUE16_Available(&testQueue) == 0);
    
    // Basic dequeueing tests.
    TEST_NUMBER(4);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE16_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK(cond, i, 1);
        cond = (j == i);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Count(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 3);
        cond = (QUEUE16_Available(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 4);
    }
    TEST_ASSERT_PASS(cond);
    
    // Removing from an empty queue should fail.
    TEST_NUMBER(5);
    TEST_ASSERT(QUEUE16_Count(&testQueue) == 0);
    TEST_ASSERT(QUEUE16_Available(&testQueue) == QUEUE_SIZE);
    TEST_ASSERT(QUEUE16_Dequeue(&testQueue, &j) == 0);
    TEST_ASSERT(QUEUE16_Count(&testQueue) == 0);
    TEST_ASSERT(QUEUE16_Available(&testQueue) == QUEUE_SIZE);
    
    // Stream data through a full queue.
    TEST_NUMBER(6);
    // Fill the queue.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 1);
        cond = (QUEUE16_Count(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 3);
    }
    
    // Stream data through the queue.
    for (; i <= 50000u; i++)
    {
        cond = (QUEUE16_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK(cond, i, 4);
        cond = (j == i - QUEUE_SIZE);
        TEST_ASSERT_BREAK(cond, i, 5);
        cond = (QUEUE16_Count(&testQueue) == QUEUE_SIZE - 1);
        TEST_ASSERT_BREAK(cond, i, 6);
        cond = (QUEUE16_Available(&testQueue) == 1);
        TEST_ASSERT_BREAK(cond, i, 7);
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 8);
        cond = (QUEUE16_Count(&testQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK(cond, i, 9);
        cond = (QUEUE16_Available(&testQueue) == 0);
        TEST_ASSERT_BREAK(cond, i, 10);
    }
    TEST_ASSERT_PASS(cond);
    
    // Stream data through a partially filled queue.
    TEST_NUMBER(7);
    QUEUE16_Clear(&testQueue);
    
    // Partially fill the queue.
    for (i = 1; i <= halfFull; i++)
    {
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 1);
        cond = (QUEUE16_Count(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 3);
    }
    
    // Stream data through the queue.
    for (; i <= 50000u; i++)
    {
        cond = (QUEUE16_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK(cond, i, 4);
        cond = (j == i - halfFull);
        TEST_ASSERT_BREAK(cond, i, 5);
        cond = (QUEUE16_Count(&testQueue) == halfFull - 1);
        TEST_ASSERT_BREAK(cond, i, 6);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - (halfFull - 1));
        TEST_ASSERT_BREAK(cond, i, 7);
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 8);
        cond = (QUEUE16_Count(&testQueue) == halfFull);
        TEST_ASSERT_BREAK(cond, i, 9);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - halfFull);
        TEST_ASSERT_BREAK(cond, i, 10);
    }
    TEST_ASSERT_PASS(cond);
    
    // Test the peek functionality.
    TEST_NUMBER(8);
    QUEUE16_Clear(&testQueue);
    
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        // Peek index is invalid.
        for (j = i - 1; j < QUEUE_SIZE; j++)
        {
            cond = (QUEUE16_Peek(&testQueue, j, &k) == 0);
            TEST_ASSERT_BREAK(cond, i, 100 + j);
        }
        if (!cond) break;
        
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Count(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 3);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 4);
        
        // Peek index is valid.
        for (j = 0; j < i; j++)
        {
            cond = (QUEUE16_Peek(&testQueue, j, &k) == 1);
            TEST_ASSERT_BREAK(cond, i, 200 + j);
            cond = (k == j + 1);
            TEST_ASSERT_BREAK(cond, i, 300 + j);
            cond = (QUEUE16_Count(&testQueue) == i);
            TEST_ASSERT_BREAK(cond, i, 400 + j);
            cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
            TEST_ASSERT_BREAK(cond, i, 500 + j);
        }
        if (!cond) break;
    }
    TEST_ASSERT_PASS(cond);
    
    // Test the peek functionality #2.
    // Remove two data elements and make sure the peek index still works properly.
    TEST_NUMBER(9);
    TEST_ASSERT_FAIL((QUEUE16_Dequeue(&testQueue, &j) == 1), 1, 1);  // Data element = 1
    TEST_ASSERT_FAIL((QUEUE16_Dequeue(&testQueue, &j) == 1), 1, 2);  // Data element = 2
    for (i = 0; i < QUEUE_SIZE - 2; i++)
    {
        cond = (QUEUE16_Peek(&testQueue, (uint8_t)i, &j) == 1);
        TEST_ASSERT_BREAK(cond, i, 3);
        cond = (j == i + 3);  // Data element should start with 3
        TEST_ASSERT_BREAK(cond, i, 4);
        cond = (QUEUE16_Count(&testQueue) == QUEUE_SIZE - 2);
        TEST_ASSERT_BREAK(cond, i, 5);
        cond = (QUEUE16_Available(&testQueue) == 2);
        TEST_ASSERT_BREAK(cond, i, 6);
    }
    TEST_ASSERT_PASS(cond);
    
    // Test the overwrite capability.
    testQueue.overwrite = 1;
    
    TEST_NUMBER(10);
    QUEUE16_Clear(&testQueue);
    
    // Fill the queue.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);
        TEST_ASSERT_BREAK(cond, i, 1);
        cond = (QUEUE16_Count(&testQueue) == i);
        TEST_ASSERT_BREAK(cond, i, 2);
        cond = (QUEUE16_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK(cond, i, 3);
    }
    
    // Stream data through the queue without dequeueing anything.
    for (; i <= 50000u; i++)
    {
        // Test peek functionality.
        for (j = 0; j < QUEUE_SIZE; j++)
        {
            cond = (QUEUE16_Peek(&testQueue, j, &k) == 1);
            TEST_ASSERT_BREAK(cond, i, 100 + j);
            cond = (k = i + j - QUEUE_SIZE);
            TEST_ASSERT_BREAK(cond, i, 200 + j);
        }
        if (!cond) break;
        
        cond = (QUEUE16_Enqueue(&testQueue, i) == 1);  // Enqueue should succeed
        TEST_ASSERT_BREAK(cond, i, 4);
        cond = (QUEUE16_Count(&testQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK(cond, i, 5);
        cond = (QUEUE16_Available(&testQueue) == 0);
        TEST_ASSERT_BREAK(cond, i, 6);
    }
    TEST_ASSERT_PASS(cond);
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
