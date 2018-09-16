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

#include <gettext.h>

#include "event.h"

using namespace YACURS;

////////////////////////////////////////////////////////
//
// class EventType
//
////////////////////////////////////////////////////////
EventType::EventType(std::string name) : _hashval(0x811c9dc5) {
    if (name.length() > MAX_NAME_LENGTH - 1)
        throw std::length_error(
            _("Event names must not be greater than 63 characters."));

    if (name.empty())
        throw std::length_error(_("Event names must not be empty."));

    // See http://isthe.com/chongo/tech/comp/fnv/
    std::string::size_type len = name.length();
    for (std::string::size_type i = 0; i < len; i++) {
        _hashval *= 0x01000193;
        _hashval ^= (uint32_t)name[i];
    }

    strncpy(_name, name.c_str(), MAX_NAME_LENGTH - 1);
    _name[MAX_NAME_LENGTH - 1] = 0;
}

EventType::EventType(const EventType& et) : _hashval(et._hashval) {
    strncpy(_name, et._name, MAX_NAME_LENGTH);
}

EventType::~EventType() {}

EventType& EventType::operator=(const EventType& rhs) {
    if (this == &rhs) return *this;

    strncpy(_name, rhs._name, MAX_NAME_LENGTH);
    _hashval = rhs._hashval;

    return *this;
}

bool EventType::operator==(const EventType& rhs) const {
    return _hashval == rhs._hashval;
}

bool EventType::operator!=(const EventType& rhs) const {
    return !operator==(rhs);
}

bool EventType::operator<(const EventType& rhs) const {
    return _hashval < rhs._hashval;
}

bool EventType::operator>(const EventType& rhs) const {
    return _hashval > rhs._hashval;
}

EventType::operator std::string() const { return _name; }

EventType::operator uint32_t() const { return _hashval; }

////////////////////////////////////////////////////////
//
// class Event
//
////////////////////////////////////////////////////////

Event::Event(const EventType et) : event_type(et), _stop(false) {}

Event::Event(const Event& e) : event_type(e.event_type), _stop(e._stop) {}

Event::~Event() {}

Event& Event::operator=(const Event& _rhs) {
    if (this == &_rhs) return *this;

    event_type = _rhs.event_type;
    _stop = _rhs._stop;

    return *this;
}

bool Event::operator==(const Event& e) const {
    return event_type == e.event_type;
}

bool Event::operator==(const EventType et) const { return event_type == et; }

const EventType Event::type() const { return event_type; }

void Event::stop(bool _s) { _stop = _s; }

bool Event::stop() const { return _stop; }

Event* Event::clone() const { return new Event(*this); }

Event::operator const EventType() const { return event_type; }

std::string Event::evt2str(const EventType et) {
    return static_cast<std::string>(et);
}
