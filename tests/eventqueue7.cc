// $Id$
//
// Test SIGUSR1/SIGUSR2 and inheritance behavior
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
	    Handler::handler(e);
	    std::cout << "AlrmHandler::handler()\r" << std::endl;
	    // During processing of the queue, USR1 is blocked, so we
	    // cheat and unblock it
	    sigset_t _sset;
	    sigemptyset(&_sset);
	    sigaddset(&_sset,SIGUSR1);
	    if (sigprocmask(SIG_UNBLOCK, &_sset, NULL))
		abort();
	    raise(SIGUSR1);
	}
};

class Usr1Handler: public Handler {
    public:
	Usr1Handler(): Handler(EVT_SIGUSR1) {}
	void handler(Event& e) {
	    Handler::handler(e);
	    std::cout << "Usr1Handler::handler()\r" << std::endl;
	    // During processing of the queue, USR2 is blocked, so we cheat and unblock it
	    sigset_t _sset;
	    sigemptyset(&_sset);
	    sigaddset(&_sset,SIGUSR2);
	    if (sigprocmask(SIG_UNBLOCK, &_sset, NULL))
		abort();
	    raise(SIGUSR2);
	}
};	

class Usr2Handler: public Handler {
    public:
	Usr2Handler(): Handler(EVT_SIGUSR2) {}
	void handler(Event& e) {
	    Handler::handler(e);
	    std::cout << "Usr2Handler::handler()\r" << std::endl;
	    EventQueue::submit(Event(EVT_QUIT));
	}
};	

int main() {

    try {
	AlrmHandler ahandler;
	Usr1Handler u1handler;
	Usr2Handler u2handler;

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	// This event connector should be replaced by the next call,
	// hence the handler must only be called once
	EventQueue::connect_event(EventConnectorMethod1<Handler>(EVT_SIGUSR1, &u1handler, &Handler::handler) );
	// overrides the previous statement
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );

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
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}
