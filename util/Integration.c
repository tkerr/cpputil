/******************************************************************************
 * Integration.c
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
 
/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Integration.h"


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
 * integrate_start
 **************************************/
void integrate_start(ICX_T* picx)
{
    picx->numSamples = 0;
    picx->lastY      = 0;
    picx->sum        = 0;
}


/**************************************
 * integrate_num_samples
 **************************************/
uint16_t integrate_num_samples(const ICX_T* picx)
{
    return picx->numSamples;
}


/**************************************
 * integrate_add_rect
 **************************************/
uint16_t integrate_add_rect(ICX_T* picx, integ_data_t yData)
{
    // Process the previous sample.
    if (picx->numSamples < 2)
    {
        // Initialize the result with the first data point.
        picx->sum = picx->lastY;
    }
    else
    {
        // Add the previous data point to the sum.
        picx->sum += picx->lastY;
    }
    
    // Save the current sample for the next process step.
    // The next process step may occur through a call to either this function
    // or integrate_result_rect().
    picx->lastY = yData;
    
    picx->numSamples++;
    return picx->numSamples;
}


/**************************************
 * integrate_result_rect
 **************************************/
integ_result_t integrate_result_rect(ICX_T* picx)
{
    // Simply return the sum of data values so far.
    // We intentionally ignore lastY here, because it increases the error
    // by theoretically extending the rectangle beyond the bounds of interest.
    return picx->sum;
}


/**************************************
 * integrate_add_trap
 **************************************/
uint16_t integrate_add_trap(ICX_T* picx, integ_data_t yData)
{
    // Process the previous sample.
    if (picx->numSamples < 2)
    {
        // Initialize the result with the first data point.
        picx->sum = picx->lastY;
    }
    else
    {
        // Add the previous data point to the sum.
        picx->sum += (picx->lastY << 1);
    }
    
    // Save the current sample for the next process step.
    // The next process step may occur through a call to either this function
    // or integrate_result_trap().
    picx->lastY = yData;
    
    picx->numSamples++;
    return picx->numSamples;
}


/**************************************
 * integrate_result_trap
 **************************************/
integ_result_t integrate_result_trap(ICX_T* picx)
{
    // Add the last data point.
    picx->sum += picx->lastY;

    return (picx->sum >> 1);
}


/**************************************
 * integrate_add_simp
 **************************************/
uint16_t integrate_add_simp(ICX_T* picx, integ_data_t yData)
{
    int isOdd = picx->numSamples & 0x0001;
    
    // Process the previous sample.
    if (picx->numSamples < 2)
    {
        // Initialize the result with the first data point.
        picx->sum = picx->lastY;
    }
    else
    {       
        if (isOdd)
        {
            // Since we start counting at one (instead of zero), odd-numbered data points are multiplied by 2.
            picx->sum += (picx->lastY << 1);
        }
        else
        {
            // Since we start counting at one (instead of zero), even-numbered data points are multiplied by 4.
            picx->sum += (picx->lastY << 2);
        }
    }
    
    // Save the current sample for the next process step.
    // The next process step may occur through a call to either this function
    // or integrate_result_simp().
    picx->lastY = yData;
    
    picx->numSamples++;
    return picx->numSamples;
}


/**************************************
 * integrate_result_simp
 **************************************/
integ_result_t integrate_result_simp(ICX_T* picx)
{
    // Add the last data point.
    picx->sum += picx->lastY;
    
    return (picx->sum / 3);
}


/*****************************************************************************
 * Private functions.
 ******************************************************************************/


// End of file.
