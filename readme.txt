File:  README
Date:  July 30, 2004
SCTK Version: 1.3

This directory contains SCTK, the NIST Scoring Toolkit.  SCTK contains
the source code, documentation and example data for the following
programs:

	sclite   V2.3 - "Score Lite",
	sc_stats V1.2 - sclite's Statistical Significance Comparison tool
	rover    V0.1 - Recognition Output Voting Error Reduction

The distribution was initially written in ANSI C on a SUN Workstation.  To
date, The code has been successfully compiled and tested on the
following platforms:

					      SCTK
Systems                                     Version 
---------------------------------------------------------------------     
SUN SunOS 4.1.4 using gcc                     
SUN Solaris SunOS 5.4 using cc                
SUN Solaris SunOS 5.5 using gcc               1.0 1.1 1.2
SGI IRIX 5.3 using cc                         
SGI IRIX 4.0.3H using gcc                     
HP 735 using gcc                              
HP using "cc"                                 1.0 1.1 1.2
DEC Alpha running DEC OSF/1 V2.0 "cc -std1"   
DEC Alpha running DEC OSF/1 V2.0 "gcc"            1.1
IBM Powerstation running AIX V3.2 using cc    1.0
IBM Powerstation running AIX V3.2 using gcc*  1.0
Linux Version 2.0.33                          1.0 1.1 1.2
Cygwin                                                    1.3
Linux Version 2.2.17                                      1.3
Linux Version 2.4.20-20-9 **                              1.3
Mac Darwin 7.4.0                                          1.3

	* compiled with warning messages, but successful self test
	** compiled and ran, but test6, which uses GNU's diff for
           alignment fails.  The use of DIFF for alignment is rarely
           used and the failure can be safely ignored.

Read install.txt to compile and install the package.  UNIX-style
manual pages and 'HTML' formatted manuals are provided in the 'doc'
directory.

			  Bundled Software:

This version of SCTK includes the CMU-Cambridge Statistical Language
Modeling Toolkit v2.  Included in STCK is the complete source code
tree and documentation.  The toolkit has been included so that SCLITE
can use a language model to compute conditional word weights for
word-weighted scoring.


			  Optional Software:
	
Sclite has the ability to use GNU's "diff" utility as an alternate 
string alignment method.  The installer has the choice whether or not
to include this ability in the compiled version.  

If the use of GNU's "diff" is desired, you will need GNU's "diff"
utility from their "diffutils" distribution version 2.7.  If you can
not locate this distribution, it is available via anonymous ftp from
the jaguar.ncsl.nist.gov server.  The compressed tar file
'diffutils-2.7.tar.Z' is located in the 'pub' directory.  When you
install diff, make sure the executable is installed as: "gdiff",
"gnudiff" or "diffgnu".  The 'config.sh' script expects it to be
called one of these.

Special Note to SGI owners: When installing GNU's "diff" make sure to
call it "gnudiff" or "diffgnu", if you use "gdiff" the executable name
may be overshadowed by a pre-existing executable.
