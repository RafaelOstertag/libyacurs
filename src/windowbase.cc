// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cassert>

#include "curs.h"
#include "cursex.h"
#include "windowbase.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
WINDOW*
WindowBase::curses_window() const {
    return *__curses_window;
 }

unsigned int
WindowBase::instance_count() const {
    return *__instance_count;
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
    if (not realized()) throw NotRealized();

    realized(false);

    assert(__curses_window!=NULL);
    assert(*__curses_window!=NULL);

    if (delwin(*__curses_window) == ERR)
	throw DelWindowFailed();

    *__curses_window = NULL;
}

int
WindowBase::refresh_handler(Event& _e) {
    assert(_e == EVT_REFRESH);
    assert(realized());
    refresh(false);
    return 0;
}

int
WindowBase::resize_handler(Event& _e) {
    assert(_e == EVT_SIGWINCH);
    assert(realized());

    EventWinCh& winch = dynamic_cast<EventWinCh&>(_e);
    resize(Area(Coordinates(0,0),winch.data()));

    return 0;
}

//
// Public
//

WindowBase::WindowBase(const Margin& _m):
    Realizeable(),
    __area(Coordinates(),Curses::inquiry_screensize()),
    __margin(_m),
    __instance_count(NULL),
    __curses_window(NULL),
    __frame(false) {

    __curses_window = new WINDOW*;
    *__curses_window = NULL;

    __instance_count = new unsigned int;
    *__instance_count = 1;

    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));
}

WindowBase::WindowBase(const WindowBase& so):
    Realizeable(so),
    __area(so.__area),
    __margin(so.__margin),
    __instance_count(so.__instance_count),
    __curses_window(so.__curses_window),
    __frame(so.__frame)
{
    (*__instance_count)++;

    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));
}

WindowBase::~WindowBase() {
    assert(__instance_count!=NULL);

    if (*__instance_count > 1) {
	(*__instance_count)--;
	return;
    }

    delete __instance_count;

    assert(__curses_window!=NULL);

    if (*__curses_window != NULL)
	if (delwin(*__curses_window) == ERR)
	    throw DelWindowFailed();

    delete __curses_window;

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));
}

WindowBase&
WindowBase::operator=(const WindowBase& so) {
    if ( this == &so)
	return *this;

    Realizeable::operator=(so);

    __instance_count = so.__instance_count;
    assert(__instance_count!=NULL);
    (*__instance_count)++;

    __curses_window = so.__curses_window;

    __area = so.__area;

    __margin = so.__margin;

    return *this;
}

void
WindowBase::margin(const Margin& _m) {
    //    assert(!isRealized());
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
WindowBase::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    assert(__curses_window!=NULL);
    assert(*__curses_window!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*__curses_window);
    else
	retval = wnoutrefresh(*__curses_window);

    if (retval == ERR)
	throw RefreshFailed();

}

void
WindowBase::resize(const Area& _a) {
    //
    // Keep in mind: a resize does not refresh!
    //
    if (!realized()) throw NotRealized();

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
    if (realized()) throw AlreadyRealized();

    assert(__area.x()>=0);
    assert(__area.y()>=0);
    assert(__area.rows()>0);
    assert(__area.cols()>0);

    Area _tmp = __area - __margin;

    assert(_tmp.x()>=0);
    assert(_tmp.y()>=0);
    assert(_tmp.rows()>0);
    assert(_tmp.cols()>0);


    assert(__curses_window!=NULL);
    assert(*__curses_window==NULL);
    *__curses_window = newwin(_tmp.rows(),
				  _tmp.cols(),
				  _tmp.y(),
				  _tmp.x());
    if (*__curses_window == NULL) {
	throw NewWindowFailed();
    }

    if (scrollok(*__curses_window, FALSE)==ERR)
	throw ScrollOKFailed();

    if (leaveok(*__curses_window, TRUE)==ERR)
	throw LeaveOKFailed();

    if (__frame) {
	if (box(*__curses_window, 0, 0) == ERR)
	    throw BoxFailed();
    }

    realized(true);
}
