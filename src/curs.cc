//
// This file is part of libyacurs.
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <unistd.h>
#include <signal.h>

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#else // HAVE_TERMIOS_H
#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif // HAVE_SYS_TERMIOS_H
#endif // HAVE_TERMIOS_H

#include <cerrno>
#include <cstdlib>
#include <cstdio>

#include "curs.h"
#include "eventqueue.h"
#include "yacursex.h"
#include "focusmanager.h"
#include "colors.h"
#include "yacursconst.h"

using namespace YACURS;

Size Curses::suspend_scrsz;
TitleBar* Curses::__title = 0;
StatusBar* Curses::__statusbar = 0;
Window* Curses::__mainwindow = 0;
bool Curses::initialized = false;
volatile bool Curses::__suspended = false;

const char* Curses::__xterm_list[] = {
    "xterm",
    "dtterm",
    "screen",
    0
};

//
// Private
//

bool
Curses::is_xterm() {
#ifdef HAVE_TERMNAME
    const char** tmp = __xterm_list;
    const char* tn = termname();

    while (*tmp != 0) {
        if (std::strstr (tn, *tmp) != 0) return true;

        tmp++;
    }

    return false;
#else
    return false;
#endif
}

//
// Protected
//
void
Curses::doupdate_handler(Event& e) {
    assert(e == EVT_DOUPDATE);

    if (doupdate() == ERR)
        throw EXCEPTIONS::CursesException("doupdate");
}

void
Curses::termresetup_handler(Event& e) {
    assert(e == EVT_TERMRESETUP);
#if defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
    // We only support resizing for Curses implementation having
    // resize_term.

    Size _tmp(Curses::inquiry_screensize() );

    // If minimum size is underrun, resizing stops.
    if (_tmp.rows() < MIN_ROWS || _tmp.cols() < MIN_COLS)
        return;

#ifdef HAVE_RESIZE_TERM
    resize_term(_tmp.rows(), _tmp.cols() );
#else // HAVE_RESIZE_TERM
# ifdef HAVE_RESIZETERM
    resizeterm(_tmp.rows(), _tmp.cols() );
# endif // HAVE_RESIZETERM
#endif // HAVE_RESIZE_TERM

    if (wclear(stdscr) == ERR)
        throw EXCEPTIONS::CursesException("wclear");

    if (wrefresh(stdscr) == ERR)
        throw EXCEPTIONS::CursesException("wrefresh");
#endif // defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
}

void
Curses::sigtstp_handler(Event& e) {
    assert(e == EVT_SIGTSTP);

    if (__suspended) return;

    int old_errno = errno;

    if (savetty() == ERR)
        EXCEPTIONS::CursesException("savetty");

    // Block SIGWINCH and SIGALRM
    sigset_t mymask, oldmask;
    (void)sigemptyset(&mymask);
    (void)sigaddset(&mymask, SIGALRM);
    (void)sigaddset(&mymask, SIGWINCH);
    (void)sigprocmask(SIG_BLOCK, &mymask, &oldmask);

    suspend_scrsz = current_screensize();

    endwin();

    __suspended = true;

    // Put to sleep
    kill(getpid(), SIGSTOP);

    // wait for SIGCONT

    // wakey, wakey, rise and shine...
    (void)sigprocmask(SIG_SETMASK, &oldmask, 0);

    errno = old_errno;
}

void
Curses::sigcont_handler(Event& e) {
    assert(e == EVT_SIGCONT);

    if (!__suspended) return;

    int old_errno = errno;

    if (resetty() == ERR)
        EXCEPTIONS::CursesException("resetty");

#if defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
    if (suspend_scrsz != inquiry_screensize() ) {
        EventQueue::submit(EVT_TERMRESETUP);
        EventQueue::submit(EventEx<Size>(EVT_SIGWINCH,
                                         Curses::inquiry_screensize() ) );
        EventQueue::submit(EVT_REFRESH);
        EventQueue::submit(EVT_DOUPDATE);
    } else {
        EventQueue::submit(Event(EVT_FORCEREFRESH) );
        EventQueue::submit(Event(EVT_REFRESH) );
        EventQueue::submit(Event(EVT_DOUPDATE) );
    }
#else // defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
    EventQueue::submit(Event(EVT_FORCEREFRESH) );
    EventQueue::submit(Event(EVT_REFRESH) );
    EventQueue::submit(Event(EVT_DOUPDATE) );
#endif // defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)

    __suspended = false;

    errno = old_errno;
}

