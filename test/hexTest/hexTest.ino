/******************************************************************************
 * hexTest.ino
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
 * 08/29/2015 - Tom Kerr
 * Modified to use aunit test structure.
 * Moved some strings to PROGMEM.
 *
 * 08/23/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno hex.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy aunit.cpp, aunit.h, UnionTypeDefs.h, hex.c, and hex.h into your sketch 
 * folder.  This sketch tests all functions in the hex.c module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "aunit.h"
#include "Hex.h"
#include "UnionTypeDefs.h"


/******************************************************************************
 * Forward references.
 ******************************************************************************/

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define NUM_TEST_VECTORS 20

 
/******************************************************************************
 * Global objects and data.
 ******************************************************************************/
 
uint8_t testVectorsBinary[] = {
    0x00, 0x09, 0x0A, 0x0F, 
    0x10, 0x19, 0x1A, 0x1F,
    0x90, 0x99, 0x9A, 0x9F,
    0xA0, 0xA9, 0xAA, 0xAF,
    0xF0, 0xF9, 0xFA, 0xFF,
    };
        
char const* testVectorsAscii[] = {
    "00", "09", "0A", "0F",
    "10", "19", "1A", "1F",
    "90", "99", "9A", "9F",
    "A0", "A9", "AA", "AF",
    "F0", "F9", "FA", "FF",
    };
    
char testResultAscii[2*sizeof(uint64_t)+1];


/******************************************************************************
 * Local data.
 ******************************************************************************/

/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * CheckString
 **************************************/ 
bool CheckString(const char* expected, const char* actual, size_t size)
{
    bool ok = (strncmp(expected, actual, size) == 0);
    return ok;
}

 
/**************************************
 * ClearTestResult
 **************************************/ 
void ClearTestResult()
{
    for (unsigned int i=0; i < 2*sizeof(uint64_t)+1; i++)
        testResultAscii[i] = 0;
}


/**************************************
 * PrintErrorAscii
 **************************************/ 
void PrintErrorAscii(const char* expected, const char* actual)
{
    Serial.print("Expected: ");
    Serial.print(expected);
    Serial.print(" Actual: ");
    Serial.println(actual);
}


/**************************************
 * PrintErrorBinary
 **************************************/ 
void PrintErrorBinary(uint64_t expected, uint64_t actual)
{
    char expectedAscii[17];
    char actualAscii[17];
    HEX_Uint64ToHex(expected, expectedAscii);
    HEX_Uint64ToHex(actual, actualAscii);
    PrintErrorAscii(expectedAscii, actualAscii);
}


/**************************************
 * Test_HEX_Uint8ToHex
 **************************************/ 
