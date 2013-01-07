// $Id$
//
// Test handler unregister
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

#include "curs.h"
#include "eventconnector.h"
#include "eventqueue.h"

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

class AlrmHandler: public Handler {
    public:
	inline AlrmHandler(): Handler(EVT_ALARM) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    if (getCalls() < 4) {
		std::cout << "alarm(1)" << std::endl;
		alarm(1);
		return 0;
	    }
	    std::cout << "QUIT" << std::endl;
	    EventQueue::inject(EventBase(EVT_QUIT));
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

int main() {

    try {
	AlrmHandler ahandler;
	AlrmHandler2 ahandler2_1;
	AlrmHandler2 ahandler2_2;
	AlrmHandler2 ahandler2_3;

	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_1, &AlrmHandler2::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_2, &AlrmHandler2::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_3, &AlrmHandler2::handler) );

	EventQueue::disconnectEvent(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_2, &AlrmHandler2::handler) );

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 4)
	    return 1;
	if (ahandler2_1.getCalls() != 4)
	    return 1;
	if (ahandler2_2.getCalls() != 0)
	    return 1;
	if (ahandler2_3.getCalls() != 4)
	    return 1;
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
