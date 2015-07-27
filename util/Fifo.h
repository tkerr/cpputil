/******************************************************************************
 * Fifo.h
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 07/26/2015 - Tom Kerr
 * Doxygen updates.
 *
 * 07/24/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Implements a simple FIFO that provides first in, first out access to a byte 
 * array.
 */

#ifndef _FIFO_H
#define _FIFO_H

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


/******************************************************************************
 * Public classes.
 ******************************************************************************/

/**
 * @class Fifo
 * This class implements a simple FIFO that provides first in, first out 
 * access to a byte array.
 *
 * This class is designed for use in embedded systems.  The Fifo class does not
 * allocate any memory or make any assumptions about the underlying memory model.  
 * The user must provide the byte array for the FIFO to use.
 */
class Fifo
{
public:

    /**
     * @brief
     * Constructor.  Defines a FIFO with a specified buffer and size.
     *
     * @param buffer The byte array buffer to use for the FIFO.
     * @param size The size of the buffer (and hence, the size of the FIFO).
     */
    Fifo(uint8_t* buffer, uint16_t size);
    
    /**
     * @brief
     * Empties the FIFO by clearing the count and reseting the head and tail pointers.
     */
    void clear(void);

    /**
     * @brief
     * Returns the number of bytes in the FIFO.
     *
     * @return The number of bytes in the FIFO.
     */
    uint16_t count(void);

    /**
     * @brief
     * Returns the number of available (empty) bytes in the FIFO.
     *
     * @return The number of available bytes in the FIFO.
     */
    uint16_t available(void);

    /**
     * @brief
     * Adds bytes to the FIFO at the tail.
     *
     * Bytes are pulled from the source.
     * Count bytes are added to the FIFO as long as the fifo can hold
     * all of the data.
     *
     * @param source Pointer to source data.
     * @param count The number of bytes to add.
     * @return The number of bytes added to the FIFO.
     */
    uint16_t add(const uint8_t* source, uint16_t count);

    /**
     * @brief
     * Removes up to max bytes from the head of the FIFO, placing the bytes at
     * the memory pointed to by the destination.
     *
     * @param dest Destination pointer.
     * @param max The maximum number of bytes to remove from the FIFO.
     * @return The number of bytes removed from the FIFO.
     */
    uint16_t remove(uint8_t* dest, uint16_t max);
    
    /**
     * @brief
     * Retrieves up to max bytes from the FIFO, placing the bytes at
     * the memory pointed to by the destination.  The bytes are not 
     * removed from the FIFO, and the FIFO pointers are not adjusted.
     *
     * @param dest Destination pointer.
     * @param max The maximum number of bytes to retrieve from the FIFO.
     * @return The number of bytes retrieved from the FIFO.
     */
    uint16_t peek(uint8_t* dest, uint16_t max);

protected:

private:
    uint8_t* mBuffer; //!< base of queue memory 
    uint8_t* mHead;   //!< current head of queue 
    uint8_t* mTail;   //!< current tail of queue 
    uint8_t* mEnd;    //!< end of queue memory 
    uint16_t mSize;   //!< number of cells in queue 
    uint16_t mCount;  //!< number of bytes in queue 
};


/******************************************************************************
 * Public functions.
 ******************************************************************************/


#endif // _FIFO_H
