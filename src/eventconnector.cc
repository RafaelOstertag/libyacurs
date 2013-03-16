// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eventconnector.h"

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
EventConnectorBase::EventConnectorBase(EVENT_TYPE _e, bool _s):
    evt(_e), __suspended(_s) {}

EventConnectorBase::EventConnectorBase(const EventConnectorBase& _ec) :
    evt(_ec.evt), __suspended(_ec.__suspended) {
}

EventConnectorBase::~EventConnectorBase() {}

EventConnectorBase&
EventConnectorBase::operator=(const EventConnectorBase& _ec) {
    evt = _ec.evt;
    __suspended = _ec.__suspended;
    return *this;
}

bool
EventConnectorBase::operator==(const EventConnectorBase& _ec) const {
    return (evt == _ec.evt) && 
	(id() == _ec.id());
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
EventConnectorBase::operator==(EVENT_TYPE _et) const {
    return _et == evt;
}

bool
EventConnectorBase::operator!=(EVENT_TYPE _et) const {
    return !operator==(_et);
}

EVENT_TYPE
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

EventConnectorBase::operator EVENT_TYPE() const {
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
EventConnectorFunction1::EventConnectorFunction1(EVENT_TYPE _e, fptr_t _func):
    EventConnectorBase(_e), __func(_func) {
    assert(__func != 0);
}

EventConnectorFunction1::EventConnectorFunction1(const EventConnectorFunction1& _ec):
    EventConnectorBase(_ec), __func(_ec.__func) {
    assert(__func != 0);
}

EventConnectorFunction1&
EventConnectorFunction1::operator=(const EventConnectorFunction1& _ec) {
    EventConnectorBase::operator=(_ec);

    assert(__func != 0);
    __func = _ec.__func;

    return *this;
}

void
EventConnectorFunction1::call(Event& _e) const {
    assert(__func != 0);

    if (suspended()) return;

    __func(_e);
}

EventConnectorBase*
EventConnectorFunction1::clone() const {
    return new EventConnectorFunction1(*this);
}
