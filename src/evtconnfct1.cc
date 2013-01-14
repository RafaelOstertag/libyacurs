// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eventconnector.h"


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
