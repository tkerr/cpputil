/******************************************************************************
 * Bcd.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 07/24/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * BCD-binary conversion routines.
 *
 * Designed for little-endian CPUs including Atmel 8-bit AVR and Microchip PIC.
 * May not work on big-endian CPUs such as AVR32 and Freescale.
 */
 
/******************************************************************************
 * System include files.
 ******************************************************************************/


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Bcd.h"
#include "UnionTypeDefs.h"


/******************************************************************************
 * Forward references.
 ******************************************************************************/


/******************************************************************************
 * Local definitions.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/


/**************************************
 * BCD_ByteToBcd
 **************************************/
uint8_t BCD_ByteToBcd(uint8_t bin)
{
    uint8_t bcd;
    bcd = (bin / 10) << 4;
    bcd += bin % 10;
    return bcd;
}


/**************************************
 * BCD_BcdToByte
 **************************************/
uint8_t BCD_BcdToByte(uint8_t bcd)
{
    uint8_t bin;
    bin = (bcd >> 4) * 10;
    bin += (bcd & 0x0f);
   return bin;
}


/**************************************
 * BCD_BinaryToBcd
 **************************************/
int BCD_BinaryToBcd(uint64_t bin, uint8_t* bcd)
{
    #define BCD_ARRAY_SIZE 10 // Array passed to us must be at least this big
    
    uint64u_t qwBin;         // Convenience variable for performance improvement
    int arrayIndexMax = 0;   // Keep track of highest BCD array index containing nonzero data
    int shiftCount = 0;      // Number of left shifts so far
    int i;
    
    qwBin.v = bin;
    
    for (i=0; i < BCD_ARRAY_SIZE; i++)
    {
        bcd[i] = 0;
    }

    // Convert binary to BCD using the 'shift and add 3' method. 
    // The exit point from the while loop occurs at the break statement.
    while (1)
    {
        // Shift the BCD array left one bit.
        for (i=arrayIndexMax; i>=0; i--)
        {
            bcd[i] <<= 1;  // Shift current byte
            if (i > 0)
            {
                // Shift bit from lower order byte into higher order byte.
                if (bcd[i-1] & 0x80)
                {
                    bcd[i] |= 0x01;
                }
            }
        }

        // Shift the binary number into the bcd array.
        // Then shift the binary number.
        if (qwBin.b[7] & 0x80)
        {
            bcd[0] |= 0x01;
        }
        qwBin.v <<= 1;

        // Adjust the shift count.
        // If we've shifted 64 times, then we're done.
        // We always end with a shift operation.
        if (++shiftCount == 64)
        {
            break;
        }

        // Check each BCD digit.
        // If 5 or greater, then add 3.
        for (i=arrayIndexMax; i>=0; i--)
        {
            if ((bcd[i] & 0x0F) >= 5)
            {
                bcd[i] += 0x03;
            }
            
            if (bcd[i] >= 0x50)
            {
                bcd[i] += 0x30;
            }
        }

        // See if we need to increment the BCD array maximum index.
        if (bcd[arrayIndexMax] >= 0x80)
        {
            arrayIndexMax++;
        }
    }
    
    return arrayIndexMax + 1;
}


/**************************************
 * BCD_BcdToAscii
 **************************************/
void BCD_BcdToAscii(const uint8_t* bcd, int size, char* ascii)
{
    int i;
    int j = 0;
    uint8_t bcdDigit;
    uint8_t hasDigits = 0; // TRUE once we've found BCD digits in the array
    
    // Initialize ASCII string to zero just in case we don't find any digits.
    ascii[0] = '0';
    ascii[1] = 0;
    
    for (i=size-1; i>=0; i--)
    {
        // Convert the most significant BCD digit.
        // Skip leading zeros.
        bcdDigit = (bcd[i] & 0xF0);
        if (hasDigits || (bcdDigit > 0))
        {
            ascii[j++] = (bcdDigit >> 4) + '0';
            hasDigits = 1;
        }
        
        // Convert the least significant BCD digit.
        // Skip leading zeros.
        bcdDigit = (bcd[i] & 0x0F);
        if (hasDigits || (bcdDigit > 0))
        {
            ascii[j++] = bcdDigit + '0';
            hasDigits = 1;
        }
    }
    
    if (j > 0)
    {
        ascii[j] = 0;
    }
}

// End of file.
