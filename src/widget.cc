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

#include "yacursex.h"
#include "widget.h"
#include "eventqueue.h"

using namespace YACURS;

//
// Private
//

Widget&
Widget::operator=(const Widget&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
void
Widget::redraw_handler(Event& _e) {
    if (realization() != REALIZED) return;

    assert(_e == EVT_REDRAW);
    assert(__widget_subwin != 0);

    __widget_subwin->clear();
}

void
Widget::force_refresh_handler(Event& _e) {
    if (realization() != REALIZED) return;

    assert(_e == EVT_FORCEREFRESH);
    assert(__widget_subwin != 0);

    __widget_subwin->touch();
}

void
Widget::unrealize() {
    UNREALIZE_ENTER;
    WidgetBase::unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
                                     EVT_FORCEREFRESH, this,
                                     &Widget::force_refresh_handler) );

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
                                     EVT_REDRAW, this,
                                     &Widget::redraw_handler) );

    assert(__widget_subwin != 0);

    try {
        // We have to clear the window since the new size might be
        // smaller, and thus leaving artifacts on the screen if we omit to
        // clear the entire subwin()
        __widget_subwin->clear();

        delete __widget_subwin;
        __widget_subwin = 0;

        // This is also needed to remove artifacts on the screen
        curses_window()->touch();
        curses_window()->refresh();
    } catch (EXCEPTIONS::CursesException&) {
        if (__widget_subwin != 0)
            delete __widget_subwin;

        __widget_subwin = 0;
        realization(UNREALIZED);
        throw;
    }

    UNREALIZE_LEAVE;
}

YACURS::INTERNAL::CursWin*
Widget::widget_subwin() const {
    return __widget_subwin;
}

//
// Public
//

Widget::Widget() : __widget_subwin(0) {
}

Widget::~Widget() {
    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
                                     EVT_FORCEREFRESH, this,
                                     &Widget::force_refresh_handler) );

    EventQueue::disconnect_event(EventConnectorMethod1<Widget>(
                                     EVT_REDRAW, this,
                                     &Widget::redraw_handler) );

    if (realization() == REALIZED) {
        assert(__widget_subwin != 0);
        delete __widget_subwin;
    }
}

void
Widget::refresh(bool immediate) {
    if (!(realization() == REALIZED ||
          realization() == REALIZING) ) return;

    assert(__widget_subwin != 0);
    assert(focusgroup_id() != FocusManager::nfgid);

    __widget_subwin->refresh(immediate);
}

void
Widget::resize(const Area& _a) {
    //
    // 1. Keep in mind: a resize does not refresh!
    //
    // 2. The actual resize has to be done in a derived class
    //
    if (!(realization() == REALIZED)) return;

    unrealize();

    position(_a);
    size_available(_a);

    realize();
}

void
Widget::realize() {
    REALIZE_ENTER;

    const Coordinates& pos = position();
    const Size& _size = size();
    const Size& size_a = size_available();

    assert(focusgroup_id() != FocusManager::nfgid);

    EventQueue::connect_event(EventConnectorMethod1<Widget>(EVT_FORCEREFRESH,
                                                            this,
                                                            &Widget::
                                                            force_refresh_handler) );

    EventQueue::connect_event(EventConnectorMethod1<Widget>(EVT_REDRAW,
                                                            this,
                                                            &Widget::
                                                            redraw_handler) );

    // We cannot assert on parent() since it might be legally 0
    // assert(parent()!=0

    assert(_size != Size::zero() );
    assert(size_a != Size::zero() );

    if (size_a.rows() < 1 ||
        _size.rows() > size_a.rows() ||
        _size.cols() > size_a.cols() )
        return;

    assert(curses_window() != 0);
    assert(__widget_subwin == 0);

    try {
        __widget_subwin = curses_window()->subwin(Area(pos, _size) );
        __widget_subwin->scrollok(false);
        __widget_subwin->leaveok(true);
    } catch (EXCEPTIONS::CursesException&) {
        realization(UNREALIZED);
        if (__widget_subwin != 0) {
            delete __widget_subwin;
            __widget_subwin = 0;
        }
        throw;
    }

    REALIZE_LEAVE;
}
