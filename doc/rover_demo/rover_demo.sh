#!/bin/sh
echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 0: Install the executabables:"
echo "    - Follow the compilation and install instructions in README.md"
echo "    - The exectuables are expected to be in ../../bin"
if [ ! -f ../../bin/rover ] ; then echo "Error: the rover executable was not found in ../../bin.  Please 'make install'"; exit 1; fi
if [ ! -f ../../bin/sclite ] ; then echo "Error: the sclite executable was not found in ../../bin.  Please 'make install'"; exit 1; fi
if [ ! -f ../../bin/sc_stats ] ; then echo "Error: the sc_stats executable was not found in ../../bin.  Please 'make install'"; exit 1; fi


echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 1: Run ROVER on three or more systems"
../../bin/rover -h ../images_src/sys1.ctm ctm -h ../images_src/sys2.ctm ctm -h ../images_src/sys3.ctm ctm -o rover.ctm -m avgconf

echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 2: Score the input systems and the rover system"
../../bin/sclite -r ../images_src/ref.stm stm -h rover.ctm ctm -o sgml
../../bin/sclite -r ../images_src/ref.stm stm -h  ../images_src/sys1.ctm ctm -O . -o sgml
../../bin/sclite -r ../images_src/ref.stm stm -h  ../images_src/sys2.ctm ctm -O . -o sgml
../../bin/sclite -r ../images_src/ref.stm stm -h  ../images_src/sys3.ctm ctm -O . -o sgml

echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 3: Produce a summary report"
cat *sgml | ../../bin/sc_stats -p -n Report -r sum prn

echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 4: Review the overall error rates"
cat Report.sys

echo "--------------------------------------------------------------------------------------------------------------------"
echo "Step 5: Review the alignments"
head -40 Report.prn


