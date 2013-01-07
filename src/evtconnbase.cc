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

EventConnectorBase::EventConnectorBase(const EventConnectorBase& _ec) {
    evt = _ec.evt;
    suspended = _ec.suspended;
}

EventConnectorBase&
EventConnectorBase::operator=(const EventConnectorBase& _ec) {
    evt = _ec.evt;
    suspended = _ec.suspended;
    return *this;
}

bool
EventConnectorBase::operator==(const EventConnectorBase& _ec) const {
    return evt == _ec.evt && this->compare(_ec);
}

bool
EventConnectorBase::operator!=(const EventConnectorBase& _ec) const {
    return !operator==(_ec);
}

bool
EventConnectorBase::operator==(const EventBase& _eb) const {
    return _eb.type() == evt;
}

bool
EventConnectorBase::operator!=(const EventBase& _eb) const {
    return !operator==(_eb);
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
