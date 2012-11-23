#define _GNU_SOURCE 
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

#include "packet.h"

void packet_parse(packet_t *pkt, uint8_t *p, int len) 
{
	char *pay;
	uint8_t *ip, *tcp, tcpf;
	uint32_t iplen, tcplen, totlen, paylen;

	memset(pkt, 0, sizeof(*pkt));
	pkt->len = len;

	if (len < 14)
		return ;

	uint16_t ethtype = *(uint16_t *)((char *)p + 12);

	if (ethtype == 0x6488) {
		// PPPOE
		if (len < 14 + 8)
			return ;
		ip = (uint8_t *)p + 14 + 8;
	} else if (ethtype == 0x0008) {
		// IP
		if (len < 14)
			return ;
		ip = (uint8_t *)p + 14;
	} else {
		// Other
		return ;
	}

	if (len < 14 + 20)
		return ;
	uint8_t iptype = ip[9];
	if (iptype == 6) {
		// TCP
		pkt->istcp = 1;
	} else if (iptype == 17) {
		// UDP
		pkt->isudp = 1;
	} else {
		return ;
	}

	iplen = (*ip&0x0f)*4;

	if (len < 14 + 20 + iplen) 
		return ;

	if (iptype == 6) {
		// TCP
		tcp = ip + iplen;
		totlen = htons(*(uint16_t*)(ip+2));
		tcplen = (*(tcp+12)&0xf0)>>2;
		pkt->sport = htons(*(uint16_t*)(tcp));
		pkt->dport = htons(*(uint16_t*)(tcp+2));
		pkt->sip = *(uint32_t*)(ip+12);
		pkt->dip = *(uint32_t*)(ip+16);
		pkt->seq = htonl(*(uint32_t*)(tcp+4));
		pkt->ack = htonl(*(uint32_t*)(tcp+8));
		pkt->tcpf = *(tcp+13);
		pkt->pay = (char *)(ip + iplen + tcplen);
		pkt->paylen = totlen - iplen - tcplen;
	} else if (iptype == 17) {
		// UDP
	}
}


