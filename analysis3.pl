#!/usr/bin/perl
#
#

for (@ARGV) {
	$tar = $_;
	$name = `basename $tar`;
	chomp $name;
	if (-f "result/$name-1") {
		exit;	
	}

	`cp $tar sample.tar`;
	`./analysis.py`;
	`cp sample.1 result/$name-1`;
	`cp sample.2 result/$name-2`;
	`./analysis2.pl > result/$name-3`;
}

