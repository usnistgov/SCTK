#!/bin/sh

usage="$0 <rttm validation script location> [-v]"

if [ $# -lt 1 ]; then
    echo $usage
    exit 1
fi

rttm=$1
exec=`file -L $rttm | perl -pe 's/.*?executable.*/1/'`
if [ "$exec" != '1' ]; then
    echo $usage
    exit 1
fi

if [ "$2" = '-v' ] ; then 
    verbose=true
else
    verbose=false
fi

for file in test*.rttm ; do
    echo "Testing $file..."
    base=`echo $file | perl -pe 's/.rttm//'`
    log="$base.log.saved"
    tmp="$base.log.tmp"
    
    if [ ! -f "$log" ] ; then
		$rttm -t -i $file > $log
    fi

    $rttm -t -i $file > $tmp
    diff_status=`diff -I 'RTTMValidator' $log $tmp | wc -l`

    if [ $diff_status -ne 0 ] ; then
		echo "   Output log differs from saved log"
		
		if [ $verbose = true ] ; then
			diff -I 'RTTMValidator' $log $tmp | sed 's/^/   /'
		fi
	else
		rm $tmp
    fi
done
