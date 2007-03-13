#!/usr/bin/perl -w

use strict;
my $operation = (defined($ARGV[0]) ? $ARGV[0] : "test");

sub runIt{
    my ($op, $testId, $options, $glm, $utm, $input, $output) = @_;
    print "   Running Test $testId\n";
    my $oldEnv = $ENV{PATH};
    $ENV{PATH} = "../rfilter1:$ENV{PATH}";
    my $com = "./csrfilt.sh $options $glm $utm < $input > tmp.out";
    my $ret = system "$com";
    die "Error: Execution failed '$com'" if ($ret != 0);
    $ENV{PATH} = $oldEnv;

    if ($op eq "setTests"){
	system "mv tmp.out $output";
    } else {
	print "      Comparing output\n";
	my $diffCom = "diff $output tmp.out";
	open (DIFF, "$diffCom |") || die "Diff command '$diffCom' Failed";
	my @diff = <DIFF>;
	close DIFF;
	die print "Error: Test $testId has failed.\n    Command: $com\n    Diff output is :\n@diff\n" if (@diff > 0);
	print "      Successful Test.  Removing tmp.out\n";
	system "rm -f tmp.out";
    }
}

runIt($operation, "stm", "-i stm -dh",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.stm.in",  "../test_suite/test.stm.-dh.out");
runIt($operation, "stm", "-i stm",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.stm.in",  "../test_suite/test.stm.out");
runIt($operation, "rttm", "-i rttm -dh",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.rttm.in",  "../test_suite/test.rttm.-dh.out");
runIt($operation, "rttm", "-i rttm",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/sastt-case2.sys.rttm",  "../test_suite/sastt-case2.sys.rttm.filt");
runIt($operation, "rttm", "-i rttm",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.rttm.in",  "../test_suite/test.rttm.out");
runIt($operation, "trn", "-i trn -dh",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.trn.in",  "../test_suite/test.trn.-dh.out");
runIt($operation, "trn", "-i trn",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.trn.in",  "../test_suite/test.trn.out");
#	./csrfilt.sh -dh $(T)/example.glm $(T)/example.utm < $(T)/test.in > $(T)/test.out
#	./csrfilt.sh -i ctm -dh $(T)/example.glm $(T)/example.utm < $(T)/test_ctm.in > $(T)/test_ctm.out
runIt($operation, "text", "-dh",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test.in",  "../test_suite/test.out");
runIt($operation, "ctm", "-i ctm",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test_ctm.in",  "../test_suite/test_ctm.out");
runIt($operation, "ctm", "-i ctm -dh",
      "../test_suite/example.glm",  "../test_suite/example.utm",  "../test_suite/test_ctm.in",  "../test_suite/test_ctm.-dh.out");


exit;

#	rm -rf testBase
#	mkdir -p testBase
#	cp ../test_suite/lvc_hyp.ctm ../test_suite/lvc_refe.stm testBase
#	cp ../test_suite/lvc_hyp.ctm testBase/lvc_hyp2.ctm
#	(cd testBase; ../hubscr.pl -p ../../csrfilt:../../def_art:../../acomp:../../hamzaNorm -l english -g ../../test_suite/example.glm -h hub5 -r lvc_refe.stm lvc_hyp.ctm lvc_hyp2.ctm)
