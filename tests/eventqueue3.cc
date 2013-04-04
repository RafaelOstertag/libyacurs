// $Id$
//
// Test several handler for event
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
        int getCalls() {
            return calls;
        }
};

class AlrmHandler: public Handler {
    public:
        AlrmHandler(): Handler(EVT_SIGALRM) {}
        void handler(Event& e) {
            Handler::handler(e);
            std::cout << "AlrmHandler::handler()\r" << std::endl;

            if (getCalls() < 4) {
                alarm(1);
                return;
            }

            std::cout << "QUIT" << std::endl;
            EventQueue::submit(Event(EVT_QUIT));
        }
};

class AlrmHandler2: public AlrmHandler {
    public:
        AlrmHandler2(): AlrmHandler() {}
        void handler(Event& e) {
            std::cout << "AlrmHandler2::handler()\r" << std::endl;
            Handler::handler(e);
        }
};

int main() {

    try {
        AlrmHandler ahandler;
        AlrmHandler2 ahandler2_1;
        AlrmHandler2 ahandler2_2;
        AlrmHandler2 ahandler2_3;
        AlrmHandler2 ahandler2_4;

        EventQueue::connect_event(EventConnectorMethod1<AlrmHandler>(EVT_SIGALRM, &ahandler,&AlrmHandler::handler) );
        EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2_1,&AlrmHandler2::handler) );
        EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2_2,&AlrmHandler2::handler) );
        EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2_3,&AlrmHandler2::handler) );
        EventQueue::connect_event(EventConnectorMethod1<AlrmHandler2>(EVT_SIGALRM, &ahandler2_4,&AlrmHandler2::handler) );

        Curses::init();
        alarm(4);
        EventQueue::run();

        if (ahandler.getCalls() != 4)
            goto _ERR;

        if (ahandler2_1.getCalls() != 4)
            goto _ERR;

        if (ahandler2_2.getCalls() != 4)
            goto _ERR;

        if (ahandler2_3.getCalls() != 4)
            goto _ERR;

        if (ahandler2_4.getCalls() != 4)
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
