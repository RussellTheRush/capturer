#ifndef __LIBPERM_RSA__
#define __LIBPERM_RSA__

#define N_BYTE_SIZE	(1024/8)
#define N2_BYTE_SIZE	(2048/8)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
int libperm_rsa_modPow(u8 *msg, u32 msgBytes, u8 *strE, u32 strELen, u8 *strN, u32 strNLen, u8 *outStr);
void dump_bytes(u8 *a, u32 l);
void printBytesAsHexString(u8 *bytes, u32 size);
#endif
