/******************************************************************************
 * Queue.c
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
 
/******************************************************************************
 * System include files.
 ******************************************************************************/


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Queue.h"


/******************************************************************************
 * Forward references.
 ******************************************************************************/
/**
 * @brief
 * Fast copy function that assumes no data overlap.
 */
static void QUEUE_Copy(char* dst, const char* src, size_t len);


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
 * QUEUE_Define
 **************************************/
void QUEUE_Define(QUEUE* queue, void* buffer, int num, int size, int overwrite)
{
    // Convert pointers to char* so we can do pointer arithmetic.
    // Some compilers allow void pointer arithmetic, while others do not.
    queue->base      = (char*)buffer;
    queue->end       = (char*)buffer + (num * size);
    queue->head      = (char*)buffer;
    queue->tail      = (char*)buffer;
    queue->num       = num;
    queue->size      = size;
    queue->count     = 0;
    queue->overwrite = overwrite;
}


/**************************************
 * QUEUE_Available
 **************************************/
int QUEUE_Available(const QUEUE* queue)
{
    return (queue->num - queue->count);
}


/**************************************
 * QUEUE_Clear
 **************************************/
void QUEUE_Clear(QUEUE* queue)
{
    queue->head  = queue->base;
    queue->tail  = queue->base;
    queue->count = 0;
}


/**************************************
 * QUEUE_Count
 **************************************/
int QUEUE_Count(const QUEUE* queue)
{
    return queue->count;
}


/**************************************
 * QUEUE_Dequeue
 **************************************/
int QUEUE_Dequeue(QUEUE* queue, void* pData)
{
    int removed = 0;
    
    // Remove data element from head.
    if (queue->count > 0)
    {
        QUEUE_Copy((char*)pData, queue->head, queue->size);
        queue->head += queue->size;
        if (queue->head >= queue->end) queue->head = queue->base;
        queue->count--;
        removed = 1;
    }
    
    return removed;
}


/**************************************
 * QUEUE_Enqueue
 **************************************/
int QUEUE_Enqueue(QUEUE* queue, const void* pData)
{
    int added = 0;
    
    // Dequeue oldest data element if queue full and overwrite flag set.
    if (queue->overwrite && (queue->count == queue->num))
    {
        queue->head += queue->size;
        if (queue->head >= queue->end) queue->head = queue->base;
        queue->count--;
    }
    
    // Add data element to tail.
    if (queue->count < queue->num)
    {
        QUEUE_Copy(queue->tail, (char*)pData, queue->size);
        queue->tail += queue->size;
        if (queue->tail >= queue->end) queue->tail = queue->base;
        queue->count++;
        added = 1;
    }
    
    return added;
}


/**************************************
 * QUEUE_Peek
 **************************************/
int QUEUE_Peek(const QUEUE* queue, int index, void* pData)
{
    int copied = 0;
    char* ptr;
    
    if ((queue->count > 0) && (index < queue->count))
    {
        ptr = queue->head + (index * queue->size);
        if (ptr >= queue->end) ptr -= (queue->end - queue->base);
        QUEUE_Copy((char*)pData, ptr, queue->size);
        copied = 1;
    }
    
    return copied;
}


/**************************************
 * QUEUE_DequeuePtr
 **************************************/
int QUEUE_DequeuePtr(QUEUE* queue)
{
    int removed = 0;
    
    // Remove data element from head.
    if (queue->count > 0)
    {
        queue->head += queue->size;
        if (queue->head >= queue->end) queue->head = queue->base;
        queue->count--;
        removed = 1;
    }
    
    return removed;
}


/**************************************
 * QUEUE_EnqueuePtr
 **************************************/
void* QUEUE_EnqueuePtr(QUEUE* queue)
{
    char* ptr = NULL;
    
    // Dequeue oldest data element if queue full and overwrite flag set.
    if (queue->overwrite && (queue->count == queue->num))
    {
        queue->head += queue->size;
        if (queue->head >= queue->end) queue->head = queue->base;
        queue->count--;
    }
    
    // Allocate data element to tail.
    if (queue->count < queue->num)
    {
        ptr = queue->tail;
        queue->tail += queue->size;
        if (queue->tail >= queue->end) queue->tail = queue->base;
        queue->count++;
    }
    
    // Return pointer to allocated data element.
    return (void*)ptr;
}


/**************************************
 * QUEUE_PeekPtr
 **************************************/
void* QUEUE_PeekPtr(const QUEUE* queue, int index)
{
    char* ptr = NULL;
    
    if ((queue->count > 0) && (index < queue->count))
    {
        ptr = queue->head + (index * queue->size);
        if (ptr >= queue->end) ptr -= (queue->end - queue->base);
    }
    
    // Return pointer to data element.
    return (void*)ptr;
}


/******************************************************************************
 * Private functions.
 ******************************************************************************/
 
/**************************************
 * QUEUE_Copy
 **************************************/ 
static void QUEUE_Copy(char* dst, const char* src, size_t len)
{
    do {*dst++ = *src++;} while(--len);
}

// End of file.
