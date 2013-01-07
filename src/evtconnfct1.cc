// $Id: curs.cc 4611 2013-01-06 13:05:12Z rafisol $

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "eventconnector.h"

//
// Protected
//
bool
EventConnectorFunction1::compare(const EventConnectorBase& eb) const {
    assert(__func != NULL);
    if (typeid(eb) == typeid(EventConnectorFunction1)) {
	EventConnectorFunction1 tmp(dynamic_cast<const EventConnectorFunction1&>(eb));
	assert(tmp.__func != NULL);
	// Please remember, the event type will be checked by the base
	// class. This method is called from the base class' operator==().
	return tmp.__func == __func;
    }
    return false;
}

//
// Public
//
EventConnectorFunction1::EventConnectorFunction1(EVENT_TYPE _e, fptr_t _func):
    EventConnectorBase(_e), __func(_func) {
    assert(__func != NULL);
}

EventConnectorFunction1::EventConnectorFunction1(const EventConnectorFunction1& _ec):
    EventConnectorBase(_ec) {
    __func = _ec.__func;
    assert(__func != NULL);
}

EventConnectorFunction1&
EventConnectorFunction1::operator=(const EventConnectorFunction1& _ec) {
    EventConnectorBase::operator=(_ec);
    __func = _ec.__func;
    assert(__func != NULL);
    return *this;
}

int
EventConnectorFunction1::call(EventBase& _a) const {
    assert(__func != NULL);
    if (isSuspended()) return -1;
    return __func(_a);
}

EventConnectorBase*
EventConnectorFunction1::clone() const {
    return new EventConnectorFunction1(*this);
}
