# $Id$

AUTOMAKE_OPTIONS = serial-tests

check_PROGRAMS = cursorbuf1 area colorparser coordinates margin \
 eventconnector1 eventqueue1 eventqueue2 eventqueue3 eventqueue4   \
 eventqueue5 eventqueue6 eventqueue7 eventqueue8 eventqueue9

interactive_tests = eventqueue9

TESTS = $(filter-out $(interactive_tests), $(check_PROGRAMS))

AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -DLOCALEDIR=\"$(localedir)\"	\
-I$(top_srcdir)/include $(ENABLEDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs $(CURSES_LIB) $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)
AM_CXXFLAGS = $(STDCXX98_FLAG)


check-local: $(interactive_tests)
	for f in $(interactive_tests) ; do \
		$(LIBTOOL) --mode=execute env LD_PRELOAD="$(top_builddir)/tests/preloadlib/.libs/libtestpreload.so" $(builddir)/$$f ; \
		test "x$$?" = "x0" -o "x$$?" = "x77" || { echo "*** $$f failed" ; exit 1 ; } ; \
	done
endif
