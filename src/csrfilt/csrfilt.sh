#!/bin/sh
#
#  File:  csrfilt.sh
#  Date:  Nov 25, 1995
#  Usage: csrfilt.sh global-map-file [ utterance-map-file ] < infile > outfile
#         Filter the input for stdin, and write the output to stdout
#
#  Desc:  This filter is used to "pre-filter" .lsn hypothesis and
#          reference transcriptions to apply a set of word-mapping rules
#          prior to scoring.  The word-mapping rules permit the 
#          elimination of ambiguous lexical representations.  The
#          filter applies the rules located in two word map files to the
#          transcriptions.  The first word map file, "glYYMMDD.map",
#          applies a set of rules globally to all transcriptions.  The
#          second, "utYYMMDD.map", applies particular rules to particular
#          utterances.  The two .map files are named so as to indicated
#          the last date they were updated.
#
#         There are 2 stages to the filter:
#         1.  Applies the utterance-specific, "utYYMMDD.map", rules to input. 
# 		To accomplish this, the perl filter,
#		'ut_rfilt.pl', is called which in turn calls rfilter1
# 		on the specified utterances.
#	  2.  Applies the global, 'glYYMMDD.map', rules to all utterances.
#		
#  Version 1.0 Feb 1, 1993
#  Version 1.1 Sep. 21, 1995
#       JGF added the option to automatically split hyphenated words
#       JGF added the option to interpret ctm files appropriately
#  Version 1.2 Oct. 20, 1995
#       JGF added the ability to pass through comments un-scathed
#       JGF changed the uppercase conversion to not effect comment lines
#  Version 1.3 Jan 17, 1996
#       JGF forced the script to add spaces to the end and beginning of each non-comment
#           line before processing.  After processing, the spaces are deleted
#  Version 1.4 Nov 24, 1996
#       WMF replaced rfilter1 with a new version which runs faster, and can handle
#           context sensitive rules
#       JGF added a new input format STM
#       JGF Made the glm file sensitive to the input type by using the notation:
#           ;;  INPUT_DEPENDENT_APPLICATION = "<REGEXP>"
#           anywhere in the glm file.  The <REGEXP> tag is a perl regular
#           expression which is matched against the input type via the
#           '-i' option.
#  Version 1.5 Aug 4, 1997
#       JGF added the "-i trn" option
#       JGF modified all exit codes to be 0 on successful, completion 1 otherwise
#  Version 1.6
#       JGF made the utterance-map-file optional. . . finally
#       JGF add the -s option
#  Version 1.7
#       JGF added the -e option to tell set the code to extended ascii
#  Version 1.8 Released around April 1998
#       JGF add the -i txt option
#  Version 1.9 Released Sept 8. 1998
#       JGF added filtering step to rfilter1 to use optionally deletable parens
#       JGF removed a missed usage of sed!!!
#  Version 1.10 
#       JGF Added rttm input type
#  Version 1.11
#       JGF Removed the use of EXE_DIR.  All utilities are expected in the path

version="1.11"

trap "/bin/rm -f /tmp/hs_filt.*.$$ ; exit 1" 1 2 3 15

# Change to the installation directory in order to execute from 
# any where other than the installation directory.
DeleteHyphens=false
UpCase=true;
ExtASCII=false;

Usage="Usage: csrfilt.sh [ -dh -s -e ] -i [ stm | ctm ] global-map-file [ utterance-map-file ] < infile > outfile
Version: $version
Desc: csrfilt.sh applys a set of text transformation rules contained in 
      'global-map-file' and the optional 'utterance-map-file' to the input
      text.  The text input is read from stdin and the modified text goes to stdout.

      The file 'rules.doc' within the distribution directory describes the
      rule file format.

    -dh     ->  replaces hyphens with spaces in hyphenated words
    -i ctm  ->  set the input type to ctm.  If a word from a ctm file is
                split, the script divides the original word's duration among
		it's constituents.
    -i stm  ->  sets the input type to stm.  This forces the mapping rules to
		apply to the text field only.
    -i trn  ->  sets the input type to trn.  This forces the mapping rules to
		apply to the text portion of the trn record.  (Default)
    -i txt  ->  sets the input type to txt, no formatting, all words are fair game.
    -i rttm ->  sets the input type to rttm.
    -s      ->  do not up-case everything
    -e      ->  textual data is extended ASCII"


