// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

WidgetBase::WidgetBase(): Realizeable(), __parent(NULL),
			  __position(), __size(),
			  __curseswindow(NULL) {
}

WidgetBase::~WidgetBase() {
}

WidgetBase::WidgetBase(const WidgetBase& _w):
    Realizeable(_w),  __parent(_w.__parent),
    __position(_w.__position), __size(_w.__size),
    __curseswindow(NULL) {
}

WidgetBase&
WidgetBase::operator=(const WidgetBase& _w) {
    Realizeable::operator=(_w);
    __parent = _w.__parent;
    __position = _w.__position;
    __size = _w.__size;
    __curseswindow = _w.__curseswindow;

    return *this;
}

void
WidgetBase::position(const Coordinates& _c) {
    __position = _c;
}

const Coordinates&
WidgetBase::position() const {
    return __position;
}

void
WidgetBase::parent(WidgetBase* _p) {
    __parent = _p;
}

WidgetBase*
WidgetBase::parent() const {
    return __parent;
}

const Size&
WidgetBase::size() const {
    return __size;
}

void 
WidgetBase::curseswindow(WINDOW* _p) {
    __curseswindow=_p;
}

WINDOW*
WidgetBase::curseswindow() const {
    return __curseswindow;
}
