// $Id: eventqueue8.cc 4620 2013-01-07 22:14:30Z rafisol $
//
// 
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
	inline virtual int handler(EventBase& e) {
	    if (e.type() != expected_evt) {
		std::cerr << "Unexpected event type" << std::endl;
		std::abort();
	    }
	    calls++;
	    return 0;
	}
	inline int getCalls() { return calls; }
	inline void reset() { calls=0; };
};

class MyHandler: public Handler {
    private:
	std::string _str;

    public:
	inline MyHandler(): Handler(EVT_KEY), _str() {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    if (typeid(e) != typeid(EventKey)) std::abort();

	    EventKey& tmp = dynamic_cast<EventKey&>(e);
	    char c = static_cast<char>(tmp.data());
	    _str.push_back(c);

	    if (_str == "Hello, World!")
	    	EventQueue::inject(EventBase(EVT_QUIT));
	    return 0;
	}
};

int main() {
    try {
	MyHandler h;
	EventQueue::connectEvent(EventConnectorMethod1<MyHandler>(EVT_KEY, &h, &MyHandler::handler) );

	Curses::init();

	EventQueue::run();

	Curses::end();
    } catch (std::exception& e) {
	Curses::end();
	std::cerr << e.what() << std::endl;
	return 1;
    }

    return 0;
}
