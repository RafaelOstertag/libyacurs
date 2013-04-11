/*
 * This file is part of libyacurs, 
 * Copyright (C) 2013  Rafael Ostertag
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

using namespace YACURS;

const char* Event::__str_table[] = {
    "EVT_QUIT",
    "EVT_SIGWINCH",
    "EVT_SIGALRM",
    "EVT_KEY",
    "EVT_REFRESH",
    "EVT_FORCEREFRESH",
    "EVT_DOUPDATE",
    "EVT_TERMRESETUP",
    "EVT_SIGUSR1",
    "EVT_SIGUSR2",
    "EVT_SIGINT",
    "EVT_FOCUS_NEXT",
    "EVT_FOCUS_PREVIOUS",
    "EVT_WINDOW_SHOW",
    "EVT_WINDOW_CLOSE",
    "EVT_BUTTON_PRESS",
    "EVT_LISTBOX_ENTER"
};

////////////////////////////////////////////////////////
//
// class Event
//
////////////////////////////////////////////////////////

Event::Event(EVENT_TYPE _et): event_type(_et), __stop(false) {}

Event::~Event() {}

bool
Event::operator==(const Event& _e) const {
    return event_type == _e.event_type;
}

bool
Event::operator==(EVENT_TYPE _et) const {
    return event_type == _et;
}

EVENT_TYPE
Event::type() const {
    return event_type;
}

void
Event::stop(bool _s) {
    __stop = _s;
}

bool
Event::stop() const {
    return __stop;
}

Event*
Event::clone() const {
    return new Event(*this);
}

Event::operator EVENT_TYPE() const {
    return event_type;
}

std::string
Event::evt2str(EVENT_TYPE _et) {
    return __str_table[_et];
}
