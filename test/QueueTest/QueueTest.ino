/******************************************************************************
 * QueueTest.ino
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
 * 09/16/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Queue.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 * Queue.c
 * Queue.h
 * Checksum.c
 * Checksum.h
 * UnionTypeDefs.h
 * aunit.cpp
 * aunit.h
 * This sketch tests all functions in the Queue.c module.
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
#include "Checksum.h"
#include "Queue.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define QUEUE_SIZE 17  //!< Use an odd size queue for testing

typedef struct _LARGE_DATA
{
    long num;
    long rand;
    char cksm;
} LARGE_DATA;

/******************************************************************************
 * Global objects and data.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
static long  queueArray[QUEUE_SIZE];
static QUEUE testQueue;

static LARGE_DATA largeDataArray[QUEUE_SIZE];
static QUEUE largeDataQueue;


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
    bool cond = false;
    long i, j, k;
    const long halfFull = QUEUE_SIZE / 2;
    
    LARGE_DATA  largeData;
    LARGE_DATA* pLargeData;
    
    TEST_WAIT();
    TEST_INIT();
    TEST_FILE();
    
    // ************************************************************************
    // * Test normal queue functions (overwrite disabled).
    // ************************************************************************

    QUEUE_Define(&testQueue, queueArray, QUEUE_SIZE, sizeof(long), 0);
    
    // Basic housekeeping tests.
    TEST_NUMBER(1);
    QUEUE_Clear(&testQueue);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&testQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&testQueue) == QUEUE_SIZE);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Basic enqueueing tests.
    TEST_NUMBER(2);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Adding to a full queue should fail.
    TEST_NUMBER(3);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&testQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&testQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Enqueue(&testQueue, &i) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Count(&testQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&testQueue) == 0);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Basic dequeueing tests.
    TEST_NUMBER(4);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (j == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Removing from an empty queue should fail.
    TEST_NUMBER(5);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&testQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&testQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Dequeue(&testQueue, &j) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Count(&testQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&testQueue) == QUEUE_SIZE);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Stream data through a full queue.
    TEST_NUMBER(6);
    // Fill the queue.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Stream data through the queue.
    for (; i <= 50000; i++)
    {
        cond = (QUEUE_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (j == i - QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == QUEUE_SIZE - 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == 0);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Stream data through a partially filled queue.
    TEST_NUMBER(7);
    QUEUE_Clear(&testQueue);
    
    // Partially fill the queue.
    for (i = 1; i <= halfFull; i++)
    {
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Stream data through the queue.
    for (; i <= 50000; i++)
    {
        cond = (QUEUE_Dequeue(&testQueue, &j) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (j == i - halfFull);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == halfFull - 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - (halfFull - 1));
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == halfFull);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - halfFull);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Test the peek functionality.
    TEST_NUMBER(8);
    QUEUE_Clear(&testQueue);
    
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        // Peek index is invalid.
        for (j = i - 1; j < QUEUE_SIZE; j++)
        {
            cond = (QUEUE_Peek(&testQueue, j, &k) == 0);
            TEST_ASSERT_BREAK2(cond, i, 100 + j);
        }
        if (!cond) break;
        
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
        
        // Peek index is valid.
        for (j = 0; j < i; j++)
        {
            cond = (QUEUE_Peek(&testQueue, j, &k) == 1);
            TEST_ASSERT_BREAK2(cond, i, j);
            cond = (k == j + 1);
            TEST_ASSERT_BREAK2(cond, i, j);
            cond = (QUEUE_Count(&testQueue) == i);
            TEST_ASSERT_BREAK2(cond, i, j);
            cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
            TEST_ASSERT_BREAK2(cond, i, j);
        }
        if (!cond) break;
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Test the peek functionality #2.
    // Remove two data elements and make sure the peek index still works properly.
    TEST_NUMBER(9);
    TEST_ASSERT_FAIL1((QUEUE_Dequeue(&testQueue, &j) == 1), 1);  // Data element = 1
    TEST_ASSERT_FAIL1((QUEUE_Dequeue(&testQueue, &j) == 1), 2);  // Data element = 2
    for (i = 0; i < QUEUE_SIZE - 2; i++)
    {
        cond = (QUEUE_Peek(&testQueue, (uint8_t)i, &j) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (j == i + 3);  // Data element should start with 3
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == QUEUE_SIZE - 2);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == 2);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // ************************************************************************
    // * Test the overwrite capability.
    // ************************************************************************

    testQueue.overwrite = 1;
    
    TEST_NUMBER(10);
    QUEUE_Clear(&testQueue);
    
    // Fill the queue.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Stream data through the queue without dequeueing anything.
    for (; i <= 50000; i++)
    {
        // Test peek functionality.
        for (j = 0; j < QUEUE_SIZE; j++)
        {
            cond = (QUEUE_Peek(&testQueue, j, &k) == 1);
            TEST_ASSERT_BREAK2(cond, i, j);
            cond = (k = i + j - QUEUE_SIZE);
            TEST_ASSERT_BREAK2(cond, i, j);
        }
        if (!cond) break;
        
        cond = (QUEUE_Enqueue(&testQueue, &i) == 1);  // Enqueue should succeed
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&testQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&testQueue) == 0);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // ************************************************************************
    // * Test cases with large data structures.
    // ************************************************************************

    // Test the ability to use data structures in the queue.
    QUEUE_Define(&largeDataQueue, largeDataArray, QUEUE_SIZE, sizeof(LARGE_DATA), 0);
    
    TEST_NUMBER(11);
    QUEUE_Clear(&largeDataQueue);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == QUEUE_SIZE);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Basic enqueueing tests.
    TEST_NUMBER(12);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        largeData.num = i;
        largeData.rand = random(LONG_MAX);
        largeData.cksm = checksum(&largeData, sizeof(largeData)-1);
        
        cond = (QUEUE_Enqueue(&largeDataQueue, &largeData) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Adding to a full queue should fail.
    TEST_NUMBER(13);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Enqueue(&largeDataQueue, &largeData) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == 0);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Basic dequeueing tests.
    TEST_NUMBER(14);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_Dequeue(&largeDataQueue, &largeData) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (largeData.num == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (checksum(&largeData, sizeof(largeData)) == 0);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Removing from an empty queue should fail.
    TEST_NUMBER(15);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Dequeue(&largeDataQueue, &largeData) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == QUEUE_SIZE);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
        
    // ************************************************************************
    // * Test the queue pointer functions with overwrite disabled.
    // ************************************************************************

    TEST_NUMBER(16);
    QUEUE_Clear(&largeDataQueue);
    
    // Basic enqueueing tests.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        pLargeData = (LARGE_DATA*) QUEUE_EnqueuePtr(&largeDataQueue);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);

        pLargeData->num = i;
        pLargeData->rand = random(LONG_MAX);
        pLargeData->cksm = checksum(pLargeData, sizeof(LARGE_DATA)-1);
        
        cond = (QUEUE_Count(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Adding to a full queue should fail.
    TEST_NUMBER(17);
    cond  = TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == 0);
    cond &= TEST_ASSERT_FAIL(QUEUE_EnqueuePtr(&largeDataQueue) == NULL);
    cond &= TEST_ASSERT_FAIL(QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
    cond &= TEST_ASSERT_FAIL(QUEUE_Available(&largeDataQueue) == 0);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Test the peek functionality.
    TEST_NUMBER(18);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        pLargeData = (LARGE_DATA*) QUEUE_PeekPtr(&largeDataQueue, i-1);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (pLargeData->num == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (checksum(pLargeData, sizeof(LARGE_DATA)) == 0);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Peek beyond queue limit.
    pLargeData = (LARGE_DATA*) QUEUE_PeekPtr(&largeDataQueue, QUEUE_SIZE);
    cond &= (pLargeData == NULL);
    TEST_ASSERT_FAIL(cond);
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Basic dequeueing tests.
    TEST_NUMBER(19);
    for (i = 1; i <= QUEUE_SIZE; i++)
    {
        cond = (QUEUE_DequeuePtr(&largeDataQueue) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_PeekPtr(&largeDataQueue, QUEUE_SIZE - i) == NULL);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;
    
    // Stream data through a partially filled queue.
    TEST_NUMBER(20);
    QUEUE_Clear(&largeDataQueue);
    
    // Partially fill the queue.
    for (i = 1; i <= halfFull; i++)
    {       
        pLargeData = (LARGE_DATA*) QUEUE_EnqueuePtr(&largeDataQueue);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);

        pLargeData->num = i;
        pLargeData->rand = random(LONG_MAX);
        pLargeData->cksm = checksum(pLargeData, sizeof(LARGE_DATA)-1);
        
        cond = (QUEUE_Count(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Stream data through the queue.
    for (; i <= 10000; i++)
    {
        pLargeData = (LARGE_DATA*) QUEUE_PeekPtr(&largeDataQueue, 0);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (pLargeData->num == i - halfFull);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (checksum(pLargeData, sizeof(LARGE_DATA)) == 0);
        TEST_ASSERT_BREAK1(cond, i);
        
        cond = (QUEUE_DequeuePtr(&largeDataQueue) == 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Count(&largeDataQueue) == halfFull - 1);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - (halfFull - 1));
        TEST_ASSERT_BREAK1(cond, i);
        
        pLargeData = (LARGE_DATA*) QUEUE_EnqueuePtr(&largeDataQueue);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);
        
        pLargeData->num = i;
        pLargeData->rand = random(LONG_MAX);
        pLargeData->cksm = checksum(pLargeData, sizeof(LARGE_DATA)-1);
        
        cond = (QUEUE_Count(&largeDataQueue) == halfFull);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - halfFull);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    if (!cond) goto Done;

    // ************************************************************************
    // * Test the queue pointer functions with overwrite enabled.
    // ************************************************************************
    
    // Stream data through a partially filled queue.
    TEST_NUMBER(21);
    QUEUE_Clear(&largeDataQueue);
    largeDataQueue.overwrite = 1;
    
    // Fill the queue.
    for (i = 1; i <= QUEUE_SIZE; i++)
    {       
        pLargeData = (LARGE_DATA*) QUEUE_EnqueuePtr(&largeDataQueue);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);

        pLargeData->num = i;
        pLargeData->rand = random(LONG_MAX);
        pLargeData->cksm = checksum(pLargeData, sizeof(LARGE_DATA)-1);
        
        cond = (QUEUE_Count(&largeDataQueue) == i);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == QUEUE_SIZE - i);
        TEST_ASSERT_BREAK1(cond, i);
    }
    
    // Stream data through the queue without dequeueing anything.
    for (; i <= 10000; i++)
    {
        pLargeData = (LARGE_DATA*) QUEUE_PeekPtr(&largeDataQueue, 0);
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (pLargeData->num == i - QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (checksum(pLargeData, sizeof(LARGE_DATA)) == 0);
        TEST_ASSERT_BREAK1(cond, i);
        
        cond = (QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == 0);
        TEST_ASSERT_BREAK1(cond, i);
        
        pLargeData = (LARGE_DATA*) QUEUE_EnqueuePtr(&largeDataQueue);  // Enqueue should succeed
        cond = (pLargeData != NULL);
        TEST_ASSERT_BREAK1(cond, i);
        
        pLargeData->num = i;
        pLargeData->rand = random(LONG_MAX);
        pLargeData->cksm = checksum(pLargeData, sizeof(LARGE_DATA)-1);
        
        cond = (QUEUE_Count(&largeDataQueue) == QUEUE_SIZE);
        TEST_ASSERT_BREAK1(cond, i);
        cond = (QUEUE_Available(&largeDataQueue) == 0);
        TEST_ASSERT_BREAK1(cond, i);
    }
    TEST_ASSERT_PASS(cond);
    //if (!cond) goto Done;
    
    // Done.  Print test statistics.
Done:
    Serial.print("Test assertion count: ");
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
