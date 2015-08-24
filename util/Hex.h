/******************************************************************************
 * Hex.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 08/23/2015 - Tom Kerr
 * Refactored the binary to hex conversion functions.
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
 * Convert an 8-bit numeric value to a hexadecimal ASCII string.
 *
 * Equivalent to sprintf(hex, "%02X", bin);
 *
 * @param bin The input value to convert.
 *
 * @param hex The converted hex string.
 *
 * @return Pointer to the converted hex string.
 */
char* HEX_Uint8ToHex(uint8_t bin, char* hex);


/**
 * @brief
 * Convert a 16-bit numeric value to a hexadecimal ASCII string.
 *
 * Equivalent to sprintf(hex, "%04X", bin);
 *
 * @param bin The input value to convert.
 *
 * @param hex The converted hex string.
 *
 * @return Pointer to the converted hex string.
 */
char* HEX_Uint16ToHex(uint16_t bin, char* hex);


/**
 * @brief
 * Convert a 32-bit numeric value to a hexadecimal ASCII string.
 *
 * Equivalent to sprintf(hex, "%08X", bin);
 *
 * @param bin The input value to convert.
 *
 * @param hex The converted hex string.
 *
 * @return Pointer to the converted hex string.
 */
char* HEX_Uint32ToHex(uint32_t bin, char* hex);


/**
 * @brief
 * Convert a 64-bit numeric value to a hexadecimal ASCII string.
 *
 * Equivalent to sprintf(hex, "%016X", bin);
 *
 * @param bin The input value to convert.
 *
 * @param hex The converted hex string.
 *
 * @return Pointer to the converted hex string.
 */
char* HEX_Uint64ToHex(uint64_t bin, char* hex);


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
