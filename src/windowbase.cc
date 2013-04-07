// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cassert>
#include <cstdlib>

#include "curs.h"
#include "yacursex.h"
#include "windowbase.h"
#include "eventqueue.h"

using namespace YACURS;

//
// Private
//
WindowBase::WindowBase(const WindowBase& wb) {
    throw NotSupported();
}

WindowBase&
WindowBase::operator=(const WindowBase&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
YACURS::INTERNAL::CursWin*
WindowBase::curses_window() const {
    return __curses_window;
}

const Area&
WindowBase::area() const {
    return __area;
}

Area
WindowBase::widget_area() const {
    Area widget_area;

    if (__frame) {
	// we have to take the frame into account, thus
	// minus Margin(1,1,1,1)
	widget_area = (__area-__margin)-Margin(1,1,1,1);
    } else {
	widget_area = __area-__margin;
    }

    return widget_area;
}

void
WindowBase::unrealize() {
    UNREALIZE_ENTER;

    assert(__curses_window!=0);

    delete __curses_window;
    __curses_window = 0;

    UNREALIZE_LEAVE;
}

//
// Public
//

WindowBase::WindowBase(const Margin& _m): 
    __area(Coordinates(),Curses::inquiry_screensize()),
    __margin(_m),
    __curses_window(0),
    __frame(false),
    __shown(false) {

    // We always want to receive this event. Therefore it was moved
    // from show() to ctor.
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));
}

WindowBase::~WindowBase() {
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));

    if (realization()==REALIZED) {
	assert(__curses_window!=0);
	delete __curses_window;
    }
}

void
WindowBase::margin(const Margin& _m) {
    if (realization()==REALIZED) throw AlreadyRealized();
    __margin = _m;
}

const Margin&
WindowBase::margin() const {
    return __margin;
}

bool
WindowBase::frame() const {
    return __frame;
}

void
WindowBase::frame(bool b) {
    __frame = b;
}

void
WindowBase::show() {
    if (realization()!=UNREALIZED) return;

    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));

    realize();
    refresh(true);
    EventQueue::submit(EventEx<WindowBase*>(EVT_WINDOW_SHOW,this));

    __shown=true;
}

void
WindowBase::close() {
    if (realization()!=REALIZED) return;

    unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));

    // We might have obstructed another window, so make sure it
    // receives a refresh.
    EventQueue::submit(EVT_REFRESH);
    EventQueue::submit(EVT_DOUPDATE);

    // Change: earlier, it was submitted before
    // EVT_REFRESH/EVT_DOUPDATE
    //
    // This caused problems with the focus manager, if a Label was
    // updated in the EVT_WINDOW_CLOSE handler.
    EventQueue::submit(EventEx<WindowBase*>(EVT_WINDOW_CLOSE,this));

    __shown=false;
}

bool
WindowBase::shown() const {
    return __shown;
}

void
WindowBase::force_refresh_handler(Event& _e) {
    if (realization()!=REALIZED) return;

    assert(_e == EVT_FORCEREFRESH);
    assert(__curses_window!=0);

    __curses_window->clearok(true);
}

void
WindowBase::refresh_handler(Event& _e) {
    assert(_e == EVT_REFRESH);
    refresh(false);
}

void
WindowBase::resize_handler(Event& _e) {
    assert(_e == EVT_SIGWINCH);
    EventEx<Size>& winch = dynamic_cast<EventEx<Size>&>(_e);
    resize(Area(Coordinates(0,0),winch.data()));
}

void
WindowBase::refresh(bool immediate) {
    if (realization()!=REALIZED && realization()!=REALIZING) return;

    assert(__curses_window!=0);

    __curses_window->refresh(immediate);
}

void
WindowBase::resize(const Area& _a) {
    //
    // Keep in mind: a resize does not refresh!
    //

    if (realization()!=REALIZED) {
	// Even if we're not realized, we keep track of the area at
	// our disposition.
	//
	// This was mainly introduced for the screen unlock dialog.
	__area = _a;
	return;
    }

    assert(_a.x()>-1);
    assert(_a.y()>-1);
    assert(_a.rows()>0);
    assert(_a.cols()>0);

    unrealize();

    __area = _a;

    realize();

}

void
WindowBase::realize() {
    REALIZE_ENTER;

    assert(__area.x()>=0);
    assert(__area.y()>=0);
    assert(__area.rows()>0);
    assert(__area.cols()>0);

    Area _tmp = __area - __margin;

    if (_tmp.x()<0 ||
	_tmp.y()<0 ||
	_tmp.rows()<MIN_WINDOW_ROWS ||
	_tmp.cols()<MIN_WINDOW_COLS) {
	realization(UNREALIZED);
	return;
    }

    assert(__curses_window==0);
    try {
	__curses_window = new YACURS::INTERNAL::CursWin(_tmp);
	__curses_window->scrollok(false);
	__curses_window->leaveok(true);
	
	if (__frame) {
	    __curses_window->box();
	}
    } catch (CursesException&) {
	if (__curses_window!=0) {
	    delete __curses_window;
	    __curses_window=0;
	}
	realization(UNREALIZED);
	throw;
    }

    REALIZE_LEAVE;
}
