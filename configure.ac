# $Id$
AC_INIT([libyacurs], [0.1], [http://bugs.guengel.ch])
AC_CONFIG_SRCDIR([src/curs.cc])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])
AC_REVISION([$Revision: 4613 $])

AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT([disable-static])
LT_LANG([C++])

# Conditional compiling on Cygwin: tests using LD_PRELOAD don't just
# work on Cygwin, so we only run them if not built on a cygwin box.
AC_MSG_CHECKING([checking whether we are on Cygwin])
case $host in
     *cygwin*)
	my_cygwin=true
	AC_MSG_RESULT([yes, disabling some tests])
	;;
     *)
        my_cygwin=false
	AC_MSG_RESULT([no])
	;;
esac
AM_CONDITIONAL([NOTCYGWIN],[test x$my_cygwin = xfalse])

#
# Needed on BSD/Linux for libtestpreload.so which is linking back into
# executable.
#
AX_CHECK_LINK_FLAG([-Wl,--export-dynamic],[AC_SUBST([LD_EXPORT_DYNAMIC],[-Wl,--export-dynamic])])

#
# Configure options
#
AC_ARG_ENABLE([debug],
		[AS_HELP_STRING([--enable-debug],
				[enable debug mode])],
		[AS_IF([test x$enableval = xyes],
		       [AC_SUBST([ENABLEDEBUG], [-DDEBUG])],
		       [AC_SUBST([ENABLEDEBUG], [-DNDEBUG])])],
		[AC_SUBST([ENABLEDEBUG],[-DNDEBUG])])

AC_ARG_ENABLE([eventqueue-debug],
		[AS_HELP_STRING([--enable-eventqueue-debug],
				[enable event queue debug mode])],
		[AS_IF([test x$enableval = xyes],
		       [AC_SUBST([EVTQDEBUG], [-DEVTQDEBUG])],
		       [AC_SUBST([EVTQDEBUG], [-DNEVTQDEBUG])])],
		[AC_SUBST([EVTQDEBUG],[-DNEVTQDEBUG])])

AC_LANG([C++])
AM_GNU_GETTEXT([external])

AC_CHECK_HEADERS([stropts.h signal.h sys/termios.h termios.h unistd.h stdint.h sys/types.h sys/ioctl.h])

AX_CXX_HEADER_STDCXX_98
AS_IF([test x$ax_cv_cxx_stdcxx_98 = xno],
	    [AC_MSG_ERROR([ISO C++ 98 not supported])])

AX_MY_WITH_CURSES
AS_IF([test x$ax_cv_curses != xyes],
	    [AC_MSG_ERROR([no curses library found])])


# resize_term() is NCurses, resizeterm() is NetBSD curses
my_saved_LIBS="$LIBS"
LIBS="$my_save_LIBS $CURSES_LIB"
AC_CHECK_FUNC([resize_term],
	[AC_DEFINE([HAVE_RESIZE_TERM],[1],[Define to 1 if you have the `resize_term' function.])],
	[AC_CHECK_FUNC([resizeterm],
		[AC_DEFINE([HAVE_RESIZETERM],[1],[Define to 1 if you have the `resizeterm' function.])])])
LIBS="$my_saved_LIBS"

# Some custom tests
AC_MSG_CHECKING([whether waddstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); waddstr(stdscr, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([WADDSTR_USE_CHAR],[1],[Set to 1 if waddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether waddnstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); waddnstr(stdscr, teststr, 4); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([WADDNSTR_USE_CHAR],[1],[Set to 1 if waddnstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddstr(stdscr, 2, 1, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDSTR_USE_CHAR],[1],[Set to 1 if mvwaddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddnstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddnstr(stdscr, 2, 1, teststr, 4); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDNSTR_USE_CHAR],[1],[Set to 1 if mvwaddnstr() doesn't use const char*])])

AC_MSG_CHECKING([whether keypad() return int])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
	#else
	#include <curses.h>
	#endif
	int retval=100;]],
				  [[initscr(); retval = keypad(stdscr, TRUE); endwin();]])],
	[AC_MSG_RESULT([yes])
	AC_DEFINE([KEYPAD_RETURN_INT],[1],[Set to 1 if keypad has a return value])],
	[AC_MSG_RESULT([no])])
AC_CONFIG_HEADERS([config.h include/libyacurscfg.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile include/Makefile po/Makefile.in])
AC_OUTPUT

AC_MSG_NOTICE([])
AC_MSG_NOTICE([  host              $host])
AC_MSG_NOTICE([  prefix            $prefix])
AC_MSG_NOTICE([  Use NLS           $USE_NLS])
AC_MSG_NOTICE([  Curses library    $CURSES_LIB])
AC_MSG_NOTICE([  Debug mode        $ENABLEDEBUG])
AC_MSG_NOTICE([  Event Queue Debug $EVTQDEBUG])
