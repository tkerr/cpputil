/******************************************************************************
 * Countdown.h
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
 * Simple countdown timer class designed for the Arduino.
 */
 
#ifndef _COUNTDOWN_H
#define _COUNTDOWN_H

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
 * @class Countdown
 * The Countdown class implements a simple countdown timer.
 *
 * The class is designed for the Arduino and uses elements of the Arduino
 * programming language, specifically the millis() function.
 */
class Countdown
{
public:

    /**
     * @brief
     * Default constructor.
     *
     * Construct a Countdown object with default values.
     */
    Countdown(void);
    
    
    /**
     * @brief
     * Constructor.
     *
     * Construct a Countdown object with the specified timeout period.
     *
     * @param millis The countdown timeout in milliseconds.
     */
    Countdown(uint32_t millis);

    
    /**
     * @brief
     * Set the countdown timeout period.
     *
     * @param millis The countdown timeout in milliseconds.
     */       
    void SetTimeout(uint32_t millis);
    
  
    /**
     * @brief
     * Start the countdown timer.
     *
     * Each call to Start() restarts the countdown timer.
     */        
    void Start(void);

    
    /**
     * @brief
     * Determine if the countdown timer has expired.
     *
     * Note that the timer automatically expires when the Arduino millis()
     * function rolls over.  This happens approximately every 50 days.
     *
     * @return true if the timer has expired, false otherwise.
     */       
    bool IsExpired(void);
    
protected:

private:

    bool     mRunning;    //!< Flag indicating if timer is running
    uint32_t mStartTime;  //!< The millisecond time stamp when the timer was started
    uint32_t mTimeoutMs;  //!< The countdown timeout in milliseconds

};
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/


#endif // _COUNTDOWN_H
