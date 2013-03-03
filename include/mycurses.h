// -*- mode: c++ -*-

#ifndef MYCURSES_H
#define MYCURSES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// In case it is compiled with g++
#ifndef _BOOL
#define _BOOL
#endif

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#include <term.h>
#elif HAVE_CURSES_H
#include <curses.h>
#else
#error "Neither ncurses.h nor curses.h available"
#endif

// My Keys.

enum {
#ifndef KEY_CTRL_A
    KEY_CTRL_A=1,
#endif
#ifndef KEY_CTRL_B
    KEY_CTRL_B=2,
#endif
#ifndef KEY_CTRL_D
    KEY_CTRL_D=4,
#endif
#ifndef KEY_CTRL_E
    KEY_CTRL_E=5,
#endif
#ifndef KEY_CTRL_F
    KEY_CTRL_F=6,
#endif
#ifndef KEY_TAB
    KEY_TAB=9,
#endif
#ifndef KEY_RETURN
    KEY_RETURN=10,
#endif
#ifndef KEY_RETURN2
    KEY_RETURN2=13,
#endif
#ifndef KEY_CTRL_K
    KEY_CTRL_K=11,
#endif
#ifndef KEY_CTRL_L
    KEY_CTRL_L=12,
#endif
#ifndef KEY_CTRL_N
    KEY_CTRL_N=14,
#endif
#ifndef KEY_CTRL_P
    KEY_CTRL_P=16,
#endif
#ifndef KEY_CTRL_U
    KEY_CTRL_U=21,
#endif
#ifndef KEY_BKSPC_SOL
    // Backspace in Solaris' Curses
    KEY_BKSPC_SOL=127,
#endif
    __KEY_MY_DUMMY__=0 // To make compiler stop complaining about comma
};

#ifdef tab
#undef tab
#endif

#ifdef HAVE_CURSES_H

#ifdef timeout
#undef timeout
inline void timeout(int n) {
    wtimeout(stdscr, n);
}
#endif

#ifdef box
#undef box
inline int box (WINDOW* win, int verch, int horch) {
    return wborder (win, verch, verch, horch, horch, 0, 0, 0, 0);
}
#endif

#ifdef clear
#undef clear
inline int clear() {
    return wclear (stdscr);
}
#endif

#ifdef erase
#undef erase
inline int erase() {
    return werase (stdscr);
}
#endif

#ifdef move
#undef move
inline int move (int y, int x) {
    return wmove (stdscr, y, x);
}
#endif

#ifdef refresh
#undef refresh
inline int refresh() {
    return wrefresh (stdscr);
}
#endif

#endif // HAVE_CURSES_H

#ifdef WADDSTR_USE_CHAR
#ifdef __SVR4
# include <stdlib.h>
# include <string.h>
#else // __SVR4
# include <cstdlib>
# include <cstring>
#endif // __SVR4

inline int waddstr_c (WINDOW* win, const char* str) {
    char* tmp_ptr = strdup(str);
    int retval = waddstr (win, tmp_ptr);
    memset (tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mywaddstr(a,b) waddstr_c(a,b)
#else // WADDSTR_USE_CHAR
#define mywaddstr(a,b) waddstr(a,b)
#endif // WADDSTR_USE_CHAR

#ifdef MVWADDSTR_USE_CHAR
#ifdef __SVR4
# include <stdlib.h>
# include <string.h>
#else // __SVR4
# include <cstdlib>
# include <cstring>
#endif // __SVR4

inline int mvwaddstr_c (WINDOW* win, int y, int x, const char* str) {
    char* tmp_ptr = strdup(str);
    int retval = mvwaddstr (win, y, x, tmp_ptr);
    memset(tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mymvwaddstr(a,b,c,d) mvwaddstr_c(a,b,c,d)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddstr(a,b,c,d) mvwaddstr(a,b,c,d)
#endif // MVWADDSTR_USE_CHAR

#ifdef MVWADDNSTR_USE_CHAR
#ifdef __SVR4
# include <stdlib.h>
# include <string.h>
#else // __SVR4
# include <cstdlib>
# include <cstring>
#endif // __SVR4

inline int mvwaddnstr_c (WINDOW* win, int y, int x, const char* str, int n) {
    char* tmp_ptr = strdup(str);
    int retval = mvwaddnstr (win, y, x, tmp_ptr, n);
    memset (tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr_c(a,b,c,d,e)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr(a,b,c,d,e)
#endif // MVWADDSTR_USE_CHAR

#endif
