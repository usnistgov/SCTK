#########   Main Makefile for the sctk  #############


####  Creation Date:  Aug 7, 1997
MAKE=make

####
sctk: 
	(cd src; $(MAKE))

config:
	(cd src; sh config.sh)

check clean dist distclean install:
	(cd src; $(MAKE) $@)


