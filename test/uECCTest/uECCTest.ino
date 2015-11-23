/******************************************************************************
 * uECCTest.ino
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
 * 10/08/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno uECC.c module test program.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 *    uECC.c, uECC.h, aunit.cpp, aunit.h, hex.c, hex.h, pcg_basic.c, pcg_basic.h, 
 *    UnionTypeDefs.h
 * This sketch tests all functions in the uECC.c module.
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
#include "uECC.h"
#include "aunit.h"
#include "Hex.h"
#include "pcg_basic.h"
#include "UnionTypeDefs.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
typedef struct _PKINFO
{
    uint8_t public_key[uECC_BYTES * 2];
    uint8_t private_key[uECC_BYTES];
    uint8_t hash[uECC_BYTES];
    uint8_t signature[uECC_BYTES * 2];
} PKINFO;


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
static pcg32_random_t rng;  //!< Random number generator structure


/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * myRNGFunction
 * Generate a random number of 'size' 
 * bytes and place it in 'dest'.
 * Always returns 1 (success).
 **************************************/
int myRNGFunction(uint8_t *dest, unsigned size)
{
    uint32u_t rand;
    unsigned  index = 0;

    while (size > 0)
    {
        rand.v = pcg32_random_r(&rng);
        
        for (unsigned i = 0; i < sizeof(uint32_t); i++)
        {
            dest[index++] = rand.b[i];
            if (--size == 0) break;
        }
    }
    
    return 1;
}


/**************************************
 * printKey
 **************************************/
void printKey(uint8_t *key, unsigned size)
{
    char hex[4];
    
    while (size > 0)
    {
        for (unsigned i = 0; i < sizeof(uint32_t); i++)
        {
            Serial.print(HEX_Uint8ToHex(*key, hex));
            key++;
            if (--size == 0) break;
        }
        Serial.print(" ");
    }
    Serial.println();
}


/**************************************
 * setup
 **************************************/
void setup(void)
{
    Serial.begin(9600);
    
    // Seed the random number generator.
    pcg32_srandom_r(&rng, 42u, 54u);
    
    // Set the random number function.
    uECC_set_rng(myRNGFunction);
}


/**************************************
 * loop
 **************************************/
void loop(void)
{   
    PKINFO Alice, Bob;
    
    bool cond;
    int  status;
    
    uint8_t computed_hash[uECC_BYTES];
    
    TEST_WAIT();
    TEST_INIT();
    TEST_FILE();
    
    Serial.print("Public key size: ");
    Serial.print(uECC_BYTES*2);
    Serial.println(" bytes");
    
    uint32_t start, end;
    uint32_t key_time = 0;
    uint32_t sign_time = 0;
    uint32_t verify_time = 0;
    
    unsigned count = 16;
    for (unsigned i = 0; i < count; i++)
    {
        // Alice and Bob each create a public and private key pair.
        start = millis();
        status = uECC_make_key(Alice.public_key, Alice.private_key);
        end = millis();
        key_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        start = millis();
        status = uECC_make_key(Bob.public_key, Bob.private_key);
        end = millis();
        key_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        // Make sure they're different.
        status = memcmp(Alice.public_key, Bob.public_key, uECC_BYTES * 2);
        cond = (status != 0);
        TEST_ASSERT_BREAK(cond);
        status = memcmp(Alice.private_key, Bob.private_key, uECC_BYTES);
        cond = (status != 0);
        TEST_ASSERT_BREAK(cond);
        
        // Alice and Bob each create a (simulated) message hash for a message they originate.
        status = myRNGFunction(Alice.hash, uECC_BYTES);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        status = myRNGFunction(Bob.hash, uECC_BYTES);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        status = memcmp(Alice.hash, Bob.hash, uECC_BYTES);
        cond = (status != 0);
        TEST_ASSERT_BREAK(cond);
        
        // Alice and Bob each create a digital signature of their message hash
        // using their private keys.
        start = millis();
        status = uECC_sign(Alice.private_key, Alice.hash, Alice.signature);
        end = millis();
        sign_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        start = millis();
        status = uECC_sign(Bob.private_key, Bob.hash, Bob.signature);
        end = millis();
        sign_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        // Sanity check to make sure the signatures are different.
        status = memcmp(Alice.signature, Bob.signature, uECC_BYTES * 2);
        cond = (status != 0);
        TEST_ASSERT_BREAK(cond);
        
        // Bob sends Alice his public key, message and signature.  
        // Alice computes a hash on Bob's message.  Alice verifies Bob's 
        // signature using Bob's public key, Bob's signature, and the hash 
        // that Alice computed on Bob's message.
        
        // Case 1: Bob's message is unaltered, and Alice's computed hash is correct.
        memcpy(computed_hash, Bob.hash, uECC_BYTES);
        start = millis();
        status = uECC_verify(Bob.public_key, computed_hash, Bob.signature);
        end = millis();
        verify_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        // Case 2: Bob's message is tampered with, so Alice's computed hash is incorrect.
        computed_hash[0]++;
        start = millis();
        status = uECC_verify(Bob.public_key, computed_hash, Bob.signature);
        end = millis();
        verify_time += (end - start);
        cond = (status == 0);  // Verify should fail
        TEST_ASSERT_BREAK(cond);
        
        // This time Alice sends Bob her public key, message and signature.
        
        // Case 1: Alice's message is unaltered, and Bob's computed hash is correct.
        memcpy(computed_hash, Alice.hash, uECC_BYTES);
        start = millis();
        status = uECC_verify(Alice.public_key, computed_hash, Alice.signature);
        end = millis();
        verify_time += (end - start);
        cond = (status == 1);
        TEST_ASSERT_BREAK(cond);
        
        // Case 2: Alice's message is altered, so Bob's computed hash is incorrect.
        computed_hash[1]++;  // Choosing a different byte to alter just for fun
        start = millis();
        status = uECC_verify(Alice.public_key, computed_hash, Alice.signature);
        end = millis();
        verify_time += (end - start);
        cond = (status == 0);  // Verify should fail
        TEST_ASSERT_BREAK(cond);
        
        Serial.print('.');
    }
    Serial.println();
    TEST_ASSERT_PASS(cond);
    
    if (cond)
    {
        Serial.print("Sample public key:    ");
        printKey(Alice.public_key, uECC_BYTES * 2);
        Serial.print("Sample private key:   ");
        printKey(Alice.private_key, uECC_BYTES);
        Serial.print("Sample signature:     ");
        printKey(Alice.signature, uECC_BYTES * 2);
        Serial.print("Average key gen time: ");
        Serial.print(key_time / (count * 2));
        Serial.println(" ms");
        Serial.print("Average sign time:    ");
        Serial.print(sign_time / (count * 2));
        Serial.println(" ms");
        Serial.print("Average verify time:  ");
        Serial.print(verify_time / (count * 4));
        Serial.println(" ms");
    }
    
    Serial.print("Test assertions:      ");
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
