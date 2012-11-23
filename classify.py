#!/usr/bin/python

from scapy.all import *
import libcmdusr
import marshal

n = 0
pc = PcapReader('big2.pcap')
totsiz = 0
while True:
	pkt = pc.read_packet()
	if pkt is None:
		break
	payload = str(pkt)
	totsiz += len(payload)
	libcmdusr.classify(payload)
	if n%1000 == 0:
		r = libcmdusr.sumup()
		r['totsiz'] = totsiz
		marshal.dump(r, open('sumup.m', 'wb+'))
		print n, len(r), '%.2f MB'%(totsiz*1.0/1024/1024)
	n += 1

