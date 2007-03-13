#!/usr/bin/perl

use strict;

my $exe = "align2html-main.pl";

my @modules = ();
my @file = ();

open EXE,"<$exe" || die "Failed to open executable $exe";
while (<EXE>){
    if ($_ =~ /my \$Installed = 0;/){
	push @file, "my \$Installed = 1;\n";
    } elsif ($_ =~ /\#\#\# Begin Local Modules/){
	do {
	    if ($_ =~ /use\s+(\S+)\s*;/){
		push @modules, $1;
	    }
	    $_ = <EXE>;
	} while ($_ !~ /\#\#\# End Local Modules/);
	my $modExp = "use\\s+(".join("|",@modules).")";
	#print "Modes = ".join(" ",@modules)." $modExp\n";
	### Insert the modules 
	foreach my $mod(@modules){
	    open (MOD, "<$mod.pm") || die "Failed to open $mod.pm";
	    while (<MOD>){
		push(@file, $_) if ($_ !~ /$modExp/);
	    }
	    close MOD;
	}
	### Reset the package
	push @file, "package main;\n"
    } elsif ($_ =~ /die "HERE DOCUMENT NOT BUILT"/){
        push @file, 'my $here = "";'."\n";
        push @file, "\$here = << 'EOTAR';\n";

#tar cf - ./js | perl -e '' | perl -e 'binmode(STDOUT); while (<STDIN>){ print unpack("u",$_) } ' | tar tvf -

	open TAR, "tar zcf - --exclude CVS ./js/wz_jsgraphics.js ./images/*gif |" || die "TAR/UUENCODE Fialed";
	binmode TAR;
	read(TAR, $_, 90000000);
	push @file, pack("u", $_);
	close TAR;
        push @file, "EOTAR\n";

	push @file, 'open UNTAR, "| (cd $outDir ; tar zxf -)" || die "Failed to UUDECODE/TAR"'.";\n";
	push @file, 'binmode(UNTAR);'."\n";
	push @file, 'print UNTAR unpack("u",$here);'."\n";
	push @file, 'close UNTAR'."\n";
    } else {
	push @file, $_;
    }
}
print @file;

#	cat align2html.pl | perl -ne 'if ($$_ =~ /die "HERE DOCUMENT NOT BUILT"/) { system "cat DATA.tgz.uu" } else { print }' > align2html-combined.pl
