#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "baseTypes.h"
#include "log.h"
typedef struct  tagEth_header{
	U8 src[6];
	U8 dest[6];
	U16 protocol;
}  __attribute__ ((__packed__)) ethHeader;
static void dumpMacAddress(U8 mac[]) {
	int i = 0;
	for (i=0; i<6; i++) {
		LOG("%02X", mac[i] & 0xFF);
		if (i != 5) {
			LOG(":");
		}
	}
}
void dumpEthHeader(ethHeader eth) {
	int i=0;
	LOG("dest mac address: ");
	dumpMacAddress(eth.src);
	LOG(" src mac address: ");
	dumpMacAddress(eth.dest);
	LOG(" protocol fileds: %04X\r\n", eth.protocol & 0xFFFF);
}
#endif
