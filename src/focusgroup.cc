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

#include <algorithm>
#include <cassert>
#include <functional>

#include "focusgroup.h"
#include "widgetbase.h"
#include "yacursdbg.h"

using namespace YACURS;

//
// Functors
//
#ifndef NDEBUG
namespace YACURS {
namespace FUNCTORS {
namespace FOCUSGROUP {
/**
 * Class for debugging.
 *
 * Functor counting the number of widgets having focus in
 * the Focus Group.
 *
 * Usually, this should either be 1 or 0.
 */
class CountFocus {
   private:
    int _count;

   public:
    CountFocus() : _count(0) {}

    void operator()(const WidgetBase* w) {
        if (w->focus()) _count++;
    }

    int count() const { return _count; }
};
}  // namespace FOCUSGROUP
}  // namespace FUNCTORS
}  // namespace YACURS
#endif

//
// Private
//

//
// Protected
//

//
// Public
//

FocusGroup::FocusGroup() : _active{false}, _focus{_widgets.end()} {
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] Created Focus Group.");
}

FocusGroup::FocusGroup(const FocusGroup& f)
    : _active{f._active}, _widgets{f._widgets}, _focus{f._focus} {}

FocusGroup::FocusGroup(FocusGroup&& f)
    : _active{f._active},
      _widgets{std::move(f._widgets)},
      _focus{std::move(f._focus)} {}

FocusGroup::~FocusGroup() {
    if (_widgets.empty()) return;

    assert(_focus != _widgets.end());
    assert((*_focus) != 0);
    (*_focus)->focus(false);
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this
                               << "] Destroyed Focus Group. Focus taken from "
                               << (void*)(*_focus));
}

FocusGroup& FocusGroup::operator=(const FocusGroup& f) {
    if (this == &f) return *this;
    _active = f._active;
    _widgets = f._widgets;
    _focus = f._focus;
    return *this;
}

FocusGroup& FocusGroup::operator=(FocusGroup&& f) {
    if (this == &f) return *this;
    _active = f._active;
    _widgets = std::move(f._widgets);
    _focus = std::move(f._focus);
    return *this;
}

void FocusGroup::activate() {
    if (_active) return;

    _active = true;

    if (_widgets.empty()) return;

    // If we are called the first time, make the first Widget getting
    // the focus. Else do not change _focus, allowing to resume the
    // last state of the focus in the group.
    if (_focus == _widgets.end()) _focus = _widgets.begin();

    assert((*_focus) != 0);

    (*_focus)->focus(true);

    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this
                               << "] Activated Focus Group. Focus given to "
                               << (void*)(*_focus));

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    FUNCTORS::FOCUSGROUP::CountFocus cf = std::for_each(
        _widgets.begin(), _widgets.end(), FUNCTORS::FOCUSGROUP::CountFocus());
    assert(cf.count() < 2);
#endif  // NDEBUG
}

void FocusGroup::deactivate() {
    if (!_active) return;

    _active = false;

    if (_widgets.empty()) return;

    assert(_focus != _widgets.end());
    assert((*_focus) != 0);

    (*_focus)->focus(false);

    DEBUGOUT(DBG_FOCUSGRP,
             "[" << (void*)this
                 << "] deactivated Focus Group and focus taken from "
                 << (void*)(*_focus));
#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    FUNCTORS::FOCUSGROUP::CountFocus cf = std::for_each(
        _widgets.begin(), _widgets.end(), FUNCTORS::FOCUSGROUP::CountFocus());
    assert(cf.count() == 0);
#endif  // NDEBUG
}

bool FocusGroup::active() const { return _active; }

void FocusGroup::add(WidgetBase* w) {
    assert(w != 0);

    // If the Focus Groups is empty but activate, we set the focus to
    // the first Widget added, so that we don't end up with an
    // activated Focus Group where no widget has the focus.
    //
    // In any other case, activate() will take (has taken) care of
    // giving the focus to a widget.
    if (_widgets.empty()) {
        _widgets.push_back(w);
        _focus = _widgets.begin();
        (*_focus)->focus(true);
#ifndef NDEBUG
        // Solaris Studio 12.3 forced me to do it that way, i.e. with
        // functor.
        FUNCTORS::FOCUSGROUP::CountFocus cf =
            std::for_each(_widgets.begin(), _widgets.end(),
                          FUNCTORS::FOCUSGROUP::CountFocus());
        assert(cf.count() == 1);
#endif  // NDEBUG
    } else {
        _widgets.push_back(w);
    }

    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] widget " << (void*)(w)
                               << " added to Focus Group");
}

