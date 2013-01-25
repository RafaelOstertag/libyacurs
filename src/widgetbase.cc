// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>

#include "widgetbase.h"

//
// Private
//

//
// Protected
//

//
// Private
//

WidgetBase::WidgetBase(): Realizeable(),
			  __curses_window(NULL),
			  __parent(NULL),
			  __position(),
			  __size_available() {
}

WidgetBase::~WidgetBase() {
}

WidgetBase::WidgetBase(const WidgetBase& _w): Realizeable(_w),
					      __curses_window(NULL),
					      __parent(_w.__parent),
					      __position(_w.__position),
					      __size_available(_w.__size_available) {
}

WidgetBase&
WidgetBase::operator=(const WidgetBase& _w) {
    Realizeable::operator=(_w);
    __parent = _w.__parent;
    __position = _w.__position;
    __curses_window = _w.__curses_window;
    __size_available = _w.__size_available;

    return *this;
}

void
WidgetBase::parent(WidgetBase* _p) {
    __parent = _p;
}

WidgetBase*
WidgetBase::parent() const {
    return __parent;
}

void
WidgetBase::curses_window(WINDOW* _p) {
    __curses_window=_p;
}

WINDOW*
WidgetBase::curses_window() const {
    return __curses_window;
}


void
WidgetBase::position(const Coordinates& _c) {
    assert(_c.x()>-1);
    assert(_c.y()>-1);
    __position = _c;
}

const Coordinates&
WidgetBase::position() const {
    return __position;
}

void
WidgetBase::size_available(const Size& _s) {
    assert(_s.rows()>0);
    assert(_s.cols()>0);
    __size_available = _s;
}

const Size&
WidgetBase::size_available() const {
    return __size_available;
}
