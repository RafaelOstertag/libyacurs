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
 * Calculate the size hint.
 *
 * Functor for calculating the size hint by finding the max columns
 * and rows size.
 *
 * Packs may return rows()>0 or cols()>0 as hint, as opposed to other
 * widget that only return one of the components >0.
 */
class GetMaxSizeHint {
    private:
	Size __size_max;

    public:
	GetMaxSizeHint(): __size_max(Size::zero()) {}
	GetMaxSizeHint(const GetMaxSizeHint& _v):
	    __size_max(_v.__size_max) {}
	GetMaxSizeHint& operator=(const GetMaxSizeHint& _v) {
	    __size_max=_v.__size_max;
	    return *this;
	}

	void operator()(const WidgetBase* w) {
	    assert(w!=NULL);
	    __size_max.cols(std::max(w->size_hint().cols(),
				      __size_max.cols()));
	    __size_max.rows(w->size_hint().rows()+__size_max.rows());
	}

	const Size& hint() const {
	    return __size_max;
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
    
    // Required since pack is a dynamically sized Widget.
    resetsize();

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
    _w->curseswindow(WidgetBase::curseswindow());
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != NULL);
    assert(!_w->realized());

    widget_list.push_back(_w);

    _w->parent(this);
    _w->curseswindow(WidgetBase::curseswindow());
}


void
Pack::remove(WidgetBase* _w) {
    assert(_w != NULL);

    widget_list.remove(_w);
}

void
Pack::curseswindow(WINDOW* _p) {
    WidgetBase::curseswindow(_p);

    // We have to make sure that the associated widgets have the same
    // curses window as we do.
    set_all_curseswindow();
}

void
Pack::size_available(const Size& _s) {
    WidgetBase::size_available(__size=_s);
}

const Size&
Pack::size() const {
    return __size;
}

Size
Pack::size_hint() const {
    if (!__hinting) return Size::zero();

    // remember that Packs may return either component >0 when
    // hinting, see also comment on VGetMaxSizeHint
    GetMaxSizeHint shint;

    shint = std::for_each(widget_list.begin(),
			 widget_list.end(),
			 shint); 
    return shint.hint();
}

void
Pack::hinting(bool _h) {
    __hinting=_h;
}

bool
Pack::hinting() const {
    return __hinting;
}

void
Pack::resetsize() {
    __size=Size::zero();
}

bool
Pack::sizechange() {
    if (parent()!=NULL)
	return parent()->sizechange();

    if (!realized()) return true;

    // We're realized
    resize(Area(position(),
		WidgetBase::size_available()) );

    // Immediate refresh.
    refresh(true);

    return true;
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

    unrealize();

    position(_a);

    size_available(_a);

    realize();
}
