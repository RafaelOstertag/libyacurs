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

#include "yacurs.h"

class Handler {
    private:
	EVENT_TYPE expected_evt;
	int calls;
    public:
	inline Handler(EVENT_TYPE expected):
	    expected_evt(expected), calls(0) {}
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
	    std::cout << "AlrmHandler::handler()\r" << std::endl;
	    EventQueue::submit(Event(EVT_QUIT));
	    return Handler::handler(e);
	}
};

class SelfRegister : public Handler {
    public:
	inline SelfRegister(): Handler(EVT_ALARM) {
	    EventQueue::connect_event(EventConnectorMethod1<SelfRegister>(EVT_ALARM, this ,&SelfRegister::handler) );
	}
	inline int handler(Event& e) {
	    std::cout << "SelfRegister::handler()\r" << std::endl;
	    return Handler::handler(e);
	}
};

class SelfRegister2 : public SelfRegister {
    public:
	inline SelfRegister2(): SelfRegister() {
	    EventQueue::connect_event(EventConnectorMethod1<SelfRegister2>(EVT_ALARM, this ,&SelfRegister2::handler) );
	}
	inline int handler(Event& e) {
	    EventQueue::submit(Event(EVT_QUIT));
	    std::cout << "SelfRegister2::handler()\r" << std::endl;
	    return SelfRegister::handler(e);
	}
};


int main() {

    try {
	AlrmHandler ahandler;

	EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler,&AlrmHandler::handler) );

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
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}
