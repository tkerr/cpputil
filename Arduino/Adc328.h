/******************************************************************************
 * Adc328.h
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
 * Analog-to-digital converter (ADC) class for the Atmel AVR ATmega328p 
 * processor. Provides direct control of the ADC.  Conversion results are
 * filtered through a median filter.
 */
 
#ifndef _ADC328_H
#define _ADC328_H

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

// ADC resolution.
#define ADC328_BITS   10
#define ADC328_MAX  1023

// ADC median filter size.  Should be an odd value.
#define ADC328_FILTER_SIZE 7

/**
 * @brief
 * ADC voltage reference selection bit masks.
 */ 
enum Adc328Ref
{
    REF_AREF = 0x00,  //!< AREF, Internal Vref turned off
    REF_AVCC = 0x40,  //!< AVcc
    REF_1V1  = 0xC0   //!< Internal 1.1V voltage reference
};

/**
 * @brief
 * ADC channel selection bit masks.
 */ 
enum Adc328Channel
{
    CHANNEL_ADC0 = 0x00,  //!< Channel 0
    CHANNEL_ADC1 = 0x01,  //!< Channel 1
    CHANNEL_ADC2 = 0x02,  //!< Channel 2
    CHANNEL_ADC3 = 0x03,  //!< Channel 3
    CHANNEL_TMP  = 0x08,  //!< Internal temperature sensor
    CHANNEL_1V1  = 0x0E,  //!< Internal 1.1V reference
    CHANNEL_GND  = 0x0F   //!< Ground
};


/******************************************************************************
 * Public functions.
 ******************************************************************************/

 
/******************************************************************************
 * Public classes.
 ******************************************************************************/

/**
 * @class Adc328
 * Analog-to-digital converter (ADC) class for the Atmel AVR ATmega328p 
 * processor. Provides direct control of the ADC. Conversion results are
 * filtered through a median filter.
 */
class Adc328
{
    
public:

    /**
     * @brief
     * Default constructor.  Currently not used.
     */
    Adc328();
    
    /**
     * @brief
     * Initialize the Adc328 object.
     *
     * @param ref The voltage reference to select, defined by the Adc328Ref
     * enumerated type.  Default is REF_AVCC.
     */
    void begin(Adc328Ref ref = REF_AVCC);
    
    /**
     * @brief
     * Set the ADC voltage reference.
     *
     * NOTE: Some settling time may be needed after changing the reference before a
     * valid ADC conversion can be obtained.  Sometimes this may be as high as 10ms.
     *
     * @param ref The voltage reference to select, defined by the Adc328Ref
     * enumerated type.
     */
    void setReference(Adc328Ref ref);
    
    /**
     * @brief
     * Perform a single ADC conversion and return the result.
     *
     * This method blocks while the conversion is performed.  Interrupts are
     * not used.
     *
     * @param channel The input channel to convert, defined by the Adc328Channel
     * enumerated type.
     *
     * @return The 10-bit ADC conversion result.
     */
    uint16_t convert(Adc328Channel channel);
    
    /**
     * @brief
     * Perform a series of ADC conversions and return the median filtered result.
     *
     * The number of samples used by the median filter is defined by the 
     * ADC328_FILTER_SIZE constant.
     *
     * This method blocks while the conversions and filtering are performed.  
     * Interrupts are not used.
     *
     * @param channel The input channel to convert, defined by the Adc328Channel
     * enumerated type.
     *
     * @return The 10-bit median filtered, ADC conversion result.
     */
    uint16_t medianConvert(Adc328Channel channel);
    
protected:

private:

    /**
     * @brief
     * Set the ADC channel used for conversion.
     *
     * @param channel The channel to select, defined by the Adc328Channel
     * enumerated type.
     */
    void setChannel(Adc328Channel channel); 
    
    /**
     * @brief
     * Perform a single ADC conversion and return the result.
     *
     * This is the common method that performs all ADC conversions.
     * The channel is first set by setChannel().
     *
     * This method blocks while the conversion is performed.  Interrupts are
     * not used.
     *
     * @return The 10-bit ADC conversion result.
     */
    uint16_t doConvert(void);
    
};


/******************************************************************************
 * External references.
 ******************************************************************************/
 
/**
 * @brief
 * Define an external reference to an Adc328 object.  
 * Used as a poor man's singleton.
 */
extern Adc328 myAdc;

 
#endif // _ADC328_H
