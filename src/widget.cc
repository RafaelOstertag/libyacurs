// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

Widget::Widget(): __parent(NULL), __area() {
}

Widget::~Widget() {
}

Widget::Widget(const Widget& _w):
    __parent(_w.parent), __area(_w.__area) {
}

void
Widget::area(const Rectangle& _a) {
    __area = _a;
}

const Rectangle&
Widget::area() const {
    return __area;
}
