// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rectangle.h"

/**
 * Event Types. One event handler can be connected to each event type per object using an Event Connector.
 *
 * @see EventConnectorBase
 * @see EventQueue
 * @group Event
 */
enum EVENT_TYPE {
    /// Terminates the EventQueue main loop
    EVT_QUIT,
    /// Notification of window size change
    EVT_WINCH,
    /// Notification of alarm signal
    EVT_ALARM,
    /// Key pressed
    EVT_KEY,
    /// Refresh request
    EVT_REFRESH,
    EVT_USR1,
    EVT_USR2
};

class EventBase {
    private:
	EVENT_TYPE event_type;
    
    public:
	EventBase(EVENT_TYPE _et);
	EventBase(const EventBase& _e);
	EventBase& operator=(const EventBase& _e);
	virtual ~EventBase();
	virtual bool operator==(const EventBase& _e) const;
	EVENT_TYPE type() const;
	virtual EventBase* clone() const;
};

template<class T>
class Event: public EventBase {
    private:
	T payload;

    public:
	Event(EVENT_TYPE _et, const T& _v): EventBase(_et) {
	    payload = _v;
	}
	Event(const Event<T>& _e): EventBase(_e) {
	    payload = _e.payload;
	}
	Event<T>& operator=(const Event<T>& _e) {
	    EventBase::operator=(_e);
	    payload = _e.payload;
	    return *this;
	}
	Event<T>* clone() const {
	    return new Event<T>(*this);
	}
	virtual T& data() { return payload; }
};

class EventWinCh: public Event<Rectangle<> > {
    public:
	EventWinCh(const Rectangle<>& _r);
	EventWinCh(const EventWinCh& _e);
	EventWinCh& operator=(const EventWinCh& _e);
	EventWinCh* clone() const;
};

class EventKey: public Event<int> {
    public:
	EventKey(const int& _r);
	EventKey(const EventKey& _e);
	EventKey& operator=(const EventKey& _e);
	EventKey* clone() const;
};

#endif // EVENT_H
