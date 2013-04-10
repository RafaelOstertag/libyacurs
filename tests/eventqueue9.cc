// $Id$
//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <iostream>
#include <cstdlib>

#include "yacurs.h"

// Used when preloading libtestpreload.so
char __test_str[]="Hello, World!";
extern "C" int __test_wgetch(void*) {
    static char* ptr=__test_str;
    return *ptr++;
}

class Handler {
    private:
        YACURS::EVENT_TYPE expected_evt;
        int calls;
    public:
        Handler(YACURS::EVENT_TYPE evt):
            expected_evt(evt), calls(0) {}
        Handler(const Handler& _h) {
            expected_evt = _h.expected_evt;
            calls = _h.calls;
        }
        virtual ~Handler() {}
        virtual void handler(YACURS::Event& e) {
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
        }
};

class MyHandler: public Handler {
    private:
        std::string _str;

    public:
        MyHandler(): Handler(YACURS::EVT_KEY), _str() {}
        void handler(YACURS::Event& e) {
            Handler::handler(e);

            if (typeid(e) != typeid(YACURS::EventEx<int>)) std::abort();

            YACURS::EventEx<int>& tmp = dynamic_cast<YACURS::EventEx<int>&>(e);
            char c = static_cast<char>(tmp.data());
            _str.push_back(c);

            if (_str == "Hello, World!")
                YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
        }
};

int main() {
    try {
        MyHandler h;
        YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MyHandler>(YACURS::EVT_KEY, &h, &MyHandler::handler) );

        YACURS::Curses::init();

        YACURS::EventQueue::run();

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
