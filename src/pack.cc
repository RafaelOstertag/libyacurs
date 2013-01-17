// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <cassert>
#include <cstdlib>

#include "pack.h"

//
// Private
//

//
// Protected
//

//
// Public
//
Pack::Pack(): WidgetBase(), widget_list() {
}

Pack::~Pack() {
}

Pack::Pack(const Pack& _p):
    WidgetBase(_p), widget_list(_p.widget_list) {
}

const Pack&
Pack::operator=(const Pack& _p) {
    WidgetBase::operator=(_p);
    widget_list = _p.widget_list;
    __size = _p.__size;

    return *this;
}

void
Pack::add_front(WidgetBase* _w) {
    assert(_w != NULL);
    widget_list.push_front(_w);
    add_size(_w);
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.push_back(_w);
    add_size(_w);
}

void
Pack::remove(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.remove(_w);

    // I see no way of maintaining the proper size when removing
    // widgets than to recalc the size from scratch.
    recalc_size();
}

const Area&
Pack::size() const {
    return __size;
}
