# -*- mode: makefile -*-
#
# Use -f on autconf and automake to force update of files.
#
# $Id$

VPATH=.:m4

configure: configure.ac config.h.in aclocal.m4 ltmain.sh Makefile.in missing ar-lib depcomp nls.m4 po.m4
	autoconf -f

aclocal.m4: configure.ac libtool.m4 ltoptions.m4 ltversion.m4 ltsugar.m4 
	aclocal -I m4

config.h.in: configure.ac aclocal.m4
	autoheader

ltmain.sh: libtool.m4 ltoptions.m4 ltversion.m4 ltsugar.m4
	libtoolize -c -i

libtool.m4:
	libtoolize -c -i

ltoptions.m4:
	libtoolize -c -i

ltversion.m4:
	libtoolize -c -i

ltsugar.m4:
	libtoolize -c -i

config.rpath:
	touch $@

nls.m4 po.m4:
	gettextize -f

Makefile.in missing ar-lib depcomp: configure.ac Makefile.am NEWS README AUTHORS ChangeLog config.rpath tests/basic/Makefile.am tests/preloadlib/Makefile.am tests/ui-interactive/Makefile.am tests/ui-non-interactive/Makefile.am
	automake -a -c -W all --gnu -f

NEWS:
	touch $@

README:
	touch $@

AUTHORS:
	touch $@

ChangeLog:
	touch $@

clean:
	rm -f aclocal.m4 config.guess config.sub configure install-sh depcomp config.h.in missing ltmain.sh ar-lib config.status stamp-h1 Makefile.in src/Makefile.in tests/Makefile.in config.rpath
	rm -rf autom4te.cache
