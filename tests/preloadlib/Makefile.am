# $Id$

# Building preload library has been taken out of the regular build
# flow, so that we don't have to rely on the noinst_ prefix, which
# doesn't produce a shared library, but a static one unable to be used
# by LD_PRELOAD. However, not using noinst_ prefix, a shared library
# will be built which is also installed. That we don't want either.

EXTRA_DIST = testpreload.cc

myCPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1			\
-DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/include $(ENABLEDEBUG)	\
-I$(top_builddir)

testpreload.lo: testpreload.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) -shared $(STDCXX98_FLAG) $(myCPPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(AM_CPPFLAGS) -DHAVE_CONFIG_H -c -o $(builddir)/$@ $<

libtestpreload.la: testpreload.lo
	$(LIBTOOL) --tag=CXX $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=link $(CXX) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath $(libdir) -version-info 0 -o $@ $<

all-local: libtestpreload.la

clean-local:
	rm -f *.la
	rm -f *.o
	rm -rf .deps
