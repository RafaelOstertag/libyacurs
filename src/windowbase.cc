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
WindowBase::getWindow() const {
    return *w;
 }

unsigned int
WindowBase::getInstanceCount() const {
    return *instances;
}

const Area&
WindowBase::area() const {
    return __area;
}

void
WindowBase::unrealize() {
    if (not realized()) throw NotRealized();

    realized(false);

    assert(w!=NULL);
    assert(*w!=NULL);

    if (delwin(*w) == ERR)
	throw DelWindowFailed();

    *w = NULL;
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
    assert(_e == EVT_WINCH);
    assert(realized());
    
    EventWinCh& winch = dynamic_cast<EventWinCh&>(_e);
    resize(winch.data());
    
    return 0;
}

//
// Public
//

WindowBase::WindowBase(const Margin& _m):
    Realizeable(), __area(Coordinates(),Curses::inquiryScreenSize()),
    __margin(_m), instances(NULL), w(NULL), __frame(false) {

    w = new WINDOW*;
    *w = NULL;

    instances = new unsigned int;
    *instances = 1;

    EventQueue::connectEvent(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<WindowBase>(EVT_WINCH,this, &WindowBase::resize_handler));
}

WindowBase::WindowBase(const WindowBase& so):
    Realizeable(so), __area(so.__area), __margin(so.__margin),
    instances(so.instances), __frame(so.__frame) {
    (*instances)++;

    EventQueue::connectEvent(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<WindowBase>(EVT_WINCH,this, &WindowBase::resize_handler));
}

WindowBase::~WindowBase() {
    if (*instances > 1) {
	(*instances)--;
	return;
    }

    assert(instances!=NULL);

    delete instances;

    assert(w!=NULL);

    if (*w != NULL) {
	int retval = delwin(*w);
	if (retval == ERR)
	    throw DelWindowFailed();
    }

    delete w;

    EventQueue::disconnectEvent(EventConnectorMethod1<WindowBase>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    EventQueue::disconnectEvent(EventConnectorMethod1<WindowBase>(EVT_WINCH,this, &WindowBase::resize_handler));
}

WindowBase&
WindowBase::operator=(const WindowBase& so) {
    if ( this == &so)
	return *this;

    Realizeable::operator=(so);

    instances = so.instances;
    assert(instances!=NULL);
    (*instances)++;

    w = so.w;

    __area = so.__area;

    __margin = so.__margin;

    return *this;
}

Size
WindowBase::size() const {
    if (__frame)
	return (__area-__margin)-Margin(1,1,1,1);
    else
	return (__area-__margin);
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

    assert(w!=NULL);
    assert(*w!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*w);
    else
	retval = wnoutrefresh(*w);

    if (retval == ERR)
	throw RefreshFailed();

}

void
WindowBase::resize(const Size& _s) {
    //
    // Keep in mind: a resize does not refresh!
    //
    if (!realized()) throw NotRealized();

    assert(_s.rows()>0);
    assert(_s.cols()>0);

    unrealize();

    __area = Coordinates(); // initialize x/y to (0/0)
    __area = _s;

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


    assert(w!=NULL);
    assert(*w==NULL);
    *w = newwin(_tmp.rows(),
		_tmp.cols(),
		_tmp.y(),
		_tmp.x());
    if (*w == NULL) {
	throw NewWindowFailed();
    }

    if (__frame) {
	if (box(getWindow(), 0, 0) == ERR)
	    throw BoxFailed();
    }

    realized(true);
}
