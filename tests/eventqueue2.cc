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
        YACURS::EVENT_TYPE expected_evt;
        int calls;
    public:
        Handler(YACURS::EVENT_TYPE expected) :
            expected_evt(expected), calls(0) {
        }

        virtual void handler(YACURS::Event& e) {
            if (e.type() != expected_evt) std::abort();

            calls++;
        }

        int getCalls() {
            return calls;
        }
};

class AlrmHandler : public Handler {
    public:
        AlrmHandler() : Handler(YACURS::EVT_SIGALRM) {
        }

        void handler(YACURS::Event& e) {
            std::cout << "AlrmHandler::handler()\r" << std::endl;
            YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
            Handler::handler(e);
        }
};

class SelfRegister : public Handler {
    public:
        SelfRegister() : Handler(YACURS::EVT_SIGALRM) {
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                                  SelfRegister>(YACURS::
                                                                EVT_SIGALRM,
                                                                this,
        &SelfRegister::handler) );
        }

        void handler(YACURS::Event& e) {
            std::cout << "SelfRegister::handler()\r" << std::endl;
            Handler::handler(e);
        }
};

class SelfRegister2 : public SelfRegister {
    public:
        SelfRegister2() : SelfRegister() {
            YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                                  SelfRegister2>(YACURS::
                                                                 EVT_SIGALRM,
                                                                 this,
        &SelfRegister2::handler) );
        }

        void handler(YACURS::Event& e) {
            YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
            std::cout << "SelfRegister2::handler()\r" << std::endl;
            SelfRegister::handler(e);
        }
};

int
main() {
    try {
        AlrmHandler ahandler;

        YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                              AlrmHandler>(YACURS::EVT_SIGALRM,
                                                           &ahandler,
                                                           &AlrmHandler::
        handler) );

        YACURS::Curses::init();
        alarm(2);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1)
            goto _ERR;

        SelfRegister2 sr2;
        alarm(2);
        YACURS::EventQueue::run();

        if (sr2.getCalls() != 1)
            goto _ERR;

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
_ERR:
    YACURS::Curses::end();
    return 1;
}
