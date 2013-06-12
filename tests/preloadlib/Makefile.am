# $Id$

EXTRA_DIST = testpreload.cc

AM_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -DLOCALEDIR=\"$(localedir)\" \
-I$(top_srcdir)/include $(ENABLEDEBUG) -I$(top_builddir) $(EVTQDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)

CXXLD = @CXXLD@

if NOTCYGWIN
all-local: libtestpreload.la
clean-local:
	rm -f libtestpreload.la

libtestpreload.la: testpreload.lo
	$(LIBTOOL) --tag=CXX --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -shared -rpath /usr/local/lib -o $@ $<

testpreload.lo: testpreload.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -shared -c -o $@ $<
endif


