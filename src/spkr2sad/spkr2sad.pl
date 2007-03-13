#!/usr/bin/perl -w

use strict;
use Data::Dumper;

my $time = 0.3;
my %data = ();
while (<>){
    my @a = split;
    if ($_ =~ /^;/){
	print;
	next;
    }
    push @{ $data{$a[1]}{$a[0]} }, \@a;
}
#print Dumper(\%data);
my $max;
foreach my $file(sort (keys %data)){
    my @d = @{ $data{$file}{"SPKR-INFO"} };
    $d[0]->[7] = "speech";
    $d[0]->[6] = "unknown";
    print join(" ",@{ $d[0] })."\n";

    if (exists($data{$file}{"SPEAKER"})){
	@d = sort {$a->[3] <=> $b->[3]} @{ $data{$file}{"SPEAKER"} };
	for (my $i=0; $i<@d ; $i++){
	    $d[$i]->[7] = "speech";
	    print join(" ",@{ $d[$i] })."\n";
	}
    }
}
