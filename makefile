#########   Main Makefile for the sctk  #############


####  Creation Date:  Aug 7, 1997
MAKE=make

check clean dist install config all:
	(cd src; $(MAKE) $@)

distclean:
	(rm -f bin/*)
	(cd src; $(MAKE) $@)

