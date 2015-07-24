/******************************************************************************
 * Fifo.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
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
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Fifo.h"


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
 * Public methods.
 ******************************************************************************/

 
/**************************************
 * Fifo::Fifo
 **************************************/
Fifo::Fifo(uint8_t* buffer, uint16_t size)
{
    mBuffer = buffer;
    mEnd    = buffer + size;
    mSize   = size;
    mHead   = buffer;
    mTail   = buffer;
    mCount  = 0;
}


/**************************************
 * Fifo::clear
 **************************************/
void Fifo::clear(void)
{
    mHead = mBuffer;
    mTail = mBuffer;
    mCount = 0;
}


/**************************************
 * Fifo::count
 **************************************/
uint16_t Fifo::count(void)
{
    return mCount;
}


/**************************************
 * Fifo::available
 **************************************/
uint16_t Fifo::available(void)
{
    return mSize - mCount;
}


/**************************************
 * Fifo::add
 **************************************/
uint16_t Fifo::add(const uint8_t* source, uint16_t count)
{
    uint16_t numadded;

    // limit the number to add to the available space
    if (count > available())
    {
        count = available();
    }
    numadded = count;
    mCount += count;
    for (; count > 0; count--)
    {
        *mTail++ = *source++;  // enqueue the byte
        if (mTail == mEnd)   // update tail with wrap
        {
            mTail = mBuffer;
        }
    }
    return numadded;
}


/**************************************
 * Fifo::remove
 **************************************/
uint16_t Fifo::remove(uint8_t* dest, uint16_t max)
{
    uint16_t numremoved;

    // limit the number removed to the number in the fifo 
    if (max > mCount)
    {
        max = mCount;
    }

    numremoved = max;
    mCount -= numremoved;
    for (; max > 0; max--)
    {
        *dest++ = *mHead++;    // dequeue the byte 
        if (mHead == mEnd)     // update head with wrap 
        {
            mHead = mBuffer;
        }
    }
    return numremoved;
}


/**************************************
 * Fifo::peek
 **************************************/
uint16_t Fifo::peek(uint8_t* dest, uint16_t max)
{
    uint16_t numretrieved;
    uint8_t* localHead = mHead;  // Do not modify FIFO pointers

    // Limit the number retrieved to the number in the fifo.
    if (max > mCount)
    {
        max = mCount;
    }

    numretrieved = max;
    
    for (; max > 0; max--)
    {
        *dest++ = *localHead++;    // retrieve the byte
        if (localHead == mEnd)     // update local head with wrap
        {
            localHead = mBuffer;
        }
    }
    return numretrieved;
}


/******************************************************************************
 * Protected methods.
 ******************************************************************************/

 
/******************************************************************************
 * Private methods.
 ******************************************************************************/
 
 
// End of file.

