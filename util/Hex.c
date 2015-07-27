/******************************************************************************
 * Hex.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 07/26/2015 - Tom Kerr
 * Created.
 ******************************************************************************/

/**
 * @file
 * @brief
 * Binary <-> hex conversion functions.
 *
 * Designed for little-endian CPUs including Atmel 8-bit AVR and Microchip PIC.
 * May not work on big-endian CPUs such as AVR32 and Freescale.
 */
 
/******************************************************************************
 * System include files.
 ******************************************************************************/


/******************************************************************************
 * Local include files.
 ******************************************************************************/
#include "Hex.h"
#include "UnionTypeDefs.h"


/******************************************************************************
 * Forward references.
 ******************************************************************************/
static uint64_t HEX_doConvert(const char* hex, uint8_t len);  //!< Perform conversion
static int      HEX_strlen(const char* str);  //!< Local implementation of strlen()
static uint8_t  HEX_toBin(char c);            //!< Convert one hex digit to binary


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define BIN2ASC(x) if (x < 10) {x += '0';} else {x = (x - 10) + 'A';}


/******************************************************************************
 * Local data.
 ******************************************************************************/
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/


/**************************************
 * HEX_BinaryToHex
 **************************************/
int HEX_BinaryToHex(uint64_t bin, char* ascii, int minSize)
{
    int size = 0;                     // Size of formatted string
    int pos = sizeof(uint64_t) * 2;   // Hex digit position
    int hasDigits = 0;
    int i;
    uint64u_t qwBin;
    
    qwBin.v = bin;
    
    if (minSize < 1) minSize = 1;
    
    for (i = sizeof(uint64_t)-1; i >= 0; i--)
    {
        char c = (qwBin.b[i] & 0xF0) >> 4;
        if ((c > 0) || hasDigits || (pos <= minSize))
        {
            BIN2ASC(c)
            ascii[size++] = c;
            hasDigits = 1;
        }
        pos--;
        
        c = (qwBin.b[i] & 0x0F);
        if ((c > 0) || hasDigits || (pos <= minSize))
        {
            BIN2ASC(c)
            ascii[size++] = c;
            hasDigits = 1;
        }
        pos--;
        
        ascii[size] = '\0';
    }

    return size;
}


/**************************************
 * HEX_HexToUint8
 **************************************/
uint8_t HEX_HexToUint8(const char* hex)
{
    uint8_t bin = 0;
    uint8_t len = HEX_strlen(hex);
    if (len <= (sizeof(bin) * 2))
    {
       bin = (uint8_t) HEX_doConvert(hex, len);
    }
    return bin;
}


/**************************************
 * HEX_HexToUint16
 **************************************/
uint16_t HEX_HexToUint16(const char* hex)
{
    uint16_t bin = 0;
    uint8_t len = HEX_strlen(hex);
    if (len <= (sizeof(bin) * 2))
    {
        bin = (uint16_t) HEX_doConvert(hex, len);
    }
    return bin;
}


/**************************************
 * HEX_HexToUint32
 **************************************/
uint32_t HEX_HexToUint32(const char* hex)
{
    uint32_t bin = 0;
    uint8_t len = HEX_strlen(hex);
    if (len <= (sizeof(bin) * 2))
    {
        bin = (uint32_t) HEX_doConvert(hex, len);
    }
    return bin;
}


/**************************************
 * HEX_HexToUint64
 **************************************/
uint64_t HEX_HexToUint64(const char* hex)
{
    uint64_t bin = 0;
    uint8_t len = HEX_strlen(hex);
    if (len <= (sizeof(bin) * 2))
    {
        bin = HEX_doConvert(hex, len);
    }
    return bin;
}
 
 
/******************************************************************************
 * Private functions.
 ******************************************************************************/

/**************************************
 * HEX_doConvert
 **************************************/  
static uint64_t HEX_doConvert(const char* hex, uint8_t len)
{
    uint64_t bin = 0;
    uint8_t  shift = 0;
    int i;
    for (i=len-1; i >= 0; i--)
    {
        uint8_t digit = HEX_toBin(hex[i]);
        if (digit < 0x10)
        {
            bin += ((uint64_t)digit << shift);
            shift += 4;
        }
        else
        {
            // Exit on the first non-hex digit.
            break;
        }
    }

    return bin;
}


/**************************************
 * HEX_strlen
 **************************************/
static int HEX_strlen(const char* str)
{
    register const char *s;
    for (s = str; *s; ++s);
    return(s - str);
}


/**************************************
 * HEX_toBin
 **************************************/
static uint8_t HEX_toBin(char c)
{
    // Initialize to an illegal value. 
    // If an invalid hex digit is passed, we return the illegal value.
    uint8_t bin = 0xFF;
    
    if ((c >= '0') && (c <= '9'))
    {
        bin = (uint8_t)(c - '0');
    }
    else if ((c >= 'A') && (c <= 'F'))
    {
        bin = (uint8_t)((c - 'A') + 10);
    }
    else if ((c >= 'a') && (c <= 'f'))
    {
        bin = (uint8_t)((c - 'a') + 10);
    }
    return bin;
}


// End of file.