void Test_HEX_Uint8ToHex()
{
    Serial.println(F("Testing HEX_Uint8ToHex()"));
    bool pass = false;
    int i;
    for (i=0; i < NUM_TEST_VECTORS; i++)
    {
        ClearTestResult();
        HEX_Uint8ToHex(testVectorsBinary[i], testResultAscii);
        pass = CheckString(testVectorsAscii[i], testResultAscii, 2*sizeof(uint8_t));
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorAscii(testVectorsAscii[i], testResultAscii);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_Uint16ToHex
 **************************************/ 
void Test_HEX_Uint16ToHex()
{
    Serial.println(F("Testing HEX_Uint16ToHex()"));
    bool pass = false;
    uint16u_t testNum;
    const char testAsc[] = "ABCD";
    char expected[5];
    
    // Quick sanity check.
    ClearTestResult();
    HEX_Uint16ToHex(0xABCDu, testResultAscii);
    pass = CheckString(testAsc, testResultAscii, 2*sizeof(uint16_t));
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorAscii(testAsc, testResultAscii);
        return;
    }
    
    // Check all combinations.
    for (testNum.v = 0; testNum.v < 65535; testNum.v++)
    {
        // Create an expected result.
        // Assumes HEX_Uint8ToHex() works correctly.
        HEX_Uint8ToHex(testNum.b[1], &expected[0]);
        HEX_Uint8ToHex(testNum.b[0], &expected[2]);
        expected[4] = 0;
        
        // Perform the test conversion.
        ClearTestResult();
        HEX_Uint16ToHex(testNum.v, testResultAscii);
        
        pass = CheckString(expected, testResultAscii, 2*sizeof(uint16_t));
        TEST_ASSERT_BREAK2(pass, testNum.v, 1);
    }
    if (!pass) PrintErrorAscii(expected, testResultAscii);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_Uint32ToHex
 **************************************/ 
void Test_HEX_Uint32ToHex()
{
    Serial.println(F("Testing HEX_Uint32ToHex()"));
    bool pass = false;
    uint32u_t testNum;
    const char testAsc[] = "01234567";
    char expected[9];
    
    // Quick sanity check.
    ClearTestResult();
    HEX_Uint32ToHex(0x01234567ul, testResultAscii);
    pass = CheckString(testAsc, testResultAscii, 2*sizeof(uint32_t));
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorAscii(testAsc, testResultAscii);
        return;
    }
    
    // Check a large number of random combinations.
    for (uint16_t i = 0; i < 10000; i++)
    {
        // Generate a random test vector.
        testNum.b[0] = (uint8_t)random(0, 256);
        testNum.b[1] = (uint8_t)random(0, 256);
        testNum.b[2] = (uint8_t)random(0, 256);
        testNum.b[3] = (uint8_t)random(0, 256);
        
        // Create an expected result.
        // Assumes HEX_Uint8ToHex() works correctly.
        HEX_Uint8ToHex(testNum.b[3], &expected[0]);
        HEX_Uint8ToHex(testNum.b[2], &expected[2]);
        HEX_Uint8ToHex(testNum.b[1], &expected[4]);
        HEX_Uint8ToHex(testNum.b[0], &expected[6]);
        expected[8] = 0;
        
        // Perform the test conversion.
        ClearTestResult();
        HEX_Uint32ToHex(testNum.v, testResultAscii);
        pass = CheckString(expected, testResultAscii, 2*sizeof(uint32_t));
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorAscii(expected, testResultAscii);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_Uint64ToHex
 **************************************/ 
void Test_HEX_Uint64ToHex()
{
    Serial.println(F("Testing HEX_Uint64ToHex()"));
    bool pass = false;
    uint64u_t testNum;
    const char testAsc[] = "0123456789ABCDEF";
    char expected[17];

    // Quick sanity check.
    ClearTestResult();
    HEX_Uint64ToHex(0x0123456789ABCDEFull, testResultAscii);
    pass = CheckString(testAsc, testResultAscii, 2*sizeof(uint64_t));
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorAscii(testAsc, testResultAscii);
        return;
    }
    
    // Check a large number of random combinations.
    for (uint16_t i = 0; i < 10000; i++)
    {
        // Generate a random test vector.
        testNum.b[0] = (uint8_t)random(0, 256);
        testNum.b[1] = (uint8_t)random(0, 256);
        testNum.b[2] = (uint8_t)random(0, 256);
        testNum.b[3] = (uint8_t)random(0, 256);
        testNum.b[4] = (uint8_t)random(0, 256);
        testNum.b[5] = (uint8_t)random(0, 256);
        testNum.b[6] = (uint8_t)random(0, 256);
        testNum.b[7] = (uint8_t)random(0, 256);
        
        // Create an expected result.
        // Assumes HEX_Uint8ToHex() works correctly.
        HEX_Uint8ToHex(testNum.b[7], &expected[0]);
        HEX_Uint8ToHex(testNum.b[6], &expected[2]);
        HEX_Uint8ToHex(testNum.b[5], &expected[4]);
        HEX_Uint8ToHex(testNum.b[4], &expected[6]);
        HEX_Uint8ToHex(testNum.b[3], &expected[8]);
        HEX_Uint8ToHex(testNum.b[2], &expected[10]);
        HEX_Uint8ToHex(testNum.b[1], &expected[12]);
        HEX_Uint8ToHex(testNum.b[0], &expected[14]);
        expected[16] = 0;
        
        // Perform the test conversion.
        ClearTestResult();
        HEX_Uint64ToHex(testNum.v, testResultAscii);
        
        pass = CheckString(expected, testResultAscii, 2*sizeof(uint64_t));
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorAscii(expected, testResultAscii);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_HexToUint8
 **************************************/ 
void Test_HEX_HexToUint8()
{
    Serial.println(F("Testing HEX_HexToUint8()"));
    bool pass = false;
    uint8_t actual;
    int i;
    for (i=0; i < NUM_TEST_VECTORS; i++)
    {
        actual = HEX_HexToUint8(testVectorsAscii[i]);
        pass = (actual == testVectorsBinary[i]);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorBinary(testVectorsBinary[i], actual);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_HexToUint16
 **************************************/ 
void Test_HEX_HexToUint16()
{
    Serial.println(F("Testing HEX_HexToUint16()"));
    bool pass = false;
    uint16_t actual;
    uint16_t testNum;
    char testAsc[5] = "ABCD";
    
    // Quick sanity check.
    testNum = 0xABCDu;
    actual = HEX_HexToUint16(testAsc);
    pass = (actual == testNum);
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorBinary(testNum, actual);
        return;
    }
    
    // Check all combinations.
    for (testNum = 0; testNum < 65535; testNum++)
    {
        // Genarate an ASCII string.
        // Assumes HEX_Uint16ToHex() works correctly.
        HEX_Uint16ToHex(testNum, testAsc);
        
        // Perform the test conversion.
        actual = HEX_HexToUint16(testAsc);
        pass = (actual == testNum);
        TEST_ASSERT_BREAK2(pass, testNum, 1);
    }
    if (!pass) PrintErrorBinary(testNum, actual);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_HexToUint32
 **************************************/ 
void Test_HEX_HexToUint32()
{
    Serial.println(F("Testing HEX_HexToUint32()"));
    bool pass = false;
    uint32_t actual;
    uint32u_t testNum;
    char testAsc[9] = "87654321";
    
    // Quick sanity check.
    testNum.v = 0x87654321ul;
    actual = HEX_HexToUint32(testAsc);
    pass = (actual == testNum.v);
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorBinary(testNum.v, actual);
        return;
    }
    
    
    // Check a large number of random combinations.
    for (uint16_t i = 0; i < 10000; i++)
    {
        // Genarate a random hex ASCII string.
        // Assumes HEX_Uint32ToHex() works correctly.
        testNum.b[0] = (uint8_t)random(0, 256);
        testNum.b[1] = (uint8_t)random(0, 256);
        testNum.b[2] = (uint8_t)random(0, 256);
        testNum.b[3] = (uint8_t)random(0, 256);
        HEX_Uint32ToHex(testNum.v, testAsc);
        
        // Perform the test conversion.
        actual = HEX_HexToUint32(testAsc);
        pass = (actual == testNum.v);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorBinary(testNum.v, actual);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * Test_HEX_HexToUint64
 **************************************/ 
void Test_HEX_HexToUint64()
{
    Serial.println(F("Testing HEX_HexToUint64()"));
    bool pass = false;
    uint64_t actual;
    uint64u_t testNum;
    char testAsc[17] = "FEDCBA9876543210";
    
    // Quick sanity check.
    testNum.v = 0xFEDCBA9876543210ull;
    actual = HEX_HexToUint64(testAsc);
    pass = (actual == testNum.v);
    TEST_ASSERT_FAIL(pass);
    if (!pass)
    {
        PrintErrorBinary(testNum.v, actual);
        return;
    }
    
    // Check a large number of random combinations.
    for (uint16_t i = 0; i < 10000; i++)
    {
        // Genarate a random hex ASCII string.
        // Assumes HEX_Uint64ToHex() works correctly.
        testNum.b[0] = (uint8_t)random(0, 256);
        testNum.b[1] = (uint8_t)random(0, 256);
        testNum.b[2] = (uint8_t)random(0, 256);
        testNum.b[3] = (uint8_t)random(0, 256);
        testNum.b[4] = (uint8_t)random(0, 256);
        testNum.b[5] = (uint8_t)random(0, 256);
        testNum.b[6] = (uint8_t)random(0, 256);
        testNum.b[7] = (uint8_t)random(0, 256);
        HEX_Uint64ToHex(testNum.v, testAsc);
        
        // Perform the test conversion.
        actual = HEX_HexToUint64(testAsc);
        pass = (actual == testNum.v);
        TEST_ASSERT_BREAK2(pass, i, 1);
    }
    if (!pass) PrintErrorBinary(testNum.v, actual);
    TEST_ASSERT_PASS(pass);
}


/**************************************
 * setup
 **************************************/ 
void setup()
{
	Serial.begin(9600);
}


/**************************************
 * loop
 **************************************/ 
void loop()
{
    TEST_WAIT();
    TEST_INIT();
    TEST_FILE();
    
    Test_HEX_Uint8ToHex();
    Test_HEX_Uint16ToHex();
    Test_HEX_Uint32ToHex();
    Test_HEX_Uint64ToHex();
    Test_HEX_HexToUint8();
    Test_HEX_HexToUint16();
    Test_HEX_HexToUint32();
    Test_HEX_HexToUint64();
    
    Serial.print("Test assertions: ");
    Serial.println(TEST_ASSERT_COUNT());
    
    TEST_DONE();
}
