/******************************************************************************
 * Checksum.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/18/2015 - Tom Kerr
 * Added ipv4_checksum().
 *
 * 09/16/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Checksum functions for detecting errors in blocks of data.
 */

#ifndef _CHECKSUM_H
#define _CHECKSUM_H

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>
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
 * Computes an 8-bit, two's complement checksum on a block of data.
 *
 * Calculates the checksum on a byte-by-byte basis.  Once the checksum is
 * calculated, data integrity verified by performing the checksum on the 
 * data + checksum and verifying that it sums to zero.
 *
 * @param pData Pointer to the block of data.
 *
 * @param len The size of the data block in bytes.
 *
 * @return The two's complement checksum on the data block.
 */
char checksum(const void* pData, size_t len);

/**
 * @brief
 * Computes the  IPv4 header checksum on a block of data.
 *
 * The checksum is calculated by forming the one's complement sum of the
 * header's 16-bit words. The result of summing the entire IP header, 
 * including checksum, should be zero if there is no corruption.
 *
 * References:
 *  + https://en.wikipedia.org/wiki/IPv4_header_checksum.
 *
 *  + "Internet Header Format". Internet Protocol DARPA Internet program 
 * protocol specification. IETF. September 1981. p. 14. STD 5. RFC 791.
 *
 * @param pData Pointer to the block of data.
 *
 * @param len The size of the data block in bytes.
 *
 * @return The one's complement checksum on the data block.
 */
uint16_t ipv4_checksum(const void* pData, size_t len);

#ifdef __cplusplus
}
#endif

#endif // _CHECKSUM_H
