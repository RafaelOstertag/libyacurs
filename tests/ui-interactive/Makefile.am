# $Id$

check_PROGRAMS = app1 checkbox1 dialog1 filedialog1 focus1 inputbox1 listbox1 listbox2 lockscreen1 messagebox1 packremove radiobox1 resize3 resize4 resize5

AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -DLOCALEDIR=\"$(localedir)\"	\
-I$(top_srcdir)/include $(ENABLEDEBUG) $(EVTQDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)

check-local: $(check_PROGRAMS)
	for f in $(check_PROGRAMS) ; do \
		$(LIBTOOL) --mode=execute env LD_PRELOAD="$(top_builddir)/tests/preloadlib/.libs/libtestpreload.so" $$f ; \
		test "x$$?" = "x0" || { echo "*** $$f failed" ; exit 1 ; } ; \
	done
endif
