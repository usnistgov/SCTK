# SCTK, the NIST Scoring Toolkit

**File:**  README.md

**Date:**  November 11, 2018

**SCTK Version:** 2.4.11

This directory contains SCTK, the NIST Scoring Toolkit.  SCTK contains the 
source code, documentation and example data for the following programs:
- sclite   V2.10  - "Score Lite",
- sc_stats V1.3  - sclite's Statistical Significance Comparison tool
- rover    V0.1  - Recognition Output Voting Error Reduction
- asclite  V1.11 - Multidimensional alignment replacement for sclite

## SCTK Basic Installation

To install compile and install *sctk*, from the main directory type the commands:

	% make config
	% make all
	% make check
	% make install
	% make doc

The `make config` command executes the script `config.sh` in the `src/sclite` directory and initialized the makefiles in the rest of the src directories.  

You can specify installation directory by changing the `PREFIX` variable in the `src/makefile` file.  By default, all executables will be installed in `bin` of this directory.  Be sure to add the installation directory to your `PATH` variable in order to access the programs.

The `make all` command will compile all toolkit executables and libraries. Compilation of `rfilter1` may fail if your version of `libC` is very new because it now includes the `strcmp*` family of functions. You can use the libC versions, and avoid compilation complaints, by following the instructions in  `src/rfilter1/makefile`.

The `make check` will perform self tests on the toolkit.  Please report any problems to Jon Fiscus at NIST with any bug reports or comments at the email address <jonathan.fiscus@nist.gov>.  Please include the version number and any other relevant information such as OS, compiler, etc.

The `make install` command will copy the executables to `$PREFIX` (see `make config` above.)

UNIX-style manual pages and HTML formatted manuals are provided in the `doc` directory.

### Bundled Software

This version of SCTK includes the CMU-Cambridge Statistical Language Modeling Toolkit v2.  Included in STCK is the complete source code tree and documentation.  The toolkit has been included so that SCLITE can use a language model to compute conditional word weights for word-weighted scoring.

### Optional Software/Notes:

**sclite**:
To use the interactive installation for *sclite*, edit the file `src/sclite/config.sh` at line 36, set the `user_interaction=1` it will ask you during the configuration (`make config`) if you want to use *GNU diff* instead of sclite and CMU-Cambridge SLM for sclite.  Those choices are not recommended and not used anymore.

*sclite* has the ability to use GNU's `diff` utility as an alternate string alignment method.  The installer has the choice whether or not to include this ability in the compiled version.  

**GNU diff**:
If the use of GNU's `diff` is desired, you will need GNU's `diff` utility from their `diffutils` distribution (version 2.7).  If you cannot locate this distribution, a copy is available at <ftp://jaguar.ncsl.nist.gov/pub/diffutils-2.7.tar>. When you install `diff`, make sure the executable is installed as `gdiff`, `gnudiff` or `diffgnu` (`config.sh` expects it to be called one of these).

*Special Note to SGI owners: When installing GNU's `diff` make sure to call it `gnudiff` or `diffgnu`. If you use `gdiff` the executable name may be overshadowed by a pre-existing executable.*

