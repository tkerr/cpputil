/******************************************************************************
 * Hex.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 07/26/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Binary <-> hex conversion functions.
 *
 * Designed for little-endian CPUs including Atmel 8-bit AVR and Microchip PIC.
 * May not work on big-endian CPUs such as AVR32 and Freescale.
 */

#ifndef _HEX_H
#define _HEX_H

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
 * Convert a binary number to a hexadecimal ASCII string.
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
 * @param ascii Pointer to an ASCII string buffer to receive the result.
 * The maximum buffer size needed is 17 bytes for a result of 2^64 - 1 = 0xFFFFFFFFFFFFFFFF.
 *
 * @param minSize The minimum size of the ASCII hex string.  The function will
 * fill with leading zeroes if needed.  Range is 1-16.
 *
 * @return The size of the ASCII hex string.
 */
int HEX_BinaryToHex(uint64_t bin, char* ascii, int minSize);


/**
 * @brief
 * Convert an ASCII string of hexadecimal digits to an 8-bit numeric value.
 *
 * Returns zero if the hex string would convert to a number greater
 * than 8 bits.
 *
 * If a non-hexdecimal character is found, the function will terminate
 * and return a partial conversion result.
 *
 * @param hex The hexadecimal string to convert.
 *
 * @return The 8-bit numeric result.
 */
uint8_t HEX_HexToUint8(const char* hex);


/**
 * @brief
 * Convert an ASCII string of hexadecimal digits to a 16-bit numeric value.
 *
 * Returns zero if the hex string would convert to a number greater
 * than 16 bits.
 *
 * If a non-hexdecimal character is found, the function will terminate
 * and return a partial conversion result.
 *
 * @param hex The hexadecimal string to convert.
 *
 * @return The 16-bit numeric result.
 */
uint16_t HEX_HexToUint16(const char* hex);


/**
 * @brief
 * Convert an ASCII string of hexadecimal digits to a 32-bit numeric value.
 *
 * Returns zero if the hex string would convert to a number greater
 * than 32 bits.
 *
 * If a non-hexdecimal character is found, the function will terminate
 * and return a partial conversion result.
 *
 * @param hex The hexadecimal string to convert.
 *
 * @return The 32-bit numeric result.
 */
uint32_t HEX_HexToUint32(const char* hex);


/**
 * @brief
 * Convert an ASCII string of hexadecimal digits to a 64-bit numeric value.
 *
 * Returns zero if the hex string would convert to a number greater
 * than 64 bits.
 *
 * If a non-hexdecimal character is found, the function will terminate
 * and return a partial conversion result.
 *
 * @param hex The hexadecimal string to convert.
 *
 * @return The 64-bit numeric result.
 */
uint64_t HEX_HexToUint64(const char* hex);


#ifdef __cplusplus
}
#endif

#endif /* _HEX_H */
