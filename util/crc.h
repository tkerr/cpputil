/**********************************************************************
 *
 * Filename:    crc.h
 * 
 * Description: A header file describing the various CRC standards.
 *
 * Notes:       
 *
 * 
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 *
 * Modification History:
 *
 * 08/30/2015 - Tom Kerr
 * Downloaded from the Barr Group website:
 *   http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
 * Modified for general embedded systems use.
 * Made compilation of crcFast conditional.
 **********************************************************************/

#ifndef _crc_h
#define _crc_h

#include <stdint.h>

/*
 * Select the CRC standard from the list that follows.
 */
#define CRC_CCITT
//#define CRC16
//#define CRC32


#if defined(CRC_CCITT)

typedef uint16_t crc_t;

#define CRC_NAME			"CRC-CCITT"
#define POLYNOMIAL			0x1021
#define INITIAL_REMAINDER	0xFFFF
#define FINAL_XOR_VALUE		0x0000
#define REFLECT_DATA		0
#define REFLECT_REMAINDER	0
#define CHECK_VALUE			0x29B1

#elif defined(CRC16)

typedef uint16_t crc_t;

#define CRC_NAME			"CRC-16"
#define POLYNOMIAL			0x8005
#define INITIAL_REMAINDER	0x0000
#define FINAL_XOR_VALUE		0x0000
#define REFLECT_DATA		1
#define REFLECT_REMAINDER	1
#define CHECK_VALUE			0xBB3D

#elif defined(CRC32)

typedef uint32_t crc_t;

#define CRC_NAME			"CRC-32"
#define POLYNOMIAL			0x04C11DB7
#define INITIAL_REMAINDER	0xFFFFFFFF
#define FINAL_XOR_VALUE		0xFFFFFFFF
#define REFLECT_DATA		1
#define REFLECT_REMAINDER	1
#define CHECK_VALUE			0xCBF43926

#else

#error "One of CRC_CCITT, CRC16, or CRC32 must be #define'd."

#endif


/*
 * If USE_CRC_FAST is defined, then crcInit, crcFast, and crcTable are compiled.
 * crcTable is a large data structure, and may not fit in smaller embedded processors.
 * Comment out USE_CRC_FAST to save memory.
 */
#define USE_CRC_FAST


#ifdef __cplusplus
extern "C" {
#endif

void  crcInit(void);
crc_t crcSlow(const uint8_t* message, uint16_t nBytes);
crc_t crcFast(const uint8_t* message, uint16_t nBytes);

#ifdef __cplusplus
}
#endif

#endif /* _crc_h */