#!/bin/sh

usage="$0 <stm validation script location>"

if [ "$#" != '1' ]; then
    echo Script missing
    echo $usage
    exit 1
fi

stm=$1
if [ ! -x "$stm" ]; then
	echo Script not executable
    echo $usage
    exit 1
fi

for file in test*.stm ; do
    echo "Testing $file..."
    base=`echo $file | perl -pe 's/.stm//'`
    log="$base.log.saved"
    tmp="$base.log.tmp"
    
    if [ ! -f "$log" ] ; then
		$stm -v -i $file > $log
    fi

    $stm -v -i $file > $tmp
    diff_status=`diff $log $tmp | wc -l`

    if [ $diff_status -ne 0 ] ; then
		echo "   Output log differs from saved log"
	else
		rm $tmp
    fi
done
