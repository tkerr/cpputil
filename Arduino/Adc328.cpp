/******************************************************************************
 * Adc328.cpp
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 09/12/2015 - Tom Kerr
 * Refactored for better performance.
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
 * Analog-to-digital converter (ADC) module for the Arduino Uno, based on the 
 * Atmel AVR ATmega328p.  Provides direct control of the ADC.
 */
 
/******************************************************************************
 * Lint options.
 ******************************************************************************/
 

/******************************************************************************
 * System include files.
 ******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Adc328.h"
 
 
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
 * Adc328::Adc328
 * Default constructor.
 **************************************/
Adc328::Adc328()
{

}


/**************************************
 * Adc328::begin
 **************************************/
void Adc328::begin(Adc328Ref ref, Adc328Channel channel)
{
    // Turn off interrupts.
    volatile uint8_t savedSREG = SREG;
    cli();
    
    // Disable ADC power reduction.
    PRR &= ~(1 << PRADC);
    
    // ADC setup.
    ADCSRA = 0x07;    // Auto trigger disable, clear ADIF & ADIE, DIV128
    ADCSRB = 0x00;
    DIDR0  = 0x0F;    // Disable digital inputs on ADC channels AD0 - AD3
    setReference(ref);
    setChannel(channel);
    ADCSRA |= 0x80;   // Enable ADC
    
    // Restore interrupt state.
    SREG = savedSREG;
}


/**************************************
 * Adc328::setChannel
 **************************************/
void Adc328::setChannel(Adc328Channel channel)
{
    uint8_t mask = 0x0F;                // Mask of ADC channel bits
    uint8_t tmpADMUX = ADMUX & ~mask;   // Strip current channel
    tmpADMUX |= (channel & mask);       // Add new channel
    ADMUX = tmpADMUX;
}


/**************************************
 * Adc328::setReference
 **************************************/
void Adc328::setReference(Adc328Ref ref)
{   
    uint8_t mask = 0xC0;                // Mask of ADC reference bits
    uint8_t tmpADMUX = ADMUX & ~mask;   // Strip current reference
    tmpADMUX |= (ref & mask);           // Add new reference
    ADMUX = tmpADMUX;
}


/**************************************
 * Adc328::convert
 **************************************/
uint16_t Adc328::convert(void)
{
    ADCSRA |= 0x40;                        // Start the conversion
    while ((ADCSRA & 0x40) != 0) {}        // Wait for conversion to complete
    uint16_t result = ADCL | (ADCH << 8);  // Get the result
    return result;
}


/**************************************
 * Adc328::medianConvert
 **************************************/
uint16_t Adc328::medianConvert()
{
    uint16_t adcArray[ADC328_FILTER_SIZE];  // Array of ADC values for median filter
        
    // Initialize an array of ADC values to the maximum value.
    for (int i = 0; i < ADC328_FILTER_SIZE; i++)
    {
        adcArray[i] = 0xFFFF;
    }
    
    // Sample and filter the ADC channel.
    for (int i = 0; i < ADC328_FILTER_SIZE; i++)
    {
        uint16_t val = convert();  // Perform a conversion
    
        // Perform an insertion sort into the data array.
        // Data is sorted from smallest to largest value.
        for (int j = 0; j < ADC328_FILTER_SIZE; j++)
        {
            if (val < adcArray[j])
            {
                // Shift the larger values to the next higher index.
                for (int k = ADC328_FILTER_SIZE - 1; k > j; k--)
                {
                    adcArray[k] = adcArray[k-1];
                }
                
                // Insert the current ADC conversion.
                adcArray[j] = val;
                break;
           }
        }
    }

    // Return the median value.
    return adcArray[ADC328_FILTER_SIZE/2];
}


/******************************************************************************
 * Protected methods.
 ******************************************************************************/
 
 
/******************************************************************************
 * Private methods.
 ******************************************************************************/
 

/******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/
 
/**
 * @brief
 * ADC interrupt service routine.
 *
 * Not currently used.
 */ 
ISR(ADC_vect)
{

}


/******************************************************************************
 * Global objects.
 ******************************************************************************/
 
/**
 * @brief
 * Define an Adc328 object.  
 * Used as a poor man's singleton.
 */
Adc328 myAdc;


// End of file.
