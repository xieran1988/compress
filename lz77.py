#!/usr/bin/python

import libcmdusr

s = open('lz77.c').read()
print 'before', len(s)
print libcmdusr.lz77(s)
