// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "area.h"

// forward declaration because windowbase.h includes event.h
class WindowBase;

/**
 * @defgroup Event Event related code
 *
 * Events, Event Queue, and Event Connectors.
 *
 * Events (@c EVENT_TYPE) can be connected to by means of Event
 * Connectors (@c EventConnectorBase). A list of Event Connectors is
 * maintained by the Event Queue (@c EventQueue). Events can be
 * submitted to the Event Queue, which in turn calls all Event
 * Connectors for a given Event.
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
    /**
     * Notification of window size change.
     *
     * Only Curses is supposed to handle this Event. Widgets and
     * Windows should not use it in order to resize.
     */
    EVT_SIGWINCH,
    /// Notification of alarm signal
    EVT_SIGALRM,
    /// Key pressed
    EVT_KEY,
    /// A refresh is usually issued after an resize. The refresh must
    /// not be immediate, i.e. no wrefresh(), but wnoutrefresh().
    EVT_REFRESH,
    /// Force curses window to refresh. This is used to completely
    /// redraw the screen upon Ctrl-L. The handler can be implemented
    /// as simple call to clearok().
    EVT_FORCEREFRESH,
    /// A doupdate is usually issued after a refresh. Only one
    /// EVT_DOUPDATE handler should exist, since curses takes care of
    /// refreshing all windows.
    EVT_DOUPDATE,
    /// Re-setup terminal. Mainly used when resizing screen or
    /// complete screen refresh. @sa EventQueue
    EVT_TERMRESETUP,
    /// SIGUSR1
    EVT_SIGUSR1,
    /// SIGUSR2
    EVT_SIGUSR2,
    EVT_SIGINT,
    /**
     * Advises Focus Manager to give focus to the next Widget in the
     * Current Focus Group.
     */
    EVT_FOCUS_NEXT,
    /**
     * Advises Focus Manager to give focus to the next Widget in the
     * Current Focus Group.
     */
    EVT_FOCUS_PREVIOUS,
    /**
     * Windows emit events of this type when shown by calling show().
     */
    EVT_WINDOW_SHOW,
    /**
     * Windows emit events of this type when closed by calling
     * close().
     */
    EVT_WINDOW_CLOSE,
    /**
     * Will be emitted when Button is pressed.
     */
    EVT_BUTTON_PRESS,
    /**
     * Will be emitted when ENTER is pressed in ListBox.
     */
    EVT_LISTBOX_ENTER
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
	/// The data transported by the event.
	T __payload;

    public:
	/**
	 * @param _et the event type
	 *
	 * @param _payload reference to the data. The data will be copied to
	 * an internal variable.
	 */
	EventEx(EVENT_TYPE _et, const T& _payload): Event(_et) {
	    __payload = _payload;
	}

	EventEx(const EventEx<T>& _e): Event(_e),
				       __payload(_e.__payload) {
	}

	EventEx<T>& operator=(const EventEx<T>& _e) {
	    Event::operator=(_e);
	    __payload = _e.__payload;
	    return *this;
	}

	/**
	 * Create an exact copy of this object.
	 *
	 * Create an exact copy of this object. The memory occupied
	 * has to be freed by the caller.
	 */
	EventEx<T>* clone() const {
	    return new EventEx<T>(*this);
	}

	/**
	 * @return a reference to the data of the event.
	 */
	virtual T& data() { return __payload; }
};

/**
 * @ingroup Event
 *
 * Event generated upon SIGWINCH.
 *
 * This event will be generated upon SIGWINCH. As payload, it holds
 * the new size of the screen.
 */
class EventWinCh: public EventEx<Size> {
    public:
	/**
	 * @param _s the new size of the screen.
	 */
	EventWinCh(const Size& _s);
	EventWinCh(const EventWinCh& _e);
	EventWinCh& operator=(const EventWinCh& _e);

	/**
	 * Create an exact copy of this object.
	 *
	 * Create an exact copy of this object. The memory occupied
	 * has to be freed by the caller.
	 */
	EventWinCh* clone() const;
};

/**
 * @ingroup Event
 *
 * This Event will be submitted upon key press.
 */
class EventKey: public EventEx<int> {
    public:
	/**
	 * @param _r the value of the key pressed.
	 */
	EventKey(const int& _r);
	EventKey(const EventKey& _e);
	EventKey& operator=(const EventKey& _e);

	/**
	 * Create an exact copy of this object.
	 *
	 * Create an exact copy of this object. The memory occupied
	 * has to be freed by the caller.
	 */
	EventKey* clone() const;
};

/**
 * @ingroup Event
 *
 * Event submitted upon Window show.
 *
 * This event has to be submitted on WindowBase::show(). It carries
 * the address of the Window being shown.
 */
class EventWindowShow: public EventEx<WindowBase*> {
    public:
	/**
	 * @param _w address of the window being closed.
	 */
	EventWindowShow(WindowBase* _w);
	EventWindowShow(const EventWindowShow& _e);
	EventWindowShow& operator=(const EventWindowShow& _e);

	/**
	 * Create an exact copy of this object.
	 *
	 * Create an exact copy of this object. The memory occupied
	 * has to be freed by the caller.
	 */
	EventWindowShow* clone() const;
};

/**
 * @ingroup Event
 *
 * Event submitted upon Window close.
 *
 * This event has to be submitted on WindowBase::close(). It carries
 * the address of the Window closed.
 */
class EventWindowClose: public EventEx<WindowBase*> {
    public:
	/**
	 * @param _w address of the window being closed.
	 */
	EventWindowClose(WindowBase* _w);
	EventWindowClose(const EventWindowClose& _e);
	EventWindowClose& operator=(const EventWindowClose& _e);

	/**
	 * Create an exact copy of this object.
	 *
	 * Create an exact copy of this object. The memory occupied
	 * has to be freed by the caller.
	 */
	EventWindowClose* clone() const;
};

#endif // EVENT_H
