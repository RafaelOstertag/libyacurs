# $Id$

lib_LTLIBRARIES = libtestpreload.la

libtestpreload_la_SOURCES = testpreload.cc

libtestpreload_la_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1			\
-DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/include $(ENABLEDEBUG)	\
-I$(top_builddir) $(EVTQDEBUG)

libtestpreload_la_LDFLAGS  = -no-install -L$(top_builddir)/src -lyacurs  $(LTLIBINTL)
