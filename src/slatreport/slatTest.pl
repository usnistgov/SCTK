#!/usr/bin/env perl
#
# $Id$

use warnings;
use strict;

system("./slatreport.pl -i ../test_suite/slat.rttm -o ../test_suite/slat.rttm.out.test -t LEXEME -s lex > ../test_suite/slat.rttm.out.test");

unlink("../test_suite/slat.rttm.out.test.LatencyDistribution.10.png");
unlink("../test_suite/slat.rttm.out.test.LatencyRatioDistribution.10.png");

my $diff = `diff ../test_suite/slat.rttm.out ../test_suite/slat.rttm.out.test`;

if($diff ne "")
{
	print "Slat Test Failed.\n";
	print "$diff\n";
	exit(1);
}
else
{
	print "Slat Test OK.\n";
	unlink("../test_suite/slat.rttm.out.test");
	exit(0);
}
