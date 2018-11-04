AC_INIT([libyacurs], [1.0.2-dev], [https://bugs.guengel.ch/enter_bug.cgi?product=YAPET])
AC_CONFIG_SRCDIR([src/curs.cc])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([check-news dist-xz color-tests])
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
LT_INIT
LT_LANG([C++])
AC_LANG([C++])

AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

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

AC_ARG_ENABLE([install],
		[AS_HELP_STRING([--disable-install],
				[disable installation of library and header files])],
		[AM_CONDITIONAL([DISABLEINSTALL],[test x$enableval = xno])],
		[AM_CONDITIONAL([DISABLEINSTALL],[test 1 = 0])])

AC_ARG_ENABLE([wchar],
		[AS_HELP_STRING([--disable-wchar],
				[disable wide character support])],
		[AS_IF([test x$enableval = xno],
		       [AC_DEFINE([DISABLE_WCHAR],[1],[Define to 1 if you want to disable wchar support.])
                        DISABLE_WCHAR=yes],
                       [DISABLE_WCHAR=no])],
		[DISABLE_WCHAR=no])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])

AC_HEADER_TIME
AC_CHECK_HEADERS([fcntl.h locale.h libintl.h stropts.h sys/termios.h termios.h sys/types.h sys/ioctl.h sys/time.h])

# Those are headers we don't protect by #ifdef's
AC_CHECK_HEADERS([stdint.h signal.h unistd.h],,[AC_MSG_ERROR([required header not found])])

AC_CHECK_HEADERS([cwchar cwctype])


AX_MY_WITH_CURSES
AS_IF([test x$ax_cv_curses != xyes],
	    [AC_MSG_ERROR([no curses library found])])

my_saved_LIBS="$LIBS"
LIBS="$my_save_LIBS $CURSES_LIB"

AC_CHECK_FUNCS([alarm getcwd memset strdup strerror strstr],,[AC_MSG_ERROR([required function not found])])
AC_CHECK_FUNCS([setlocale])

if test x"$DISABLE_WCHAR" = xno
then
	AC_CHECK_FUNCS([iswprint],,[AC_MSG_ERROR([wchar support enabled, but no iswprint(). Try with --disable-wchar.])])
fi

# resize_term() is NCurses, resizeterm() is NetBSD curses
AC_CHECK_FUNCS([resize_term resizeterm],[break])

# Some custom tests
AC_MSG_CHECKING([for termname])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	char *tmp;]],
	[[tmp = termname()]])],
	[AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_TERMNAME],[1],[Set to 1 if you have termname])
	],
	[AC_MSG_RESULT([no])])

AC_MSG_CHECKING([whether waddstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); waddstr(stdscr, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([WADDSTR_USE_CHAR],[1],[Set to 1 if waddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether waddnstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); waddnstr(stdscr, teststr, 4); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([WADDNSTR_USE_CHAR],[1],[Set to 1 if waddnstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddstr(stdscr, 2, 1, teststr); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDSTR_USE_CHAR],[1],[Set to 1 if mvwaddstr() doesn't use const char*])])

AC_MSG_CHECKING([whether mvwaddnstr() use const char*])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	const char teststr[] = "Const string";]],
				  [[initscr(); mvwaddnstr(stdscr, 2, 1, teststr, 4); endwin();]])],
	[AC_MSG_RESULT([yes])],
	[AC_MSG_RESULT([no])
	AC_DEFINE([MVWADDNSTR_USE_CHAR],[1],[Set to 1 if mvwaddnstr() doesn't use const char*])])

AC_MSG_CHECKING([whether keypad() return int])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[
	#if defined HAVE_NCURSESW_CURSES_H
	#	 include <ncursesw/curses.h>
	#elif defined HAVE_NCURSESW_H
	#	 include <ncursesw.h>
	#elif defined HAVE_NCURSES_CURSES_H
	#	 include <ncurses/curses.h>
	#elif defined HAVE_NCURSES_H
	#	 include <ncurses.h>
	#elif defined HAVE_CURSES_H
	#	 include <curses.h>
	#else
	#	 error "SysV or X/Open-compatible Curses header file required"
	#endif
	int retval=100;]],
				  [[initscr(); retval = keypad(stdscr, TRUE); endwin();]])],
	[AC_MSG_RESULT([yes])
	AC_DEFINE([KEYPAD_RETURN_INT],[1],[Set to 1 if keypad has a return value])],
	[AC_MSG_RESULT([no])])

LIBS="$my_saved_LIBS"

AC_CONFIG_HEADERS([config.h include/libyacurscfg.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile include/Makefile po/Makefile.in tests/basic/Makefile tests/ui-interactive/Makefile tests/ui-non-interactive/Makefile tests/preloadlib/Makefile])
AC_OUTPUT

AC_MSG_NOTICE([])
AC_MSG_NOTICE([  host              $host])
AC_MSG_NOTICE([  prefix            $prefix])
AC_MSG_NOTICE([  Use NLS           $USE_NLS])
AC_MSG_NOTICE([  Curses library    $CURSES_LIB])
AC_MSG_NOTICE([  Debug mode        $ENABLEDEBUG])
AC_MSG_NOTICE([  Disable wchar     $DISABLE_WCHAR])
