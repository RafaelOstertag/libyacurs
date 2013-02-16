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
    if (!(realization()==REALIZED ||
	  realization()==REALIZING)) return;

    if (werase(curses_window())==ERR)
	throw EraseFailed();

    if (__linetext.length()<1) return;

    assert(area().cols()>=MIN_COLS);
    if (static_cast<std::string::size_type>(area().cols())<=__linetext.length()) {
	// Since we are here, the text is too big for the screen
	// width, so we can't align anyway.
	if (mymvwaddnstr(curses_window(),
			 0,0,
			 __linetext.c_str(),
			 area().cols()-1)==ERR)
	    throw AddStrFailed();
	if (winsch(curses_window(),'>')==ERR)
	    throw WInsChFailed();
    } else {
	int hpos=0;
	switch (__alignment) {
	case LEFT:
	    // Nothing to do, since hpos is == 0
	    assert(hpos==0);
	    break;
	case CENTER:
	    assert(static_cast<std::string::size_type>(area().cols())>=__linetext.length());
	    hpos=(area().cols()-__linetext.length())/2;
	    break;
	case RIGHT:
	    assert(static_cast<std::string::size_type>(area().cols())>=__linetext.length());
	    hpos=area().cols()-__linetext.length();
	    break;
	}

	// We don't check the return code, since we max out the space
	// when doing right alignment.
	//
	//	if (mymvwaddstr(curses_window(),
	//		hpos,0,
	//		__linetext.c_str())==ERR)
	//  throw AddStrFailed();

	(void)mymvwaddstr(curses_window(),
			 0,hpos,
			  __linetext.c_str());
    }
}

//
// Public
//
LineObject::LineObject(POSITION _pos, const std::string& _t):
    WindowBase(), __linetext(_t), __position(_pos), __alignment(LEFT) {
    // We don't have to connect to EVT_REFRESH and EVT_RESIZE, since
    // that has already been done by BaseWindow. We simply have to
    // override the handler functions.
}

LineObject::~LineObject() {
}

LineObject::LineObject(const LineObject& lo):
    WindowBase(lo),
    __linetext(lo.__linetext),
    __position(lo.__position),
    __alignment(lo.__alignment)
{
}

LineObject&
LineObject::operator=(const LineObject& lo) {
    WindowBase::operator=(lo);

    __linetext = lo.__linetext;
    __position = lo.__position;
    __alignment = lo.__alignment;

    return *this;
}
void
LineObject::line(const std::string& _str) {
    __linetext = _str;
    // Refresh is responsible for taking care of whether or not the
    // refresh can happen, for instance, it cannot happen if the
    // object is not realized.
    refresh(true);
}

void
LineObject::alignment(ALIGNMENT _a) {
    __alignment=_a;

    if (realization()==REALIZED)
	refresh(true);
}

LineObject::ALIGNMENT
LineObject::alignment() const {
    return __alignment;
}

std::string
LineObject::line() const {
    return __linetext;
}

void
LineObject::realize() {
    REALIZE_ENTER;
    compute_margin();
    WindowBase::realize();
    REALIZE_LEAVE;
}

void
LineObject::refresh(bool immediate) {
    if (realization()!=REALIZED) return;
    put_line();

    YAPET::UI::Colors::set_color(curses_window(), YAPET::UI::DEFAULT);
    YAPET::UI::Colors::set_bg(curses_window(), YAPET::UI::DEFAULT);

    WindowBase::refresh(immediate);

}
