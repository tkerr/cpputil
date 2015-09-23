/******************************************************************************
 * Queue16.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/12/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * The Queue16 module implements a first-in, first-out collection of unsigned
 * 16-bit integers.  Intended for sensor data collection in embedded systems.
 *
 * The queue can be configured as a traditional queue or as a moving window
 * buffer by clearing or setting the overwrite flag.  As a traditional queue, 
 * no further data can be added when it is full.  As a moving window, new data 
 * replaces the oldest data when the queue is full.
 *
 * Note that appropriate locking mechanisms must be used if these functions are
 * used in interrupt service routines or by multiple threads.  The functions do
 * not disable interrupts or use mutexes for thread safe access.
 */
 
/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdint.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Queue16.h"


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
 * QUEUE16_Define
 **************************************/
void QUEUE16_Define(QUEUE16* queue, uint16_t* buffer, uint8_t size, uint8_t overwrite)
{
    queue->base      = buffer;
    queue->size      = size;
    queue->overwrite = overwrite;
    queue->head      = 0;
    queue->tail      = 0;
    queue->count     = 0;
}


/**************************************
 * QUEUE16_Available
 **************************************/
uint8_t QUEUE16_Available(const QUEUE16* queue)
{
    return (queue->size - queue->count);
}


/**************************************
 * QUEUE16_Clear
 **************************************/
void QUEUE16_Clear(QUEUE16* queue)
{
    queue->head  = 0;
    queue->tail  = 0;
    queue->count = 0;
}


/**************************************
 * QUEUE16_Count
 **************************************/
uint8_t QUEUE16_Count(const QUEUE16* queue)
{
    return queue->count;
}


/**************************************
 * QUEUE16_Dequeue
 **************************************/
int QUEUE16_Dequeue(QUEUE16* queue, uint16_t* pData)
{
    int removed = 0;
    
    // Remove element from head.
    if (queue->count > 0)
    {
        *pData = queue->base[queue->head];
        queue->head++;
        if (queue->head >= queue->size) queue->head -= queue->size;
        queue->count--;
        removed = 1;
    }
    
    return removed;
}


/**************************************
 * QUEUE16_Enqueue
 **************************************/
int QUEUE16_Enqueue(QUEUE16* queue, uint16_t data)
{
    int added = 0;
    
    // Dequeue oldest data element if queue full and overwrite flag set.
    if (queue->overwrite && (queue->count == queue->size))
    {
        queue->head++;
        if (queue->head >= queue->size) queue->head -= queue->size;
        queue->count--;
    }
    
    // Add element to tail.
    if (queue->count < queue->size)
    {
        queue->base[queue->tail] = data;
        queue->tail++;
        if (queue->tail >= queue->size) queue->tail -= queue->size;
        queue->count++;
        added = 1;
    }
    
    return added;
}


/**************************************
 * QUEUE16_Peek
 **************************************/
int QUEUE16_Peek(const QUEUE16* queue, uint8_t index, uint16_t* pData)
{
    int copied = 0;
    uint8_t offset;
    
    if ((queue->count > 0) && (index < queue->count))
    {
        offset = queue->head + index;
        if (offset >= queue->size) offset -= queue->size;
        *pData = queue->base[offset];
        copied = 1;
    }
    
    return copied;
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/


// End of file.
