#!/usr/bin/python

import os, struct

os.system('rm -rf sample/*')
os.system('tar -xf sample.tar -C sample')

of = open('sample.1', 'w+')
of2 = open('sample.2', 'w+')

def comp_get_size(path):
	os.system('rm -rf sample/a.7z')
	os.system('7z a sample/a.7z %s > /dev/null' % path)
	origsize = os.path.getsize(path)
	compsize = os.path.getsize('sample/a.7z')
	per = compsize*1./origsize
	return origsize, compsize, per


for path in os.popen('find sample -type f'):
	path = path.strip()
	f = open(path, 'rb').read()
	if len(f) < 32:
		continue
	if ord(f[0]) == 1:
		l = ord(f[1])
		sport, dport = struct.unpack('II', f[2:2+l])
		origsize, compsize, per = comp_get_size(path)
		of.write('%d %d %.2f\n' % (sport, origsize, per))
of.close()

os.system('cat sample/*.cap > sample/all.cap')
origsize, compsize, per = comp_get_size('sample/all.cap')
of2.write('%d %d %.2f\n' % (origsize, compsize, per))
of2.close()

