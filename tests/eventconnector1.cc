// $Id$
//
// Test handler disconnect when in handler.
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "eventconnector.h"

class Handler {
    private:
	EVENT_TYPE expected_evt;
	int calls;
    public:
	inline Handler(EVENT_TYPE evt):
	    expected_evt(evt), calls(0) {}
	inline Handler(const Handler& _h) {
	    expected_evt = _h.expected_evt;
	    calls = _h.calls;
	}
	inline virtual ~Handler() {}
	inline virtual int handler(EventBase& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	    return 0;
	}
	inline int getCalls() { return calls; }
};

class WinChHandler: public Handler {
    public:
	inline WinChHandler(): Handler(EVT_WINCH) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    return 0;
	}
};

class AlrmHandler: public Handler {
    public:
	inline AlrmHandler(): Handler(EVT_ALARM) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    return 0;
	}
};

class AlrmHandler2: public AlrmHandler {
    public:
	inline AlrmHandler2(): AlrmHandler() {}
	inline int handler(EventBase& e) {
	    return Handler::handler(e);
	}
};

class Static1 {
    public:
	static int fcthandler1(EventBase&) {
	    return 0;
	}
};

class Static2 {
    public:
	static int fcthandler1(EventBase&) {
	    return 0;
	}
};

int main() {

    WinChHandler winch1;
    WinChHandler winch2;
    AlrmHandler ahandler1;
    AlrmHandler ahandler2;
    AlrmHandler2 ahandler2_1;
    AlrmHandler2 ahandler2_2;

    EventConnectorMethod1<WinChHandler> wconnector1(EVT_WINCH, &winch1, &WinChHandler::handler);
    EventConnectorMethod1<WinChHandler> wconnector2(EVT_WINCH, &winch2, &WinChHandler::handler);

    EventConnectorMethod1<AlrmHandler> aconnector1(EVT_ALARM, &ahandler1, &AlrmHandler::handler);
    EventConnectorMethod1<AlrmHandler> aconnector2(EVT_ALARM, &ahandler2, &AlrmHandler::handler);

    EventConnectorMethod1<AlrmHandler2> aconnector2_1(EVT_ALARM, &ahandler2_1, &AlrmHandler2::handler);
    EventConnectorMethod1<AlrmHandler2> aconnector2_2(EVT_ALARM, &ahandler2_2, &AlrmHandler2::handler);
    EventConnectorMethod1<AlrmHandler2> aconnector2_2_(EVT_ALARM, &ahandler2_2, &AlrmHandler2::handler);
    EventConnectorMethod1<Handler> ehandler(EVT_ALARM, &ahandler2_2, &Handler::handler);

    if (wconnector1 == wconnector2)
	return 1;

    if (wconnector1 != wconnector1)
	return 1;

    if (wconnector1 == aconnector1)
	return 1;

    if (aconnector1 == aconnector2_1)
	return 1;
    
    if (aconnector2_2 != aconnector2_2_)
	return 1;

    if (aconnector2_2 != ehandler)
	return 1;

    EventConnectorBase *ptr1, *ptr2;

    ptr1 = &wconnector1;
    ptr2 = &wconnector2;

    if ( *ptr1 == *ptr2 )
	return 1;

    ptr1 = &aconnector2_2;
    ptr2 = &aconnector2_2_;

    if ( *ptr1 != *ptr2 )
	return 1;

    EventConnectorFunction1 efh1(EVT_USR1, &Static1::fcthandler1);
    EventConnectorFunction1 efh1_1(EVT_USR1, &Static1::fcthandler1);
    EventConnectorFunction1 efh2(EVT_USR1, &Static2::fcthandler1);

    if (efh1 != efh1)
	return 1;

    if (efh1 != efh1_1)
	return 1;

    if (efh1 == efh2)
	return 1;

    return 0;
}
