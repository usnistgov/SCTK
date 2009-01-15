#!/usr/bin/env perl
#
# $Id$

use warnings;
use strict;
use Getopt::Long;
use Data::Dumper;
use Pod::Usage;

unless (eval "use Statistics::Descriptive; 1")
{
	die "'Statistics::Descriptive' is not available on your Perl installation.\n",
	"Please see 'http://search.cpan.org/search?query=statistics+descriptive&mode=all' for installation information\n";
}

my $version = "$1" if('$Revision$' =~ /(\d+\.\d+)/);

my $man = 0;
my $help = 0;
my $rttmfile = "";
my $outputfile = "slatreport";
my $typeslist = "";
my $stypeslist = "";
my $filelist = "";
my $chnllist = "";
my $namelist = "";
my $histogrampartitions = 2;

Getopt::Long::Configure("no_ignore_case");

GetOptions
(
	'help|h'        => \$help,
	'man|m'         => \$man,
	'version'       => sub { my $name = $0; $name =~ s/.*\/(.+)/$1/; print "$name version $version\n"; exit(0); },
	'i=s'           => \$rttmfile,
	'o=s'           => \$outputfile,
	'type|t=s'      => \$typeslist,
	'stype|s=s'     => \$stypeslist,
	'file|f=s'      => \$filelist,
	'chnl|c=s'      => \$chnllist,
	'name|n=s'      => \$namelist,
	'Histogram|H=i' => \$histogrampartitions,
) or pod2usage(1);

# Docs
pod2usage(1) if $help;
pod2usage(-exitvalue => 0, -verbose => 2) if $man;

# Checking the inputs
pod2usage("Error: RTTM file must be specified.\n") if ($rttmfile eq "");

my @Files = split(/,/, $filelist);
my @Chnl = split(/,/, $chnllist);
my @Names = split(/,/, $namelist);
my @Types = split(/,/, $typeslist);
my @STypes = split(/,/, $stypeslist);

my $data = LoadRTTM($rttmfile);
my ($LatencyMean, $LatencyRatioMean, $LatencyDistribution, $LatencyRatioDistribution) = Compute($data, $histogrampartitions, \@Files, \@Chnl, \@Names, \@Types, \@STypes);

print "Latency Mean:       $LatencyMean\n";

my $maxv = -1;

for(sort {$a <=> $b} keys %$LatencyDistribution)
{
	print "  key = $_, count = $LatencyDistribution->{$_}\n";
	$maxv = $_ if($_ > $maxv);
}

BuildPNG($LatencyDistribution, "$outputfile.LatencyDistribution.$histogrampartitions", "Latency Distribution - $histogrampartitions partitions", $maxv);

print "\n";
print "Latency Ratio Mean: $LatencyRatioMean\n";

$maxv = -1;

for(sort {$a <=> $b} keys %$LatencyRatioDistribution)
{
	print "  key = $_, count = $LatencyRatioDistribution->{$_}\n";
	$maxv = $_ if($_ > $maxv);
}

BuildPNG($LatencyRatioDistribution, "$outputfile.LatencyRatioDistribution.$histogrampartitions", "Latency Ratio Distribution - $histogrampartitions partitions", $maxv);

################
### Funtions ###
################
sub BuildPNG
{
	my ($data, $filename, $title, $maxx) = @_;
	
	# Create the dat file
	open(FILE, ">", "$filename.dat")
		or die "Cannot open file '$filename.dat'";
	
	for(sort {$a <=> $b} keys %$data)
	{
		print FILE "$_ $data->{$_}\n";
	}
		
	close(FILE);
	
	open(FILE, ">", "$filename.plt")
		or die "Cannot open file '$filename.plt'";
	
	print FILE "set terminal png medium\n";
	print FILE "set nokey\n";
	print FILE "set title \"$title\"\n";
	print FILE "set xrange [0:$maxx]\n";
	print FILE "set ytics 1\n";	
	print FILE "set style fill solid border -1\n";
	print FILE "set boxwidth 0.5 relative\n";
	print FILE "plot '$filename.dat' using 1:2 with boxes\n";

	close(FILE);
	
	system("cat $filename.plt | gnuplot > $filename.png");
	
	unlink("$filename.dat");
	unlink("$filename.plt");
}

sub Compute
{
	my ($data, $HistPar, $Fil, $Chn, $Nam, $Typ, $STy) = @_;
	
	my $statLacency = Statistics::Descriptive::Full->new();
	my $statLacencyRatio = Statistics::Descriptive::Full->new();
	
	foreach my $f (keys %{ $data })
	{
		next if(! IsElement($f, $Fil));
		
		foreach my $c (keys %{ $data->{$f} })
		{
			next if(! IsElement($c, $Chn));
			
			foreach my $n (keys %{ $data->{$f}{$c} })
			{
				next if(! IsElement($n, $Nam));
				
				foreach my $t (keys %{ $data->{$f}{$c}{$n} })
				{
					next if(! IsElement($t, $Typ));
					
					foreach my $s (keys %{ $data->{$f}{$c}{$n}{$t} })
					{
						next if(! IsElement($s, $STy));
						
						my $TBEG = $data->{$f}{$c}{$n}{$t}{$s}{TBEG};
						my $TEND = $data->{$f}{$c}{$n}{$t}{$s}{TEND};
						my $TSLAT = $data->{$f}{$c}{$n}{$t}{$s}{TSLAT};
						
						$statLacency->add_data($TSLAT-$TEND);
						$statLacencyRatio->add_data(($TSLAT-$TEND)/($TEND-$TBEG));
					}
				}
			}
		}
	}
	
	my %ld = $statLacency->frequency_distribution($HistPar);
	my %lrd = $statLacencyRatio->frequency_distribution($HistPar);
	
	return( $statLacency->mean(),
	        $statLacencyRatio->mean(), 
		    \%ld,
		    \%lrd );
}

sub IsElement
{
	my ($e, $l) = @_;
	
	return 1
		if(scalar(@$l) == 0);
	
	for(my $i=0; $i<scalar(@$l); $i++)
	{
		return 1
			if($l->[$i] =~ $e);
	}
	
	return 0;
}

sub LoadRTTM
{
	my ($file) = @_;
	
	open(FILE, "<", $file)
		or die "Cannot open file '$file'";
	
	my %h;
	
	while ( <FILE> )
	{
		chomp;
		next if($_ =~ /^\;;/);
		next if($_ =~ /^\s*$/);
		my @a = split(/\s+/, $_);
		
		die "This file '$file' is not properly formated: SLAT information missing"
			if(scalar(@a) != 10);
		
		next if($a[9] =~ /<NA>/);
		
		$h{$a[1]}{$a[2]}{$a[7]}{$a[0]}{$a[6]}{TBEG} = $a[3];
		$h{$a[1]}{$a[2]}{$a[7]}{$a[0]}{$a[6]}{TEND} = $a[3]+$a[4];
		$h{$a[1]}{$a[2]}{$a[7]}{$a[0]}{$a[6]}{TSLAT} = $a[9];
	}
		
	close(FILE);
	
	return \%h;
}