/******************************************************************************
 * UnionTypeDefs.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 * 
 * Modification History:
 *
 * 10/30/2015 - Tom Kerr
 * Made int64u_t and uint64u_t conditional.  64-bit values are not
 * supported by all compilers.
 *
 * 09/17/2015 - Tom Kerr
 * Augmented _uint32u_t and _uint32u_t with word and long word arrays.
 *
 * 07/26/2015 - Tom Kerr
 * Doxygen updates.
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
 
// Define these if the complier supports 64-bit values.
// Otherwise, comment them out.
#define HAS_INT64
#define HAS_UINT64

typedef union _int16u_t  //! Union data type for a 16-bit signed integer
{
    int16_t v;                   //!< The composite value
    uint8_t b[sizeof(int16_t)];  //!< Unsigned byte array
} int16u_t;


typedef union _int32u_t  //! Union data type for a 32-bit signed integer
{
    int32_t v;                   //!< The composite value
    uint8_t b[sizeof(int32_t)];  //!< Unsigned byte array
} int32u_t;
  

#if defined (HAS_INT64)
typedef union _int64u_t  //! Union data type for a 64-bit signed integer
{
    int64_t v;                   //!< The composite value
    uint8_t b[sizeof(int64_t)];  //!< Unsigned byte array
} int64u_t;
#endif

 
typedef union _uint16u_t  //! Union data type for a 16-bit unsigned integer
{
    uint16_t v;                    //!< The composite value
    uint8_t  b[sizeof(uint16_t)];  //!< Unsigned byte array
} uint16u_t;


typedef union _uint32u_t  //! Union data type for a 32-bit unsigned integer
{
    uint32_t v;                      //!< The composite value
    uint16_t w[sizeof(uint32_t)/2];  //!< Unsigned word array
    uint8_t  b[sizeof(uint32_t)];    //!< Unsigned byte array
} uint32u_t;


#if defined (HAS_UINT64)
typedef union _uint64u_t  //! Union data type for a 64-bit unsigned integer
{
    uint64_t v;                      //!< The composite value
    uint16_t l[sizeof(uint64_t)/4];  //!< Unsigned long word array
    uint16_t w[sizeof(uint64_t)/2];  //!< Unsigned word array
    uint8_t  b[sizeof(uint64_t)];    //!< Unsigned byte array
} uint64u_t;
#endif

#endif // _UNION_TYPE_DEFS_H
