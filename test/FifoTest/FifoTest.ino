/******************************************************************************
 * FifoTest.ino
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
 * 08/28/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno Fifo.cpp module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, Fifo.cpp and Fifo.h into your sketch folder.  
 * This sketch tests all functions in the Fifo.cpp module.
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
#include "Fifo.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define FIFO_SIZE 17u


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/
uint8_t fifoBuf[FIFO_SIZE];
uint8_t dataBuf[FIFO_SIZE+1];

// A global fifo for testing.
Fifo TestFifo(fifoBuf, FIFO_SIZE);


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
    //Serial.write("Press any key to start: ");
    //while (Serial.available() < 1) {}
    //Serial.read();
    //Serial.write("\r\n");
    
    TEST_WAIT();
    TEST_INIT();
    TEST_FILE();
    
    TestFifo.clear();
    
    // Fifo should be empty.
    TEST_NUMBER(1); 
    TEST_ASSERT(TestFifo.count() == 0);
    TEST_NUMBER(2); 
    TEST_ASSERT(TestFifo.available() == FIFO_SIZE);
    
    // Start filling the fifo one item at a time.
    uint8_t i;
    TEST_NUMBER(3); 
    for (i=1; i<=FIFO_SIZE; i++)
    {
        TEST_ASSERT_BREAK2((TestFifo.add(&i, 1) == 1), i, 1);
        TEST_ASSERT_BREAK2((TestFifo.count() == i), i, 2);
        TEST_ASSERT_BREAK2((TestFifo.available() == (FIFO_SIZE - i)), i, 3);
        TEST_ASSERT_BREAK2((TestFifo.peek(&dataBuf[0], 1) == 1), i, 4);
        TEST_ASSERT_BREAK2((dataBuf[0] == 1), i, 5);  // Peek should never consume fifo bytes
    }
    TEST_ASSERT_PASS(i > FIFO_SIZE);
    
    // Fifo should be full.
    TEST_NUMBER(4); 
    TEST_ASSERT(TestFifo.add(&i, 1) == 0);
    
    // Peek at the data without consuming it.
    TEST_NUMBER(5); 
    TEST_ASSERT(TestFifo.peek(&dataBuf[1], FIFO_SIZE) == FIFO_SIZE);
    TEST_NUMBER(6); 
    for (i=1; i<=FIFO_SIZE; i++)
    {
        TEST_ASSERT_BREAK2((dataBuf[i] == i), i, 1);
    }
    TEST_ASSERT_PASS(i > FIFO_SIZE);
    
    // Start removing the fifo one item at a time.
    TEST_NUMBER(7); 
    dataBuf[0] = 0;
    for (i=1; i<=FIFO_SIZE; i++)
    {
        TEST_ASSERT_BREAK2((TestFifo.remove(&dataBuf[i], 1) == 1), i, 1);
        TEST_ASSERT_BREAK2((dataBuf[i] == i), i, 2);
        TEST_ASSERT_BREAK2((TestFifo.count() == (FIFO_SIZE - i)), i, 3);
        TEST_ASSERT_BREAK2((TestFifo.available() == i), i, 4);
    }
    TEST_ASSERT_PASS(i > FIFO_SIZE);
    
    // Add multiple bytes at a time.
    TEST_NUMBER(8); 
    TestFifo.clear();
    bool pass = false;
    uint8_t total = 0;
    uint8_t group = 3;
    while (TestFifo.available() > group)
    {
        pass = false;
        TEST_ASSERT_BREAK2((TestFifo.add(&dataBuf[0], group) == group), 1, 0);
        total += group;
        TEST_ASSERT_BREAK2((TestFifo.count() == total), 2, 0);
        pass = true;
    }
    TEST_ASSERT_PASS(pass);
    
    // Only a subset of the full group should be added.
    TEST_NUMBER(9); 
    uint8_t remain = TestFifo.available();
    TEST_ASSERT(remain < group);
    TEST_NUMBER(10); 
    TEST_ASSERT(TestFifo.add(&dataBuf[0], group) == remain);
    TEST_NUMBER(11); 
    TEST_ASSERT(TestFifo.count() == FIFO_SIZE);
    TEST_NUMBER(12); 
    TEST_ASSERT(TestFifo.available() == 0);
    
    // Remove multiple bytes at a time.
    TEST_NUMBER(13); 
    total = 0;
    while (TestFifo.count() > group)
    {
        pass = false;
        TEST_ASSERT_BREAK2((TestFifo.remove(&dataBuf[0], group) == group), 1, 0);
        total += group;
        TEST_ASSERT_BREAK2((TestFifo.available() == total), 2, 0);
        pass = true;
    }
    TEST_ASSERT_PASS(pass);
    
    // Only a subset of the full group should be added.
    TEST_NUMBER(14); 
    remain = TestFifo.count();
    TEST_ASSERT(remain < group);
    TEST_NUMBER(15); 
    TEST_ASSERT(TestFifo.remove(&dataBuf[0], group) == remain);
    TEST_NUMBER(16); 
    TEST_ASSERT(TestFifo.count() == 0);
    TEST_NUMBER(17); 
    TEST_ASSERT(TestFifo.available() == FIFO_SIZE);
    
    // Data integrity test.
    TEST_NUMBER(18); 
    TestFifo.clear();
    group = 3;
    uint8_t data_in = 0;
    uint8_t data_out = 0;
    
    // Seed the fifo.
    for (data_in=0; data_in<5; data_in++)
        TestFifo.add(&data_in, 1);
    
    // Run a bunch of bytes through the fifo.
    // Make sure the head and tail pointers loop many times.
    for (uint16_t j = 0; j < 10000; j++)
    {
        // Add a group of bytes to the fifo.
        dataBuf[0] = data_in++;
        dataBuf[1] = data_in++;
        dataBuf[2] = data_in++;
        TEST_ASSERT_BREAK2((TestFifo.add(dataBuf, group) == group), j, 1);
        
        // Remove a group of bytes from the fifo.
        TEST_ASSERT_BREAK2((TestFifo.remove(dataBuf, group) == group), j, 2);
        for (i=0; i<group; i++)
        {
            pass = false;
            TEST_ASSERT_BREAK2((dataBuf[i] == data_out), j, 3);
            data_out++;
            pass = true;
        }
        
        if (!pass) break;
    }
    TEST_ASSERT_PASS(pass);
    
    // Test residual data in the fifo.
    TEST_NUMBER(19); 
    TEST_ASSERT(TestFifo.count() == 5);  // We seeded the fifo with this many bytes above
    TEST_NUMBER(20); 
    TestFifo.clear();
    TEST_ASSERT(TestFifo.count() == 0);
    
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
