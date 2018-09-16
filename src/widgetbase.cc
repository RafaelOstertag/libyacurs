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

#include <cassert>
#include <cstdlib>

#include "focusmanager.h"
#include "widgetbase.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

WidgetBase::WidgetBase(const WidgetBase&) { throw EXCEPTIONS::NotSupported(); }

WidgetBase& WidgetBase::operator=(const WidgetBase&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
YACURS::INTERNAL::CursWin* WidgetBase::curses_window() const {
    return __curses_window;
}

FocusManager::fgid_t WidgetBase::focusgroup_id() const { return __fgid; }

WidgetBase* WidgetBase::parent() const { return __parent; }

void WidgetBase::can_focus(bool _can_focus) { __can_focus = _can_focus; }

//
// Private
//

WidgetBase::WidgetBase()
    : __curses_window(0),
      __fgid(FocusManager::nfgid),
      __can_focus(false),
      __focus(false),
      __parent(0) {}

WidgetBase::~WidgetBase() {
    if (__can_focus && __fgid != FocusManager::nfgid)
        FocusManager::focus_group_remove(__fgid, this);
}

void WidgetBase::parent(WidgetBase* _p) { __parent = _p; }

void WidgetBase::curses_window(YACURS::INTERNAL::CursWin* _p) {
    __curses_window = _p;
}

void WidgetBase::focusgroup_id(FocusManager::fgid_t _id) {
    // Only make changes to the Focus Group if Focus Group ID differs.
    //
    // This ensures that the currently focused Widget does not loose
    // the focus when resizing.
    if (__can_focus && __fgid != _id) {
        // Remove the widget from the current focus group, if possible
        if (__fgid != FocusManager::nfgid)
            FocusManager::focus_group_remove(__fgid, this);

        // Add the widget to the new focus group if possible
        if (_id != FocusManager::nfgid)
            FocusManager::focus_group_add(_id, this);
    }
    __fgid = _id;
}

void WidgetBase::position(const Coordinates& _c) {
    assert(_c.x() > -1);
    assert(_c.y() > -1);
    __position = _c;
}

const Coordinates& WidgetBase::position() const { return __position; }

void WidgetBase::size_available(const Size& _s) {
    assert(_s.rows() > 0);
    assert(_s.cols() > 0);
    __size_available = _s;
}

const Size& WidgetBase::size_available() const { return __size_available; }

bool WidgetBase::can_focus() const { return __can_focus; }

void WidgetBase::focus(bool _f) {
    if (!__can_focus) throw EXCEPTIONS::CannotFocus();

    __focus = _f;
}

bool WidgetBase::focus() const {
    if (!__can_focus) throw EXCEPTIONS::CannotFocus();

    return __focus;
}

void WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    // Intentionally empty

    UNREALIZE_LEAVE;
}