inputtype="trn"
while test ! "`echo $1| egrep '^-'`" = "" ; do
    if test "$1" = "-dh" ; then
	DeleteHyphens=true
    elif test "$1" = "-s" ; then
	UpCase=false
    elif test "$1" = "-e" ; then
	ExtASCII=true
    elif test "$1" = "-i" ; then
	if test "$2" = "ctm" -o "$2" = "stm" -o "$2" = "trn" -o "$2" = "txt" -o "$2" = "rttm" ; then
	    inputtype=$2
	    shift
	else
	    echo "$Usage"
	    echo "$2"
	    echo "Error: -i option requires either \"ctm\", \"stm\", \"txt\" or \"trn\" not $2"
	    exit 1
	fi
    else
	echo "$Usage"
	echo ""
	echo "Error: Unrecognized option $1"
	 exit 1
    fi
    shift;
done

if test $# -lt 1 -o $# -gt 2 ; then
	echo "$Usage"
	echo ""
	echo "Error: requires one or two command line arguements"
	exit 1
fi

glob_map=$1
if test ! -f "$glob_map" ; then
	echo "$Usage"
	echo ""
	echo "Error: Global-map-file '$glob_map' does not exist"
	exit 1
fi

if test $# -eq 2 ; then
	utt_map=$2
	if  test ! -f "$utt_map" ; then
		echo "$Usage"
		echo ""
		echo "Error: Utterance-map-file '$utt_map' does not exist"
		exit 1
	fi
else
	utt_map=""
fi

