//
// This file is part of libyacurs, 
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

#include <stdexcept>
#include <cassert>
#include <cstdlib>

#include "curs.h"
#include "yacursex.h"
#include "window.h"
#include "area.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "colors.h"

using namespace YACURS;

//
// Private
//
Window::Window(const Window&){
    throw EXCEPTIONS::NotSupported();
}

Window&
Window::operator=(const Window&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
void
Window::key_event_handler(Event& _e) {
    assert(_e==EVT_KEY);

    if (__fgid!=FocusManager::active_focus_group() ||
	__hot_keys.empty() ) return;

    EventEx<int>& event = dynamic_cast<EventEx<int>&>(_e);

    std::map<int,HotKey*>::iterator it=__hot_keys.find(event.data());
    if (it!=__hot_keys.end()) {
	if (it->second!=0) {
	    it->second->action();
	    _e.stop(true);
	}
    }
}


void
Window::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_event_handler));

    if (__widget) __widget->unrealize();
    WindowBase::unrealize();

    // With the introduction of Focus Group IDs, we have to destroy
    // the Focus Group AFTER the Widget has been removed from the
    // Focus Group.
    UNREALIZE_LEAVE;
}

//
// Public
//

Window::Window(const Margin& m): WindowBase(m),
				 __widget(0),
				 __fgid((fgid_t)-1) {
    // It is imperative that a new Focus Group is created before the
    // Widget is realized()!
    __fgid = FocusManager::new_focus_group();
}

Window::~Window() {
    EventQueue::disconnect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_event_handler));

    FocusManager::destroy_focus_group(__fgid);

    if (!__hot_keys.empty()) {
	std::map<int,HotKey*>::iterator it=__hot_keys.begin();
	while(it!=__hot_keys.end()) {
	    if (it->second!=0)
		delete it->second;
	    it++;
	}
	__hot_keys.clear();
    }


    __fgid = (fgid_t)-1;
}

void
Window::widget(WidgetBase* _w) {
    __widget = _w;
}

WidgetBase*
Window::widget() const {
    return __widget;
}

void
Window::add_hotkey(const HotKey& hk) {
    if (__hot_keys[hk.key()]!=0)
	    delete __hot_keys[hk.key()];

    __hot_keys[hk.key()]=hk.clone();
}

void
Window::remove_hotkey(const HotKey& hk) {
    std::map<int,HotKey*>::iterator it=__hot_keys.find(hk.key());
    if (it!=__hot_keys.end()) {
	assert(it->second!=0);
	delete it->second;
	__hot_keys.erase(it);
    }
    return;
}

void
Window::refresh(bool immediate) {
    if (realization()!=REALIZED) return;

    curses_window()->set_color(YACURS::DEFAULT);

    // Setting background also helps getting rid of artifacts of
    // overlapped windows.
    curses_window()->set_bg(YACURS::DEFAULT);

    WindowBase::refresh(immediate);

    assert(__fgid!=(fgid_t)-1);
    FocusManager::focus_group_activate(__fgid);

    if (__widget) __widget->refresh(immediate);
}

void
Window::realize() {
    REALIZE_ENTER;
    WindowBase::realize();

    FocusManager::focus_group_activate(__fgid);

    EventQueue::connect_event(EventConnectorMethod1<Window>(EVT_KEY,this, &Window::key_event_handler));

    if (__widget) {
	assert(__widget->realization()==UNREALIZED);

	// This is imperative, so that the widget also is aware of the
	// new curses window in case we're called in the course of a
	// resize.
	__widget->curses_window(curses_window());

	// This widget does not have another widget as parent.
	__widget->parent(0);

	__widget->focusgroup_id(__fgid);

	__widget->position(widget_area());

	__widget->size_available(widget_area());

	__widget->realize();
    }
    REALIZE_LEAVE;
}
