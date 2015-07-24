/******************************************************************************
 * Countdown.cpp
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
 * Simple countdown timer class based on the Arduino millis() function.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include "Arduino.h"


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Countdown.h"
 
 
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
 * Countdown::Countdown
 **************************************/
Countdown::Countdown(void)
{
    mRunning = false;
    mStartTime = 0;
    mTimeoutMs = 0;
}


/**************************************
 * Countdown::Countdown
 **************************************/
Countdown::Countdown(uint32_t millis)
{
    mRunning = false;
    mStartTime = 0;
    mTimeoutMs = millis;
}


/**************************************
 * Countdown::SetTimeout
 **************************************/
void Countdown::SetTimeout(uint32_t millis)
{
    mTimeoutMs = millis;
}


/**************************************
 * Countdown::Start
 **************************************/
void Countdown::Start(void)
{
    mStartTime = millis();
    mRunning = true;
}


/**************************************
 * Countdown::IsExpired
 **************************************/
bool Countdown::IsExpired(void)
{
    // Return true if timer is not running.
    if (!mRunning)
    {
        return true;
    }
    
    bool expired = false;
    uint32_t now = millis();
    
    if (now >= mStartTime)
    {
        // See if timeout period has been met or exceeded.
        if ((now - mStartTime) >= mTimeoutMs)
        {
            expired = true;
        }
    }
    else
    {
        // millis() function has rolled over.  Automatically expire the timer.
        expired = true;
    }
    
    // Timer is expired.  Reset parameters.
    if (expired)
    {
        mRunning = false;
        mStartTime = 0;
    }
    
    return expired;
}


/******************************************************************************
 * Protected methods.
 ******************************************************************************/

 
/******************************************************************************
 * Private methods.
 ******************************************************************************/
 

// End of file.
