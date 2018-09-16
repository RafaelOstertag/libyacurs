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

#include <cassert>
#include <cstdlib>

#include "eventqueue.h"
#include "widget.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
void Widget::redraw_handler(Event& e) {
    if (realization() != REALIZED) return;

    assert(e == EVT_REDRAW);
    assert(_widget_subwin != 0);

    _widget_subwin->clear();
}

void Widget::force_refresh_handler(Event& e) {
    if (realization() != REALIZED) return;

    assert(e == EVT_FORCEREFRESH);
    assert(_widget_subwin != 0);

    _widget_subwin->touch();
}

void Widget::unrealize() {
    UNREALIZE_ENTER;
    WidgetBase::unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
        EVT_FORCEREFRESH, this, &Widget::force_refresh_handler));

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
        EVT_REDRAW, this, &Widget::redraw_handler));

    assert(_widget_subwin != 0);

    try {
        // We have to clear the window since the new size might be
        // smaller, and thus leaving artifacts on the screen if we omit to
        // clear the entire subwin()
        _widget_subwin->clear();

        delete _widget_subwin;
        _widget_subwin = 0;

        // This is also needed to remove artifacts on the screen
        curses_window()->touch();
        curses_window()->refresh();
    } catch (EXCEPTIONS::CursesException&) {
        if (_widget_subwin != 0) delete _widget_subwin;

        _widget_subwin = 0;
        realization(UNREALIZED);
        throw;
    }

    UNREALIZE_LEAVE;
}

YACURS::INTERNAL::CursWin* Widget::widget_subwin() const {
    return _widget_subwin;
}

//
// Public
//

Widget::Widget() : _widget_subwin(0) {}

Widget::~Widget() {
    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
        EVT_FORCEREFRESH, this, &Widget::force_refresh_handler));

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
        EVT_REDRAW, this, &Widget::redraw_handler));

    if (realization() == REALIZED) {
        assert(_widget_subwin != 0);
        delete _widget_subwin;
    }
}

void Widget::refresh(bool immediate) {
    if (!(realization() == REALIZED || realization() == REALIZING)) return;

    assert(_widget_subwin != 0);
    assert(focusgroup_id() != FocusManager::nfgid);

    _widget_subwin->refresh(immediate);
}

void Widget::resize(const Area& a) {
    //
    // 1. Keep in mind: a resize does not refresh!
    //
    // 2. The actual resize has to be done in a derived class
    //
    if (!(realization() == REALIZED)) return;

    unrealize();

    position(a);
    size_available(a);

    realize();
}

void Widget::realize() {
    REALIZE_ENTER;

    const Coordinates& pos = position();
    const Size& _size = size();
    const Size& size_a = size_available();

    assert(focusgroup_id() != FocusManager::nfgid);

    EventQueue::connect_event(EventConnectorMethod1<Widget>(
        EVT_FORCEREFRESH, this, &Widget::force_refresh_handler));

    EventQueue::connect_event(EventConnectorMethod1<Widget>(
        EVT_REDRAW, this, &Widget::redraw_handler));

    // We cannot assert on parent() since it might be legally 0
    // assert(parent()!=0

    assert(_size != Size::zero());
    assert(size_a != Size::zero());

    if (size_a.rows() < 1 || _size.rows() > size_a.rows() ||
        _size.cols() > size_a.cols())
        return;

    assert(curses_window() != 0);
    assert(_widget_subwin == 0);

    try {
        _widget_subwin = curses_window()->subwin(Area(pos, _size));
        _widget_subwin->scrollok(false);
        _widget_subwin->leaveok(true);
    } catch (EXCEPTIONS::CursesException&) {
        realization(UNREALIZED);
        if (_widget_subwin != 0) {
            delete _widget_subwin;
            _widget_subwin = 0;
        }
        throw;
    }

    REALIZE_LEAVE;
}
