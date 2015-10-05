/******************************************************************************
 * pcgTest.ino
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
 * 09/30/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Arduino Uno pcg_basic.c module test program.  PCG is a fast, space-efficient, 
 * statistically good algorithm for random number generation.
 *
 * Contains the setup() and loop() functions for the program.  Also uses the
 * aunit pseudo test framework to execute the tests.
 *
 * Copy the following files into your sketch folder:
 * + pcg_basic.c
 * + pcg_basic.h
 * + aunit.cpp
 * + aunit.h
 *
 * This sketch tests all functions in the pcg_basic.c module.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "aunit.h"
#include "pcg_basic.h"
 
 
/******************************************************************************
 * Forward references.
 ******************************************************************************/
 

/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define PCG32_RAND_MAX         4294967295ul
#define PGC32_RAND_MAX_D       (double)(4294967295.0)
#define PCG32_EXPECTED_MEAN    2147483647ul
#define PCG32_EXPECTED_MEAN_D  (double)(2147483647.0)
#define NUM_BINS               128


/******************************************************************************
 * Global objects and data.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
static pcg32_random_t rng;
static int32_t bin[NUM_BINS];


/******************************************************************************
 * Public functions.
 ******************************************************************************/
 

/**************************************
 * setup
 **************************************/
void setup(void)
{
    Serial.begin(9600);
	
	// Start with a deterministic seed.
	pcg32_srandom_r(&rng, 42u, 54u);
}


/**************************************
 * loop
 **************************************/
void loop(void)
{
	TEST_FILE();
    TEST_WAIT();
    TEST_INIT();
    
	// Initialization.
	uint32_t numTrials = NUM_BINS * 2000ul;
	uint32_t rand;
	int32_t  frequency = 0;
	uint64_t randSum = 0;
	
	// Used for calculating pi.
	double x, y, r, pi;
	uint32_t inside = 0;
	
	// Initialize the histogram bins.
	for (int i = 0; i < NUM_BINS; i++ )
	{
		bin[i] = 0;
	}
	
	Serial.print("Running ");
	Serial.print(numTrials);
	Serial.println(" trials");
	
	// Generate a bunch of random numbers and calculate statistics.
	// Two random numbers are generated each time through the loop.
	uint32_t j = numTrials / 2;
	do
	{
		// First random number.
		rand = pcg32_random_r(&rng);
		randSum += rand;
		
		// Calculate the ones frequency.
		uint32_t mask = 0x00000001;
		for (int k = 0; k < 32; k++)
		{
			if ((rand & mask) > 0)
				frequency += 1;
			else
				frequency -= 1;
			mask <<= 1;
		}
		
		// Increment the corresponding histogram bin.
		int binNum = (int)(rand >> 25);
		bin[binNum]++;
		
		// Monte Carlo pi calculation.
		x = (double)rand / PGC32_RAND_MAX_D;
		
		// Second random number.
		rand = pcg32_random_r(&rng);
		randSum += rand;
		
		// Calculate the ones frequency.
		mask = 0x00000001;
		for (int k = 0; k < 32; k++)
		{
			if ((rand & mask) > 0)
				frequency += 1;
			else
				frequency -= 1;
			mask <<= 1;
		}
		
		// Increment the corresponding histogram bin.
		binNum = (int)(rand >> 25);
		bin[binNum]++;
		
		// Monte Carlo pi calculation.
		y = (double)rand / PGC32_RAND_MAX_D;
		r = sqrt((x * x) + (y * y));
		if (r <= 1.0) inside++;
	} while (--j > 0);
	
	// Print the ones frequency.
	double percent = ((double) frequency / (double) numTrials) * 100.0;
	Serial.println();
	Serial.print("Ones frequency: ");
	Serial.print(frequency);
	Serial.print(" (");
	Serial.print(percent, 3);
	Serial.println("%)");
	
	// Compute and print the random number mean.
	uint32_t randMean = randSum / numTrials;
	double error = ((double)randMean - PCG32_EXPECTED_MEAN_D) / PCG32_EXPECTED_MEAN_D * 100.0;
	Serial.print("Mean:     ");
	Serial.println(randMean);
	Serial.print("Expected: ");
	Serial.println(PCG32_EXPECTED_MEAN);
	Serial.print("Error:    ");
	Serial.print(error, 3);
	Serial.println("%");
	
	// Determine uniformity over a number of bins.
	int32_t expected_i = numTrials / NUM_BINS;     // Theoretical expected value
	double  expected_d = (double)expected_i;
	double  chiSquare = 0.0;                       // Chi-squared statistic
	int32_t minBin = expected_i * 2;
	int32_t maxBin = 0;
	int64_t errSum = 0;
	for (int i = 0; i < NUM_BINS; i++)
	{
		if (bin[i] < minBin) minBin = bin[i];
		if (bin[i] > maxBin) maxBin = bin[i];
		
		int32_t delta_i = bin[i] - expected_i;
		double  delta_d = (double)bin[i] - expected_d;
		
		errSum += abs(delta_i);
		chiSquare += delta_d * delta_d / expected_d;
	}
	
	Serial.println();
	Serial.print("Num bins:   ");
	Serial.println(NUM_BINS);
	
	error = (((double)minBin - expected_d) / expected_d) * 100.0;
	Serial.print("Min count:  ");
	Serial.print(minBin);
	Serial.print(" (");
	Serial.print(error, 3);
	Serial.println("%)");
	
	error = (((double)maxBin - expected_d) / expected_d) * 100.0;
	Serial.print("Max count:  ");
	Serial.print(maxBin);
	Serial.print(" (");
	Serial.print(error, 3);
	Serial.println("%)");
	
	int32_t errMean = errSum / NUM_BINS;
	percent = ((double)errMean / expected_d) * 100.0;
	Serial.print("Mean error: ");
	Serial.print(errMean);
	Serial.print(" ");
	Serial.print(percent, 3);
	Serial.println("%");
	
	Serial.print("Chi-squared statistic: ");
	Serial.println(chiSquare, 4);
	
	// Monte Carlo estimation of pi
	pi = 8.0 * ((double)inside / (double)numTrials);
	double pi_expected = 3.1415926;
	percent = (pi - pi_expected) / pi_expected * 100.0;
	Serial.println();
	Serial.print("pi = ");
	Serial.print(pi, 6);
	Serial.print(" (");
	Serial.print(percent, 3);
	Serial.println("%)");
	
	TEST_DONE();
	Serial.println();
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/



/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/

 // End of file.
