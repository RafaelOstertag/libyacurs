// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "area.h"

/**
 * @defgroup Event Event related code
 */

/**
 * @ingroup Event
 *
 * Event Types that can be connected to.
 *
 * One event handler can be connected to each event type
 * per object using an Event Connector.
 *
 * @see EventConnectorBase
 * @see EventQueue
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
    /// A refresh is usually issued after an resize
    EVT_REFRESH,
    /// A doupdate is usually issued after a refresh
    EVT_DOUPDATE,
    /// Re-setup terminal. Mainly used when resizing screen
    EVT_TERMRESETUP,
    /// SIGUSR1
    EVT_USR1,
    /// SIGUSR2
    EVT_USR2
};

/**
 * @ingroup Event
 *
 * An Event is generated (key stroke, signal) and submitted into the
 * EventQueue. The event queue passes the Event to EventConnectors
 * (i.e. calls the function or method connected to the event).
 */
class Event {
    private:
	/// The type of the Event
	EVENT_TYPE event_type;

    public:
	/**
	 * @param _et the event type
	 */
	Event(EVENT_TYPE _et);
	Event(const Event& _e);
	/**
	 * Assigns one event to anothoer
	 *
	 * @param _e reference to to rhs
	 *
	 * @return reference to *this.
	 */
	Event& operator=(const Event& _e);
	virtual ~Event();
	/**
	 * Tests Event objects for equality.
	 *
	 * @param _e reference to rhs
	 *
	 * @return \c true if the event type of the events are
	 * equal. \c false otherwise.
	 */
	virtual bool operator==(const Event& _e) const;
	/**
	 * Tests whether the given event type is equal to type of this
	 * object.
	 *
	 * @param _et rhs EVENT_TYPE
	 *
	 * @return \c true if the event types are equal, \c false
	 * otherwise.
	 */
	bool operator==(EVENT_TYPE _et) const;
	/**
	 * Get the event type of the object.
	 *
	 * @return event type of the object.
	 */
	EVENT_TYPE type() const;
	/**
	 * Create a copy of the object. The caller is responsible for
	 * freeing the memory of the object returned.
	 *
	 * @return pointer to copy of this object. The memory has to
	 * be freed by the caller.
	 */
	virtual Event* clone() const;
};

/**
 * @ingroup Event
 *
 * Extends Event and adds a payload to the event in order to pass
 * information to the EventConnectors.
 *
 * The data may be modified by EventConnectors.
 */
template<class T>
class EventEx: public Event {
    private:
	/// The data transported
	T payload;

    public:
	/**
	 * @param _et the event type
	 *
	 * @param _v reference to the data. The data will be copied to
	 * an internal variable.
	 */
	EventEx(EVENT_TYPE _et, const T& _v): Event(_et) {
	    payload = _v;
	}
	EventEx(const EventEx<T>& _e): Event(_e) {
	    payload = _e.payload;
	}
	EventEx<T>& operator=(const EventEx<T>& _e) {
	    Event::operator=(_e);
	    payload = _e.payload;
	    return *this;
	}
	EventEx<T>* clone() const {
	    return new EventEx<T>(*this);
	}
	/**
	 * @return a reference to the data of the event.
	 */
	virtual T& data() { return payload; }
};

/**
 * Event generated upon SIGWINCH.
 *
 * This event will be generated
 *
 * @ingroup Event
 */
class EventWinCh: public EventEx<Size> {
    public:
	EventWinCh(const Size& _s);
	EventWinCh(const EventWinCh& _e);
	EventWinCh& operator=(const EventWinCh& _e);
	EventWinCh* clone() const;
};

/**
 * @ingroup Event
 */
class EventKey: public EventEx<int> {
    public:
	EventKey(const int& _r);
	EventKey(const EventKey& _e);
	EventKey& operator=(const EventKey& _e);
	EventKey* clone() const;
};

#endif // EVENT_H
