# $Id$

AUTOMAKE_OPTIONS = serial-tests

check_PROGRAMS = input1 app1 checkbox1 dialog1 filedialog1		\
filedialog2 filedialog3 focus1 inputbox1 listbox1 listbox2		\
lockscreen1 messagebox1 messagebox2 messagebox3 packremove radiobox1	\
resize3 resize4 resize5

AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -DLOCALEDIR=\"$(localedir)\"	\
-I$(top_srcdir)/include $(ENABLEDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)

check-local: $(check_PROGRAMS)
	for f in $(check_PROGRAMS) ; do \
		$(LIBTOOL) --mode=execute env LD_PRELOAD="$(top_builddir)/tests/preloadlib/.libs/libtestpreload.so" $(builddir)/$$f ; \
		retval=$$? ; \
		if test "x$$retval" = "x77" ; then \
			echo "*** $$f skipped" ; \
		else \
			test "x$$retval" = "x0" || { echo "*** $$f failed" ; exit 1 ; } ; \
		fi ; \
	done
endif
