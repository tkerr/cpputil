/******************************************************************************
 * sha256Test.ino
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
 * 08/29/2015 - Tom Kerr
 * Moved some strings to PROGMEM.
 *
 * 08/21/2015 - Tom Kerr
 * Initial creation.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno sha256.c module test program.
 *
 * Contains the setup() and loop() functions for the program.
 * Also contains all necessary test functions.
 *
 * Copy sha256.c and sha256.h into your sketch folder.   
 * This sketch performs a SHA-256 test against standard NIST test vectors.
 *
 * Output is printed and must be inspected against the results documented
 * in TestVectors.txt.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <ctype.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "sha256.h"

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

/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
/**************************************
 * print_hash
 **************************************/ 
void print_hash(uint8_t hash[])
{
    int  i, j;
    char buf[4];
    Serial.print(F("Hash: "));
    for (i=0; i < 8; i++)
    {
        for (j=0; j < 4; j++)
        {
            sprintf(buf, "%02X", hash[i*4+j]);
            Serial.print(buf);
        }
        Serial.print(" ");
    }
    Serial.println();
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
	SHA256_CTX ctx;
	uint32_t start, stop;
	
	char text1[] = "";
    char text2[] = "abc";
    char text3[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    char text4[] = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";
    char text5[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    uint8_t hash[32];
	
	Serial.print(F("Press a key to begin: "));
	while (!Serial.available()) {}
    Serial.read();
	Serial.println();
	
	// Hash one
    Serial.print(F("Text: "));
    Serial.println(text1);
    start = millis();
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)text1, strlen(text1));
    sha256_final(&ctx, hash);
    stop = millis();
    print_hash(hash);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    // Hash two
    Serial.print(F("Text: "));
    Serial.println(text2);
    start = millis();
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)text2, strlen(text2));
    sha256_final(&ctx, hash);
    stop = millis();
    print_hash(hash);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    // Hash three
    Serial.print(F("Text: "));
    Serial.println(text3);
    start = millis();
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)text3, strlen(text3));
    sha256_final(&ctx, hash);
    stop = millis();
    print_hash(hash);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    // Hash four
    Serial.print(F("Text: "));
    Serial.println(text4);
    start = millis();
    sha256_init(&ctx);
    sha256_update(&ctx, (uint8_t*)text4, strlen(text4));
    sha256_final(&ctx, hash);
    stop = millis();
    print_hash(hash);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
    
    // Hash five
    Serial.println(F("Text: 1 million repetitions of the character 'a'"));
    uint16_t i = 0;
    start = millis();
    sha256_init(&ctx);
    for (i=0; i < 10000; i++)
    {
        sha256_update(&ctx, (uint8_t*)text5, strlen(text5));
    }
    sha256_final(&ctx, hash);
    stop = millis();
    print_hash(hash);
    Serial.print(stop-start);
    Serial.println(F(" ms"));
}
