# $Id$
AC_INIT([libyacurs],[0.1], [rafi@guengel.ch])
AC_CONFIG_SRCDIR([src/curs.cc])
AM_INIT_AUTOMAKE
AC_REVISION([$Revision: 4613 $])

AC_CONFIG_MACRO_DIR([m4])


AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
AM_PROG_LIBTOOL
LT_INIT

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


AX_CHECK_LINK_FLAG([-Wl,--export-dynamic],[AC_SUBST([LD_EXPORT_DYNAMIC],[-Wl,--export-dynamic])])



AC_LANG([C++])
AM_GNU_GETTEXT([external])

AC_CHECK_HEADERS([stropts.h signal.h sys/termios.h termios.h unistd.h stdint.h sys/types.h sys/ioctl.h])
AC_CHECK_HEADERS([cerrno cstdlib cstring exception string stdexcept stack iostream cstdio queue list algorithm functional typeinfo cassert cstdint])

AC_CHECK_HEADER([ncurses.h],
	[
		have_ncurses_h=yes
		AC_DEFINE([HAVE_NCURSES_H],[1],
			[Define to 1 if you have ncurses.h])
	],[
		have_ncurses_h=no
	]
)
AC_CHECK_HEADER([curses.h],
	[
		have_curses_h=yes
		AC_DEFINE([HAVE_CURSES_H],[1],[Define to 1 if you have curses.h])
	],[
		have_curses_h=no
	]
)

AS_IF([test "x$have_ncurses_h" = xyes],
	    [AC_CHECK_LIB([ncurses],[initscr],[],
		[AC_CHECK_LIB([curses],[initscr],[],
			[AC_MSG_ERROR([curses/ncurses library required])])]
		)],
      [test "x$have_curses_h" = xyes],
      	    [AC_CHECK_LIB([curses],[initscr],[],
		[AC_MSG_ERROR([curses library required])]
	    )],
      [AC_MSG_ERROR([neither ncurses nor curses library available])])

# resize_term() is NCurses, resizeterm() is NetBSD curses
AC_CHECK_FUNCS([resize_term resizeterm])

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
