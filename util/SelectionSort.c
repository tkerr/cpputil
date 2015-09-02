/******************************************************************************
 * SelectionSort.c
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
 * While @c quicksort is a fast sorting method, its recursive nature may not be 
 * suitable for small embedded systems.
 *
 * @c selectsort is provided as an alternative.  Although its worst case performance
 * is O(n^2), it is not recursive and only requires one memory element for storage.
 *
 * A @c selectsort function is provided for each integral data type.
 */
 
/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "SelectionSort.h"


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
 * selectsort_int8
 **************************************/
void selectsort_int8(int8_t* base, size_t num)
{
    size_t i, j;
    int8_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/**************************************
 * selectsort_int16
 **************************************/
void selectsort_int16(int16_t* base, size_t num)
{
    size_t i, j;
    int16_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/**************************************
 * selectsort_int32
 **************************************/
void selectsort_int32(int32_t* base, size_t num)
{
    size_t i, j;
    int32_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/**************************************
 * selectsort_uint8
 **************************************/
void selectsort_uint8(uint8_t* base, size_t num)
{
    size_t i, j;
    uint8_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/**************************************
 * selectsort_uint16
 **************************************/
void selectsort_uint16(uint16_t* base, size_t num)
{
    size_t i, j;
    uint16_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/**************************************
 * selectsort_uint32
 **************************************/
void selectsort_uint32(uint32_t* base, size_t num)
{
    size_t i, j;
    uint32_t temp;
    
    for (i = 0; i < num - 1; i++)
    {
        for (j = i + 1; j < num; j++)
        {
            if (base[j] < base[i])
            {
                temp = base[i];
                base[i] = base[j];
                base[j] = temp;
            }
        }
    }
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/
 
 
// End of file.