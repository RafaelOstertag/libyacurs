// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "cursex.h"
#include "pack.h"

//
// Private
//
void
Pack::set_all_curseswindow() {
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::bind2nd(std::mem_fun<void,WidgetBase,WINDOW*>(&WidgetBase::curseswindow),WidgetBase::curseswindow()));
}

//
// Protected
//
void
Pack::unrealize() {
    if (!realized()) throw NotRealized();
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::unrealize));

    realized(false);
}

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
 
    return *this;
}

void
Pack::add_front(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());
    widget_list.push_front(_w);

    _w->parent(this);

    _w->curseswindow(WidgetBase::curseswindow());

    _w->window(window());

    add_size(_w);
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());

    widget_list.push_back(_w);

    _w->parent(this);

    _w->curseswindow(WidgetBase::curseswindow());

    _w->window(window());

    add_size(_w);
}

void
Pack::remove(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.remove(_w);
    _w->window(NULL);

    // I see no way of maintaining the proper size when removing
    // widgets than to recalc the size from scratch.
    recalc_size();
}

void
Pack::curseswindow(WINDOW* _p) {
    WidgetBase::curseswindow(_p);

    // We have to make sure that the associated widgets have the same curses
    // window as we do.
    set_all_curseswindow();
}

void
Pack::refresh(bool immediate) {
    if (!realized()) throw NotRealized();
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::bind2nd(std::mem_fun(&WidgetBase::refresh),immediate));
}

void
Pack::resize(const Area& _a) {
    if (!realized()) throw NotRealized();

    position(_a);
    size_available(_a);

    unrealize();
    realize();
}

