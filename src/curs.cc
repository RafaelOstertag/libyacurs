// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif // HAVE_SIGNAL_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif // HAVE_SYS_IOCTL_H

#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#else
#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif // HAVE_TERMIOS_H
#endif // HAVE_SYS_TERMIOS_H

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "curs.h"
#include "eventqueue.h"

WINDOW* Curses::w = NULL;
StatusLine* Curses::statusline = NULL;
LineObject* Curses::title = NULL;
Window* Curses::mainwindow = NULL;
bool Curses::initialized = false;
Rectangle<> Curses::scrdim;

//
// Private
//

//
// Protected
//
int
Curses::doupdate_handler(EventBase& e) {
    if (doupdate() == ERR)
	throw DoupdateFailed();

    return 0;
}

int 
Curses::termresetup_handler(EventBase& e) {
    if (del_curterm(cur_term)==ERR)
	throw DelCurTermFailed();

    if (setupterm(NULL, fileno(stdout), NULL) == ERR)
	throw SetupTermFailed();

    if (werase(stdscr) == ERR)
	throw EraseFailed();

    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();

    return 0;
}

//
// Public
//
void
Curses::init() {
    if (Curses::initialized)
	throw AlreadyInitialized();

    w = initscr();
    if (w == NULL)
	throw UnableToInitialize();

    scrdim = inquiryScreenSize();

    EventQueue::connectEvent(EventConnectorFunction1(EVT_DOUPDATE, Curses::doupdate_handler));
    EventQueue::connectEvent(EventConnectorFunction1(EVT_TERMRESETUP, Curses::termresetup_handler));

#if !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))
    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();
#endif // !(defined(_XOPEN_CURSES) || defined(NCURSES_VERSION))

    if (nonl() == ERR)
	throw NoNLFailed();

    initialized = true;
}

void
Curses::end() {
    if (!initialized) throw NotInitialized();

    int retval = endwin();
    if (retval == ERR)
	throw EndWinError();

    initialized = false;
    w = NULL;
}

void
Curses::run() {
    if (!initialized) throw NotInitialized();

    if (title) {
	title->realize();
	title->refresh(false);
    }

    if (statusline) {
	statusline->realize();
	statusline->refresh(false);
    }

    if (mainwindow) {
	mainwindow->realize();
	mainwindow->refresh(false);
    }

    if (doupdate() == ERR)
	throw DoupdateFailed();

    EventQueue::run();
}

void
Curses::setTitle(LineObject* _title) {
    title = _title;
}

LineObject*
Curses::getTitle() {
    return title;
}

void
Curses::unsetTitle() {
    title = NULL;
}

void
Curses::setStatusLine(StatusLine* _sl) {
    statusline = _sl;
}

StatusLine*
Curses::getStatusLine() {
    return statusline;
}

void
Curses::unsetStatusLine() {
    statusline = NULL;
}

void
Curses::setWindow(Window* _w) {
    mainwindow = _w;
}

Window*
Curses::getWindow() {
    return mainwindow;
}

void
Curses::unsetWindow() {
    mainwindow = NULL;
}

Rectangle<>
Curses::inquiryScreenSize() {
    winsize ws;
    Rectangle<> __scrdim;

    __scrdim.x(0);
    __scrdim.y(0);

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
	if (ws.ws_row > 0 && ws.ws_col > 0) {
	    __scrdim.rows(ws.ws_row);
	    __scrdim.cols(ws.ws_col);
	} else {
	    throw WinSizeInvalid();
	}
    } else {
	char* crows = std::getenv("LINES");
	char* ccols = std::getenv("COLUMNS");

	if ( crows != NULL && ccols != NULL ) {
	    int _rows = std::atoi(crows);
	    int _cols = std::atoi(ccols);

	    if ( _rows > 0 && _cols > 0 ) {
		__scrdim.rows(_rows);
		__scrdim.cols(_cols);
	    } else {
		throw UnableToGetWinSize();
	    }
	} else {
	    throw UnableToGetWinSize();
	}
    }
    return __scrdim;

#if 0
    int x, y, nrows, ncols;
    getbegyx(w, y, x);
    getmaxyx(w, nrows, ncols);

    scrdim.x(x);
    scrdim.y(y);
    scrdim.rows(nrows);
    scrdim.cols(ncols);
#endif
}
