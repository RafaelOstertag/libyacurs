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
EventBase::EventBase(EVENT_TYPE _et): event_type(_et) {}

EventBase::EventBase(const EventBase& _e) {
    event_type = _e.event_type;
}

EventBase&
EventBase::operator=(const EventBase& _e) {
    event_type = _e.event_type;
    return *this;
}

EventBase::~EventBase() {}

bool
EventBase::operator==(const EventBase& _e) const {
    return event_type == _e.event_type;
}

bool
EventBase::operator==(EVENT_TYPE _et) const {
    return event_type == _et;
}

EVENT_TYPE
EventBase::type() const {
    return event_type;
}

EventBase*
EventBase::clone() const {
    return new EventBase(*this);
}

///////

EventWinCh::EventWinCh(const Rectangle<>& _r):
    Event<Rectangle<> >(EVT_WINCH, _r) {}

EventWinCh::EventWinCh(const EventWinCh& _e):
    Event<Rectangle<> >(_e) {}


EventWinCh&
EventWinCh::operator=(const EventWinCh& _e) {
    Event<Rectangle<> >::operator=(_e);
    return *this;
}

EventWinCh*
EventWinCh::clone() const {
    return new EventWinCh(*this);
}

///////

EventKey::EventKey(const int& _r): Event<int>(EVT_KEY, _r) {}

EventKey::EventKey(const EventKey& _e): Event<int>(_e) {}

EventKey&
EventKey::operator=(const EventKey& _e) {
    Event<int>::operator=(_e);
    return *this;
}

EventKey*
EventKey::clone() const {
    return new EventKey(*this);
}
