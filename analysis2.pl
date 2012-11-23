#!/usr/bin/perl
#
#

sub sumup {
	($col) = @_;
	open F, "sample.1";
	$left = 1<<32;
	$right = 0;
	while (<F>) {
		chomp;
		@cols = split / /;
		$val = @cols[$col];
		$left = $val if ($val < $left);
		$right = $val if ($val > $right);
		push @vals, $val;
	}
	$len = $right - $left;
	$cnt = 5;
	$partlen = $len*1./$cnt;
	for ($i = 0; $i < $cnt; $i++) {
		$l = $left + $partlen*$i;
		$r = $l + $partlen;
		$c = 0;
		for $val (@vals) {
			if ($val > $l && $val < $r) {
				$c++;
			}
		}
		print "  [$l, $r] = $c\n";
	}
}

print "filesize:\n";
sumup(1);

print "compratio:\n";
sumup(2);

print "tot:\n";
print "  ",`cat sample.2`;

