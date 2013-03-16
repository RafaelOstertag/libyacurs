// $Id$
//
// Test (un)suspending of events
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
	void reset() { calls=0; };
};

class AlrmHandler: public Handler {
    public:
	AlrmHandler(): Handler(EVT_SIGALRM) {}
	void handler(Event& e) {
	    Handler::handler(e);
	    std::cout << "AlrmHandler2::handler()\r" << std::endl;
	    // During processing of the queue, USR1 is blocked, so we cheat and unblock it
	    sigset_t _sset;
	    sigemptyset(&_sset);
	    sigaddset(&_sset,SIGUSR1);
	    if (sigprocmask(SIG_UNBLOCK, &_sset, 0))
		abort();
	    raise(SIGUSR1);
	}
};

class Usr1Handler: public Handler {
    private:
	bool quit;
    public:
	Usr1Handler(bool q=false): Handler(EVT_SIGUSR1), quit(q) {}
	void handler(Event& e) {
	    Handler::handler(e);
	    std::cout << "Usr1Handler::handler()\r" << std::endl;
	    if (quit)
		EventQueue::submit(Event(EVT_QUIT));
	    else {
		// During processing of the queue, USR2 is blocked, so we cheat
		// and unblock it
		sigset_t _sset;
		sigemptyset(&_sset);
		sigaddset(&_sset,SIGUSR2);
		if (sigprocmask(SIG_UNBLOCK, &_sset, 0))
		    abort();
		raise(SIGUSR2);
	    }
	}
	void setQuit(bool q) { quit=q; }
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
	Usr2Handler u2handler, u2handler2, u2handler3;

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );

	Curses::init();
	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler2.getCalls() != 1)
	    goto _ERR;
	if (u2handler2.getCalls() != 1)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );
	//
	// u2handler* may not have any calls
	//
	EventQueue::suspend_all(EVT_SIGUSR2);

	// u1handler must terminate queue loop
	u1handler.setQuit(true);

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 0)
	    goto _ERR;
	if (u2handler2.getCalls() != 0)
	    goto _ERR;
	if (u2handler3.getCalls() != 0)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );
	//
	// u2handler* must have calls
	//
	EventQueue::unsuspend_all(EVT_SIGUSR2);

	u1handler.setQuit(false);

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler2.getCalls() != 1)
	    goto _ERR;
	if (u2handler3.getCalls() != 1)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );
	//
	// only u2handler2 must have calls
	//
	EventQueue::suspend_except(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

	u1handler.setQuit(false);

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 0)
	    goto _ERR;
	if (u2handler2.getCalls() != 1)
	    goto _ERR;
	if (u2handler3.getCalls() != 0)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );
	//
	// only u2handler2 must have no calls
	//

	EventQueue::suspend_all(EVT_SIGUSR2);
	EventQueue::unsuspend_except(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler2.getCalls() != 0)
	    goto _ERR;
	if (u2handler3.getCalls() != 1)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler, &AlrmHandler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr1Handler>(EVT_SIGUSR1, &u1handler, &Usr1Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler) );
	EventQueue::connect_event(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler) );
	//
	// only u2handler2 must have no calls
	//

	EventQueue::suspend(EventConnectorMethod1<Usr2Handler>(EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;
	if (u1handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler.getCalls() != 1)
	    goto _ERR;
	if (u2handler2.getCalls() != 0)
	    goto _ERR;
	if (u2handler3.getCalls() != 1)
	    goto _ERR;

	ahandler.reset();
	u1handler.reset();
	u2handler.reset();
	u2handler2.reset();
	u2handler3.reset();

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
