// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "yacursex.h"
#include "pack.h"
#include "eventqueue.h"

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
		  std::bind2nd(std::mem_fun<void,WidgetBase,YACURS::INTERNAL::CursWin*>(&WidgetBase::curses_window),WidgetBase::curses_window()));
}

void
Pack::set_all_focusgroup_id() {
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::bind2nd(std::mem_fun<void,WidgetBase,fgid_t>(&WidgetBase::focusgroup_id),WidgetBase::focusgroup_id()));
}

void
Pack::refresh_all_widgets(bool i) {
   std::for_each(widget_list.begin(),
		 widget_list.end(),
		 std::bind2nd(std::mem_fun<void,WidgetBase,bool>(&WidgetBase::refresh),i));
}

void
Pack::take_over(WidgetBase* _w) {
    assert(_w!=0);

    _w->parent(this);
    _w->curses_window(WidgetBase::curses_window());
    _w->focusgroup_id(WidgetBase::focusgroup_id());
}


Pack&
Pack::operator=(const Pack& _p) {
    throw NotSupported();
    return *this;
}
//
// Protected
//
void
Pack::unrealize() {
    UNREALIZE_ENTER;
    WidgetBase::unrealize();

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::unrealize));

    // Required since pack is a dynamically sized Widget.
    reset_size();

    UNREALIZE_LEAVE;
}

//
// Public
//
Pack::Pack(): __hinting(true),
	      __always_dynamic(false) {
}

Pack::~Pack() {
    // We don't unrealize widget, since they may be already deleted by
    // the user, so we would try to reference invalid memory.
    //
    // We also don't want to force users of removing widgets from
    // packs before the packs can be deleted.
    //
    // May be we could implement some self removal of widgets from
    // packs on destruction...
}

std::list<WidgetBase*>::size_type
Pack::widgets() const {
    return widget_list.size();
}

void
Pack::add_front(WidgetBase* _w) {
    assert(_w != 0);

    widget_list.push_front(_w);
    take_over(_w);
    
    if (realization() == REALIZED) {
	if (_w->realization() == REALIZED)
	    _w->unrealize();

	size_change();
    }
}

void
Pack::add_back(WidgetBase* _w) {
    assert(_w != 0);

    widget_list.push_back(_w);

    take_over(_w);


    if (realization() == REALIZED) {
	if (_w->realization() == REALIZED)
	    _w->unrealize();

	size_change();
    }
}

void
Pack::remove(WidgetBase* _w) {
    assert(_w != 0);

    //    if (realization()!=UNREALIZED) throw AlreadyRealized();

    widget_list.remove(_w);

    if (_w->realization() != UNREALIZED)
	_w->unrealize();

    _w->focusgroup_id((fgid_t)-1);

    if (realization() == REALIZED)
	size_change();
}

void
Pack::curses_window(YACURS::INTERNAL::CursWin* _p) {
    WidgetBase::curses_window(_p);

    // We have to make sure that the associated widgets have the same
    // curses window as we do.
    set_all_curses_window();
}

void
Pack::focusgroup_id(fgid_t _id) {
    WidgetBase::focusgroup_id(_id);

    // We have to make sure that the associated widgets have the same
    // Focus Group ID as we do.
    set_all_focusgroup_id();
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
    Size non_dynamic=calc_size_non_dynamic();
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
    if (parent()!=0)
	return parent()->size_change();

    if (realization()!=REALIZED) return true;

    //
    // We're realized
    //

    // We don't use resize() because we would loose the Focus Group
    // ID. So we implement our own resize() again here.
    //
    //resize(Area(position(),
    //		WidgetBase::size_available()) );

    fgid_t save_id = WidgetBase::focusgroup_id();
    Area _save_area = Area(position(), WidgetBase::size_available());

    unrealize();

    focusgroup_id(save_id);

    position(_save_area);

    size_available(_save_area);

    realize();

    // There may be many widgets involved in the resize, thus we emit
    // the same event sequence as by a screen resize. A call
    // refresh(true) produces to much flicker.
    //
    //refresh(true)

    EventQueue::submit(EVT_REFRESH);
    EventQueue::submit(EVT_DOUPDATE);

    return true;
}

bool
Pack::can_focus() const {
    return false;
}

void
Pack::focus(bool) {
    throw CannotFocus();
}

bool
Pack::focus() const {
    return false;
}

void
Pack::refresh(bool immediate) {
    if (!(realization()==REALIZED ||
	  realization()==REALIZING) ) return;

    refresh_all_widgets(immediate);
}

void
Pack::resize(const Area& _a) {
    if (realization()!=REALIZED) return;

    unrealize();

    position(_a);

    size_available(_a);

    realize();
}
