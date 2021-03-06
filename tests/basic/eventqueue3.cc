// $Id$
//
// Test several handler for event
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "yacurs.h"

class Handler {
   private:
    YACURS::EventType expected_evt;
    int calls;

   public:
    Handler(YACURS::EventType evt) : expected_evt(evt), calls(0) {}

    virtual ~Handler() {}

    virtual void handler(YACURS::Event& e) {
        if (e.type() != expected_evt) std::abort();

        calls++;
    }

    int getCalls() { return calls; }
};

class AlrmHandler : public Handler {
   public:
    AlrmHandler() : Handler(YACURS::EVT_SIGALRM) {}

    void handler(YACURS::Event& e) {
        Handler::handler(e);
        std::cout << "AlrmHandler::handler()\r" << std::endl;

        if (getCalls() < 4) {
            alarm(1);
            return;
        }

        std::cout << "QUIT" << std::endl;
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
    }
};

class AlrmHandler2 : public AlrmHandler {
   public:
    AlrmHandler2() : AlrmHandler() {}

    void handler(YACURS::Event& e) {
        std::cout << "AlrmHandler2::handler()\r" << std::endl;
        Handler::handler(e);
    }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

    try {
        AlrmHandler ahandler;
        AlrmHandler2 ahandler2_1;
        AlrmHandler2 ahandler2_2;
        AlrmHandler2 ahandler2_3;
        AlrmHandler2 ahandler2_4;

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2_1, &AlrmHandler2::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2_2, &AlrmHandler2::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2_3, &AlrmHandler2::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2_4, &AlrmHandler2::handler));

        YACURS::Curses::init();
        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 4) goto _ERR;

        if (ahandler2_1.getCalls() != 4) goto _ERR;

        if (ahandler2_2.getCalls() != 4) goto _ERR;

        if (ahandler2_3.getCalls() != 4) goto _ERR;

        if (ahandler2_4.getCalls() != 4) goto _ERR;

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
