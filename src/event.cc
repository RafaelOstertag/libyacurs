// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

////////////////////////////////////////////////////////
//
// class Event
//
////////////////////////////////////////////////////////

Event::Event(EVENT_TYPE _et): event_type(_et) {}

Event::Event(const Event& _e) {
    event_type = _e.event_type;
}

Event&
Event::operator=(const Event& _e) {
    event_type = _e.event_type;
    return *this;
}

Event::~Event() {}

bool
Event::operator==(const Event& _e) const {
    return event_type == _e.event_type;
}

bool
Event::operator==(EVENT_TYPE _et) const {
    return event_type == _et;
}

EVENT_TYPE
Event::type() const {
    return event_type;
}

Event*
Event::clone() const {
    return new Event(*this);
}
