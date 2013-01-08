// $Id$
//
// Test SIGUSR1/SIGUSR2
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
	    raise(SIGUSR1);
	    return 0;
	}
};

class Usr1Handler: public Handler {
    public:
	inline Usr1Handler(): Handler(EVT_USR1) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    raise(SIGUSR2);
	    return 0;
	}
};	

class Usr2Handler: public Handler {
    public:
	inline Usr2Handler(): Handler(EVT_USR2) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    EventQueue::inject(EventBase(EVT_QUIT));
	    return 0;
	}
};	

int main() {

    try {
	AlrmHandler ahandler;
	Usr1Handler u1handler;
	Usr2Handler u2handler;

	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<Usr1Handler>(EVT_USR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<Usr2Handler>(EVT_USR2, &u2handler, &Usr2Handler::handler) );

	Curses::init();
	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 1)
	    goto _ERR;
	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	goto _ERR;
    }

    return 0;

 _ERR:
    Curses::end();
    return 1;
}