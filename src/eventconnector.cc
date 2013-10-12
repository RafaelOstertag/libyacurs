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
EventConnectorBase::EventConnectorBase(const EventType _e, bool _s) :
    evt(_e), __suspended(_s) {
}

EventConnectorBase::~EventConnectorBase() {
}

bool
EventConnectorBase::operator==(const EventConnectorBase& _ec) const {
    return (evt == _ec.evt) &&
           (id() == _ec.id() );
}

bool
EventConnectorBase::operator!=(const EventConnectorBase& _ec) const {
    return !operator==(_ec);
}

bool
EventConnectorBase::operator==(const Event& _eb) const {
    return _eb.type() == evt;
}

bool
EventConnectorBase::operator!=(const Event& _eb) const {
    return !operator==(_eb);
}

bool
EventConnectorBase::operator==(const EventType _et) const {
    return _et == evt;
}

bool
EventConnectorBase::operator!=(const EventType _et) const {
    return !operator==(_et);
}

const EventType
EventConnectorBase::type() const {
    return evt;
}

void
EventConnectorBase::suspended(bool _s) {
    __suspended = _s;
}

bool
EventConnectorBase::suspended() const {
    return __suspended;
}

EventConnectorBase::operator const EventType() const {
    return evt;
}

////////////////////////////////////////////////////////
//
// class EventConnectorFunction1
//
////////////////////////////////////////////////////////

//
// Protected
//
uintptr_t
EventConnectorFunction1::id() const {
    return (uintptr_t)__func;
}

//
// Public
//
EventConnectorFunction1::EventConnectorFunction1(const EventType _e,
                                                 fptr_t _func) :
    EventConnectorBase(_e), __func(_func) {
    assert(__func != 0);
}

void
EventConnectorFunction1::call(Event& _e) const {
    assert(__func != 0);

    if (suspended() ) return;

    __func(_e);
}

EventConnectorBase*
EventConnectorFunction1::clone() const {
    return new EventConnectorFunction1(*this);
}
