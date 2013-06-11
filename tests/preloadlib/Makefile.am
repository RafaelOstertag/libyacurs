# $Id$

AM_LDFLAGS  = -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
if NOTCYGWIN
# LD_EXPORT_DYNAMIC is basically only used for interactive_tests and
# only works on non-cygwin systems
AM_LDFLAGS += $(LD_EXPORT_DYNAMIC)

libtestpreload_la

else
EXTRA_DIST = testpreload.cc
endif
