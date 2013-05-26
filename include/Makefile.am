# $Id$

EXTRA_DIST = libyacurscfg.h.in

includeyacursdir=$(includedir)/yacurs
includeyacurs_HEADERS = sigaction.h libyacurscfg.h coordinates.h	\
 yacursex.h curstr.h curswin.h cursorbuf.h curs.h eventconnector.h	\
 event.h eventqueue.h lineobject.h margin.h mycurses.h pack.h		\
 realizeable.h area.h windowbase.h titlebar.h statusbar.h vpack.h	\
 widgetbase.h hotkey.h window.h lockscreen.h yacurs.h hpack.h label.h	\
 dynlabel.h size.h widget.h focusgroup.h focusmanager.h button.h	\
 colors.h colorparser.h input.h listbox.h yacursconst.h yacurstypes.h	\
 dialog.h messagebox.h messagebox2.h inputbox.h filesavedialog.h	\
 fileloaddialog.h checkbox.h radiobox.h unlockdialog.h			\
 unlockdiadefault.h evtqueuestats.h

noinst_HEADERS = gettext.h

AM_CPPFLAGS = -DPACKAGE=\"$(PACKAGE)\" -DLOCALEDIR=\"$(localedir)\"	\
$(ENABLEDEBUG) $(EVTQDEBUG)
