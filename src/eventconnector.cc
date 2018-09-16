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

#include "eventconnector.h"

using namespace YACURS;

////////////////////////////////////////////////////////
//
// class EventConnectorBase
//
////////////////////////////////////////////////////////

//
// Protected
//

//
// Public
//
EventConnectorBase::EventConnectorBase(const EventType e, bool s)
    : evt(e), _suspended(s) {}

EventConnectorBase::~EventConnectorBase() {}

bool EventConnectorBase::operator==(const EventConnectorBase& ec) const {
    return (evt == ec.evt) && (id() == ec.id());
}

bool EventConnectorBase::operator!=(const EventConnectorBase& ec) const {
    return !operator==(ec);
}

bool EventConnectorBase::operator==(const Event& eb) const {
    return eb.type() == evt;
}

bool EventConnectorBase::operator!=(const Event& eb) const {
    return !operator==(eb);
}

bool EventConnectorBase::operator==(const EventType et) const {
    return et == evt;
}

bool EventConnectorBase::operator!=(const EventType et) const {
    return !operator==(et);
}

const EventType EventConnectorBase::type() const { return evt; }

void EventConnectorBase::suspended(bool s) { _suspended = s; }

bool EventConnectorBase::suspended() const { return _suspended; }

EventConnectorBase::operator const EventType() const { return evt; }

////////////////////////////////////////////////////////
//
// class EventConnectorFunction1
//
////////////////////////////////////////////////////////

//
// Protected
//
uintptr_t EventConnectorFunction1::id() const { return (uintptr_t)_func; }

//
// Public
//
EventConnectorFunction1::EventConnectorFunction1(const EventType e, fptr_t func)
    : EventConnectorBase(e), _func(func) {
    assert(_func != 0);
}

void EventConnectorFunction1::call(Event& e) const {
    assert(_func != 0);

    if (suspended()) return;

    _func(e);
}

EventConnectorBase* EventConnectorFunction1::clone() const {
    return new EventConnectorFunction1(*this);
}
