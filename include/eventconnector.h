// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTCONNECTOR_H
#define EVENTCONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#ifdef HAVE_FUNCTIONAL
#include <functional>
#endif // HAVE_FUNCTIONAL

#ifdef HAVE_TYPEINFO
#include <typeinfo>
#endif // HAVE_TYPEINFO

#include "event.h"

/**
 * An Event Connector connects an event (@c EVENT_TYPE) to an handler
 * (function, or member function).
 *
 * When connecting an event to a member function, it is expected that
 * exactly one member function for a particular event per object is
 * called. Connecting a particular event to several member functions
 * of the same object result in only the last connection made taking
 * effect (i.e. the current connection replaces any previous
 * connections).
 *
 * This uniqueness is not enforced by the event connector itself, but
 * an event connector has to provide
 */
class EventConnectorBase {
    private:
	EVENT_TYPE evt;
	bool suspended;

    protected:
	virtual bool compare(const EventConnectorBase& ) const = 0;
	void setSuspended(bool _s);

    public:
	virtual const void* handler_ptr() const = 0;

	EventConnectorBase(EVENT_TYPE _e, bool _s = false);
	EventConnectorBase(const EventConnectorBase& _ec);
	virtual ~EventConnectorBase() {}

	EventConnectorBase& operator=(const EventConnectorBase& _ec);
	bool operator==(const EventConnectorBase& _ec) const;
	bool operator!=(const EventConnectorBase& _ec) const;
	bool operator==(const EventBase& _eb) const;
	bool operator!=(const EventBase& _eb) const;

	EVENT_TYPE type() const;
	bool isSuspended() const;
	void suspend();
	void unsuspend();

	/** Call the handler
	 *
	 * @param the event.
	 *
	 * @return the value returned by the handler.
	 */
	virtual int call(EventBase&) const = 0;
	virtual EventConnectorBase* clone() const = 0;
};

template<class T /* Type of object called */>
class EventConnectorMethod1: public EventConnectorBase {
    public:
	typedef int (T::*__mem_fun_t)(EventBase&);
	typedef T* _obj_ptr_t;
    private:
	__mem_fun_t __func;
	_obj_ptr_t __obj_ptr;

    protected:
	bool compare(const EventConnectorBase& eb) const {
	    assert( __func != NULL );
	    assert( __obj_ptr != NULL );

	    return eb.handler_ptr() == this->handler_ptr();
	}

	const void* handler_ptr() const {
	    return static_cast<const void*>(__obj_ptr);
	}

    public:
	EventConnectorMethod1(EVENT_TYPE _e,
			      _obj_ptr_t _obj_ptr,
			      __mem_fun_t _func): EventConnectorBase(_e),
						  __func(_func),
						  __obj_ptr(_obj_ptr) {
	    assert(__func != NULL);
	    assert(__obj_ptr != NULL);
	}
	EventConnectorMethod1(const EventConnectorMethod1<T>& _ec): EventConnectorBase(_ec),
								    __func(_ec.__func),
								    __obj_ptr(_ec.__obj_ptr) {
	    assert(__func != NULL);
	    assert(__obj_ptr != NULL);
	}
	EventConnectorMethod1<T>& operator=(const EventConnectorMethod1<T>& _ec) {
	    EventConnectorBase::operator=(_ec);
	    __func = _ec.__func;
	    assert(__func != NULL);
	    __obj_ptr = _ec.__obj_ptr;
	    assert(__obj_ptr != NULL);
	    return *this;
	}

	/**
	 * @return the value returned by the member function called,
	 * or -1 if the connector is suspended
	 */
	int call(EventBase& _a) const {
	    assert(__obj_ptr != NULL);
	    assert(__func != NULL);
	    if (isSuspended()) return -1;
	    return (__obj_ptr->*__func)(_a);
	}

	EventConnectorBase* clone() const {
	    return new EventConnectorMethod1<T>(*this);
	}
};

class EventConnectorFunction1: public EventConnectorBase {
    public:
	typedef int (*fptr_t)(EventBase&);
    private:
	fptr_t __func;

    protected:
	bool compare(const EventConnectorBase& eb) const;
	// Not used
	const void* handler_ptr() const;

    public:
	EventConnectorFunction1(EVENT_TYPE _e, fptr_t _func);
	EventConnectorFunction1(const EventConnectorFunction1& _ec);
	EventConnectorFunction1& operator=(const EventConnectorFunction1& _ec);
	int call(EventBase& _a) const;
	EventConnectorBase* clone() const;
};

#endif // EVENTCONNECTOR_H
