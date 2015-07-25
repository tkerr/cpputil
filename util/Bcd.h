/******************************************************************************
 * Bcd.h
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

#ifndef _BCD_H
#define _BCD_H

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/


/******************************************************************************
* Public definitions.
******************************************************************************/


/******************************************************************************
 * Public functions.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Convert a binary byte to BCD format. 
 * 
 * Binary value must be from 0 to 99 for this to work properly.
 *
 * @param bin A binary value in the range 0-99.
 *
 * @return A BCD value in the range 0x00-0x99.
 */
uint8_t BCD_ByteToBcd(uint8_t bin);


/**
 * @brief
 * Convert a BCD number to a binary value. 
 * 
 * Only works for valid BCD numbers (0x00-0x99).
 *
 * @param bcd The BCD value to convert.
 *
 * @return A binary value in the range 0-99.
 */
uint8_t BCD_BcdToByte(uint8_t bcd);


/**
 * @brief
 * Convert a a binary number to a BCD array.
 *
 * Converts a binary number up to 64 bits to a BCD array.
 * Each byte in the BCD array contains two BCD digits.
 * Note that bcd[0] contains the least significant digits.
 *
 * This function assumes that the bcd array is at least 10 bytes in size,
 * regardless of the size of the binary value passed in.  This accounts
 * for the maximum value of 2^64 = 18,446,744,073,709,551,616 which requires
 * an array of 10 bytes.
 *
 * @param bin The binary value to convert.
 *
 * @param bcd Pointer to an array of at least 10 bytes to contain the BCD digits.
 *
 * @return The number of bytes in the bcd array that contain BCD digits.
 */
int BCD_BinaryToBcd(uint64_t bin, uint8_t* bcd);


/**
 * @brief
 * Convert an array of BCD digits to a zero-terminated ASCII string.
 *
 * @param bcd Pointer to an array of BCD digits.  Note that bcd[0] contains
 * the least significant digits.
 *
 * @param size The BCD array size in bytes.  
 * Example: if size = 10 then there are 19-20 BCD digits.
 *
 * @param ascii Pointer to a zero-terminated ASCII string.
 */
void BCD_BcdToAscii(const uint8_t* bcd, int size, char* ascii);


#ifdef __cplusplus
}
#endif

#endif /* _BCD_H */
