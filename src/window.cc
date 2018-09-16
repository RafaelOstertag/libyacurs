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
#include <stdexcept>

#include "area.h"
#include "colors.h"
#include "curs.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "window.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
void Window::key_event_handler(Event& e) {
    assert(e == EVT_KEY);

    if (_fgid != FocusManager::active_focus_group() || _hot_keys.empty())
        return;

#ifdef YACURS_USE_WCHAR
    EventEx<wint_t>& event = dynamic_cast<EventEx<wint_t>&>(e);
#else
    EventEx<int>& event = dynamic_cast<EventEx<int>&>(e);
#endif

    std::map<int, HotKey*>::iterator it = _hot_keys.find(event.data());
    if (it != _hot_keys.end()) {
        if (it->second != 0) {
            it->second->action();
            e.stop(true);
        }
    }
}

void Window::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Window>(
        EVT_KEY, this, &Window::key_event_handler));

    if (_widget) _widget->unrealize();
    WindowBase::unrealize();

    // With the introduction of Focus Group IDs, we have to destroy
    // the Focus Group AFTER the Widget has been removed from the
    // Focus Group.
    UNREALIZE_LEAVE;
}

//
// Public
//

Window::Window(const Margin& m)
    : WindowBase(m), _widget(0), _fgid(FocusManager::nfgid) {
    // It is imperative that a new Focus Group is created before the
    // Widget is realized()!
    _fgid = FocusManager::new_focus_group();
}

Window::~Window() {
    EventQueue::disconnect_event(EventConnectorMethod1<Window>(
        EVT_KEY, this, &Window::key_event_handler));

    FocusManager::destroy_focus_group(_fgid);

    if (!_hot_keys.empty()) {
        std::map<int, HotKey*>::iterator it = _hot_keys.begin();
        while (it != _hot_keys.end()) {
            if (it->second != 0) delete it->second;
            it++;
        }
        _hot_keys.clear();
    }

    _fgid = FocusManager::nfgid;
}

void Window::widget(WidgetBase* w) { _widget = w; }

WidgetBase* Window::widget() const { return _widget; }

void Window::add_hotkey(const HotKey& hk) {
    if (_hot_keys[hk.key()] != 0) delete _hot_keys[hk.key()];

    _hot_keys[hk.key()] = hk.clone();
}

void Window::remove_hotkey(const HotKey& hk) {
    std::map<int, HotKey*>::iterator it = _hot_keys.find(hk.key());

    if (it != _hot_keys.end()) {
        assert(it->second != 0);
        delete it->second;
        _hot_keys.erase(it);
    }
    return;
}

void Window::refresh(bool immediate) {
    if (realization() != REALIZED) return;

    curses_window()->set_color(color());

    WindowBase::refresh(immediate);

    assert(_fgid != FocusManager::nfgid);
    FocusManager::focus_group_activate(_fgid);

    if (_widget) _widget->refresh(immediate);
}

void Window::realize() {
    REALIZE_ENTER;
    WindowBase::realize();

    FocusManager::focus_group_activate(_fgid);

    EventQueue::connect_event(EventConnectorMethod1<Window>(
        EVT_KEY, this, &Window::key_event_handler));

    if (_widget) {
        assert(_widget->realization() == UNREALIZED);

        // This is imperative, so that the widget also is aware of the
        // new curses window in case we're called in the course of a
        // resize.
        _widget->curses_window(curses_window());

        // This widget does not have another widget as parent.
        _widget->parent(0);

        _widget->focusgroup_id(_fgid);

        _widget->position(widget_area());

        _widget->size_available(widget_area());

        _widget->realize();
    }
    REALIZE_LEAVE;
}
