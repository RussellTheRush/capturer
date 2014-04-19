#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "baseTypes.h"
typedef struct  tagEth_header{
	U8 src[6];
	U8 dest[6];
	U16 protocol;
}  __attribute__ ((__packed__)) ethHeader;
#endif
