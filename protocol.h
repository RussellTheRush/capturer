#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "baseTypes.h"
#include "log.h"
typedef struct  tagEth_header{
	U8 src[6];
	U8 dest[6];
	U16 type;
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
	LOG(" type: %04X\r\n", eth.type & 0xFFFF);
}

typedef struct tagIpHeader {
	U8 version:4;
	U8 headerLen:4;
	U8 serviceType;
	U16 totalLen;
	U16 identification;
	U16 flags:3;
	U16 fragOff:13;
	U8  ttl;
	U8  protocol;
	U16 checkSum;
	U32 srcAddress;
	U32 destAddress;
} __attribute__ ((__packed__)) ipHeader;

void dumpIpHeader(ipHeader ip) {
	U8 *pc;
	int i;
	LOG("version: %d, headerlen: %d\r\n", ip.version & 0xf, ip.headerLen&0xf);
	LOG("src ip address: ");
	for (pc=(U8 *)&(ip.srcAddress),i=0; i<4; pc++,i++ ) {
		LOG("%d", *pc & 0xFF);
		if (i != 3) LOG(".");
	}
	LOG(", dest ip address: ");
	for (pc=(U8 *)&(ip.destAddress),i=0; i<4; pc++,i++ ) {
		LOG("%d", *pc & 0xFF);
		if (i != 3) LOG(".");
	}
	
	LOG(", protocol :%02X\r\n", ip.protocol & 0xFF);
}

#define ETH_TYPE_IP 0x0800
#endif
