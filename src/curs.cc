//
// This file is part of libyacurs, 
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

#include <cstdlib>

#include "curs.h"
#include "eventqueue.h"
#include "yacursex.h"
#include "focusmanager.h"
#include "colors.h"

using namespace YACURS;

StatusLine* Curses::__statusline = 0;
LineObject* Curses::__title = 0;
Window* Curses::__mainwindow = 0;
bool Curses::initialized = false;

//
// Private
//

//
// Protected
//
void
Curses::doupdate_handler(Event& e) {
    assert(e==EVT_DOUPDATE);

    if (doupdate() == ERR)
	throw EXCEPTIONS::CursesException("doupdate");

}

void
Curses::termresetup_handler(Event& e) {
    assert(e==EVT_TERMRESETUP);
#ifdef HAVE_RESIZE_TERM
    // We only support resizing for Curses implementation having
    // resize_term.

    Size _tmp(Curses::inquiry_screensize());

    // If minimum size is underrun, resizing stops.
    if (_tmp.rows()<MIN_ROWS || _tmp.cols()<MIN_COLS) 
	return;

    resize_term(_tmp.rows(), _tmp.cols());

    if (wclear(stdscr) == ERR)
	throw EXCEPTIONS::CursesException("wclear");

    if (wrefresh(stdscr) == ERR)
	throw EXCEPTIONS::CursesException("wrefresh");
#endif // HAVE_RESIZE_TERM
}

//
// Public
//
void
Curses::init() {
    if (Curses::initialized)
	throw EXCEPTIONS::AlreadyInitialized();

    if (initscr() == 0)
	throw EXCEPTIONS::UnableToInitialize();

#if ENABLE_NLS
    bindtextdomain(PACKAGE, LOCALEDIR);
#endif

    YACURS::Colors::init_colors();

    EventQueue::connect_event(EventConnectorFunction1(EVT_DOUPDATE, Curses::doupdate_handler));
    EventQueue::connect_event(EventConnectorFunction1(EVT_TERMRESETUP, Curses::termresetup_handler));

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

    if (__statusline) __statusline->show();

    if (__mainwindow) __mainwindow->show();

    EventQueue::run();

    if (__mainwindow) __mainwindow->close();

    if (__statusline) __statusline->close();

    if (__title) __title->close();

    FocusManager::uninit();
}

void
Curses::title(LineObject* _title) {
    __title = _title;
}

LineObject*
Curses::title() {
    return __title;
}

void
Curses::statusline(StatusLine* _sl) {
    __statusline = _sl;
}

StatusLine*
Curses::statusline() {
    return __statusline;
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
#ifdef HAVE_RESIZE_TERM
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

	if ( crows != 0 && ccols != 0 ) {
	    int _rows = std::atoi(crows);
	    int _cols = std::atoi(ccols);

	    if ( _rows > 0 && _cols > 0 ) {
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
    if (__scrdim.rows()<MIN_ROWS)
	__scrdim.rows(MIN_ROWS);
    if (__scrdim.cols()<MIN_COLS)
	__scrdim.cols(MIN_COLS);
#else // HAVE_RESIZE_TERM
    int nrows, ncols;
    getmaxyx(stdscr, nrows, ncols);

    __scrdim.rows(nrows);
    __scrdim.cols(ncols);
#endif // HAVE_RESIZE_TERM

    return __scrdim;
}
