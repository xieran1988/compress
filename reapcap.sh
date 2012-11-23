#!/bin/bash

cd cap
while true; do
	if [ "`ls *.cap`" != "" ]; then
		tar cf `date +'%Y-%m-%d-%H-%M-%S'`.tar *.cap
		rm -rf *.cap
	fi
	sleep 60
done

