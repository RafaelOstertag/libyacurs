# $Id$

AUTOMAKE_OPTIONS = serial-tests

non_interactive_tests = cursinit coordinates margin area colorparser	\
    eventconnector1 eventqueue1 eventqueue2 eventqueue3 eventqueue4	\
    eventqueue5 eventqueue6 eventqueue7 eventqueue8 curswin1 curswin2	\
    basic1 basic2 resize1 resize2 pack1 pack2 pack3 pack4 pack5 pack6	\
    pack7 pack8 pack9 packresize1 dynlabel1 widget1 widget2 widget3

interactive_tests = eventqueue9 resize3 resize4 resize5 focus1 app1	\
    listbox1 listbox2 dialog1 messagebox1 inputbox1 filedialog1		\
    checkbox1 radiobox1 packremove lockscreen1

check_PROGRAMS  =  $(non_interactive_tests) $(interactive_tests)

# interactive tests must not be listed here
TESTS = $(non_interactive_tests)

AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/include	\
$(ENABLEDEBUG) $(EVTQDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)
endif


EXTRA_DIST = testpreload.cc

if NOTCYGWIN
all-local: libtestpreload.la
clean-local:
	rm -f libtestpreload.la

libtestpreload.la: testpreload.lo
	$(LIBTOOL) --tag=CXX --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -shared -rpath /usr/local/lib -o $@ $<

testpreload.lo: testpreload.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -shared -c -o $@ $<

check-local: $(interactive_tests)
	for f in $(interactive_tests) ; do \
		$(LIBTOOL) --mode=execute env LD_PRELOAD="$(builddir)/.libs/libtestpreload.so" $$f ; \
		test "x$$?" = "x0" || { echo "*** $$f failed" ; exit 1 ; } ; \
	done
endif
