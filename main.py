#!/usr/bin/python

from scapy.all import *
import libcmdusr

pc = PcapReader('big2.pcap')
before = 0
after = 0
n = 0
s = ''
while True:
	pkt = pc.read_packet()
	if pkt is None:
		break
	payload = str(pkt.payload)
	if len(s) + len(payload) < 1024*888:
		s += payload
	else:
		r = libcmdusr.lz77(s)
		before += len(s)
		after += r
		s = ''
	n += 1
	if n % 1000 == 0:
		print n, before, after

