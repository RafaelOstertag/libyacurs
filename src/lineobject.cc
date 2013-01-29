// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curs.h"
#include "eventqueue.h"
#include "cursex.h"
#include "lineobject.h"
#include "colors.h"

//
// Private
//

void
LineObject::compute_margin() {
#ifndef NDEBUG
    Margin __m_debug;
#endif // NDEBUG
    switch (__position) {
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
LineObject::put_line() {
    if (!realized()) return;

    if (werase(curses_window())==ERR)
	throw EraseFailed();

    if (__linetext.length()<1) return;

    assert(area().cols()>=MIN_COLS);
    if (static_cast<std::string::size_type>(area().cols())<__linetext.length()+1) {
	if (mymvwaddnstr(curses_window(),
			 0,0,
			 __linetext.c_str(),
			 area().cols()-1)==ERR)
	    throw AddStrFailed();
	if (winsch(curses_window(),'>')==ERR)
	    throw AddStrFailed();
    } else {
	if (mymvwaddstr(curses_window(),
			0,0,
			__linetext.c_str())==ERR)
	    throw AddStrFailed();
    }

}

//
// Public
//
LineObject::LineObject(POSITION _pos, const std::string& _t):
    WindowBase(), __linetext(_t), __position(_pos) {
    // We don't have to connect to EVT_REFRESH and EVT_RESIZE, since
    // that has already been done by BaseWindow. We simply have to
    // override the handler functions.
}

LineObject::~LineObject() {
}

LineObject::LineObject(const LineObject& lo): 
    WindowBase(lo),
    __linetext(lo.__linetext),
    __position(lo.__position) 
{
}

LineObject&
LineObject::operator=(const LineObject& lo) {
    WindowBase::operator=(lo);

    __linetext = lo.__linetext;
    __position = lo.__position;

    return *this;
}

void
LineObject::realize() {
    if (realized()) throw AlreadyRealized();

    compute_margin();
    WindowBase::realize();
}

void
LineObject::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    put_line();

    YAPET::UI::Colors::set_color(curses_window(), YAPET::UI::DEFAULT);

    WindowBase::refresh(immediate);
}

void
LineObject::line(const std::string& _str) {
    __linetext = _str;
    if (realized())
	refresh(true);
}

std::string
LineObject::line() const {
    return __linetext;
}
