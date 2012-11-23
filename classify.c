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

#include "classify.h"

static classify_t classify[CLASSIFY_HASH_SIZE];

static void classify_hash_insert(const char *str, int flag, int cnt)
{
	int h = ((int)str)*13 + flag*23;
	h = h % CLASSIFY_HASH_SIZE;
	while (classify[h].h && classify[h].h != h && h < HASH_SIZE)
		h++;
	if (h == CLASSIFY_HASH_SIZE)
		return ;
	classify[h].h = h;
	classify[h].str = str;
	classify[h].flag = flag;
	classify[h].cnt += cnt;
}

void classify_sumup(classify_t *h, int *len)
{
	int i, j = 0;
	for (i = 0; i < CLASSIFY_HASH_SIZE; i++) {
		if (classify[i].h)
			h[j++] = classify[i];
	}
	*len = j;
}

void classify(packet_t *pkt) 
{
	if (pkt->istcp) {
		classify_hash_insert("tcp", 0, pkt->len);
		hash_insert("tport", dport, len);
	}
	if (pkt->isudp) {
		classify_hash_insert("udp", 0, pkt->len);
	}

	if (iptype == 6) {
		tcp = ip + iplen;
		totlen = htons(*(uint16_t*)(ip+2));
		tcplen = (*(tcp+12)&0xf0)>>2;
		sport = htons(*(uint16_t*)(tcp));
		dport = htons(*(uint16_t*)(tcp+2));
		sip = *(uint32_t*)(ip+12);
		dip = *(uint32_t*)(ip+16);
		seq = htonl(*(uint32_t*)(tcp+4));
		ack = htonl(*(uint32_t*)(tcp+8));
		tcpf = *(tcp+13);
		pay = (char *)(ip + iplen + tcplen);
		paylen = totlen - iplen - tcplen;
		hash_insert("tport", dport, len);
	} else if (iptype == 17) {
	}
}

