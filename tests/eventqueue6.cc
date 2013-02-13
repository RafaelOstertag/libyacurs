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

#include "yacurs.h"

class Handler {
    private:
	EVENT_TYPE expected_evt;
	int calls;
    public:
	Handler(EVENT_TYPE evt):
	    expected_evt(evt), calls(0) {}
	Handler(const Handler& _h) {
	    expected_evt = _h.expected_evt;
	    calls = _h.calls;
	}
	virtual ~Handler() {}
	virtual void handler(Event& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	}
	int getCalls() { return calls; }
};

class AlrmHandler: public Handler {
    public:
	AlrmHandler(): Handler(EVT_SIGALRM) {}
	void handler(Event& e) {
	    std::cout << "AlrmHandler::handler()\r" << std::endl;
	    EventQueue::submit(Event(EVT_QUIT));
	    Handler::handler(e);
	}
};

class AlrmHandler2: public AlrmHandler {
    private:
	int calls_handler2;
	int calls_handler3;
    public:
	AlrmHandler2(): AlrmHandler(), calls_handler2(0), calls_handler3(0) {}
	void handler(Event& e) {
	    std::cout << "AlrmHandler2::handler()\r" << std::endl;
	    Handler::handler(e);
	}
	void handler2(Event& e) {
	    std::cout << "AlrmHandler2::handler2()\r" << std::endl;
	    calls_handler2++;
	}
	void handler3(Event& e) {
	    std::cout << "AlrmHandler2::handler3()\r" << std::endl;
	    calls_handler3++;
	}
	int getCalls2() const {return calls_handler2;}
	int getCalls3() const {return calls_handler3;}
};

int main() {

    try {
	AlrmHandler ahandler;
	AlrmHandler2 ahandler2;

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	// Should be overriden by next call
	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler2) );
	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler) );
	// AlrmHandler2::handler should not be called now
	EventQueue::disconnect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler) );

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGWINCH, &ahandler2, &AlrmHandler2::handler3) );
	
	Curses::init();
	EventQueue::submit(EventEx<Size>(EVT_SIGWINCH, Area() ));
	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (ahandler2.getCalls() != 0)
	    goto _ERR;
	if (ahandler2.getCalls2() != 0)
	    goto _ERR;
	if (ahandler2.getCalls3() != 1)
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