void FocusGroup::remove(WidgetBase* w) {
    assert(w != 0);
    assert(!_widgets.empty());

    // Will be set to true, if the removed widget had the focus
    bool removed_had_focus = false;

    // Find out, whether the Widget to be removed has the focus, if
    // so, give the focus to another widget.
    if ((*_focus) == w) {
        w->focus(false);

        removed_had_focus = true;

        if (++_focus == _widgets.end()) _focus = _widgets.begin();
    }

    _widgets.remove(w);

    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] widget " << (void*)(w)
                               << " removed from Focus Group");

    // No more widgets left, we cannot assign focus to anything.
    if (_widgets.empty()) return;

    // Make sure we don't point at the end of the list
    if (removed_had_focus) (*_focus)->focus(true);

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    FUNCTORS::FOCUSGROUP::CountFocus cf = std::for_each(
        _widgets.begin(), _widgets.end(), FUNCTORS::FOCUSGROUP::CountFocus());
    assert(cf.count() < 2);
#endif  // NDEBUG
}

void FocusGroup::focus_next() {
    assert(!_widgets.empty());
    if (!_active) return;

    // If activate() has been called, _focus must point to something,
    // but surely not the end().
    assert(_focus != _widgets.end());

    // Not that it would be an error calling focus(false) on a Widget
    // not having the focus, but it would show that something within
    // the FocusGroup is weird.
    assert((*_focus)->focus());

    // remove focus of current Widget.
    (*_focus)->focus(false);
    (*_focus)->refresh(true);
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus_next) widget "
                               << (void*)(*_focus) << " lost focus");

    // Then, advance to the next widget. If we hit the end, we start
    // at the beginning again.
    if (++_focus == _widgets.end()) _focus = _widgets.begin();

    (*_focus)->focus(true);
    (*_focus)->refresh(true);

    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus_next) widget "
                               << (void*)(*_focus) << " got focus");

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    FUNCTORS::FOCUSGROUP::CountFocus cf = std::for_each(
        _widgets.begin(), _widgets.end(), FUNCTORS::FOCUSGROUP::CountFocus());
    assert(cf.count() == 1);
#endif  // NDEBUG
}

void FocusGroup::focus_previous() {
    assert(!_widgets.empty());
    if (!_active) return;

    // If activate() has been called, _focus must point to something,
    // but surely not the end().
    assert(_focus != _widgets.end());

    // Not that it would be an error calling focus(false) on a Widget
    // not having the focus, but it would show that something within
    // the FocusGroup is weird.
    assert((*_focus)->focus());

    // remove focus of current Widget.
    (*_focus)->focus(false);
    (*_focus)->refresh(true);
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus_previous) widget "
                               << (void*)(*_focus) << " lost focus");

    // Then, advance to the previous widget. If we are already at the
    // start, wrap to the last widget
    if (_focus == _widgets.begin()) _focus = _widgets.end();

    _focus--;

    (*_focus)->focus(true);
    (*_focus)->refresh(true);
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus_previous) widget "
                               << (void*)(*_focus) << " got focus");

#ifndef NDEBUG
    // Solaris Studio 12.3 forced me to do it that way, i.e. with
    // functor.
    FUNCTORS::FOCUSGROUP::CountFocus cf = std::for_each(
        _widgets.begin(), _widgets.end(), FUNCTORS::FOCUSGROUP::CountFocus());
    assert(cf.count() == 1);
#endif  // NDEBUG
}

void FocusGroup::refocus() const {
    if (_widgets.empty()) return;

    (*_focus)->focus(true);
    (*_focus)->refresh(true);
    DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] widget " << (void*)(*_focus)
                               << " refocused");
}

void FocusGroup::reset() {
    if (_widgets.empty()) return;

    if (_active && _focus != _widgets.end()) {
        (*_focus)->focus(false);
        (*_focus)->refresh(true);
        DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus reset) widget "
                                   << (void*)(*_focus) << " lost focus");
    }

    _focus = _widgets.begin();

    if (_active) {
        (*_focus)->focus(true);
        (*_focus)->refresh(true);
        DEBUGOUT(DBG_FOCUSGRP, "[" << (void*)this << "] (focus reset) widget "
                                   << (void*)(*_focus) << " got focus");
    }
}
