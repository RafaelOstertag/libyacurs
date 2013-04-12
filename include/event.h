// -*- mode: c++ -*-
//
// This file is part of libyacurs, 
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#include "area.h"

namespace YACURS {

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
	/**
	 * Terminates the EventQueue main loop
	 */
	EVT_QUIT,
	/**
	 * Notification of window size change.
	 *
	 * Only Curses is supposed to handle this Event. Widgets and
	 * Windows should not use it in order to resize.
	 */
	EVT_SIGWINCH,
	/**
	 * Notification of alarm signal
	 */
	EVT_SIGALRM,
	/**
	 * Key pressed
	 */
	EVT_KEY,
	/**
	 * A refresh is usually issued after an resize. The refresh must
	 * not be immediate, i.e. no wrefresh(), but wnoutrefresh().
	 */
	EVT_REFRESH,
	/**
	 * Force curses window to refresh. This is used to completely
	 * redraw the screen upon Ctrl-L. The handler can be implemented
	 * as simple call to clearok().
	 */
	EVT_FORCEREFRESH,
	/**
	 *
	 * A doupdate is usually issued after a refresh. Only one
	 * VT_DOUPDATE handler should exist, since curses takes care of
	 * refreshing all windows.
	 */
	EVT_DOUPDATE,
	/**
	 * Re-setup terminal. Mainly used when resizing screen or
	 * complete screen refresh. @sa EventQueue
	 */
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
	    /**
	     * The type of the Event
	     */
	    EVENT_TYPE event_type;

	    /**
	     * Indicate whether or not the Event should not be processed
	     * further.
	     */
	    bool __stop;
	

	    static const char* __str_table[];

	public:
	    /**
	     * @param _et the event type
	     */
	    Event(EVENT_TYPE _et);

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
	     * Set stop flag.
	     *
	     * When stop flag is set (@c true) the event will no be
	     * processed further.
	     *
	     * @param _s when set to @c true, the event will not be
	     * processed further.
	     */
	    void stop(bool _s);

	    /**
	     * Get stop flag.
	     *
	     * @return @c true, the event will not be processed further,
	     * @c false otherwise.
	     */
	    bool stop() const;

	    /**
	     * Create a copy of the object. The caller is responsible for
	     * freeing the memory of the object returned.
	     *
	     * @return pointer to copy of this object. The memory has to
	     * be freed by the caller.
	     */
	    virtual Event* clone() const;

	    operator EVENT_TYPE() const;

	    static std::string evt2str(EVENT_TYPE _et);
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
	    EventEx(EVENT_TYPE _et, const T& _payload): Event(_et),
							__payload(_payload) {
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
}

#endif // EVENT_H
