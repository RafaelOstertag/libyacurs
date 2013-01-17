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

WidgetBase::WidgetBase(): Realizeable(), __parent(NULL), __area(), __size() {
}

WidgetBase::~WidgetBase() {
}

WidgetBase::WidgetBase(const WidgetBase& _w):
    Realizeable(_w),  __parent(_w.__parent),
    __area(_w.__area), __size(_w.__size) {
}

const WidgetBase&
WidgetBase::operator=(const WidgetBase& _w) {
    Realizeable::operator=(_w);
    __parent = _w.__parent;
    __area = _w.__area;
    __size = _w.__size;

    return *this;
}

void
WidgetBase::area(const Area& _a) {
    __area = _a;
}

const Area&
WidgetBase::area() const {
    return __area;
}

void
WidgetBase::parent(WidgetBase* _p) {
    __parent = _p;
}

WidgetBase*
WidgetBase::parent() const {
    return __parent;
}

const Area&
WidgetBase::size() const {
    return __size;
}
