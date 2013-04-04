// $Id$
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

// Used when preloading libtestpreload.so
char __test_str[]="Hello, World!";
extern "C" int __test_wgetch(void*) {
    static char* ptr=__test_str;
    return *ptr++;
}

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
            if (e.type() != expected_evt) {
                std::cerr << "Unexpected event type" << std::endl;
                std::abort();
            }

            calls++;
        }
        int getCalls() {
            return calls;
        }
        void reset() {
            calls=0;
        };
};

class MyHandler: public Handler {
    private:
        std::string _str;

    public:
        MyHandler(): Handler(EVT_KEY), _str() {}
        void handler(Event& e) {
            Handler::handler(e);

            if (typeid(e) != typeid(EventEx<int>)) std::abort();

            EventEx<int>& tmp = dynamic_cast<EventEx<int>&>(e);
            char c = static_cast<char>(tmp.data());
            _str.push_back(c);

            if (_str == "Hello, World!")
                EventQueue::submit(Event(EVT_QUIT));
        }
};

int main() {
    try {
        MyHandler h;
        EventQueue::connect_event(EventConnectorMethod1<MyHandler>(EVT_KEY, &h, &MyHandler::handler) );

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
