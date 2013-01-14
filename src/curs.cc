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
Curses::doupdate_handler(Event& e) {
    if (doupdate() == ERR)
	throw DoupdateFailed();

    return 0;
}

int
Curses::termresetup_handler(Event& e) {
#ifdef HAVE_RESIZE_TERM
    // We only support resizing for Curses implementation having
    // resize_term.

    Rectangle<> __tmp(Curses::inquiryScreenSize());

    resize_term(__tmp.rows(), __tmp.cols());

    if (wclear(stdscr) == ERR)
	throw EraseFailed();

    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();
#endif // HAVE_RESIZE_TERM
    
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

    if (nonl() == ERR)
	throw NoNLFailed();

    if (cbreak() == ERR)
	throw CbreakFailed();

    if (noecho() == ERR)
	throw NoEchoFailed();
    
    // Curses clears stdscr upon first call to getch, which may
    // produce undesired results, i.e. already created Curses Windows
    // may be overwritten. Therefore we refresh stdscr preventive.
    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();

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
    // If we have resize term, we need to get the actual terminal
    // size, because we have the ability to easily resize, and thus
    // support it.
    //
    // Without resize_term(), we return the size of stdscr, since
    // resizing is not support then

    Rectangle<> __scrdim;
#ifdef HAVE_RESIZE_TERM
    winsize ws;


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
#else // HAVE_RESIZE_TERM
    int nrows, ncols;
    getmaxyx(w, nrows, ncols);

    __scrdim.x(0);
    __scrdim.y(0);
    __scrdim.rows(nrows);
    __scrdim.cols(ncols);
#endif // HAVE_RESIZE_TERM

    return __scrdim;
}
