// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
#include "cursex.h"
#include "focusmanager.h"
#include "colors.h"

StatusLine* Curses::__statusline = NULL;
LineObject* Curses::__title = NULL;
Window* Curses::__mainwindow = NULL;
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
	throw DoupdateFailed();

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
	throw EraseFailed();

    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();
#endif // HAVE_RESIZE_TERM
}

//
// Public
//
void
Curses::init() {
    if (Curses::initialized)
	throw AlreadyInitialized();

    if (initscr() == NULL)
	throw UnableToInitialize();

    EventQueue::connect_event(EventConnectorFunction1(EVT_DOUPDATE, Curses::doupdate_handler));
    EventQueue::connect_event(EventConnectorFunction1(EVT_TERMRESETUP, Curses::termresetup_handler));

    if (nonl() == ERR)
	throw NoNLFailed();

    if (cbreak() == ERR)
	throw CbreakFailed();

    if (noecho() == ERR)
	throw NoEchoFailed();

    if (keypad(stdscr, TRUE) == ERR)
	throw KeyPadFailed();

    if (leaveok(stdscr, TRUE) == ERR)
	throw LeaveOKFailed();

    // We don't fail if that doesn't work, so no check on retval.
    curs_set(0);

    YAPET::UI::Colors::init_colors();

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

    // On FreeBSD, for instance, endwin() does not clear screen, so we
    // unconditionally issue calls to do so
    if (wclear(stdscr) == ERR)
	throw ClearFailed();
    if (wrefresh(stdscr) == ERR)
	throw RefreshFailed();


    if (endwin() == ERR)
	throw EndWinError();

    // Widgets and Windows delete after the EventQueue loop has
    // terminated will have put Event Connector Removal requests into
    // the EventQueue, which are not processed (of course, the
    // EventQueue terminated). Calling EventQueue::cleanup() will
    // remove the requests and free the associated memory
    EventQueue::cleanup();

    initialized = false;
}

void
Curses::run() {
    if (!initialized) throw NotInitialized();

    if (__title) __title->show();

    if (__statusline) __statusline->show();

    if (__mainwindow) __mainwindow->show();

    FocusManager::init();

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
