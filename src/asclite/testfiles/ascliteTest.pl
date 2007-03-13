#!/usr/bin/perl -w 

# asclite
# Author: Jerome Ajot, Nicolas Radde, Chris Laprun
#
# This software was developed at the National Institute of Standards and Technology by
# employees of the Federal Government in the course of their official duties.  Pursuant to
# Title 17 Section 105 of the United States Code this software is not subject to copyright
# protection within the United States and is in the public domain. asclite is
# an experimental system.  NIST assumes no responsibility whatsoever for its use by any party.
#
# THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
# OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
# OR FITNESS FOR A PARTICULAR PURPOSE.
 
use strict;

my $scliteCom = "../../sclite/sclite";
my $ascliteCom = "../core/asclite";
my $compatOutDir = "scliteCompatTestOutDir";
my $ascliteTestOutDir = "ascliteTestOutDir";
my $failure = 0;
use Getopt::Long;

my $Usage = "Usage: ascliteTest.pl -s (all|sastt|std|mdm04|mdm04ByFile|cts04|mdmVariations|passed|notpassed) [ -m ]\n";
my $suite = "std";    ## OR all | mdm04 | bn04 | cts04 | mdmVariations
my $bigMem = 0;
my $result = GetOptions("s=s" => \$suite,
			"m" => \$bigMem);
die "Aborting:\n$Usage\n" if (!$result);


if ($suite =~ /^(std|all|passed)$/)
{
### Nulls not fully implmented so the next test is commented out JGF
### RunCompatTest("CT-basic",             "-i spu_id", "-r basic.trn trn","-h basic.trn trn CT-basic");
    RunCompatTest("CT-stmctm-i-x-x",               "", "-r stmctm.stm stm", "-h stmctm.ctm ctm CT-stmctm-i-x-x");
    RunCompatTest("CT-rttmstm-i-x-x",              "", "-r rttmstm.stm stm","-h rttmstm.rttm rttm CT-rttmstm-i-x-x");
    RunCompatTest("CT-trn-i-x-x","         -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-i-x-x");
    RunCompatTest("CT-trn-i-F-x","    -F   -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-i-F-x");
    RunCompatTest("CT-trn-i-x-D","      -D -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-i-x-D");
    RunCompatTest("CT-trn-i-F-D","   -F -D -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-i-F-D");
    RunCompatTest("CT-trn-s-x-x","-s    -D -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-s-x-x");
    RunCompatTest("CT-trn-s-F-x","-s -F    -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-s-F-x");
    RunCompatTest("CT-trn-s-x-D","-s    -D -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-s-x-D");
    RunCompatTest("CT-trn-s-F-D","-s -F -D -i spu_id", "-r trn.ref trn",    "-h trn.hyp trn CT-trn-s-F-D");

    RunAscliteTest("CT-overlap.correct-i-x-x", "", "-r overlap.correct.stm stm", "-h overlap.correct.ctm ctm CT-overlap.correct-i-x-x");
    RunAscliteTest("CT-overlap-i-x-x",         "", "-r overlap.stm stm",         "-h overlap.ctm ctm CT-overlap-i-x-x");
}

if ($suite =~ /^(mdm04ByFile|all|notpassed)$/)
{
    TestMDM_2004_byFile();
}
    
if ($suite =~ /^(mdm04|all|notpassed)$/)
{
    RunAscliteTest("CT-mdm-full-i-F-D-ov1", "-F -D -overlap-limit 1", "-r rt04s.040420.mdm.overlap.stm.filt stm",
		   "-h  rt04s.040810.mdm.overlap.ctm.filt ctm CT-mdm-full-i-F-D-ov1");
    RunAscliteTest("CT-mdm-full-i-F-D-ov2", "-F -D -overlap-limit 2", "-r rt04s.040420.mdm.overlap.stm.filt stm",
		   "-h  rt04s.040810.mdm.overlap.ctm.filt ctm CT-mdm-full-i-F-D-ov2");
}

if ($suite =~ /^(sastt|all|notpassed)$/)
{
    RunAscliteTest("sastt-1", "-F -D -spkr-align sastt.map.csv", "-r sastt.ref.rttm rttm", "-h sastt.sys.rttm rttm sastt-1");
}

if ($suite =~ /^(bn04|all|notpassed)$/)
{
    RunCompatTest("CT-eng-bnews-i-F-D","   -F -D","-r eng-bnews.stm.filt stm","-h eng-bnews.ctm.filt ctm CT-eng-bnews-i-F-D");
}

if ($suite =~ /^(cts04|all|notpassed)$/)
{
    RunCompatTest("CT-eng-cts-i-F-D","     -F -D","-r eng-cts.stm.filt stm","-h eng-cts.ctm.filt ctm CT-eng-cts-i-F-D");
}