**utf\_filt**:
`utf_filt` is using SGML external software and modules like SGMLS perl module and `nsgmls` program which have been updated and continued: OpenSP (<http://sourceforge.net/project/showfiles.php?group_id=2115&package_id=2869>). Version v1.5.2 of OpenSP have been tested and working with the current 
version of `utf_filt`.

*Special Note to Cygwin users:* it has been reported that compilation of `rfilter1` can fail in some case, please read the OPTIONS part of the `rfilter1/makefile` and adapt accordingly before retrying compilation.


**64 bits Compilation**:
With big alignments, sctk needs to be compiled in 64 bits. By default, the C/C++ software are compiled in 32 bits with the options (`-Os`) but can be compiled in 64 bits. To do so, `-m64` is added to the CFLAGS variable in `src/asclite/core/makefile`, `src/asclite/test/makefile`, `src/rfilter1/makefile` and `src/sclite/makefile`.

Example of `CFLAGS` for OSX 10.4+: `-fast -m64 -arch x86_64 -arch ppc64`

## Known to compile on:

The distribution was initially written in ANSI C on a SUN Workstation.  To date, the code has been successfully compiled and tested on the following platforms:

Systems                                       | Version 
------------------------------------------    |---------------------------     
SUN SunOS 4.1.4 using gcc                     |   
SUN Solaris SunOS 5.4 using cc                |   
SUN Solaris SunOS 5.5 using gcc               |   1.0 - 1.2
SGI IRIX 5.3 using cc                         |   
SGI IRIX 4.0.3H using gcc                     |   
HP 735 using gcc                              |   
HP using "cc"                                 | 1.0 - 1.2
DEC Alpha running DEC OSF/1 V2.0 "cc -std1"   |  
DEC Alpha running DEC OSF/1 V2.0 "gcc"        | 1.1
IBM Powerstation running AIX V3.2 using cc    | 1.0
IBM Powerstation running AIX V3.2 using gcc [*]  | 1.0
Linux Version 2.0.33                          | 1.0 - 1.2
Cygwin                                        | 1.3 - 2.0B2
Linux Version 2.2.17                          | 1.3 - 2.0B2
Linux Version 2.4.20-20-9 [**]                  | 1.3 - 2.1.7
Linux Version 2.6.19.fc5                      | 2.2.0 - 2.2.4
Linux Version 2.6.9-5.EL (gcc 3.4.6-3)        | 2.2.0 - 2.2.4
Darwin Version 7.4.0                          | 1.3
Darwin Version 8.5.0 (gcc v4.0.1 build 5367)  | 2.1.2 - 2.3-rc1 
Darwin Version 8.11.0 (gcc v4.0.1 build 5367) | 2.3-rc2 - 2.4.0
Darwin Version 9.6.0 (gcc v4.0.1 build 5490)  | 2.3.6 - 2.4.0
RedHat Fedora 9 (gcc v4.3.0-8)                | 2.3.9 - 2.4.0
Darwin Version 10.8.0 (gcc version 4.2.1 (Apple Inc. build 5666) | 2.4.2 - 2.4.8

[\*] compiled with warning messages, but successful self test

[\*\*] compiled and ran, but test6, which uses GNU's diff for alignment fails.  The use of DIFF for alignment is rarely used and the failure can be safely ignored.

## Docker

A Dockerfile is included to build and use SCTK without compiling the codebase for your particular platform.

To build:

```
docker build -t sctk .
```

### Hypothesis Test
To run `sclite` on a reference file and hypothesis file, you need to map a volume to your host.

Linux or a Mac:
```
docker run -it -v $PWD:/var/sctk sctk sclite -i wsj -r ref.txt -h hyp.txt
```
Windows:
```
docker run -it -v /myhostpath:/var/sctk sctk sclite -i wsj -r ref.txt -h hyp.txt
```

### sc_stats
To run more advanced features of the SDK, shell into the container with a volume mapped to your local files:
```
docker run -it -v $PWD:/var/sctk sctk sh
/var/sctk #
```
Then run sclite piped to sc_stats:
```
sclite -i wsj -r ref.txt -h hyp1.txt -h hyp2.txt -o sgml stdout | sc_stats -p -t mapsswe -u
sc_stats: 1.3
Beginning Multi-System comparisons and reports
    Performing the Matched Pair Sentence Segment (Word Error) Test
    Printing Unified Statistical Test Reports
        Output written to 'Ensemble.stats.unified'

Successful Completion
```

## Contact

If you have questions, please address them to Jonathan Fiscus at <jonathan.fiscus@nist.gov>

## License

See LICENSE.md for details or <http://www.nist.gov/open/license.cfm>