# capture and clean up the input
perl -pe 'if ($_ !~ /^;;/) {
	if ("'$UpCase'" eq "true") { 
	   tr/a-z/A-Z/; 
	   if ("'$ExtASCII'" eq "true") { tr/\340-\377/\300-\337/; }
        }
	s/^/ /; s/$/ /; s/\(/ \(/; s/[ \t]+/ /g; }' > /tmp/hs_filt.orig.$$

# automatically modify the global mapping file to only include regions
# which apply to the input type.

cat $glob_map | perl -e '
	($type="'$inputtype'") =~ tr/A-Z/a-z/;
	$applies = 1;
	while (<>){
		if ($_ =~ /^;;\s+INPUT_DEPENDENT_APPLICATION\s*=\s*\"([^\"]*)\"/){
			($exp = $1) =~ tr/A-Z/a-z/;
			if ($type =~ /$exp/){ $applies = 1; } else { $applies = 0; }
		}
		if ($applies == 1){	print;		}
	} ' >  /tmp/hs_filt.glm.$$

# perform the filter on the utterance specific rules
if test "$inputtype" = "ctm" ; then
	cp /tmp/hs_filt.orig.$$ /tmp/hs_filt.out.$$
elif test "$inputtype" = "rttm" ; then
	cp /tmp/hs_filt.orig.$$ /tmp/hs_filt.out.$$
elif test "$inputtype" = "stm" ; then
	cat /tmp/hs_filt.orig.$$ | \
	perl -e '$out1 = "'/tmp/hs_filt.outext.$$'";  $out2 = "'/tmp/hs_filt.out.$$'"; 
		 open(IN,"<-") || die("Error: failed to open input stm file");
		 open(OUT1,">$out1") || die("Error: failed to open output \"$out1\" of the stm file");
		 open(OUT2,">$out2") || die("Error: failed to open output \"$out2\" of the stm file");
		 while(<IN>){
			 if ($_ =~ /^;;/) { 
				 print OUT1 $_; 
				 print OUT2 "\n";
			 } elsif ($_ =~ /^(\s*\S+\s*\S+\s*\S+\s*\S+\s*\S+\s*<\S+>)(.*)$/){
				 print OUT1 $1."\n";
				 print OUT2 $2."\n";
			 } elsif ($_ =~ /^(\s*\S+\s*\S+\s*\S+\s*\S+\s*\S+)(.*)$/){
				 print OUT1 $1."\n";
				 print OUT2 $2."\n";		
			 } else {
				 die("Error: Parse of stm line failed");
			 }
		 }
		 close IN; close OUT1; close OUT2;'
elif test "$inputtype" = "trn" ; then
         cat /tmp/hs_filt.orig.$$ | \
	 perl -e '$out1 = "'/tmp/hs_filt.outext.$$'";  $out2 = "'/tmp/hs_filt.out.$$'"; 
		 open(IN,"<-") || die("Error: failed to open input stm file");
		 open(OUT1,">$out1") || die("Error: failed to open output \"$out1\" of the stm file");
		 open(OUT2,">$out2") || die("Error: failed to open output \"$out2\" of the stm file");
		 while(<IN>){
			 if ($_ =~ /^;;/) { 
				 print OUT2 $_; 
				 print OUT1 "\n";
			 } elsif ($_ =~ /^(.*)(\(.*\))\s*$/){
				 print OUT2 $1."\n";
				 print OUT1 $2."\n";
			} else {
				die("Error: Parse of trn line failed");
			}
		}
		close IN; close OUT1; close OUT2; '
		# echo "outext "; cat /tmp/hs_filt.outext.$$
		# echo "out"; 	cat /tmp/hs_filt.out.$$
elif test "$inputtype" = "txt" ; then
	 cat /tmp/hs_filt.orig.$$ | \
	 perl -e '$out1 = "'/tmp/hs_filt.outext.$$'";  $out2 = "'/tmp/hs_filt.out.$$'"; 
		 open(IN,"<-") || die("Error: failed to open input stm file");
		 open(OUT1,">$out1") || die("Error: failed to open output \"$out1\" of the stm file");
		 open(OUT2,">$out2") || die("Error: failed to open output \"$out2\" of the stm file");
		 while(<IN>){
			 if ($_ =~ /^;;/) { 
				 print OUT1 $_; 
				 print OUT2 "\n";
			 } else {
				 print OUT1 "\n";
				 print OUT2 $_;
 			 }
		}
		close IN; close OUT1; close OUT2; '
		# echo "outext "; cat /tmp/hs_filt.outext.$$
		# echo "out"; 	cat /tmp/hs_filt.out.$$
else 
	if test ! "$utt_map" = "" ; then
	        ut_rfilt.pl $utt_map /tmp/hs_filt.orig.$$ \
			/tmp/hs_filt.out.$$
	else 
		cp /tmp/hs_filt.orig.$$ /tmp/hs_filt.out.$$
	fi
fi

filt_com="cat /tmp/hs_filt.out.$$ | perl -pe 's/\(/( /g; s/\)/ )/g;' | rfilter1 /tmp/hs_filt.glm.$$ | perl -pe 's/\(\s+/(/g; s/\s+\)/)/g;' "
if test "$DeleteHyphens" = "true" ; then
    filt_com="$filt_com | perl -pe 's/([^ \(0-9])-([^ \)0-9])/\$1 \$2/g; s/([^\(\) 0-9])-([^\(\) 0-9])/\$1 \$2/g'"
fi

if test "$inputtype" = "ctm" ; then
cat > /tmp/hs_filt.ctm.$$ << EOF
  while (<>){
    if (\$_ =~ /^;;/) {print} 
    else {
	s/^\s+//;
	s/([{}])/ \1 /g;
	@l = split;
	if (\$#l == 4) {print}
	else {
	    if (\$l[\$#l] =~ /^[0-9\.-]*\$/) {\$limit=\$#l-1;\$conf=\$l[\$#l]} 
	    else {                         \$limit=\$#l;\$conf=""} ; 

	    \$data = join(" ",splice(@l,4,\$limit - 4 + 1));
	    @sets = split(/\//,\$data);
	    if (\$#sets > 0) {
		printf "%s %s * * %s <ALT_BEGIN>\n",\$l[0],\$l[1],\$word,\$conf;
	    }
	    for (\$sn = 0; \$sn <= \$#sets; \$sn++){
		\$set = \$sets[\$sn];
		\$set =~ s/[{}]//g;
		\$set =~ s/^\s+//;
		@words = split(/\s+/,\$set);
		\$dur = \$l[3] / (\$#words + 1);
		\$i=0;
		foreach \$word(@words){
		    printf "%s %s %.3f %.3f %s %s\n",\$l[0],\$l[1],\$l[2]+(\$dur * \$i),\$dur,\$word,\$conf;
		    \$i ++;
		}
		if (\$#sets > 0 && \$sn != \$#sets){
		    printf "%s %s * * %s <ALT>\n",\$l[0],\$l[1],\$word,\$conf;
		}
	    }
	    if (\$#sets > 0) {
		printf "%s %s * * %s <ALT_END>\n",\$l[0],\$l[1],\$word,\$conf;
	    }
	}
    }
  }
EOF
    filt_com="$filt_com |perl /tmp/hs_filt.ctm.$$"
elif test "$inputtype" = "rttm" ; then
cat > /tmp/hs_filt.ctm.$$ << EOF
while (<>){
    if (\$_ =~ /^;;/) {print}
    else {
        s/^\s+//;
        s/([{}])/ \1 /g;
        @l = split;
        if (\$#l == 8) {print}
        else {
                (\$start, \$dur) = splice(@l, 3, 2);
                @words = splice(@l, 3, @l-6);
                \$i=0;
                foreach \$word(@words){
                    print "\$l[0] \$l[1] \$l[2] ".(\$start + (\$dur/@words*\$i))." ".(\$dur/@words)." \$word \$l[3] \$l[4] \$l[5]\n";
                    \$i++;
                }
        }
    }
}
EOF
    chmod +x /tmp/hs_filt.ctm.$$
    filt_com="$filt_com | perl /tmp/hs_filt.ctm.$$"
fi

if test "$inputtype" = "ctm" ; then
	eval $filt_com |perl -pe 'if ($_ !~ /^;/) {s/^[ \t]+//; s/[ \t]+$//; s/[ \t]+/ /g}'
elif test "$inputtype" = "rttm" ; then
	eval $filt_com |perl -pe 'if ($_ !~ /^;/) {s/^[ \t]+//; s/[ \t]+$//; s/[ \t]+/ /g}'
elif test "$inputtype" = "stm" ; then
	eval $filt_com | \
	perl -e '$inext = "'/tmp/hs_filt.outext.$$'";
		open(IN,"<$inext") || die("Error: failed to open input \"$out1\" of the stm file");
		while (! eof(STDIN)){
			$text = <STDIN>;
			$ext = <IN>;
			chop($ext);
			print "$ext$text";
		}
		close IN' | \
	perl -pe 'if ($_ !~ /^;/) {s/^[ \t]+//; s/[ \t]+$//; s/[ \t]+/ /g}'
elif test "$inputtype" = "trn" ; then
	eval $filt_com | \
	perl -e '$inext = "'/tmp/hs_filt.outext.$$'";
		open(IN,"<$inext") || die("Error: failed to open input \"$out1\" of the stm file");
		while (! eof(STDIN)){
			$text = <STDIN>;
			$ext = <IN>;
			chop($text);
			print "$text$ext";
		}
		close IN' | \
	perl -pe 'if ($_ !~ /^;/) {s/^[ \t]+//; s/[ \t]+$//; s/[ \t]+/ /g}'
else
	eval $filt_com | \
	perl -pe 'if ($_ !~ /^;/) {s/^[ \t]+//; s/[ \t]+$//; s/[ \t]+/ /g}'
fi


# clean up
/bin/rm -f /tmp/hs_filt.*.$$

exit 0
