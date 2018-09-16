//
// This file is part of libyacurs.
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>

#include "eventqueue.h"
#include "pack.h"
#include "yacursex.h"

using namespace YACURS;

//
// Functors
//

//
// Private
//
void Pack::set_all_curses_window() {
    std::for_each(
        widget_list.begin(), widget_list.end(),
        std::bind2nd(std::mem_fun<void, WidgetBase, YACURS::INTERNAL::CursWin*>(
                         &WidgetBase ::curses_window),
                     WidgetBase::curses_window()));
}

void Pack::set_all_focusgroup_id() {
    std::for_each(
        widget_list.begin(), widget_list.end(),
        std::bind2nd(std::mem_fun<void, WidgetBase, FocusManager::fgid_t>(
                         &WidgetBase ::focusgroup_id),
                     WidgetBase::focusgroup_id()));
}

void Pack::refresh_all_widgets(bool i) {
    std::for_each(
        widget_list.begin(), widget_list.end(),
        std::bind2nd(std::mem_fun<void, WidgetBase, bool>(&WidgetBase::refresh),
                     i));
}

void Pack::take_over(WidgetBase* w) {
    assert(w != 0);

    w->parent(this);
    w->curses_window(WidgetBase::curses_window());
    w->focusgroup_id(WidgetBase::focusgroup_id());
}

//
// Protected
//
void Pack::unrealize() {
    UNREALIZE_ENTER;
    WidgetBase::unrealize();

    std::for_each(widget_list.begin(), widget_list.end(),
                  std::mem_fun(&WidgetBase::unrealize));

    // Required since pack is a dynamically sized Widget.
    reset_size();

    UNREALIZE_LEAVE;
}

//
// Public
//
Pack::Pack() : _hinting(true), _always_dynamic(false) {}

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

std::list<WidgetBase*>::size_type Pack::widgets() const {
    return widget_list.size();
}

void Pack::add_front(WidgetBase* w) {
    assert(w != 0);

    widget_list.push_front(w);
    take_over(w);

    if (realization() == REALIZED) {
        if (w->realization() == REALIZED) w->unrealize();

        size_change();
    }
}

void Pack::add_back(WidgetBase* w) {
    assert(w != 0);

    widget_list.push_back(w);

    take_over(w);

    if (realization() == REALIZED) {
        if (w->realization() == REALIZED) w->unrealize();

        size_change();
    }
}

void Pack::remove(WidgetBase* w) {
    assert(w != 0);

    //    if (realization()!=UNREALIZED) throw AlreadyRealized();

    widget_list.remove(w);

    if (w->realization() != UNREALIZED) w->unrealize();

    w->focusgroup_id(FocusManager::nfgid);

    if (realization() == REALIZED) size_change();
}

void Pack::curses_window(YACURS::INTERNAL::CursWin* p) {
    WidgetBase::curses_window(p);

    // We have to make sure that the associated widgets have the same
    // curses window as we do.
    set_all_curses_window();
}

void Pack::focusgroup_id(FocusManager::fgid_t id) {
    WidgetBase::focusgroup_id(id);

    // We have to make sure that the associated widgets have the same
    // Focus Group ID as we do.
    set_all_focusgroup_id();
}

void Pack::size_available(const Size& s) {
    WidgetBase::size_available(_size = s);
}

Size Pack::size() const {
    // If we are always dynamic, simply return the current value of
    // _size, which may be Size::zero() if a call was made to
    // reset_size().
    if (_always_dynamic) return _size;

    // Since we are not always dynamic, see if we can figure out the
    // size. This only succeeds if there are no dynamically sized
    // widgets.
    Size non_dynamic = calc_size_non_dynamic();
    if (non_dynamic != Size::zero()) return non_dynamic;

    // _size might have been set by a former call to size_available()
    // so return that.
    return _size;
}

void Pack::hinting(bool h) {
    _hinting = h;
    size_change();
}

bool Pack::hinting() const { return _hinting; }

void Pack::always_dynamic(bool d) {
    _always_dynamic = d;
    size_change();
}

bool Pack::always_dynamic() const { return _always_dynamic; }

void Pack::reset_size() { _size = Size::zero(); }

bool Pack::size_change() {
    if (parent() != 0) return parent()->size_change();

    if (realization() != REALIZED) return true;

    //
    // We're realized
    //

    // We don't use resize() because we would loose the Focus Group
    // ID. So we implement our own resize() again here.
    //
    // resize(Area(position(),
    //		WidgetBase::size_available()) );

    FocusManager::fgid_t save_id = WidgetBase::focusgroup_id();
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
    // refresh(true)

    EventQueue::submit(EVT_REFRESH);
    EventQueue::submit(EVT_DOUPDATE);

    return true;
}

bool Pack::can_focus() const { return false; }

void Pack::focus(bool) { throw EXCEPTIONS::CannotFocus(); }

bool Pack::focus() const { return false; }

void Pack::refresh(bool immediate) {
    if (!(realization() == REALIZED || realization() == REALIZING)) return;

    refresh_all_widgets(immediate);
}

void Pack::resize(const Area& a) {
    if (realization() != REALIZED) return;

    unrealize();

    position(a);

    size_available(a);

    realize();
}
