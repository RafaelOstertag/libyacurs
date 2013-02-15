// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cassert>

#include "debug.h"

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
    UNREALIZE_ENTER;

    DEBUGOUT("-- IN: WindowBase::unrealize()");
    DEBUGOUT(*this);

    assert(__curses_window!=NULL);
    assert(*__curses_window!=NULL);

    if (delwin(*__curses_window) == ERR) {
	realization(UNREALIZED);
	throw DelWindowFailed();
    }

    *__curses_window = NULL;
    
    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: WindowBase::unrealize()");
    
    UNREALIZE_LEAVE;
}

void
WindowBase::force_refresh_handler(Event& _e) {
    if (realization()!=REALIZED) return;

    assert(_e == EVT_FORCEREFRESH);
    assert(__curses_window!=NULL);
    assert(*__curses_window!=NULL);

    if (clearok(*__curses_window, TRUE)==ERR)
	throw ClearOKFailed();
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
}

WindowBase::~WindowBase() {
    assert(__instance_count!=NULL);

    if (*__instance_count > 1) {
	(*__instance_count)--;
	return;
    }

    delete __instance_count;

    assert(__curses_window!=NULL);

    bool delwin_failed=false;
    if (realization()==REALIZED)
	delwin_failed=delwin(*__curses_window)==ERR;

    delete __curses_window;

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));
   
    if (delwin_failed)
	throw DelWindowFailed();
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
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));

    realize();
    refresh(true);
    EventQueue::submit(EventEx<WindowBase*>(EVT_WINDOW_SHOW,this));
}

void
WindowBase::close() {
    if (realization()!=REALIZED) return;

    unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this, &WindowBase::resize_handler));

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
}


void
WindowBase::refresh(bool immediate) {
    if (realization()!=REALIZED && realization()!=REALIZING) return;

    DEBUGOUT("-- IN: WindowBase::refresh()");
    DEBUGOUT(*this);

    assert(__curses_window!=NULL);
    assert(*__curses_window!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*__curses_window);
    else
	retval = wnoutrefresh(*__curses_window);

    if (retval == ERR)
	throw RefreshFailed();

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: WindowBase::refresh()");
}

void
WindowBase::resize(const Area& _a) {
    //
    // Keep in mind: a resize does not refresh!
    //
    if (realization()!=REALIZED) return;
    DEBUGOUT("-- IN: WindowBase::resize(): _a=" + static_cast<std::string>(_a));
    DEBUGOUT(*this);

    assert(_a.x()>-1);
    assert(_a.y()>-1);
    assert(_a.rows()>0);
    assert(_a.cols()>0);

    unrealize();

    __area = _a;

    realize();
    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: WindowBase::resize()");
}

void
WindowBase::realize() {
    REALIZE_ENTER;

    DEBUGOUT("-- IN: WindowBase::realize()");
    DEBUGOUT(*this);

    assert(__area.x()>=0);
    assert(__area.y()>=0);
    assert(__area.rows()>0);
    assert(__area.cols()>0);

    Area _tmp = __area - __margin;
    DEBUGOUT("window Area:" + static_cast<std::string>(_tmp));


    if (_tmp.x()<0 ||
	_tmp.y()<0 ||
	_tmp.rows()<MIN_WINDOW_ROWS ||
	_tmp.cols()<MIN_WINDOW_COLS) {
	realization(UNREALIZED);
	return;
    }

    assert(__curses_window!=NULL);
    assert(*__curses_window==NULL);
    *__curses_window = newwin(_tmp.rows(),
				  _tmp.cols(),
				  _tmp.y(),
				  _tmp.x());
    if (*__curses_window == NULL) {
	realization(UNREALIZED);
	throw NewWindowFailed();
    }

    if (scrollok(*__curses_window, FALSE)==ERR) {
	realization(UNREALIZED);
	delwin(*__curses_window);
	*__curses_window=NULL;
	throw ScrollOKFailed();
    }

    if (leaveok(*__curses_window, TRUE)==ERR) {
	realization(UNREALIZED);
	delwin(*__curses_window);
	*__curses_window=NULL;
	throw LeaveOKFailed();
    }

    if (__frame) {
	if (box(*__curses_window, 0, 0) == ERR) {
	    realization(UNREALIZED);
	    delwin(*__curses_window);
	    *__curses_window=NULL;
	    throw BoxFailed();
	}
    }

    DEBUGOUT(*this);
    DEBUGOUT("-- OUT: WindowBase::realize()");

    REALIZE_LEAVE;
}

WindowBase::operator std::string() const {
    return "WindowBase{\n\t(Screen Area:" + 
	static_cast<std::string>(__area) + ")\n\t(Margin:" +
	static_cast<std::string>(__margin) + ")\n}";
}
