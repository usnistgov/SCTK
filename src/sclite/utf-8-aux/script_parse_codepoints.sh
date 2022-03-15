#!/bin


### Cut/paste from https://www.utf8-chartable.de/unicode-utf8-table.pl?start=1024
cpd=codepoints.txt

grep 'CYRILLIC CAPITAL' $cpd | sed 's/.*CYRILLIC CAPITAL //' | sort -u | \
    while read letter ; do
	n=`egrep " $letter\$" $cpd | wc -l | awk '{print $1}'`
	echo "letter: $letter" n=$n
	if [ "$n" != 2 ] ; then
	    echo Fail
	else
	    ### { 0x00D1, 0x00f1}  //Ñ ->ñ};
	    sm_hex=`cat $cpd | grep "SMALL $letter$" | awk '{print "0x"$1}' | sed 's/U+//'`
	    sm_chr=`cat $cpd | grep "SMALL $letter$" | awk '{print $2}'`
	    ca_hex=`cat $cpd | grep "CAPITAL $letter$" | awk '{print "0x"$1}' | sed 's/U+//'`
	    ca_chr=`cat $cpd | grep "CAPITAL $letter$" | awk '{print $2}'`
	    echo "  { $ca_hex, $sm_hex},  // $ca_chr, $sm_chr  -- $letter"
	fi
    done

