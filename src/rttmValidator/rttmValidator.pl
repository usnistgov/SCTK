#!/usr/bin/perl -w
#
########################################################
# History:
#   v00: Initial version - validate a given RTTM file
#
########################################################

use strict;
use Getopt::Std;

my $VERSION = "v00";

my $USAGE = "\n\n$0 [-dvh] -i <RTTM file>\n\n".
    "Description: This Perl program (version $VERSION) validates a given RTTM file.\n".
    "Options:\n".
    "  -u            : disable check that ensures all LEXEMEs belong to some SU object\n".
    "  -s            : disable check that ensures all LEXEMEs belong to some SPEAKER object\n".
    "  -v            : make it verbose\n".
    "  -h            : print a help message\n".
    "Input:\n".
    "  -i <RTTM file>: an RTTM file\n\n";

my $NUM_FIELDS = 9; # count from 1

my $ROUNDING_THRESHOLD = 0.001;

########################################################
# Main
#
########################################################
{
    my ($date, $time) = date_time_stamp();
    my $commandline = join(" ", @ARGV);

    use vars qw ($opt_i $opt_u $opt_s $opt_v $opt_h);
    getopts('i:usvh');
    die ("$USAGE") if ($opt_h) || (! $opt_i);

    my @mde_types = ();
    push (@mde_types, "SU") if ! $opt_u;
    push (@mde_types, "SPEAKER") if ! $opt_s;

    print "$0 (version $VERSION) run on $date at $time\n" if $opt_v;
    print "command line:  ", $0, " (version ", $VERSION, ") ", $commandline, "\n" if $opt_v;

    my (%rttm_data, $data_domain);
    get_rttm_data(\%rttm_data, \$data_domain, $opt_i);

    # debug
    #
    my $debug = 0;
    my $count;
    if ($debug) {
	my ($src, $chnl, $spkr, $type, $token);
	foreach $src (keys %rttm_data) {
	    foreach $chnl (keys %{$rttm_data{$src}}) {
		foreach $spkr (keys %{$rttm_data{$src}{$chnl}}) {
		    foreach $type (keys %{$rttm_data{$src}{$chnl}{$spkr}}) {
			$count = 0;
			foreach $token (@{$rttm_data{$src}{$chnl}{$spkr}{$type}}) {
			    print "[$src $chnl $spkr $type $count]\t$token->{TYPE}\t$token->{SRC}\t$token->{CHNL}\t$token->{TBEG}\t$token->{TDUR}\t$token->{ORTHO}\t$token->{STYPE}\t$token->{SPKR}\t$token->{CONF}\n";
			    $count++;
			}
		    }
		}
	    }
	}
    }

    if (check_syntax_errors(\%rttm_data, $data_domain, $opt_i) &&
	check_logic_errors(\%rttm_data, \@mde_types)){
	exit 0;
    }
    exit 1;
}

########################################################
# Subroutines
#
########################################################
sub get_rttm_data {

    # read the RTTM file
    #
    my ($data, $domain, $file) = @_;

    my (@fields, $token);

    my $line_num = 0;

    ${$domain} = "";

    open DATA, "$file" or die ("ERROR: Unable to open $file\n");
    while (<DATA>) {
	$line_num++;
	chomp;
	if (/^\s*;;EXP-ID:\s*(.*)/) {
	    my $exp_id = $1;
	    if ($exp_id =~ /bnews/i) {
		${$domain} = "bn";
	    } elsif ($exp_id =~ /cts/i) {
		${$domain} = "cts";
	    }
	} elsif (! /^\s*;;/ && ! /^\s*$/) {
	    @fields = split;
	    undef $token;
	    $token->{FIELD_COUNT} = scalar (@fields);
            $token->{LOC} = "line $line_num";
	    $token->{TYPE} = shift @fields;
	    $token->{SRC} = shift @fields;
	    $token->{CHNL} = shift @fields;
	    $token->{TBEG} = shift @fields;
	    $token->{TDUR} = shift @fields;
	    $token->{ORTHO} = shift @fields;
	    $token->{STYPE} = shift @fields;
	    $token->{SPKR} = shift @fields;
	    $token->{CONF} = shift @fields;
	    push (@{$data->{$token->{SRC}}{$token->{CHNL}}{$token->{SPKR}}{$token->{TYPE}}}, $token);
	}
    }
    close DATA;
}

