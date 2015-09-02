/******************************************************************************
 * SelectionSort.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/01/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Selection sort implementation designed primarily for embedded systems.
 *
 * Most implementations of libc already provide an implemenation of @c quicksort.
 * While it is a fast sorting method, its recursive nature may not be suitable
 * for small embedded systems.
 *
 * @c selectsort is provided as an alternative.  Although its worst case performance
 * is O(n^2), it is not recursive and only requires one memory element for storage.
 *
 * A @c selectsort function is provided for each integral data type.
 */

#ifndef _SELECTION_SORT_H
#define _SELECTION_SORT_H

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
 * Selection sort algorithm for @c int8_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c int8_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_int8(int8_t* base, size_t num);

/**
 * @brief
 * Selection sort algorithm for @c int16_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c int16_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_int16(int16_t* base, size_t num);

/**
 * @brief
 * Selection sort algorithm for @c int32_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c int32_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_int32(int32_t* base, size_t num);

/**
 * @brief
 * Selection sort algorithm for @c uint8_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c uint8_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_uint8(uint8_t* base, size_t num);

/**
 * @brief
 * Selection sort algorithm for @c uint16_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c uint16_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_uint16(uint16_t* base, size_t num);

/**
 * @brief
 * Selection sort algorithm for @c unt32_t data types.
 *
 * Sorts the @c num elements of the array pointed to by @c base.
 *
 * The function does not return any value, but modifies the content of the 
 * array pointed to by @c base reordering its elements as defined by @c compar.
 *
 * @param base Pointer to the first object of the @c uint32_t array to be sorted.
 *
 * @param num Number of elements in the array pointed to by @c base.
 */
void selectsort_uint32(uint32_t* base, size_t num);

#ifdef __cplusplus
}
#endif

#endif // _SELECTION_SORT_H
