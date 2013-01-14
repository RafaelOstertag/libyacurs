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

#include "yacurs.h"

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
	inline virtual int handler(Event& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	    return 0;
	}
	inline int getCalls() { return calls; }
};

class AlrmHandler: public Handler {
    public:
	inline AlrmHandler(): Handler(EVT_ALARM) {}
	inline int handler(Event& e) {
	    Handler::handler(e);
	    std::cout << "AlrmHandler::handler()\r" << std::endl;
	    if (getCalls() < 4) {
		alarm(1);
		return 0;
	    }
	    std::cout << "QUIT" << std::endl;
	    EventQueue::submit(Event(EVT_QUIT));
	    return 0;
	}
};

class AlrmHandler2: public AlrmHandler {
    public:
	inline AlrmHandler2(): AlrmHandler() {}
	inline int handler(Event& e) {
	    std::cout << "AlrmHandler2::handler()\r" << std::endl;
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

	Curses::init();
	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_1.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_2.getCalls() != 0)
	    goto _ERR;
	if (ahandler2_3.getCalls() != 4)
	    goto _ERR;
	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}
