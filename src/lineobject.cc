// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curs.h"
#include "eventqueue.h"
#include "cursex.h"
#include "lineobject.h"

//
// Private
//

void
LineObject::computeMargin(const Rectangle& _s) {
    Rectangle _tmp_r;

    if (_s == Rectangle() )
	_tmp_r = Curses::inquiryScreenSize();
    else
	_tmp_r= _s;

#ifndef NDEBUG
    Margin __m_debug;
#endif // NDEBUG
    switch (position) {
    case POS_TOP:
#ifdef NDEBUG
	setMargin(Margin(0, 0, _tmp_r.rows()-1,0));
#else // NDEBUG
	__m_debug = Margin(0, 0, _tmp_r.rows()-1,0);
	assert(__m_debug.bottom()>=0);
	setMargin(__m_debug);
#endif //NDEBUG
	break;
    case POS_BOTTOM:
#ifdef NDEBUG
	setMargin(Margin(_tmp_r.rows()-1, 0, 0, 0));
#else // NDEBUG
	__m_debug = Margin(_tmp_r.rows()-1, 0, 0, 0);
	assert(__m_debug.top()>=0);
	setMargin(__m_debug);
#endif
	break;
    }
}

//
// Protected
//

void
LineObject::putLine() {
    if (!realized()) return;

    int retval = werase(getWindow());
    if (retval == ERR)
	throw EraseFailed();

    retval = mymvwaddstr(getWindow(),
			 0,0,
			 linetext.c_str());
    if (retval == ERR)
	throw AddStrFailed();

    refresh(true);
}

int
LineObject::refresh_handler(Event& _e) {
    assert(_e.type() == EVT_REFRESH);
    putLine();
    refresh(false);
    return 0;
}

int
LineObject::resize_handler(Event& _e) {
    assert(_e.type() == EVT_WINCH);
    
    EventWinCh& winch = dynamic_cast<EventWinCh&>(_e);

    // set the margin in order to achieve the position
    computeMargin(winch.data()); 

    // resize() of ScreenObject
    resize(winch.data()); 
    
    return 0;
}

//
// Public
//
LineObject::LineObject(POSITION _pos, const std::string& _t):
    ScreenObject(), linetext(_t), position(_pos) {
    EventQueue::connectEvent(EventConnectorMethod1<LineObject>(EVT_REFRESH,this, &LineObject::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<LineObject>(EVT_WINCH,this, &LineObject::resize_handler));
}

LineObject::~LineObject() {
    EventQueue::disconnectEvent(EventConnectorMethod1<LineObject>(EVT_REFRESH,this, &LineObject::refresh_handler));
    EventQueue::disconnectEvent(EventConnectorMethod1<LineObject>(EVT_WINCH,this, &LineObject::resize_handler));
}

LineObject::LineObject(const LineObject& lo):
    ScreenObject(lo), linetext(lo.linetext), position(lo.position) {

    EventQueue::connectEvent(EventConnectorMethod1<LineObject>(EVT_REFRESH,this, &LineObject::refresh_handler));
    EventQueue::connectEvent(EventConnectorMethod1<LineObject>(EVT_WINCH,this, &LineObject::resize_handler));
}

LineObject&
LineObject::operator=(const LineObject& lo) {
    ScreenObject::operator=(lo);

    linetext = lo.linetext;
    position = lo.position;

    return *this;
}

void
LineObject::realize() {
    computeMargin();
    ScreenObject::realize();
    putLine();
}

void
LineObject::line(const std::string& _str) {
    linetext = _str;
    putLine();
}

std::string
LineObject::line() const { return linetext; }
