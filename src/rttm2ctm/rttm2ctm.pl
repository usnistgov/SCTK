#!/usr/bin/perl -w

use strict;
use Data::Dumper;

my %sort_order = ("SPKRINFO"       => 0,
		  "NOSCORE"         =>  1,
		  "NORTMETADATA"  =>  2,
		  "SEGMENT"         =>  3,
		  "SPEAKER"         =>  4,
		  "SU"              =>  5,
		  "A/P"             =>  6,
		  "CB"              =>  7,
		  "IP"              =>  8,
		  "EDIT"            =>  9,
		  "FILLER"          =>  10,
		  "NON-SPEECH"      => 11,
		  "NON-LEX"         => 12,
		  "LEXEME"          => 13,
		  "SUboundary"      => 14);
my %spkrinfo;
my %stm_file;
 while (<>) {
     next if ($_ =~ /;;/);
     my $wrdExp = '[\]\[\S\%\{'."\\'".'\<\>.-]+';
     my $txtExp = "$wrdExp|\\($wrdExp\\)|<NA>";
#     my $txtExp = ".*|<NA>";
     if (/(SPKR-INFO|SEGMENT|LEXEME|NON-LEX|CB|SU|EDIT|FILLER|IP|NOSCORE|SPEAKER|NORTMETADATA|NON-SPEECH|A\/P)\s+(\S+)\s+(\d+)\s+(\d*\.?\d+|<NA>)\s+(\d*\.?\d+|<NA>)\s+($txtExp)\s+([\w&-]+|<NA>)\s+(\S+|<NA>)\s+(\d*\.?\d+|<NA>)/) {
#     print "($1, $2, $3, $4, $5, $6, $7, $8, $9, (defined($10) ? $10 : undef))\n";
       my ($type, $file, $chan, $beg, $dur, $token, $stype, $spkr, $conf, $slat) = ($1, $2, $3, $4, $5, $6, $7, $8, $9, (defined($10) ? $10 : undef));
       if ($1 eq "LEXEME" && $7 eq "lex"){
         print "$file $chan $beg $dur $token $conf\n";
       }
     } else {
  	   die "malformed line $.\n--> $_\n";
     }
 }