sub check_syntax_errors {
    # check for syntax errors such as missing fields,
    # invalid field values, etc.
    # 
    my ($data, $domain, $file) = @_;

    my $pass = 1;

    my ($src, $chnl, $spkr, $type, $token);

    $file =~ s/\..*$//;
    $file =~ s/.*\///;

    foreach $src (keys %{$data}) {
	foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
		    foreach $token (@{$data->{$src}{$chnl}{$spkr}{$type}}) {
			# make sure that we have the correct number of fields
			#
			if ($token->{FIELD_COUNT} != $NUM_FIELDS) {
			    print "ERROR: This record has $token->{FIELD_COUNT} fields instead of the required $NUM_FIELDS fields; see $token->{LOC}\n";
			    $pass = 0;
			}
			
			# for the following checks to work, all the fields must be defined
			#
			if (defined ($token->{TYPE}) && defined ($token->{SRC}) &&
			    defined ($token->{CHNL}) && defined ($token->{TBEG}) &&
			    defined ($token->{TDUR}) && defined ($token->{ORTHO}) &&
			    defined ($token->{STYPE}) && defined ($token->{SPKR}) &&
			    defined ($token->{CONF})) {

			    # make sure the type field is a valid RTTM type
			    #
			    if ($token->{TYPE} !~ /^(SEGMENT|NOSCORE|NO_RT_METADATA|LEXEME|NON-LEX|NON-SPEECH|FILLER|EDIT|SU|IP|CB|A\/P|SPEAKER|SPKR-INFO)$/i) {
				print "ERROR: Invalid RTTM type; see field (0) in $token->{LOC}\n";
				$pass = 0;
			    }
			    
			    # make sure the source field matches the base filename.
			    # This won't work if people concat all the files into one.
			    # Comment out this check out
			    #
#			    if ($token->{SRC} !~ /^$file$/i) {
#				print "ERROR: Source field doesn't match the input file's base filename; see field (2) in $token->{LOC}\n";
#				$pass = 0;
#			    }
			    
			    # make sure the channel ID has a value of 1 or 2 but 1 for BN data
			    #
			    if ($token->{CHNL} !~ /^(1|2)$/) {
				print "ERROR: Invalid channel ID; see field (3) in $token->{LOC}\n";
				$pass = 0;
			    } elsif ($domain =~ /bn/i && $token->{CHNL} != 1) {
				print "ERROR: Expected channel ID to be 1 for BN data; see field (3) in $token->{LOC}\n";
				$pass = 0;
			    }
			    
			    # make sure that if it's a SPKR-INFO the start time is <NA>
			    # else the start time must be a number and is non-negative
			    #
			    if ($token->{TYPE} =~ /SPKR-INFO/i) {
				if ($token->{TBEG} !~ /<NA>/i) {
				    print "ERROR: $token->{TYPE} should not have any start time; see field (4) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } else {
				if ($token->{TBEG} !~ /^(\-{0,1}\d+\.?\d*|\-{0,1}\.\d+)\**$/) {
				    print "ERROR: Expected start time to be a number; see field (4) in $token->{LOC}\n";
				    $pass = 0;
				} else {
				    my $tbeg = $token->{TBEG};
				    $tbeg =~ s/\*//;
				    if ($tbeg < 0) {
					print "ERROR: Negative start time; see field (4) in $token->{LOC}\n";
					$pass = 0;
				    }
				}
			    }
			    
			    # make sure that if it's a SPKR-INFO, an IP, or a CB the duration is <NA>
			    # else the duration must be a number and is non-negative
			    #
			    if ($token->{TYPE} =~ /(SPKR-INFO|IP|CB)/i) {
				if ($token->{TDUR} !~ /^<NA>$/i) {
				    print "ERROR: $token->{TYPE} should not have any duration; see field (5) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } else {
				if ($token->{TDUR} !~ /^(\-{0,1}\d+\.?\d*|\-{0,1}\.\d+)\**$/) {
				    print "ERROR: Expected duration to be a number; see field (5) in $token->{LOC}\n";
				    $pass = 0;
				} else {
				    my $tdur = $token->{TDUR};
				    $tdur =~ s/\*//;
				    if ($tdur < 0) {
					print "ERROR: Negative duration; see field (5) in $token->{LOC}\n";
					$pass = 0;
				    }
				}
			    }
			    
			    # make sure that if it's not a LEXEME or a NON-LEX it doesn't have any orthography
			    #
			    if ($token->{TYPE} =~ /(SEGMENT|NOSCORE|NO_RT_METADATA|NON-SPEECH|FILLER|EDIT|SU|IP|CB|A\/P|SPEAKER|SPKR-INFO)/i) {
				if ($token->{ORTHO} !~ /^<NA>$/i) {
				    print "ERROR: Value for the orthography field for $token->{TYPE} should be <NA>; see field (6) in $token->{LOC}\n";
				    $pass = 0;
				}
			    }
			    
			    # make sure the subtype for various type is valid
			    #
			    if ($token->{TYPE} =~ /SEGMENT/i) {
				if ($token->{STYPE} !~ /^(eval|<NA>)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /(NOSCORE|NO_RT_METADATA|A\/P|SPEAKER)/i) {
				if ($token->{STYPE} !~ /^<NA>$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /LEXEME/i) {
				if ($token->{STYPE} !~ /^(lex|fp|frag|un-lex|for-lex|alpha|acronym|interjection|propernoun|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
				if ($token->{STYPE} =~ /alpha/i && $token->{ORTHO} !~ /^([A-Z]\.|[A-Z]\.\'s)$/i) {
				    print "ERROR: Invalid orthography for alpha $token->{TYPE}; see field (6) in $token->{LOC}\n";
				    $pass = 0;
			        } 
                                if ($token->{ORTHO} !~ /^[\[\]a-zA-Z\.\-\']+$/) {
		                    print "ERROR: Invalid orthography for $token->{TYPE}; see field (6) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /NON-LEX/i) {
				#### fix lipsmack!!!!
				if ($token->{STYPE} !~ /^(laugh|breath|lipsmack|cough|sneeze|other)$/) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /NON-SPEECH/i) {
				if ($token->{STYPE} !~ /^(noise|music|other)$/) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /FILLER/i) {
				if ($token->{STYPE} !~ /^(filled_pause|discourse_marker|explicit_editing_term|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /EDIT/i) {
				if ($token->{STYPE} !~ /^(repetition|restart|revision|simple|complex|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /SU/i) {
				if ($token->{STYPE} !~ /^(statement|question|incomplete|backchannel|unannotated|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /IP/i) {
				if ($token->{STYPE} !~ /^(edit|filler|edit\&filler|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /CB/i) {
				if ($token->{STYPE} !~ /^(coordinating|clausal|other)$/i) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    } elsif ($token->{TYPE} =~ /SPKR-INFO/i) {
				if ($token->{STYPE} !~ /^(adult_male|adult_female|child|unknown)$/) {
				    print "ERROR: Invalid $token->{TYPE} subtype; see field (7) in $token->{LOC}\n";
				    $pass = 0;
				}
			    }
			    
			    # make sure that the confidence field is a number it's from 0-1 else it's <NA>
			    #
			    if ($token->{CONF} !~ /<NA>/) {
				if ($token->{CONF} !~ /^(\-{0,1}\d+\.?\d*|\-{0,1}\.\d+)$/) {
				    print "ERROR: Expected confidence value to be a number or <NA>; see field (9) in $token->{LOC}\n";
				    $pass = 0;
				} elsif ($token->{CONF} < 0 || $token->{CONF} > 1) {
				    print "ERROR: Expected confidence value to be [0,1]; see field (9) in $token->{LOC}\n";
				    $pass = 0;
				}
			    }
			    
			    # make sure that the speaker id field for certain types match the speaker id
			    # given in the SPKR-INFO object
			    #
			    if ($type !~ /(SPKR-INFO|NO_RT_METADATA|NOSCORE|NON-SPEECH)/i && ! find_speaker($token->{SPKR}, $data)) {
				print "ERROR: Speaker $token->{SPKR} doesn't match any of the speaker IDs in SPKR-INFO objects; see $token->{LOC}\n";
				$pass = 0;
			    }
			}
                    }
                }
	    }
	}
    }
    return $pass;
}

sub check_logic_errors {
    # check for logic errors such as overlapping words from the same speaker, etc.
    #
    my ($data, $mde_types) = @_;

    my ($src, $chnl, $spkr, $type, $token);

    # sort the data 
    #
    foreach $src (keys %{$data}) {
        foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
		    if ($type !~ /SPKR-INFO/i) {
			@{$data->{$src}{$chnl}{$spkr}{$type}} = 
			    sort {$a->{TBEG} <=> $b->{TBEG}} @{$data->{$src}{$chnl}{$spkr}{$type}};
		    }
		}
	    }
	}
    }

    if (check_metadata_overlap($data) &&
	check_metadata_content($data) &&
	check_partial_word_coverage($data)) {
	foreach my $type (@$mde_types) {
	    if (! ensure_word_covered_by_metadata_of_type($data, $type)) {
		return 0;
	    }
	}
	return 1;
    } else {
	return 0;
    }
}

sub check_metadata_overlap {
    # check if two metadata objects of the same type
    # from the same speaker overlap
    #
    my ($data) = @_;

    my ($src, $chnl, $spkr, $type, $token, $pass);
    $pass = 1;

    foreach $src (keys %{$data}) {
	foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
		    if ($type !~ /(SPKR-INFO|IP|CB)/i) {
			my $prev_etime = 0;
			foreach $token (@{$data->{$src}{$chnl}{$spkr}{$type}}) {
			    if (($prev_etime - $token->{TBEG}) > $ROUNDING_THRESHOLD) {
				print "ERROR: Speaker $spkr has $type ending at $prev_etime and starting at $token->{TBEG}; ${type}s overlap; see $token->{LOC}\n";
				$pass = 0;
			    }
			    $prev_etime = $token->{TBEG} + $token->{TDUR};
			}
		    }
		}
	    }
	}
    }
    return $pass;
}

sub check_metadata_content {
    # check if a metadata object of type SU, EDIT, or FILLER contains any words.
    # The words could be missing or simply didn't get transcribed
    #
    my ($data) = @_;

    my ($src, $chnl, $spkr, $type, $token, $pass);
    $pass = 1;

    foreach $src (keys %{$data}) {
	foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
		    if ($type =~ /(SU|EDIT|FILLER)/i) {
			foreach $token (@{$data->{$src}{$chnl}{$spkr}{$type}}) {
			    if (! find_word($data->{$src}{$chnl}{$spkr}{LEXEME}, $token->{TBEG}, $token->{TBEG} + $token->{TDUR})) {
				print "ERROR: $type at $token->{TBEG} contains no words; see $token->{LOC}\n";
				$pass = 0;
			    }
			}
		    }
		}
	    }
	}
    }
    return $pass;
}

sub check_partial_word_coverage {
    # check if a given word partially overlaps with any metadata object
    #
    my ($data) = @_;
    
    my ($src, $chnl, $spkr, $type, $token, $pass, $mde_type);
    $pass = 1;
    
    foreach $src (keys %{$data}) {
	foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
		    if ($type =~ /LEXEME/i) {
			foreach $token (@{$data->{$src}{$chnl}{$spkr}{$type}}) {
			    foreach $mde_type (keys %{$data->{$src}{$chnl}{$spkr}}) {
				if ($mde_type =~ /(SU|EDIT|FILLER)/i) {
				    if (find_partial_coverage($data->{$src}{$chnl}{$spkr}{$mde_type}, $token->{TBEG}, $token->{TBEG} + $token->{TDUR})) {
					print "ERROR: word at $token->{TBEG} is partially covered by $mde_type object; see $token->{LOC}\n";
					$pass = 0;
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    return $pass;
}

sub ensure_word_covered_by_metadata_of_type {
    # make sure that all words belong to some metadata object of the given type.
    # Right now the given type is always SU
    #
    my ($data, $mde_type) = @_;

    my ($src, $chnl, $spkr, $type, $token, $pass);
    $pass = 1;

    if ($mde_type) {
	foreach $src (keys %{$data}) {
	    foreach $chnl (keys %{$data->{$src}}) {
		foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		    foreach $type (keys %{$data->{$src}{$chnl}{$spkr}}) {
			if ($type =~ /LEXEME/i) {
			    foreach $token (@{$data->{$src}{$chnl}{$spkr}{$type}}) {
				if (! find_type($data->{$src}{$chnl}{$spkr}{$mde_type}, $token->{TBEG}, $token->{TBEG} + $token->{TDUR})) {
				    print "ERROR: word at $token->{TBEG} doesn't belong to any $mde_type object; see $token->{LOC}\n";
				    $pass = 0;
				}
			    }
			}
		    }
		}
	    }
	}
    }
    return $pass;
}

sub find_partial_coverage {
    # determine if a given word (indicated by its start and end times)
    # partially overlap with any metadata object of the given type
    #
    my ($data, $start, $end) = @_;

    my $token;

    foreach $token (@{$data}) {
	my $curr_beg = $token->{TBEG};
	my $curr_end = $token->{TBEG} + $token->{TDUR};

	# case 1       #########
	#        ####
        #
	# case 2 ########
	#                 ####
	#
	# case 3 ########
	#          ####
	#
	# everything else is partially covered
	#
	if (less_than($end, $curr_beg) || equal_to($end, $curr_beg)) {
	    # case 1
	    next;
	} elsif (greater_than($start, $curr_end) || equal_to($start, $curr_end)) {
	    # case 2
	    next;
	} elsif ( ( greater_than($start, $curr_beg) || equal_to($start, $curr_beg) ) &&
		  ( less_than($end, $curr_end) || equal_to($end, $curr_end) ) ) {
	    # case 3
	    next;
	} else {
	    # others
	    return 1;
	}
    }
    return 0;
}

sub find_type {
    # determine if a given word (indicated by its start and end times)
    # belongs to any metadata object of the given type
    #
    my ($data, $start, $end) = @_;

    my $token;
    foreach $token (@{$data}) {
	if ($start >= $token->{TBEG} && $end <= $token->{TBEG} + $token->{TDUR}) {
	    return 1;
	}
    }
    return 0;
}

sub find_word {
    # determine if a given metadata object (indicated by its start and end times)
    # contains any word
    #
    my ($data, $start, $end) = @_;

    my $token;
    foreach $token (@{$data}) {
	if ($token->{TBEG} >= $start && $token->{TBEG} + $token->{TDUR} <= $end) {
	    return 1;
	}
    }
    return 0;
}

sub find_speaker {
    # determine if a given speaker is in the data
    #
    my ($src_spkr, $data) = @_;

    my ($src, $chnl, $spkr, $token, $curr_spkr);
    foreach $src (keys %{$data}) {
	foreach $chnl (keys %{$data->{$src}}) {
	    foreach $spkr (keys %{$data->{$src}{$chnl}}) {
		foreach $token (@{$data->{$src}{$chnl}{$spkr}{'SPKR-INFO'}}) {
		    $curr_spkr = $token->{SPKR};
		    if ($src_spkr =~ /^$curr_spkr$/i) {
			return 1;
		    }
		}
	    }
	}
    }
    return 0;
}

sub less_than {
    my ($a, $b) = @_;
    if ($a + $ROUNDING_THRESHOLD < $b) {
	return 1;
    }
    return 0;
}

sub greater_than {
    my ($a, $b) = @_;
    if ($a > $b + $ROUNDING_THRESHOLD) {
	return 1;
    }
    return 0;
}

sub equal_to {
    my ($a, $b) = @_;
    if ( abs($a - $b) < $ROUNDING_THRESHOLD) {
	return 1;
    }
    return 0;
}

sub date_time_stamp {
    my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime();
    my @months = qw(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec);
    my ($date, $time);

    $time = sprintf "%2.2d:%2.2d:%2.2d", $hour, $min, $sec;
    $date = sprintf "%4.4s %3.3s %s", 1900+$year, $months[$mon], $mday;
    return ($date, $time);
}
