/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*
* This code is released into the public domain free of any restrictions. 
* The author requests acknowledgement if the code is used, but does not 
* require it. This code is provided free of any liability and without 
* any quality claims by the author.
*
* Modification History:
*
* 08/21/2015 – Tom Kerr
* Obtained from http://bradconte.com/sha256_c and modified for 
* AVR/Arduino build environment.
*********************************************************************/

#ifndef _SHA256_H
#define _SHA256_H

/*************************** HEADER FILES ***************************/
#include <stdint.h>

/****************************** MACROS ******************************/
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest


#ifdef __cplusplus
extern "C" {
#endif


/**************************** DATA TYPES ****************************/
typedef struct {
   uint8_t data[64];
   uint32_t datalen;
   uint64_t bitlen;
   uint32_t state[8];
} SHA256_CTX;

void sha256_init(SHA256_CTX *ctx);

void sha256_update(SHA256_CTX *ctx, const uint8_t data[], size_t len);

void sha256_final(SHA256_CTX *ctx, uint8_t hash[]);

#ifdef __cplusplus
}
#endif

#endif  // _SHA256_H
