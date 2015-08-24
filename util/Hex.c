/******************************************************************************
 * Hex.c
 * Copyright (c) 2015 Thomas Kerr
 *
 * Released under the MIT License (MIT). 
 * See http://opensource.org/licenses/MIT
 *
 * Modification History:
 *
 * 08/23/2015 - Tom Kerr
 * Refactored the binary to hex conversion functions.
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
static void     HEX_byteToHex(uint8_t bin, char* hex);  //!< Convert one byte to hex
static uint64_t HEX_doConvert(const char* hex, uint8_t len);  //!< Perform conversion
static int      HEX_strlen(const char* str);  //!< Local implementation of strlen()
static uint8_t  HEX_toBin(char c);            //!< Convert one hex digit to binary


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
#define BIN2ASC(a, b) b = (a < 10) ? a + '0' : (a - 10) + 'A';


/******************************************************************************
 * Local data.
 ******************************************************************************/
 
 
/******************************************************************************
 * Public functions.
 ******************************************************************************/


/**************************************
 * HEX_Uint8ToHex
 **************************************/
char* HEX_Uint8ToHex(uint8_t bin, char* hex)
{
    HEX_byteToHex(bin, hex);
    hex[2] = '\0';
    return hex;
}


/**************************************
 * HEX_Uint16ToHex
 **************************************/
char* HEX_Uint16ToHex(uint16_t bin, char* hex)
{
    uint16u_t uBin;
    uint8_t i;
    
    uBin.v = bin;
    
    for (i=0; i < sizeof(uint16_t); i++)
    {
        HEX_byteToHex(uBin.b[(sizeof(uint16_t) - 1) - i], &hex[i<<1]);
    }
    hex[2*sizeof(uint16_t)] = '\0';
    return hex;
}


/**************************************
 * HEX_Uint32ToHex
 **************************************/
char* HEX_Uint32ToHex(uint32_t bin, char* hex)
{
    uint32u_t uBin;
    uint8_t i;
    
    uBin.v = bin;
    
    for (i=0; i < sizeof(uint32_t); i++)
    {
        HEX_byteToHex(uBin.b[(sizeof(uint32_t) - 1) - i], &hex[i<<1]);
    }
    hex[2*sizeof(uint32_t)] = '\0';
    return hex;
}


/**************************************
 * HEX_Uint64ToHex
 **************************************/
char* HEX_Uint64ToHex(uint64_t bin, char* hex)
{
    uint64u_t uBin;
    uint8_t i;
    
    uBin.v = bin;
    
    for (i=0; i < sizeof(uint64_t); i++)
    {
        HEX_byteToHex(uBin.b[(sizeof(uint64_t) - 1) - i], &hex[i<<1]);
    }
    hex[2*sizeof(uint64_t)] = '\0';
    return hex;
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
 
static void HEX_byteToHex(uint8_t bin, char* hex)
{
    uint8_t c = (bin >> 4) & 0x0F;
    BIN2ASC(c, hex[0])
    c = bin & 0x0F;
    BIN2ASC(c, hex[1])
}


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
