# $Id$

AUTOMAKE_OPTIONS = serial-tests

check_PROGRAMS = basic1 basic2 cursinit cursorbuf1 curswin1 curswin2 \
dynlabel1 dynlabel2 pack1 pack2 pack3 pack4 pack5 pack6 pack7 pack8  \
pack9 packresize1 resize1 resize2 widget1 widget2 widget3 listbox1

TESTS = $(check_PROGRAMS)

AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1 -DLOCALEDIR=\"$(localedir)\"	\
-I$(top_srcdir)/include $(ENABLEDEBUG) $(EVTQDEBUG)

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)
endif
