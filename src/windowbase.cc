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
#include <cerrno>
#include <cstdlib>

#include "curs.h"
#include "eventqueue.h"
#include "windowbase.h"
#include "yacursconst.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
YACURS::INTERNAL::CursWin* WindowBase::curses_window() const {
    return _curses_window;
}

const Area& WindowBase::area() const { return _area; }

Area WindowBase::widget_area() const {
    Area widget_area;

    if (_frame) {
        // we have to take the frame into account, thus
        // minus Margin(1,1,1,1)
        widget_area = (_area - _margin) - Margin(1, 1, 1, 1);
    } else {
        widget_area = _area - _margin;
    }

    return widget_area;
}

void WindowBase::unrealize() {
    UNREALIZE_ENTER;

    assert(_curses_window != 0);

    delete _curses_window;
    _curses_window = 0;

    UNREALIZE_LEAVE;
}

bool WindowBase::on_close() { return true; }

//
// Public
//

WindowBase::WindowBase(const Margin& m)
    : _area(Coordinates(), Curses::inquiry_screensize()),
      _margin(m),
      _curses_window(0),
      _frame(false),
      _shown(false),
      _color(DEFAULT) {
    // We always want to receive this event. Therefore it was moved
    // from show() to ctor.
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(
        EVT_SIGWINCH, this, &WindowBase::resize_handler));
}

WindowBase::~WindowBase() {
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_REDRAW, this, &WindowBase::redraw_handler));

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_FORCEREFRESH, this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_REFRESH, this, &WindowBase ::refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_SIGWINCH, this, &WindowBase::resize_handler));

    if (realization() == REALIZED) {
        assert(_curses_window != 0);
        delete _curses_window;
    }
}

void WindowBase::margin(const Margin& m) {
    if (realization() == REALIZED) throw EXCEPTIONS::AlreadyRealized();
    _margin = m;
}

const Margin& WindowBase::margin() const { return _margin; }

bool WindowBase::frame() const { return _frame; }

void WindowBase::frame(bool b) { _frame = b; }

void WindowBase::color(COLOROBJ c) { _color = c; }

COLOROBJ
WindowBase::color() const { return _color; }

void WindowBase::show() {
    if (realization() != UNREALIZED) return;

    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(
        EVT_REDRAW, this, &WindowBase::redraw_handler));

    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(
        EVT_FORCEREFRESH, this, &WindowBase::force_refresh_handler));
    EventQueue::connect_event(EventConnectorMethod1<WindowBase>(
        EVT_REFRESH, this, &WindowBase::refresh_handler));

    realize();
    refresh(true);
    EventQueue::submit(EventEx<WindowBase*>(EVT_WINDOW_SHOW, this));

    _shown = true;
}

void WindowBase::close() {
    if (realization() != REALIZED) return;

    if (!on_close()) return;

    unrealize();

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_REDRAW, this, &WindowBase::redraw_handler));

    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_FORCEREFRESH, this, &WindowBase::force_refresh_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<WindowBase>(
        EVT_REFRESH, this, &WindowBase ::refresh_handler));

    // We might have obstructed another window, so make sure it
    // receives a refresh.
    //
    // See also Window::refresh().
    EventQueue::submit(EVT_FORCEREFRESH);
    EventQueue::submit(EVT_REFRESH);
    EventQueue::submit(EVT_DOUPDATE);

    // Change: earlier, it was submitted before
    // EVT_REFRESH/EVT_DOUPDATE
    //
    // This caused problems with the focus manager, if a Label was
    // updated in the EVT_WINDOW_CLOSE handler.
    EventQueue::submit(EventEx<WindowBase*>(EVT_WINDOW_CLOSE, this));

    _shown = false;
}

bool WindowBase::shown() const { return _shown; }

void WindowBase::redraw_handler(Event& e) {
    if (realization() != REALIZED) return;

    assert(e == EVT_REDRAW);
    assert(_curses_window != 0);

    _curses_window->clear();
}

void WindowBase::force_refresh_handler(Event& e) {
    if (realization() != REALIZED) return;

    assert(e == EVT_FORCEREFRESH);
    assert(_curses_window != 0);

    _curses_window->touch();
}

void WindowBase::refresh_handler(Event& e) {
    assert(e == EVT_REFRESH);
    refresh(false);
}

void WindowBase::resize_handler(Event& e) {
    assert(e == EVT_SIGWINCH);
    EventEx<Size>& winch = dynamic_cast<EventEx<Size>&>(e);
    resize(Area(Coordinates(0, 0), winch.data()));
}

void WindowBase::refresh(bool immediate) {
    if (realization() != REALIZED && realization() != REALIZING) return;

    assert(_curses_window != 0);

    _curses_window->refresh(immediate);
}

void WindowBase::resize(const Area& a) {
    //
    // Keep in mind: a resize does not refresh!
    //

    if (realization() != REALIZED) {
        // Even if we're not realized, we keep track of the area at
        // our disposition.
        //
        // This was mainly introduced for the screen unlock dialog.
        _area = a;
        return;
    }

    assert(a.x() > -1);
    assert(a.y() > -1);
    assert(a.rows() > 0);
    assert(a.cols() > 0);

    unrealize();

    _area = a;

    realize();
}

void WindowBase::realize() {
    REALIZE_ENTER;

    assert(_area.x() >= 0);
    assert(_area.y() >= 0);
    assert(_area.rows() > 0);
    assert(_area.cols() > 0);

    Area _tmp = _area - _margin;

    if (_tmp.x() < 0 || _tmp.y() < 0 || _tmp.rows() < MIN_WINDOW_ROWS ||
        _tmp.cols() < MIN_WINDOW_COLS) {
        realization(UNREALIZED);
        return;
    }

    assert(_curses_window == 0);
    try {
        _curses_window = new YACURS::INTERNAL::CursWin(_tmp, _color);
        _curses_window->scrollok(false);
        _curses_window->leaveok(true);

        if (_frame) {
            _curses_window->box();
        }
    } catch (EXCEPTIONS::CursesException&) {
        if (_curses_window != 0) {
            delete _curses_window;
            _curses_window = 0;
        }
        realization(UNREALIZED);
        throw;
    }

    REALIZE_LEAVE;
}
