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
EventType::EventType(std::string name): __hashval(0x811c9dc5) {
    if (name.length()>MAX_NAME_LENGTH-1)
	throw std::length_error(_("Event names must not be greater than 63 characters."));

    if (name.empty()) 
	throw std::length_error(_("Event names must not be empty."));

    // See http://isthe.com/chongo/tech/comp/fnv/
    std::string::size_type len = name.length();
    for (std::string::size_type i=0; i<len; i++) {
	__hashval *= 0x01000193;
	__hashval ^= (uint32_t)name[i];
    }

    strncpy(__name, name.c_str(), MAX_NAME_LENGTH-1);
    __name[MAX_NAME_LENGTH-1] = 0;
}

EventType::~EventType() {}

bool
EventType::operator==(const EventType& rhs) const {
    return __hashval == rhs.__hashval;
}

bool
EventType::operator!=(const EventType& rhs) const {
    return !operator==(rhs);
}

bool
EventType::operator<(const EventType& rhs) const {
    return __hashval < rhs.__hashval;
}

bool
EventType::operator>(const EventType& rhs) const {
    return __hashval > rhs.__hashval;
}

EventType::operator std::string() const {
    return __name;
}

EventType::operator uint32_t() const {
    return __hashval;
}

////////////////////////////////////////////////////////
//
// class Event
//
////////////////////////////////////////////////////////

Event::Event(const EventType _et) : event_type(_et), __stop(false) {
}

Event::~Event() {
}

bool
Event::operator==(const Event& _e) const {
    return event_type == _e.event_type;
}

bool
Event::operator==(const EventType _et) const {
    return event_type == _et;
}

const EventType
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

Event::operator const EventType() const {
    return event_type;
}

std::string
Event::evt2str(const EventType _et) {
    return static_cast<std::string>(_et);
}
