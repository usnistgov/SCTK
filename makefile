#########   Main Makefile for the sctk  #############


####  Creation Date:  Aug 7, 1997
MAKE=make

check clean install config all:
	(cd src; $(MAKE) $@)

distclean:
	(rm -f bin/*)
	(cd src; $(MAKE) $@)

dist:
	echo "Make the distribution"
	echo "sctk-"`grep Version: readme.txt | head -1 | perl -pe 's/.*Version:\s+//; s/\s+\$$//; s/\s+/_/g'` > .fname	
	echo "Building a release version" `cat .fname`
	echo "Verifying and up-to-date CVS copy"
	cvs -d casper:/slp/sware/cvs up 
	cvs -d casper:/slp/sware/cvs  co -d `cat .fname` sctk
	echo "Building the TAR file"
	tar czf `cat .fname`"-"`date +%Y%m%d-%H%M`.tgz --exclude CVS `cat .fname` 
	rm -r `cat .fname`
