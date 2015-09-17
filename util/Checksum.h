/******************************************************************************
 * Checksum.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/16/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Computes a two's complement checksum on a block of data.
 */

#ifndef _CHECKSUM_H
#define _CHECKSUM_H

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>


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
 * Calculates the checksum on a byte-by-byte basis.
 *
 * @param pData Pointer to the block of data.
 *
 * @param len The size of the data block in bytes.
 *
 * @return The two's complement checksum on the data block.
 */
char checksum(const void* pData, size_t len);

#ifdef __cplusplus
}
#endif

#endif // _CHECKSUM_H
