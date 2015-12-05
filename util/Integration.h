/******************************************************************************
 * Integration.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 11/23/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * One-dimensional numerical integration functions for embedded systems.
 *
 * These functions are designed for performing numerical integration on a 
 * stream of sensor data sampled at regular time intervals.
 */

#ifndef _INTEGRATION_H
#define _INTEGRATION_H

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
 * This defines the data type used for the X and Y values in the numerical 
 * integration operation. 
 * Modify it to suit your application needs.
 */ 
typedef int16_t integ_data_t;


/**
 * @brief
 * This defines the data type used for the intermediate and final result
 * of the numerical integration operation.  It might need to be a "bigger" 
 * data type than integ_data_t.
 * Modify it to suit your application needs.
 */ 
typedef int32_t integ_result_t;


/**
 * @brief
 * Structure holding an integration operation context.
 */
typedef struct _ICX_T
{
    size_t         numSamples;  //!< Number of samples in the integration
    integ_data_t   lastY;       //!< The last Y (data) value added to the integration
    integ_result_t sum;         //!< The current integration sum
} ICX_T;
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 * Initialize an integration operation.
 *
 * Initializes an integration operation.  
 * Call this function to initialize the operation, then successively 
 * call one of the integrate_add_ functions to add data points to the 
 * integration.  Then call the corresponding integrate_result_ function
 * to compute and return the integration result.
 *
 * These integration methods assume that the data points are equally spaced 
 * (e.g., in time) on a uniform grid.
 *
 * Mixing integration methods will produce incorrect results.  For example,
 * once data points are added using integrate_add_trap(), the correct result
 * must be obtained using integrate_result_trap().
 *
 * @param picx Pointer to an integration context structure.
 */
void integrate_start(ICX_T* picx);


/**
 * @brief
 * Convenience function to return the current number of samples in the
 * integration operation.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @return The number of data points added to the integration operation.
 */
uint16_t integrate_num_samples(const ICX_T* picx);


/**
 * @brief
 * Perform an incremental integration step using the Rectangle Method.
 *
 * See integrate_start() for details.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @param yData The current Y data value.
 *
 * @return The number of data points added to the integration operation.
 */
uint16_t integrate_add_rect(ICX_T* picx, integ_data_t yData);


/**
 * @brief
 * Compute and return the integration result using the Rectangle Method.
 *
 * See integrate_start() for details.
 *
 * Calling this function affects the integration sum.  Adding additional
 * data points after calling this function may lead to incorrect results.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @return The current integration result.
 */
integ_result_t integrate_result_rect(ICX_T* picx);


/**
 * @brief
 * Perform an incremental integration step using the Trapezoidal Method.
 *
 * See integrate_start() for details.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @param yData The current Y data value.
 *
 * @return The number of data points added to the integration operation.
 */
uint16_t integrate_add_trap(ICX_T* picx, integ_data_t yData);


/**
 * @brief
 * Compute and return the integration result using the Trapezoidal Method.
 *
 * See integrate_start() for details.
 *
 * Calling this function affects the integration sum.  Adding additional
 * data points after calling this function may lead to incorrect results.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @return The current integration result.
 */
integ_result_t integrate_result_trap(ICX_T* picx);


/**
 * @brief
 * Perform an incremental integration step using Simpson's Rule.
 *
 * See integrate_start() for details.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @param yData The current Y data value.
 *
 * @return The number of data points added to the integration operation.
 */
uint16_t integrate_add_simp(ICX_T* picx, integ_data_t yData);


/**
 * @brief
 * Compute and return the integration result using the Simpson's Rule.
 *
 * Calling this function affects the integration sum.  Adding additional
 * data points after calling this function may lead to incorrect results.
 *
 * See integrate_start() for details.
 *
 * @param picx Pointer to an integration context structure.
 *
 * @return The current integration result.
 */
integ_result_t integrate_result_simp(ICX_T* picx);


#ifdef __cplusplus
}
#endif

#endif // _INTEGRATION_H
