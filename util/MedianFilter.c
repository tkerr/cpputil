/******************************************************************************
 * MedianFilter.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 08/30/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * A median filter implementation designed primarily for embedded systems.
 */
 
/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "MedianFilter.h"


/******************************************************************************
 * Forward references.
 ******************************************************************************/


/******************************************************************************
 * Local definitions.
 ******************************************************************************/


/******************************************************************************
 * Local data.
 ******************************************************************************/
 
static median_t sFilterBuf[MEDIAN_FILTER_SIZE]; //!< The median filter buffer
static uint8_t  sCount = 0;                     //!< The number of elements contained the filter


 
/******************************************************************************
 * Public functions.
 ******************************************************************************/
 
 
/**************************************
 * MedianInit
 **************************************/
void MedianInit(void)
{
    uint8_t i = 0;
    for(; i < MEDIAN_FILTER_SIZE; i++)
    {
        sFilterBuf[i] = MEDIAN_VALUE_MAX;
    }
    sCount = 0;
}


/**************************************
 * MedianAdd
 **************************************/
uint8_t MedianAdd(median_t value)
{
    uint8_t i;
    uint8_t j;
    
    // Add the value to the filter buffer in order from smallest to largest.
    for (i = 0; i < MEDIAN_FILTER_SIZE; i++)
    {
        if (value < sFilterBuf[i])
        {
            // Shift values to make room for the new one.
            for (j = MEDIAN_FILTER_SIZE-1; j > i; j--)
            {
                sFilterBuf[j] = sFilterBuf[j-1];
            }
            
            // Insert the new value.
            sFilterBuf[i] = value;
            break;
        }
    }
    
    if (++sCount > MEDIAN_FILTER_SIZE) sCount = MEDIAN_FILTER_SIZE;
    
    return sCount;
}


/**************************************
 * MedianCount
 **************************************/
uint8_t MedianCount(void)
{
    return sCount;
}


/**************************************
 * MedianValue
 **************************************/
median_t MedianValue(void)
{
    return sFilterBuf[MEDIAN_FILTER_SIZE/2];
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/
 
 
// End of file.