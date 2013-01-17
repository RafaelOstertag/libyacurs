// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

//
// Private
//

//
// Protected
//

//
// Public
//
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

///////

EventWinCh::EventWinCh(const Area& _r):
    EventEx<Area >(EVT_WINCH, _r) {}

EventWinCh::EventWinCh(const EventWinCh& _e):
    EventEx<Area >(_e) {}


EventWinCh&
EventWinCh::operator=(const EventWinCh& _e) {
    EventEx<Area >::operator=(_e);
    return *this;
}

EventWinCh*
EventWinCh::clone() const {
    return new EventWinCh(*this);
}

///////

EventKey::EventKey(const int& _r): EventEx<int>(EVT_KEY, _r) {}

EventKey::EventKey(const EventKey& _e): EventEx<int>(_e) {}

EventKey&
EventKey::operator=(const EventKey& _e) {
    EventEx<int>::operator=(_e);
    return *this;
}

EventKey*
EventKey::clone() const {
    return new EventKey(*this);
}
