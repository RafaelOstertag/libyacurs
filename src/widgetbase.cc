// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>

#include "widgetbase.h"
#include "cursex.h"

//
// Private
//

//
// Protected
//
WINDOW*
WidgetBase::curses_window() const {
    return __curses_window;
}

fgid_t
WidgetBase::focusgroup_id() const {
    return __fgid;
}

WidgetBase*
WidgetBase::parent() const {
    return __parent;
}

//
// Private
//

WidgetBase::WidgetBase(): Realizeable(),
			  __curses_window(NULL),
			  __fgid((fgid_t)-1),
			  __parent(NULL),
			  __position(),
			  __size_available() {
}

WidgetBase::~WidgetBase() {
}

WidgetBase::WidgetBase(const WidgetBase& _w): Realizeable(_w),
					      __curses_window(_w.__curses_window),
					      __fgid(_w.__fgid),
					      __parent(_w.__parent),
					      __position(_w.__position),
					      __size_available(_w.__size_available) {
}

WidgetBase&
WidgetBase::operator=(const WidgetBase& _w) {
    Realizeable::operator=(_w);
    __curses_window = _w.__curses_window;
    __fgid = _w.__fgid;
    __parent = _w.__parent;
    __position = _w.__position;
    __size_available = _w.__size_available;

    return *this;
}

void
WidgetBase::parent(WidgetBase* _p) {
    __parent = _p;
}

void
WidgetBase::curses_window(WINDOW* _p) {
    __curses_window=_p;
}

void
WidgetBase::focusgroup_id(fgid_t _id) {
    __fgid=_id;
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