if ($suite =~ /^(mdmVariations|all|notpassed)$/)
{
	### Variations:
	#Align: word | wordTime 0.0 | wordTime 1.0 | wordTime 5.0
	#Prune: none | 0.5 | 1.0 | 5.0
	#Overlap: 1 | 2 | 3
	
	my @word_options = ("", "-word-time-align 0", "-word-time-align 1000", "-word-time-align 5000");
	my @prune_options = ("", "-time-prune 500", "-time-prune 1000", "-time-prune 5000");
	my @overlap_options = ("", "-overlap-limit 1", "-overlap-limit 2", "-overlap-limit 3");
	
	my @word_name = ("", "w0", "w1", "w5");
	my @prune_name = ("", "p05", "p10", "p50");
	my @overlap_name = ("", "ov1", "ov2", "ov3");
	
	my $word_it;
	my $prune_it;
	my $overlap_it;
	
	my $base_options = "-F -D";
	my $base_name = "CT-mdm_2mtg_300sec-i-F-D";
	
	for ($word_it=0; $word_it<4; $word_it++)
	{
		for ($prune_it=0; $prune_it<4; $prune_it++)
		{
			for ($overlap_it=0; $overlap_it<4; $overlap_it++)
			{
				my $current_options = "$base_options $word_options[$word_it] $prune_options[$prune_it] $overlap_options[$overlap_it]";
				my $current_name = "$base_name";
				
				if($word_it > 0)
				{
					$current_name = "$current_name-$word_name[$word_it]";
				}
				
				if($prune_it > 0)
				{
					$current_name = "$current_name-$prune_name[$prune_it]";
				}
				
				if($overlap_it > 0)
				{
					$current_name = "$current_name-$overlap_name[$overlap_it]";
				}
				
				RunAscliteTest(	"$current_name", "$current_options", "-r mdm_2mtg_300sec.stm.filt stm",	"-h mdm_2mtg_300sec.ctm.filt ctm $current_name");
			}
		}
	}
}

die "Errors: Occured.  Exiting with non-zero code" if ($failure);
exit 0;

sub RunCompatTest
{
    my ($testId, $opts, $refOpts, $hypOpts) = @_;
    
    if (! -f "$compatOutDir/$testId.sgml")
    {
        print "Building Authoritative SGML file: $opts, $refOpts, $hypOpts\n";
        system "$scliteCom $opts $refOpts $hypOpts -o sgml stdout -f 0 | perl -pe 's/(creation_date=\")[^\"]+/\$1/' > $compatOutDir/$testId.sgml";
    }
	
    print "Comparing asclite to Authoritative SGML file: $opts, $refOpts, $hypOpts\n";
    my $com = "$ascliteCom $opts $refOpts $hypOpts -o sgml stdout -f 0";
    my $ret = system "$com |  perl -pe 's/(creation_date=\")[^\"]+/\$1/' > $compatOutDir/$testId.sgml.asclite";
        
    if ($ret != 0)
    {
        print "Error: Execution of '$com' returned code != 0\n";
    } 
    else
    { 
        my $diffCom = "diff $compatOutDir/$testId.sgml $compatOutDir/$testId.sgml.asclite";
        open (DIFF, "$diffCom |") || die "Diff command '$diffCom' Failed";
        my @diff = <DIFF>;
        close DIFF;
    
        if (@diff > 0)
        {
            print "Error: Test $testId has failed.  Diff output is :\n@diff\n" ;
            $failure = 1;
        } 
        else 
        {
            print "      Successful Test.  Removing $testId.sgml.asclite\n";
            system "rm -rf $compatOutDir/$testId.sgml.asclite";
        }	
    }
}

sub RunAscliteTest
{
    my ($testId, $opts, $refOpts, $hypOpts) = @_;
    
    if (! -f "$ascliteTestOutDir/$testId.sgml")
    {
        print "Building Authoritative SGML file: $opts, $refOpts, $hypOpts\n";
        system "$ascliteCom $opts $refOpts $hypOpts -o sgml stdout -f 0 | perl -pe 's/(creation_date=\")[^\"]+/\$1/' > $ascliteTestOutDir/$testId.sgml";
    } 
    else
    {
        print "Comparing asclite to Authoritative SGML file: $opts, $refOpts, $hypOpts\n";
        my $com = "$ascliteCom $opts $refOpts $hypOpts -o sgml stdout -f 0";
        my $ret = system "$com |  perl -pe 's/(creation_date=\")[^\"]+/\$1/' > $ascliteTestOutDir/$testId.sgml.asclite";
	
        if ($ret != 0)
        {
            print "Error: Execution of '$com' returned code != 0\n";
        }
        else 
        { 
            my $diffCom = "diff $ascliteTestOutDir/$testId.sgml $ascliteTestOutDir/$testId.sgml.asclite";
            open (DIFF, "$diffCom |") || die "Diff command '$diffCom' Failed";
            my @diff = <DIFF>;
            close DIFF;
            
            if (@diff > 0)
            {
                print "Error: Test $testId has failed.  Diff output is :\n@diff\n" ;
                $failure = 1;
            }
            else
            {
                print "      Successful Test.  Removing $testId.sgml.asclite\n";
                system "rm -rf $ascliteTestOutDir/$testId.sgml.asclite";
            }	
        }
    }
}

sub TestMDM_2004_byFile
{
    my $ref = "rt04s.040420.mdm.overlap.stm.filt";
    my $hyp = "rt04s.040810.mdm.overlap.ctm.filt";
    
    foreach my $file(split(/\s+/, `grep -v \\; $ref | awk '{print \$1}' | sort -u`))
    {
        print "File $file\n";
        system "grep '^$file' $ref > /tmp/bysrc.ref";
        system "grep '^$file' $hyp > /tmp/bysrc.hyp";
        RunAscliteTest("CT-mdm-full-i-F-D-ov1-$file", "-F -D -overlap-limit 1", "-r /tmp/bysrc.ref stm","-h /tmp/bysrc.hyp ctm CT-mdm-full-i-F-D-ov1-$file");
    }
}
