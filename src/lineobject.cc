//
// This file is part of libyacurs, 
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#include <cstdlib>

#include "curs.h"
#include "eventqueue.h"
#include "yacursex.h"
#include "lineobject.h"
#include "colors.h"

using namespace YACURS;

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

LineObject::LineObject(const LineObject&) {
    throw NotSupported();
}

LineObject&
LineObject::operator=(const LineObject& lo) {
    throw NotSupported();
    return *this;
}
//
// Protected
//

void
LineObject::put_line() {
    if (!(realization()==REALIZED ||
	  realization()==REALIZING)) return;

    curses_window()->erase();

    if (__linetext.length()<1) return;

    assert(area().cols()>=MIN_COLS);
    if (static_cast<std::string::size_type>(area().cols())<=__linetext.length()) {
	// Since we are here, the text is too big for the screen
	// width, so we can't align anyway.
	CurStr tmp(__linetext, Coordinates(0,0));
	curses_window()->addstrx(tmp);
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

	CurStr tmp(__linetext,Coordinates(hpos,0));
	curses_window()->addstr(tmp);
    }
}

//
// Public
//
LineObject::LineObject(POSITION _pos, const std::string& _t):
    __linetext(_t), __position(_pos), __alignment(LEFT) {
}

LineObject::~LineObject() {
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

    curses_window()->set_color(YACURS::DEFAULT);

    WindowBase::refresh(immediate);

}
