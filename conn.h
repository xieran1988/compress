#pragma once

#include <stdint.h>
#include "packet.h"

typedef struct {
	FILE *fp;
	unsigned int h;
	int seq, ack;
	int totsiz;
	uint32_t sip, dip;
	uint16_t sport, dport;
	int capidx;
} conn_t ;

void conn_track(packet_t *pkt);
void conn_alarm(double t);

#define CONN_HASH_SIZE 300

