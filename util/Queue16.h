/******************************************************************************
 * Queue16.h
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

#ifndef _QUEUE16_H
#define _QUEUE16_H

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
 * The queue structure that defines a specific queue. 
 */
typedef struct _QUEUE16
{
    uint16_t* base;       //!< Base of queue memory
    uint8_t   head;       //!< Index of queue head; data elements are removed from here
    uint8_t   tail;       //!< Index of queue tail; data elements are added here
    uint8_t   size;       //!< The number of data elements that the queue can hold
    uint8_t   count;      //!< Number of data elements currently enqueued
    uint8_t   overwrite;  //!< Overwrite flag; non-zero allows oldest data to be overwritten
} QUEUE16;
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief
 * Prepares a QUEUE16 structure by initializing the supplied structure and 
 * buffer as an empty queue.
 *
 * Further references to the queue are made through this structure.
 *
 * @param queue Pointer to the QUEUE16 structure that defines the queue
 * 
 * @param buffer The buffer to use for the queue.  Organized as an array 
 * of uint16_t elements.
 * 
 * @param size The number of uint16_t elements in the buffer.
 *
 * @param overwrite Overwrite flag.  If zero, then data elements are not 
 * overwritten, and attempts to enqueue a data element into a full queue 
 * will fail.  If nonzero, then the oldest data elements are overwritten
 * when data is enqueued into a full queue.
 */
void QUEUE16_Define(QUEUE16* queue, uint16_t* buffer, uint8_t size, uint8_t overwrite);


/**
 * @brief
 * Returns the number of available (empty) data elements in the queue.
 *
 * @param queue Pointer to the queue
 *
 * @return The number of available data elements.
 */
uint8_t QUEUE16_Available(const QUEUE16* queue);

/**
 * @brief
 * Clears all objects from the queue.
 *
 * @param queue Pointer to the queue
 */
void QUEUE16_Clear(QUEUE16* queue);

/**
 * @brief
 * Returns the number of data elements in the queue.
 *
 * @param queue Pointer to the queue
 *
 * @return The number of data elements in the queue.
 */
uint8_t QUEUE16_Count(const QUEUE16* queue);

/**
 * @brief
 * Remove the oldest data element from the queue and return it.
 *
 * @param queue Pointer to the queue
 *
 * @param pData Pointer to a variable to receive the data element.
 *
 * @return The number of objects dequeued (0 or 1).  Returns 0 if
 * the queue is empty.
 */
int QUEUE16_Dequeue(QUEUE16* queue, uint16_t* pData);

/**
 * @brief
 * Add a data element to the queue.
 *
 * @param queue Pointer to the queue
 *
 * @param data The data object to add.
 *
 * @return The number of objects enqueued (0 or 1).  Returns 0 if
 * the queue is full and overwriting is disabled.
 */
int QUEUE16_Enqueue(QUEUE16* queue, uint16_t data);

/**
 * @brief
 * Return a data element from the queue without removing it.
 *
 * @param queue Pointer to the queue
 *
 * @param index The index of the data element to return in the range 
 * 0 to (QUEUE16_Count() - 1). Index zero always points to the most recent data 
 * element enqueued.
 *
 * @param pData Pointer to a location to receive the data.  A data element is 
 * only returned if the index is valid.
 *
 * @return The number of objects returned (0 or 1).
 */
int QUEUE16_Peek(const QUEUE16* queue, uint8_t index, uint16_t* pData);

#ifdef __cplusplus
}
#endif

#endif // _QUEUE16_H
