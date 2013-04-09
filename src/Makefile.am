# $Id$

lib_LTLIBRARIES = libyacurs.la
libyacurs_la_LDFLAGS = -version-info 0
libyacurs_la_CPPFLAGS = -DPACKAGE=\"$(PACKAGE)\" -DLOCALEDIR=\"$(localedir)\" -I$(top_srcdir)/include
libyacurs_la_SOURCES = coordinates.cc size.cc margin.cc area.cc		\
   curstr.cc curswin.cc event.cc eventconnector.cc eventqueue.cc	\
   evtqueuestats.cc curs.cc realizeable.cc statusline.cc		\
  windowbase.cc window.cc lockscreen.cc lineobject.cc widgetbase.cc	\
  pack.cc vpack.cc hpack.cc widget.cc label.cc dynlabel.cc		\
  yacursex.cc colors.cc colorparser.cc focusgroup.cc focusmanager.cc	\
  button.cc dialog.cc messagebox.cc messagebox2.cc inputbox.cc		\
  filesavedialog.cc fileloaddialog.cc checkbox.cc radiobox.cc		\
  unlockdialog.cc unlockdiadefault.cc

