// $Id$
//
// Test basic functionality of EventQueue by injecting events using EventQueue::submit()
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

class WinChHandler: public Handler {
    public:
	inline WinChHandler(): Handler(EVT_WINCH) {}
	inline int handler(Event& e) {
	    Handler::handler(e);
	    Size tmp(dynamic_cast<EventWinCh&>(e).data());
	    std::cout << "WinChHandler\r" << std::endl;
	    return 0;
	}
};

class AlrmHandler: public Handler {
    public:
	inline AlrmHandler(): Handler(EVT_ALARM) {}
	inline int handler(Event& e) { 
	    std::cout << "WinChHandler\r" << std::endl;
	    return Handler::handler(e); 
	}
};

int main() {
    try {
	WinChHandler whandler;
	AlrmHandler ahandler;

	EventQueue::connectEvent(EventConnectorMethod1<WinChHandler>(EVT_WINCH, &whandler,&WinChHandler::handler) );
	EventQueue::connectEvent(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler,&AlrmHandler::handler) );

	EventQueue::submit(EventWinCh(Area ()));
	EventQueue::submit(Event(EVT_ALARM));
	EventQueue::submit(Event(EVT_QUIT));

	Curses::init();
	// EventQueue:run() blocks in getch().
	ungetch('a');

	EventQueue::run();

	if (whandler.getCalls() != 1)
	    goto _ERR;
	if (ahandler.getCalls() != 1)
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
