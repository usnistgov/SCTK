#########   Main Makefile for the sctk  #############


####  Creation Date:  Aug 26, 2004
MAKE=make
### Set the installation prefix directory to the default.  (If
### relative paths are used, the are relative to two directories below
### 'src'.)
PREFIX=../..

all clean distclean install:
	(cd sclite; $(MAKE) $@)
	(cd rfilter1; $(MAKE) $@)
	(cd csrfilt; $(MAKE) $@)
	(cd chfilt; $(MAKE) $@)
	(cd hamzaNorm; $(MAKE) $@)
	(cd acomp; $(MAKE) $@)	
	(cd def_art; $(MAKE) $@)
	(cd hubscr; $(MAKE) $@)
	(cd utf_filt; $(MAKE) $@)

config:
	(cd sclite; sh config.sh -prefix=${PREFIX})
	(cd rfilter1; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)
	(cd csrfilt; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd chfilt; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd hamzaNorm; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd acomp; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd def_art; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd hubscr; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	
	(cd utf_filt; sed -e 's:PREFIX=.*:PREFIX='${PREFIX}':' < makefile.in > makefile)	

check: all
	(cd sclite; $(MAKE) $@)
	(cd rfilter1; $(MAKE) $@)
	(cd csrfilt; $(MAKE) $@)
	(cd chfilt; $(MAKE) $@)
	(cd hamzaNorm; $(MAKE) $@)
	(cd acomp; $(MAKE) $@)
	(cd def_art; $(MAKE) $@)
	(cd hubscr; $(MAKE) $@)
	(cd utf_filt; $(MAKE) $@)
