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
// Functors
//

/**
 * Functor for setting size on associated widgets.
 *
 * This class simply sets the size_available() to the size() reported by the
 * widget. At a later stage, this class may become obsolete due to the
 * requirement of having dynamically sized widgets, that really rely upon the
 * size_available() provided by the parent Widget.
 */
class SetSimpleSize {
    public:
	void operator()(WidgetBase* _w) {
	    _w->size_available(_w->size());
	}
};


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

    // We have to recalc the size because of dynamically sized widgets
    recalc_size();
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());

    widget_list.push_back(_w);

    _w->parent(this);

    _w->curseswindow(WidgetBase::curseswindow());

    _w->window(window());

    // We have to recalc the size because of dynamically sized widgets
    recalc_size();
}


void
Pack::remove(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.remove(_w);
    _w->window(NULL);

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
Pack::size_available(const Size& _s) {
    WidgetBase::size_available(_s);
    __size = _s;

    // Recalc size. recalc_size() called in add_*() might not took effect,
    // since add_*() can be called without any size_available() set.
    recalc_size();
}

const Size&
Pack::size() const {
    return __size;
}

void
Pack::resetsize() {
    __size=Size(0,0);
}

bool
Pack::sizechange() {
    abort();
    return false;
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
