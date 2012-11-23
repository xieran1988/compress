#pragma once

#include <stdint.h>

typedef struct {
	uint32_t sip, dip;
	uint16_t sport, dport;
	int seq, ack;
	uint8_t *pay;
	int paylen;
	char istcp:1;
	char isudp:1;
	char outgoing:1;
	int linkid;
	uint8_t tcpf;
	int len;
} packet_t ;

void packet_parse(packet_t *pkt, uint8_t *p, int len);

