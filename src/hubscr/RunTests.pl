#!/usr/bin/perl -w

use strict;
my $operation = (defined($ARGV[0]) ? $ARGV[0] : "test");

sub runIt{
    my ($op, $testId, $options, $glm, $hub, $lang, $ref, $systems) = @_;
    my $baseDir = $testId.".base";
    my $outDir = $testId.($op eq "setTests" ? ".base" : ".test");
    print "   Running test '$testId', operation '$op', directory '$outDir'\n";
    system ("mkdir -p $outDir");
    system ("rm -f $outDir/test* $outDir/lvc*");
    ### Copy files
    foreach my $file($glm, $ref, split(/\s+/,$systems)){
	system("cp $file $outDir");
    }
    ### make all file names relative to the Outdir
    my ($refRoot) = $ref; $refRoot =~ s:.*/::;
    my ($glmRoot) = $glm; $glmRoot =~ s:.*/::;
    my ($systemsRoot) = "";
    foreach $_(split(/\s+/, $systems)){
	$_ =~ s:.*/::;
	$systemsRoot .= " ".$_;
    }
    print "      Executing command\n";
    my $oldEnv = $ENV{PATH};
    $ENV{PATH} = "";
    my $com = "(cd $outDir; ../hubscr.pl $options -p ../../rfilter1:../../csrfilt:../../def_art:../../acomp:../../hamzaNorm:../../sclite:$oldEnv ".
	"-l $lang -g $glmRoot -h $hub -r $refRoot $systemsRoot > log)";
    my $ret = system "$com";
    die "Error: Execution failed" if ($ret != 0);
    $ENV{PATH} = $oldEnv;
    if ($op ne "setTests"){
	print "      Comparing output\n";
	my $diffCom = "diff -x CVS -I '[cC]reation[ _]date' -r $baseDir $outDir";
	open (DIFF, "$diffCom |") || die "Diff command '$diffCom' Failed";
	my @diff = <DIFF>;
	close DIFF;
	die print "Error: Test $testId has failed.  Diff output is :\n@diff\n" if (@diff > 0);
	print "      Successful Test.  Removing $outDir\n";
	system "rm -rf $outDir";
    }
}

runIt($operation, "testArb", "-H -d", "../test_suite/test.arb2004.glm", "hub5", "arabic",
      "../test_suite/test.arb2004.txt.stm", 
      "../test_suite/test.arb2004.txt.ctm");
exit
runIt($operation, "test1", "", "../test_suite/example.glm", "hub5", "english",
      "../test_suite/lvc_refe.stm", 
      "../test_suite/lvc_hyp.ctm ../test_suite/lvc_hyp2.ctm");





exit;

#	rm -rf testBase
#	mkdir -p testBase
#	cp ../test_suite/lvc_hyp.ctm ../test_suite/lvc_refe.stm testBase
#	cp ../test_suite/lvc_hyp.ctm testBase/lvc_hyp2.ctm
#	(cd testBase; ../hubscr.pl -p ../../csrfilt:../../def_art:../../acomp:../../hamzaNorm -l english -g ../../test_suite/example.glm -h hub5 -r lvc_refe.stm lvc_hyp.ctm lvc_hyp2.ctm)
