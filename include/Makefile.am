# $Id$

EXTRA_DIST = libyacurscfg.h.in

includeyacursdir=$(includedir)/yacurs

# add header files here, they will be put to noinst_HEADERS or
# includeyacurs_HEADERS depending on whether or not the library will
# be installed (--disable-install flag).
MYinstheaders= sigaction.h libyacurscfg.h coordinates.h yacursex.h	\
 curstr.h curswin.h cursorbuf.h curs.h eventconnector.h event.h		\
 eventqueue.h lineobject.h margin.h yacurscurses.h pack.h		\
 realizeable.h area.h windowbase.h titlebar.h statusbar.h vpack.h	\
 widgetbase.h hotkey.h window.h lockscreen.h yacurs.h hpack.h label.h	\
 dynlabel.h size.h widget.h focusgroup.h focusmanager.h button.h	\
 colors.h colorparser.h input.h listbox.h yacursconst.h yacurstypes.h	\
 dialog.h messagebox.h messagebox2.h messagebox3.h inputbox.h		\
 filedialog.h filesavedialog.h fileloaddialog.h checkbox.h radiobox.h	\
 unlockdialog.h unlockdiadefault.h evtqueuestats.h rule.h vrule.h	\
 hrule.h spacer.h yacursdbg.h

if DISABLEINSTALL
noinst_HEADERS = gettext.h $(MYinstheaders)
else
includeyacurs_HEADERS = $(MYinstheaders)
noinst_HEADERS = gettext.h
endif

AM_CPPFLAGS = -DPACKAGE=\"$(PACKAGE)\" -DLOCALEDIR=\"$(localedir)\"	\
$(ENABLEDEBUG)

