/* -*- mode: c++ -*-
 *
 * This file is part of libyacurs, 
 * Copyright (C) 2013  Rafael Ostertag
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

// $Id$

#ifndef EVENTCONNECTOR_H
#define EVENTCONNECTOR_H

#include <stdint.h>

#include <cstdlib>
#include <string>
#include <cassert>
#include <functional>
#include <typeinfo>

#include "event.h"

namespace YACURS {
    /**
     * @ingroup Event
     *
     * An Event Connector connects an event (@c EVENT_TYPE) to an
     * handler (function, or member function).
     *
     * When connecting an event to a member function, it is expected
     * that exactly one member function per object for a particular
     * event is called. Connecting a particular event to several
     * member functions of the same object result in only the last
     * connection made taking effect (i.e. the current connection
     * replaces any previous connections).
     *
     * This uniqueness is not enforced by the event connector itself,
     * but an event connector has to provide an equality operator
     * allowing to compare event connector for equality.
     *
     * See operator==() for more information about equality.
     *
     * @see Event
     * @see EventQueue
     */
    class EventConnectorBase {
	private:
	    /// The event type the handler is connected to.
	    EVENT_TYPE evt;

	    /**
	     * indication whether (@c true) or not (@c false) the
	     * event handler is suspended. Suspended event handler
	     * won't be called.
	     */
	    bool __suspended;

	public:
	    /**
	     * Constructor.
	     *
	     * @param _e the event type
	     * @param _s whether or not the handler is suspended.
	     */
	    EventConnectorBase(EVENT_TYPE _e, bool _s = false);

	    virtual ~EventConnectorBase();

	    /**
	     * The value returned by id() together with the event type has
	     * to uniquely identifiy the event connector
	     *
	     * @return value of type @c uintptr_t uniquely identifying the
	     * handler.
	     */
	    virtual uintptr_t id() const = 0;

	    /**
	     * Tests two event connector for equality. It tests for
	     * equality by comparing the event type of both connectors and
	     * equality of the value returned by id().
	     *
	     * When comparing member function event connectors, the
	     * following must hold:
	     *
	     * <pre>
	     *  class base {
	     *   public:
	     *   virtual int handler(Event&);
	     *  };
	     *  class derived : public base {
	     *   public:
	     *   int handler(Event&);
	     *  };
	     *
	     *  derived d();
	     *  base* bptr=\&d;
	     *
	     *  EventConnectorBase(&d,&derived::handler) == EventConnectorBase(bptr, &base::handler);
	     * </pre>
	     *
	     * @return @c true if evt and id() are equal, @c false
	     * otherwise.
	     */
	    bool operator==(const EventConnectorBase& _ec) const;

	    bool operator!=(const EventConnectorBase& _ec) const;

	    /**
	     * Compare the event type.
	     *
	     * Compares only the event type of the given Event.
	     *
	     * @param _eb reference to an Event object.
	     *
	     * @return @c true if the value of evt and _eb.type() are
	     * equal, @c false otherwise.
	     */
	    bool operator==(const Event& _eb) const;

	    bool operator!=(const Event& _eb) const;

	    /**
	     * Compare the event type.
	     *
	     * Compares only the event type.
	     *
	     * @param _et event type
	     *
	     * @return @c true if _evt and @c _et are equal, @c false
	     * otherwise.
	     */
	    bool operator==(EVENT_TYPE _et) const;

	    bool operator!=(EVENT_TYPE _et) const;

	    /**
	     * Returns the event the handler is connected to.
	     *
	     * @return the EVENT_TYPE
	     */
	    EVENT_TYPE type() const;

	    /**
	     * Set the suspended state to the given value.
	     *
	     * @param _s boolean value indicating whether (@c true) or not
	     * (@c false) the event handler is called upon the occurrence
	     * of the event.
	     */
	    void suspended(bool _s);

	    /**
	     * Query the suspended state.
	     *
	     * @return @c true if event connector is suspended, @c false
	     * otherwise.
	     */
	    bool suspended() const;

	    /**
	     * Call the handler and pass the event
	     *
	     * @param _e event object that caused the call.
	     *
	     * @return the value returned by the handler.
	     */
	    virtual void call(Event& _e) const = 0;

	    /**
	     * The EventQueue creates a local copy of the EventConnector
	     * object. This method has to return a copy of the object. The
	     * caller is responsible for freeing the memory.
	     *
	     * @return pointer to a derived EventConnectorBase object. The
	     * caller is responsible for freeing the memory.
	     */
	    virtual EventConnectorBase* clone() const = 0;

	    operator EVENT_TYPE() const;
    };

    /**
     * @ingroup Event
     *
     * Connect a member function to an event.
     *
     * The signature of the handler has to be
     *
     *  <code>Class::handler(Event&)</code>
     *
     */
    template<class T /* Type of object called */>
    class EventConnectorMethod1: public EventConnectorBase {
	public:
	    typedef void (T::*__mem_fun_t)(Event&);
	    typedef T* _obj_ptr_t;
	private:
	    /// Holds the pointer to the member function of a class of
	    /// type T
	    __mem_fun_t __func;
	    /// Holds the pointer to the object of type T
	    _obj_ptr_t __obj_ptr;

	public:
	    /**
	     * Constructs an EventConnectorMethod1 object. The handler is
	     * connected to a particular event has to have the following
	     * signature:
	     *
	     * <code>Class::handler(Event&)</code>
	     *
	     * @param _e the event type to connect
	     *
	     * @param _obj_ptr pointer to the object whos member function
	     * will be called.
	     *
	     * @param _func the address of the member function to be
	     * called.
	     *
	     * For instance:
	     *
	     * <pre>
	     *  class A {
	     *   public:
	     *    int event_handler(Event& _e);
	     *  };
	     *
	     *  A a;
	     *  EventConnector1<A> c(EVT_RESIZE, &a, &A::event_handler);
	     * </pre>
	     */
	    EventConnectorMethod1(EVENT_TYPE _e,
				  _obj_ptr_t _obj_ptr,
				  __mem_fun_t _func): EventConnectorBase(_e),
						      __func(_func),
						      __obj_ptr(_obj_ptr) {
		assert(__func != 0);
		assert(__obj_ptr != 0);
	    }

	    /**
	     * The id returned is the pointer to the object converted to
	     * an uintptr_t.
	     *
	     * @return the id used for testing for equality with other
	     * event connectors.
	     */
	    uintptr_t id() const { return (uintptr_t)__obj_ptr; }

	    /**
	     * The EventQueue calls this function and passes a reference
	     * to the event to it.
	     *
	     * @param _e the event that caused the call.
	     *
	     * @return the value returned by the member function called,
	     * or -1 if the connector is suspended
	     */
	    void call(Event& _e) const {
		assert(__obj_ptr != 0);
		assert(__func != 0);

		if (suspended()) return;
		(__obj_ptr->*__func)(_e);
	    }

	    /**
	     * Creates a copy of this object. The caller is responsible
	     * for freeing the memory.
	     *
	     * @return pointer to the newly created object.
	     */
	    EventConnectorBase* clone() const {
		return new EventConnectorMethod1<T>(*this);
	    }
    };

    /**
     * @ingroup Event
     *
     * Connect a function to an event.
     *
     * The signature of the function connected has to be
     *
     *  <code>int fct(Event&)</code>
     *
     * or
     *
     *  <code>static int Class::handler(Event&)</code>
     */
    class EventConnectorFunction1: public EventConnectorBase {
	public:
	    /// Type of the function pointer that will be called upon an
	    /// event.
	    typedef void (*fptr_t)(Event&);

	private:
	    /// Holds the pointer to the function to be called.
	    fptr_t __func;

	public:
	    /**
	     * Create an event connector to a function.
	     *
	     * @param _e the type of the event
	     *
	     * @param _func the pointer to the function.
	     */
	    EventConnectorFunction1(EVENT_TYPE _e, fptr_t _func);

	    /**
	     * The id returned is the pointer to the function converted to
	     * an uintptr_t.
	     *
	     * @return the id used for testing for equality with other
	     * event connectors.
	     */
	    uintptr_t id() const;

	    /**
	     * The EventQueue calls this function and passes a reference
	     * to the event to it.
	     *
	     * @param _e the event that caused the call.
	     *
	     * @return the value returned by the function called,
	     * or -1 if the connector is suspended
	     */
	    void call(Event& _e) const;

	    /**
	     * Creates a copy of this object. The caller is responsible
	     * for freeing the memory.
	     *
	     * @return pointer to the newly created object.
	     */
	    EventConnectorBase* clone() const;
    };
}

#endif // EVENTCONNECTOR_H
