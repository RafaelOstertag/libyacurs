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
LineObject::computeMargin() {
#ifndef NDEBUG
    Margin __m_debug;
#endif // NDEBUG
    switch (position) {
    case POS_TOP:
#ifdef NDEBUG
	margin(Margin(0, 0, area().rows()-1,0));
#else // NDEBUG
	__m_debug = Margin(0, 0, area().rows()-1,0);
	assert(__m_debug.bottom()>=0);
	margin(__m_debug);
#endif //NDEBUG
	break;
    case POS_BOTTOM:
#ifdef NDEBUG
	margin(Margin(area().rows()-1, 0, 0, 0));
#else // NDEBUG
	__m_debug = Margin(area().rows()-1, 0, 0, 0);
	assert(__m_debug.top()>=0);
	margin(__m_debug);
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

    // resize() of WindowBase
    resize(winch.data());

    return 0;
}

//
// Public
//
LineObject::LineObject(POSITION _pos, const std::string& _t):
    WindowBase(), linetext(_t), position(_pos) {
    // We don't have to connect to EVT_REFRESH and EVT_RESIZE, since that has
    // already been done by BaseWindow. We simply have to override the handler
    // functions.
}

LineObject::~LineObject() {
}

LineObject::LineObject(const LineObject& lo):
    WindowBase(lo), linetext(lo.linetext), position(lo.position) {

}

LineObject&
LineObject::operator=(const LineObject& lo) {
    WindowBase::operator=(lo);

    linetext = lo.linetext;
    position = lo.position;

    return *this;
}

void
LineObject::realize() {
    computeMargin();
    WindowBase::realize();
    putLine();
}

void
LineObject::line(const std::string& _str) {
    linetext = _str;
    putLine();
}

std::string
LineObject::line() const {
    return linetext;
}
