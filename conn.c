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
#include <math.h>

#include "conn.h"

static conn_t conn_pool[CONN_HASH_SIZE];
static int nr_conn;
static int tot_conn_cap;
static int s_nrconn, s_io;

static conn_t *conn_hash_find(unsigned int h) 
{
	int j;
	conn_t *c;

	for (j = 0; j < CONN_HASH_SIZE; j++) {
		c = &conn_pool[j];
		if (c->h == h || !c->h) 
			return c;
	}
	return NULL;
}

static void conn_hash_free(conn_t *c)
{
	memset(c, 0, sizeof(*c));
}

static unsigned int hash(int sip, int dip, int sport, int dport)
{
	return (unsigned int)((sip+dip)*333 + (sport+dport)*23);
}

static void wr8(FILE *fp, uint8_t val)
{
	fwrite(&val, 1, 1, fp);
}

static void wr32(FILE *fp, uint32_t val)
{
	fwrite(&val, 1, 4, fp);
}

void conn_track(packet_t *pkt) 
{
	conn_t *c;
	unsigned int h;

	if (!pkt->istcp) 
		return ;

	h = hash(pkt->sip, pkt->dip, pkt->sport, pkt->dport); 
	c = conn_hash_find(h);

	if (pkt->tcpf == 0x012) {
		// SYN, ACK
		if (!c) 
			return ;
		if (!c->h) {
			c->h = h;
			c->ack = pkt->ack;
			c->seq = pkt->seq;
			c->sip = pkt->sip;
			c->dip = pkt->dip;
			c->sport = pkt->sport;
			c->dport = pkt->dport;
			nr_conn++;
			{
				int i;
				char path[256];
				sprintf(path, "cap/%d.cap", tot_conn_cap);
				c->fp = fopen(path, "wb+");
				if (!c->fp) {
					printf("fopen failed\n");
					exit(0);
				}
				wr8(c->fp, 1);
				wr8(c->fp, 8);
				wr32(c->fp, c->sport);
				wr32(c->fp, c->dport);
				wr8(c->fp, 0);
				tot_conn_cap++;
			}
		}
		return ;
	}

	if (!c || !c->h)
		return ;

	/*
	int tx, rx;
	if (c->dip == pkt->dip && c->dport == pkt->dport && 
			c->sip == pkt->sip && c->sport == pkt->sport) {
		tx = pkt->ack - c->ack;
		rx = pkt->seq - c->seq;
	} else 
	if (c->dip == pkt->sip && c->dport == pkt->sport && 
		  c->sip == pkt->dip && c->sport == pkt->dport) {
		tx = pkt->ack - c->seq;
		rx = pkt->seq - c->ack;
	} else {
//		printf("hash collision: %u\n", c->h);
//		exit(0);
	}
	*/
	s_io += pkt->len;

	c->totsiz += pkt->len;
	if (c->fp)
		fwrite(pkt->pay, pkt->paylen, 1, c->fp);

	if (pkt->tcpf & 5) {
		// FIN, ACK
		//printf("tcp tx:%d rx:%d tot:%d nr_conn:%d nr_conn_cap:%d\n", 
		//		tx, rx, c->totsiz, nr_conn, nr_conn_cap);
		if (c->fp) 
			fclose(c->fp);
		nr_conn--;
		conn_hash_free(c);
		s_nrconn++;
	}
}

void conn_alarm(double t)
{
	printf("elapsed %.2lf nrconn=%d io=%.2fK\n", 
				 t, s_nrconn, s_io/1024.
				 );

	s_nrconn = 0;
	s_io = 0;
}

