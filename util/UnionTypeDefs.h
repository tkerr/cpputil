/******************************************************************************
 * UnionTypeDefs.h
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
 * Union data types used for transferring data to/from byte arrays,
 * EEPROM storage, etc.  Objects can be accessed either by their composite
 * data type or as an unsigned byte array.
 */
 
#ifndef _UNION_TYPE_DEFS_H
#define _UNION_TYPE_DEFS_H

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

typedef union _int16u_t  //!< Union data type for a 16-bit signed integer
{
    int16_t v;                   //!< The composite value
    uint8_t b[sizeof(int16_t)];  //!< Unsigned byte array
} int16u_t;


typedef union _int32u_t  //!< Union data type for a 32-bit signed integer
{
    int32_t v;                   //!< The composite value
    uint8_t b[sizeof(int32_t)];  //!< Unsigned byte array
} int32u_t;
  
  
typedef union _uint16u_t  //!< Union data type for a 16-bit unsigned integer
{
    uint16_t v;                    //!< The composite value
    uint8_t  b[sizeof(uint16_t)];  //!< Unsigned byte array
} uint16u_t;


typedef union _uint32u_t  //!< Union data type for a 32-bit unsigned integer
{
    uint32_t v;                    //!< The composite value
    uint8_t  b[sizeof(uint32_t)];  //!< Unsigned byte array
} uint32u_t;


#endif // _UNION_TYPE_DEFS_H
