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


//
// Protected
//
YACURS::INTERNAL::CursWin* WidgetBase::curses_window() const {
    return _curses_window;
}

FocusManager::fgid_t WidgetBase::focusgroup_id() const { return _fgid; }

WidgetBase* WidgetBase::parent() const { return _parent; }

void WidgetBase::can_focus(bool can_focus) { _can_focus = can_focus; }

//
// Private
//

WidgetBase::WidgetBase()
    : _curses_window(0),
      _fgid(FocusManager::nfgid),
      _can_focus(false),
      _focus(false),
      _parent(0) {}

WidgetBase::~WidgetBase() {
    if (_can_focus && _fgid != FocusManager::nfgid)
        FocusManager::focus_group_remove(_fgid, this);
}

void WidgetBase::parent(WidgetBase* p) { _parent = p; }

void WidgetBase::curses_window(YACURS::INTERNAL::CursWin* p) {
    _curses_window = p;
}

void WidgetBase::focusgroup_id(FocusManager::fgid_t id) {
    // Only make changes to the Focus Group if Focus Group ID differs.
    //
    // This ensures that the currently focused Widget does not loose
    // the focus when resizing.
    if (_can_focus && _fgid != id) {
        // Remove the widget from the current focus group, if possible
        if (_fgid != FocusManager::nfgid)
            FocusManager::focus_group_remove(_fgid, this);

        // Add the widget to the new focus group if possible
        if (id != FocusManager::nfgid)
            FocusManager::focus_group_add(id, this);
    }
    _fgid = id;
}

void WidgetBase::position(const Coordinates& c) {
    assert(c.x() > -1);
    assert(c.y() > -1);
    _position = c;
}

const Coordinates& WidgetBase::position() const { return _position; }

void WidgetBase::size_available(const Size& s) {
    assert(s.rows() > 0);
    assert(s.cols() > 0);
    _size_available = s;
}

const Size& WidgetBase::size_available() const { return _size_available; }

bool WidgetBase::can_focus() const { return _can_focus; }

void WidgetBase::focus(bool f) {
    if (!_can_focus) throw EXCEPTIONS::CannotFocus();

    _focus = f;
}

bool WidgetBase::focus() const {
    if (!_can_focus) throw EXCEPTIONS::CannotFocus();

    return _focus;
}

void WidgetBase::unrealize() {
    UNREALIZE_ENTER;

    // Intentionally empty

    UNREALIZE_LEAVE;
}
