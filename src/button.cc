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

#include "button.h"
#include "colors.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
void Button::key_handler(Event& e) {
    assert(e.type() == EVT_KEY);

    if (!focus()) return;

#ifdef YACURS_USE_WCHAR
    EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(e);
#else
    EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(e);
#endif

    switch (ekey.data()) {
        case KEY_DOWN:
        case KEY_RIGHT:
        case KEY_TAB:
            EventQueue::submit(EVT_FOCUS_NEXT);
            break;

        case KEY_UP:
        case KEY_LEFT:
        case KEY_BTAB:
            EventQueue::submit(EVT_FOCUS_PREVIOUS);
            break;

        case KEY_ENTER:
        case KEY_RETURN:
        case KEY_RETURN2:
            if (!_enabled) return;
            EventQueue::submit(EventEx<Button*>(EVT_BUTTON_PRESS, this));
            break;
    }
}

void Button::realize() {
    REALIZE_ENTER;

    Label::realize();

    EventQueue::connect_event(
        EventConnectorMethod1<Button>(EVT_KEY, this, &Button::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    REALIZE_LEAVE;
}

void Button::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(
        EventConnectorMethod1<Button>(EVT_KEY, this, &Button::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    Label::unrealize();

    UNREALIZE_LEAVE;
}

//
// Public
//

Button::Button(const std::string& b) : _enabled(true) {
    can_focus(true);

    // We want our label() to be called, so we don't use the Label()
    // constructor to set the label
    label(b);
}

Button::~Button() {
    EventQueue::disconnect_event(
        EventConnectorMethod1<Button>(EVT_KEY, this, &Button::key_handler));
}

void Button::label(const std::string& l) {
    _tmp_label = l;
    if (_enabled)
        Label::label("[ " + l + " ]");
    else
        Label::label("( " + l + " )");
}

const std::string& Button::label() const { return _tmp_label; }

void Button::enabled(bool _f) {
    _enabled = _f;
    // Re-set label
    label(_tmp_label);
}

bool Button::enabled() const { return _enabled; }

void Button::refresh(bool immediate) {
    if (realization() != REALIZED) return;

    assert(widget_subwin() != 0);

    if (focus()) {
        color(YACURS::BUTTON_FOCUS);
        widget_subwin()->leaveok(false);
    } else {
        color(YACURS::BUTTON_NOFOCUS);
        widget_subwin()->leaveok(true);
    }

    Label::refresh(immediate);
}
