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
inline void
EventConnectorBase::setSuspended(bool _s) {
    suspended = _s;
}

//
// Public
//
EventConnectorBase::EventConnectorBase(EVENT_TYPE _e, bool _s):
    evt(_e), suspended(_s) {}

EventConnectorBase::EventConnectorBase(const EventConnectorBase& _ec) :
    evt(_ec.evt), suspended(_ec.suspended) {
}

EventConnectorBase::~EventConnectorBase() {}

EventConnectorBase&
EventConnectorBase::operator=(const EventConnectorBase& _ec) {
    evt = _ec.evt;
    suspended = _ec.suspended;
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

bool
EventConnectorBase::isSuspended() const {
    return suspended;
}

void
EventConnectorBase::suspend() {
    setSuspended(true);
}

void
EventConnectorBase::unsuspend() {
    setSuspended(false);
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
    assert(__func != NULL);
}

EventConnectorFunction1::EventConnectorFunction1(const EventConnectorFunction1& _ec):
    EventConnectorBase(_ec), __func(_ec.__func) {
    assert(__func != NULL);
}

EventConnectorFunction1&
EventConnectorFunction1::operator=(const EventConnectorFunction1& _ec) {
    EventConnectorBase::operator=(_ec);

    assert(__func != NULL);
    __func = _ec.__func;

    return *this;
}

int
EventConnectorFunction1::call(Event& _e) const {
    assert(__func != NULL);

    if (isSuspended()) return -1;

    return __func(_e);
}

EventConnectorBase*
EventConnectorFunction1::clone() const {
    return new EventConnectorFunction1(*this);
}
