# $Id$

check_PROGRAMS = cursinit coordinates margin area eventconnector1 eventqueue1 \
  eventqueue2 eventqueue3 eventqueue4 eventqueue5 eventqueue6 eventqueue7     \
  eventqueue8 eventqueue9 windowrefs basic1 basic2 resize1 resize2 resize3    \
  widget1 widget2 widget3
# eventqueue9 and resize3 are interactive tests. They must not be listed here
TESTS = $(filter-out eventqueue9 resize3, $(check_PROGRAMS))
AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/src -lyacurs