//
// Public
//
void
Curses::init(const std::string& colors) {
    if (Curses::initialized)
        throw EXCEPTIONS::AlreadyInitialized();

    if (initscr() == 0)
        throw EXCEPTIONS::UnableToInitialize();

#ifdef ENABLE_NLS
    bindtextdomain(PACKAGE, LOCALEDIR);
#endif

    YACURS::Colors::init_colors(colors);

    EventQueue::connect_event(EventConnectorFunction1(EVT_DOUPDATE,
                                                      Curses::doupdate_handler) );
    EventQueue::connect_event(EventConnectorFunction1(EVT_TERMRESETUP,
                                                      Curses::
                                                      termresetup_handler) );

    EventQueue::connect_event(EventConnectorFunction1(EVT_SIGTSTP,
                                                      Curses::
                                                      sigtstp_handler) );

    EventQueue::connect_event(EventConnectorFunction1(EVT_SIGCONT,
                                                      Curses::
                                                      sigcont_handler) );

    if (nonl() == ERR)
        throw EXCEPTIONS::CursesException("nonl");

    if (cbreak() == ERR)
        throw EXCEPTIONS::CursesException("cbreak");

    if (noecho() == ERR)
        throw EXCEPTIONS::CursesException("noecho");

    if (keypad(stdscr, TRUE) == ERR)
        throw EXCEPTIONS::CursesException("keypad");

    if (leaveok(stdscr, TRUE) == ERR)
        throw EXCEPTIONS::CursesException("leaveok");

    if (intrflush(stdscr, FALSE) == ERR)
        throw EXCEPTIONS::CursesException("intrflush");

    // We don't fail if that doesn't work, so no check on retval.
    curs_set(0);

    wbkgd(stdscr, ' ' | Colors::color_pair(DEFAULT) );

#if NCURSES_VERSION_PATCH < 20100313
    wattrset(stdscr, Colors::color_pair(DEFAULT) );
#else
    if (wattrset(stdscr, Colors::color_pair(DEFAULT) ) == ERR)
        throw EXCEPTIONS::CursesException("wattrset");
#endif

    // Curses clears stdscr upon first call to getch, which may
    // produce undesired results, i.e. already created Curses Windows
    // may be overwritten. Therefore we refresh stdscr preventively.
    if (wrefresh(stdscr) == ERR)
        throw EXCEPTIONS::CursesException("wrefresh");

    initialized = true;
}

void
Curses::end() {
    if (!initialized) throw EXCEPTIONS::NotInitialized();

    // On FreeBSD, for instance, endwin() does not clear screen, so we
    // unconditionally issue calls to do so
    if (wclear(stdscr) == ERR)
        throw EXCEPTIONS::CursesException("wclear");
    if (wrefresh(stdscr) == ERR)
        throw EXCEPTIONS::CursesException("wfresh");

    if (endwin() == ERR)
        throw EXCEPTIONS::CursesException("endwin");

    // Widgets and Windows deleted after the EventQueue loop has
    // terminated will have put Event Connector Removal requests into
    // the EventQueue, which are not processed (of course, the
    // EventQueue is terminated). Calling EventQueue::cleanup() will
    // remove the requests and free the associated memory
    EventQueue::cleanup();

    initialized = false;
}

void
Curses::run() {
    if (!initialized) throw EXCEPTIONS::NotInitialized();

    FocusManager::init();

    if (__title) __title->show();

    if (__statusbar) __statusbar->show();

    if (__mainwindow) __mainwindow->show();

    EventQueue::run();

    if (__mainwindow) __mainwindow->close();

    if (__statusbar) __statusbar->close();

    if (__title) __title->close();

    FocusManager::uninit();
}

void
Curses::title(TitleBar* _title) {
    __title = _title;
}

TitleBar*
Curses::title() {
    return __title;
}

void
Curses::statusbar(StatusBar* _sl) {
    __statusbar = _sl;
}

StatusBar*
Curses::statusbar() {
    return __statusbar;
}

void
Curses::mainwindow(Window* _w) {
    __mainwindow = _w;
}

Window*
Curses::mainwindow() {
    return __mainwindow;
}

Size
Curses::inquiry_screensize() {
    if (!initialized) throw EXCEPTIONS::NotInitialized();

    // If we have resize term, we need to get the actual terminal
    // size, because we have the ability to easily resize, and thus
    // support it.
    //
    // Without resize_term(), we return the size of stdscr, since
    // resizing is not support then

    Size __scrdim;
#if defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
    winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
        if (ws.ws_row > 0 && ws.ws_col > 0) {
            __scrdim.rows(ws.ws_row);
            __scrdim.cols(ws.ws_col);
        } else {
            throw EXCEPTIONS::WinSizeInvalid();
        }
    } else {
        char* crows = std::getenv("LINES");
        char* ccols = std::getenv("COLUMNS");

        if (crows != 0 && ccols != 0) {
            int _rows = std::atoi(crows);
            int _cols = std::atoi(ccols);

            if (_rows > 0 && _cols > 0) {
                __scrdim.rows(_rows);
                __scrdim.cols(_cols);
            } else {
                throw EXCEPTIONS::UnableToGetWinSize();
            }
        } else {
            throw EXCEPTIONS::UnableToGetWinSize();
        }
    }

    // Make sure we don't underrun the minimum size. The SIGWINCH
    // handler does also check and not initiate a resize term.
    if (__scrdim.rows() < MIN_ROWS)
        __scrdim.rows(MIN_ROWS);
    if (__scrdim.cols() < MIN_COLS)
        __scrdim.cols(MIN_COLS);
#else // defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)
    __scrdim = current_screensize();
#endif // defined(HAVE_RESIZE_TERM) || defined(HAVE_RESIZETERM)

    return __scrdim;
}

Size
Curses::current_screensize() {
    int nrows, ncols;

    getmaxyx(stdscr, nrows, ncols);

    return Size(nrows, ncols);
}

void
Curses::set_terminal_title(const std::string& _str) {
    if (Curses::is_xterm()) {
	fprintf (stdout, "%c]0;%s%c", '\033', _str.c_str(), '\007');
	fflush (stdout);
    }
}
