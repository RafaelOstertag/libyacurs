# $Id$

if DISABLEINSTALL
noinst_LTLIBRARIES = libyacurs.la
else
lib_LTLIBRARIES = libyacurs.la
endif

libyacurs_la_LDFLAGS = -version-info 0 $(CURSES_LIB) $(LTLIBINTL)

libyacurs_la_CPPFLAGS = -D_XOPEN_SOURCE_EXTENDED=1	\
-DPACKAGE=\"$(PACKAGE)\" -DLOCALEDIR=\"$(localedir)\"	\
-I$(top_srcdir)/include $(ENABLEDEBUG)

libyacurs_la_SOURCES = sigaction.cc coordinates.cc size.cc margin.cc	\
 area.cc curstr.cc curswin.cc cursorbuf.cc event.cc eventconnector.cc	\
 eventqueue.cc evtqueuestats.cc curs.cc realizeable.cc titlebar.cc	\
 statusbar.cc windowbase.cc hotkey.cc window.cc lockscreen.cc		\
 lineobject.cc widgetbase.cc pack.cc vpack.cc hpack.cc widget.cc	\
 label.cc dynlabel.cc yacursex.cc colors.cc colorparser.cc		\
 focusgroup.cc focusmanager.cc button.cc dialog.cc messagebox.cc	\
 messagebox2.cc messagebox3.cc inputbox.cc filedialog.cc		\
 filesavedialog.cc fileloaddialog.cc checkbox.cc radiobox.cc		\
 unlockdialog.cc unlockdiadefault.cc rule.cc hrule.cc vrule.cc		\
 spacer.cc yacursdbg.cc

