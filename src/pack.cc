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


//
// Private
//
void
Pack::set_all_curses_window() {
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::bind2nd(std::mem_fun<void,WidgetBase,WINDOW*>(&WidgetBase::curses_window),WidgetBase::curses_window()));
}

void
Pack::refresh_all_widgets(bool i) {
   std::for_each(widget_list.begin(),
		 widget_list.end(),
		 std::bind2nd(std::mem_fun<void,WidgetBase,bool>(&WidgetBase::refresh),i));
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
    
    // Required since pack is a dynamically sized Widget.
    reset_size();

    realized(false);
}

//
// Public
//
Pack::Pack(): WidgetBase(), 
	      __size(), 
	      __hinting(true),
	      widget_list() {
}

Pack::~Pack() {
}

Pack::Pack(const Pack& _p): WidgetBase(_p),
			    __size(_p.__size),
			    __hinting(_p.__hinting),
			    widget_list(_p.widget_list) {
}

const Pack&
Pack::operator=(const Pack& _p) {
    WidgetBase::operator=(_p);
    __size = _p.__size;
    __hinting = _p.__hinting;
    widget_list = _p.widget_list;

    return *this;
}

void
Pack::add_front(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());

    widget_list.push_front(_w);

    _w->parent(this);
    _w->curses_window(WidgetBase::curses_window());
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());

    widget_list.push_back(_w);

    _w->parent(this);
    _w->curses_window(WidgetBase::curses_window());
}


void
Pack::remove(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.remove(_w);
}

void
Pack::curses_window(WINDOW* _p) {
    WidgetBase::curses_window(_p);

    // We have to make sure that the associated widgets have the same
    // curses window as we do.
    set_all_curses_window();
}

void
Pack::size_available(const Size& _s) {
    WidgetBase::size_available(__size=_s);
}

Size
Pack::size() const {
    // If we are always dynamic, simply return the current value of
    // __size, which may be Size::zero() if a call was made to
    // reset_size().
    if (__always_dynamic) return __size;

    // Since we are not always dynamic, see if we can figure out the
    // size. This only succeeds if there are no dynamically sized
    // widgets.
    Size non_dynamic=calc_size();
    if (non_dynamic!=Size::zero())
	return non_dynamic;

    // __size might have been set by a former call to size_available()
    // so return that.
    return __size;
}

void
Pack::hinting(bool _h) {
    __hinting=_h;
    size_change();
}

bool
Pack::hinting() const {
    return __hinting;
}

void
Pack::always_dynamic(bool _d) {
    __always_dynamic = _d;
    size_change();
}

bool
Pack::always_dynamic() const {
    return __always_dynamic;
}

void
Pack::reset_size() {
    __size=Size::zero();
}

bool
Pack::size_change() {
    if (parent()!=NULL)
	return parent()->size_change();

    if (!realized()) return true;

    // We're realized
    resize(Area(position(),
		WidgetBase::size_available()) );

    // Immediate refresh.
    refresh(true);

    

    return true;
}

bool
Pack::can_focus() const {
    //
    // TO BE IMPLEMENTED
    //
    abort();
    return false;
}

void
Pack::focus() {
    //
    // TO BE IMPLEMENTED
    //
    abort();
}

bool
Pack::has_focus() const {
    //
    // TO BE IMPLEMENTED
    //
    abort();
    return false;
}


void
Pack::refresh(bool immediate) {
    if (!realized()) throw NotRealized();

    refresh_all_widgets(immediate);
}

void
Pack::resize(const Area& _a) {
    if (!realized()) throw NotRealized();

    unrealize();

    position(_a);

    size_available(_a);

    realize();
}
