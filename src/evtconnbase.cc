// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eventconnector.h"

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
