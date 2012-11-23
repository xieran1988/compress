#!/bin/bash

top=/root/xcache/netsniff-ng/src/build
xb_libso=./libcap.so $top/netsniff-ng/netsniff-ng --in eth1 -s

