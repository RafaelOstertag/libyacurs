// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdlib>

#include "widget.h"

//
// Private
//

//
// Protected
//

//
// Private
//

Widget::Widget(): Realizeable(), __parent(NULL), __area() {
}

Widget::~Widget() {
}

Widget::Widget(const Widget& _w):
    Realizeable(_w),  __parent(_w.__parent), __area(_w.__area) {
}

const Widget&
Widget::operator=(const Widget& _w) {
    Realizeable::operator=(_w);
    __parent = _w.__parent;
    __area = _w.__area;

    return *this;
}

void
Widget::area(const Rectangle& _a) {
    __area = _a;
}

const Rectangle&
Widget::area() const {
    return __area;
}

void
Widget::parent(Widget* _p) {
    __parent = _p;
}

Widget*
Widget::parent() const {
    return __parent;
}
