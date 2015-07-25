/******************************************************************************
 * Crc16.h
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
 * 07/24/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Fast CRC-16 calculation routine.
 *
 * Based on the algorithm described in "C Programmer's Guide to
 * NetBIOS", W.David Schwaderer, pp 169-198, Howard W. Sams & Co.
 * ISBN 0-672-22638-3.
 *
 * See also "A Painless Guide to CRC Error Detection Algorithms"
 * by Ross N. Williams.  http://www.zlib.net/crc_v3.txt
 *
 * A standard CRC-16 uses the polynomial divisor
 * x16 + x15 + x2 + 1 = 1 1000 0000 0000 0011.
 * Performance is estimated as detecting 99.995% of all possible
 * errors in a 4,000 byte data stream.  Performance will vary for
 * other data sizes.
 */

#ifndef _CRC16_H
#define _CRC16_H

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
 * Calculates the CRC-16 value of the specified data array.
 *
 * This is a right-shift CRC with a zero seed value.
 *
 * @param pdata Pointer to an array of data bytes.
 *
 * @param size The size of the data array in bytes.
 *
 * @return The CRC-16 value of the data array.
 */
uint16_t CRC16_Calc(const uint8_t* pdata, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif // _CRC16_H
