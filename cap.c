#include <stdio.h>
#include <stdint.h>
#include "conn.h"
#include "packet.h"

int xb_init() 
{
	return 0;
}

int xb_process(uint8_t *p, int len, uint8_t *opt)
{
	packet_t pkt;
	packet_parse(&pkt, (uint8_t *)p, len);
	conn_track(&pkt);
	if (opt[0] == 1) {
		double t = *(double *)(opt+1);
		conn_alarm(t);
	}
	return 0;
}

