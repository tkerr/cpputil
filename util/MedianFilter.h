/******************************************************************************
 * MedianFilter.h
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
 *
 * Usage:
 * - Call MedianInit() to initialize the filter each time before use.
 * - Call MedianAdd() multiple times to add samples to the filter.
 * - Call MedianValue() to obtain the median filter result.
 */

#ifndef _MEDIAN_FILTER_H
#define _MEDIAN_FILTER_H

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

/**
 * @brief
 * Set the data type of the median filter here.
 */
typedef uint16_t median_t;


/**
 * @brief
 * Specify the maximum value of the median filter data type.
 *
 * This should be the maximum value for the data type selected, or a value
 * greater than or equal to the maximum value that can be added to the filter.
 * For example, if filtering 12-bit ADC values, the maximum value should be
 * at least 0x0FFF.
 */
#define MEDIAN_VALUE_MAX 0xFFFF


/**
 * @brief
 * Sets the median filter size. Should be an odd number.
 */
#define MEDIAN_FILTER_SIZE 7


/******************************************************************************
 * Public functions.
 ******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Initialize the median filter.
 *
 * Call this function once each time prior to using the filter. 
 * Then call MedianAdd() multiple times to add values to the filter.
 */
void MedianInit(void);


/**
 * @brief
 * Add a value to the median filter.
 *
 * Call MedianInit() to initialize the filter, then call this function 
 * multiple times to add values to the filter.  Adding additional values 
 * beyond the filter size (defined by MEDIAN_FILTER_SIZE) could generate
 * incorrect results.
 *
 * @param value The value to add.
 *
 * @return The number of elements in the filter.
 */
uint8_t MedianAdd(median_t value);


/**
 * @brief
 * Return the number of elements added to the median filter.
 *
 * @return A value between 0 and MEDIAN_FILTER_SIZE, inclusive.
 */
uint8_t MedianCount(void);


/**
 * @brief
 * Return the median value of the elements in the filter.
 * 
 * Call MedianInit() to initialize the filter, then call MedianAdd() multiple
 * times to add values to the filter.  When MEDIAN_FILTER_SIZE values have
 * been added, call this function to obtain the median value.
 */
median_t MedianValue(void);


#ifdef __cplusplus
}
#endif

#endif // _MEDIAN_FILTER_H
