/******************************************************************************
 * Queue.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/15/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * The Queue module implements a generic first-in, first-out collection of 
 * data elements.
 *
 * The queue can be configured as a traditional queue or as a moving window
 * buffer by clearing or setting the overwrite flag.  As a traditional queue, 
 * no further data can be added when it is full.  As a moving window, new data 
 * replaces the oldest data when the queue is full.
 *
 * Note that appropriate locking mechanisms must be used if these functions are
 * used in interrupt service routines or by multiple threads.  The functions do
 * not disable interrupts or use mutexes for thread safe access.
 *
 * QUEUE_Enqueue(), QUEUE_Dequeue(), and QUEUE_Peek() copy data to/from the queue.
 *
 * QUEUE_EnqueuePtr(), QUEUE_DequeuePtr(), and QUEUE_PeekPtr() return pointers
 * to queued data elements were data can be copied or manipulated directly.
 * These functions are intended for embedded systems with extreme memory 
 * limitations or for applications that need to avoid the latency involved in 
 * copying large data elements to/from the queue.
 */

#ifndef _QUEUE_H
#define _QUEUE_H

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <stdlib.h>


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
typedef struct _QUEUE
{
    char* base;       //!< Base of queue memory
    char* end;        //!< Pointer to the end of queue memory
    char* head;       //!< Pointer to queue head; data elements are removed from here
    char* tail;       //!< Pointer to queue tail; data elements are added here
    int   num;        //!< The number of data elements that the queue can hold
    int   size;       //!< The size of each data element in bytes
    int   count;      //!< Number of data elements currently enqueued
    int   overwrite;  //!< Overwrite flag; non-zero allows oldest data to be overwritten
} QUEUE;
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief
 * Prepares a QUEUE structure by initializing the supplied structure and 
 * buffer as an empty queue.
 *
 * Further references to the queue are made through this structure.
 *
 * @param queue Pointer to the QUEUE structure that defines the queue
 * 
 * @param buffer The buffer to use for the queue.  Organized as a
 * contiguous array of data elements.
 * 
 * @param num The number of data elements in the buffer
 *
 * @param size The size of each data element in bytes
 *
 * @param overwrite Overwrite flag.  If zero, then data elements are not 
 * overwritten, and attempts to enqueue a data element into a full queue 
 * will fail.  If nonzero, then the oldest data elements are overwritten
 * when data is enqueued into a full queue.
 */
void QUEUE_Define(QUEUE* queue, void* buffer, int num, int size, int overwrite);


/**
 * @brief
 * Returns the number of available (empty) data elements in the queue.
 *
 * @param queue Pointer to the queue
 *
 * @return The number of available data elements.
 */
int QUEUE_Available(const QUEUE* queue);

/**
 * @brief
 * Clears all objects from the queue.
 *
 * @param queue Pointer to the queue
 */
void QUEUE_Clear(QUEUE* queue);

/**
 * @brief
 * Returns the number of data elements in the queue.
 *
 * @param queue Pointer to the queue
 *
 * @return The number of data elements in the queue.
 */
int QUEUE_Count(const QUEUE* queue);

/**
 * @brief
 * Remove the oldest data element from the queue and return it.
 *
 * @param queue Pointer to the queue
 *
 * @param pData Pointer to a location to receive the data
 *
 * @return The number of data elements dequeued (0 or 1).  Returns 0 if
 * the queue is empty.
 */
int QUEUE_Dequeue(QUEUE* queue, void* pData);

/**
 * @brief
 * Remove and discard the oldest data element from the queue.
 *
 * Intended to be used in conjunction with QUEUE_EnqueuePtr() and QUEUE_PeekPtr().
 * These two functions manipulate the data elements directly in queue memory.
 * When finished with the queue element, call QUEUE_DequeuePtr() to remove
 * and discard it.
 *
 * @param queue Pointer to the queue
 *
 * @return The number of data elements dequeued (0 or 1).  Returns 0 if
 * the queue is empty.
 */
int QUEUE_DequeuePtr(QUEUE* queue);

/**
 * @brief
 * Add a data element to the queue.
 *
 * @param queue Pointer to the queue
 *
 * @param pData Pointer to the data element to add.
 *
 * @return The number of data elements enqueued (0 or 1).  Returns 0 if
 * the queue is full and overwriting is disabled.
 */
int QUEUE_Enqueue(QUEUE* queue, const void* pData);

/**
 * @brief
 * Allocate a data element in the queue and return a pointer to it.
 *
 * Intended to be used in conjunction with QUEUE_PeekPtr() and QUEUE_DequeuePtr().
 * Use this function to allocate a data element in queue memory.  Once allocated,
 * the data element can be manipulated directly via the returned pointer.
 *
 * @param queue Pointer to the queue
 *
 * @return A pointer to the allocated queue element, or NULL if the queue is full.
 */
void* QUEUE_EnqueuePtr(QUEUE* queue);

/**
 * @brief
 * Return a data element from the queue without removing it.
 *
 * @param queue Pointer to the queue
 *
 * @param index The index of the data element to return in the range 
 * 0 to (QUEUE_Count() - 1). Index zero always points to the most recent data 
 * element enqueued.
 *
 * @param pData Pointer to a location to receive the data.  A data element is 
 * only returned if the index is valid.
 *
 * @return The number of objects returned (0 or 1).
 */
int QUEUE_Peek(const QUEUE* queue, int index, void* pData);

/**
 * @brief
 * Return a pointer to an existing data element in the queue without removing it.
 *
 * Intended to be used in conjunction with QUEUE_EnqueuePtr and QUEUE_DequeuePtr().
 * Use this function to get a pointer to a specified data element in queue memory.
 * The data element can be manipulated directly via the returned pointer.
 *
 * @param queue Pointer to the queue
 *
 * @param index The index of the data element to return in the range 
 * 0 to (QUEUE_Count() - 1). Index zero always points to the most recent data 
 * element enqueued.
 *
 * @return A pointer to the specified queue element, or NULL if the index is invalid.
 */
void* QUEUE_PeekPtr(const QUEUE* queue, int index);

#ifdef __cplusplus
}
#endif

#endif // _QUEUE_H
