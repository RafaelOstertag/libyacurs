# $Id$

non_interactive_tests = cursinit coordinates margin area		\
 eventconnector1 eventqueue1 eventqueue2 eventqueue3 eventqueue4	\
 eventqueue5 eventqueue6 eventqueue7 eventqueue8 basic1 basic2		\
 resize1 resize2 pack1 pack2 pack3 pack4 pack5 pack6 pack7 pack8	\
 pack9 packresize1 dynlabel1 widget1 widget2 widget3

interactive_tests = eventqueue9 resize3 resize4 resize5 focus1 app1	\
 listbox1 listbox2 dialog1 messagebox1 inputbox1 filedialog1 checkbox1 radiobox1

check_PROGRAMS =  $(non_interactive_tests) $(interactive_tests)
# interactive tests must not be listed here
TESTS = $(non_interactive_tests)
#TESTS_ENVIRONMENT = LD_PRELOAD="./.libs/libtestpreload.so" ; export LD_PRELOAD ;
AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -I$(top_srcdir)/include
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests
AM_LDFLAGS = -L$(top_srcdir)/src -lyacurs $(LD_EXPORT_DYNAMIC)

all-local: libtestpreload.la
clean-local:
	rm -f libtestpreload.la

libtestpreload.la: testpreload.lo
	$(LIBTOOL) --tag=CXX --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(AM_LDFLAGS) $(LDFLAGS) -rpath /usr/local/lib -o $@ $<

testpreload.lo: testpreload.cc
	$(LIBTOOL) --tag=CXX --mode=compile $(CXX) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o $@ $<

check-local: $(interactive_tests)
	for f in $(interactive_tests) ; do \
		$(LIBTOOL) --mode=execute env LD_PRELOAD="./.libs/libtestpreload.so" $$f ; \
		test "x$$?" = "x0" || { echo "*** $$f failed" ; exit 1 ; } ; \
	done
