// $Id$
//
// Test EventQueue by using alarm()
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
	inline Handler(EVENT_TYPE expected):
	    expected_evt(expected), calls(0) {}
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
	    EventQueue::inject(EventBase(EVT_QUIT));
	    return Handler::handler(e);
	}
};

class SelfRegister : public Handler {
    public:
	inline SelfRegister(): Handler(EVT_ALARM) {
	    EventQueue::connectEvent(EventConnectorMethod1<SelfRegister>(EVT_ALARM, this ,&SelfRegister::handler) );
	}
	inline int handler(EventBase& e) {
	    return Handler::handler(e);
	}
};

class SelfRegister2 : public SelfRegister {
    public:
	inline SelfRegister2(): SelfRegister() {
	    EventQueue::connectEvent(EventConnectorMethod1<SelfRegister2>(EVT_ALARM, this ,&SelfRegister2::handler) );
	}
	inline int handler(EventBase& e) {
	    EventQueue::inject(EventBase(EVT_QUIT));
	    return SelfRegister::handler(e);
	}
};


int main() {

    try {
	AlrmHandler ahandler;

	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler,&AlrmHandler::handler) );

	Curses::init();
	alarm(2);
	EventQueue::run();

	if (ahandler.getCalls() != 1)
	    goto _ERR;

	SelfRegister2 sr2;
	alarm(2);
	EventQueue::run();

	if (sr2.getCalls() != 1)
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
