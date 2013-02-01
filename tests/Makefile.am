# $Id$

check_PROGRAMS = cursinit coordinates margin area eventconnector1     \
    eventqueue1 eventqueue2 eventqueue3 eventqueue4 eventqueue5	      \
    eventqueue6 eventqueue7 eventqueue8 eventqueue9 windowrefs basic1 \
    basic2 resize1 resize2 resize3 resize4 widget1 widget2 pack1 pack2 pack3  \
   pack4 pack5 pack6 pack7 pack8 pack9 packresize1 focus1
# eventqueue9 and resize3 are interactive tests. They must not be listed here
TESTS = $(filter-out eventqueue9 resize3 resize4 focus1, $(check_PROGRAMS))
AM_DEFAULT_SOURCE_EXT = .cc

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_LDFLAGS = -L$(top_srcdir)/src -lyacurs $(LIBPTHREAD) $(LIBRT)
