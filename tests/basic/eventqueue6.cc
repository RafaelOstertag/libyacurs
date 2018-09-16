// $Id$
//
// Test handler disconnect when in handler.
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
        std::cout << "AlrmHandler::handler()\r" << std::endl;
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
        Handler::handler(e);
    }
};

class AlrmHandler2 : public AlrmHandler {
   private:
    int calls_handler2;
    int calls_handler3;

   public:
    AlrmHandler2() : AlrmHandler(), calls_handler2(0), calls_handler3(0) {}

    void handler(YACURS::Event& e) {
        std::cout << "AlrmHandler2::handler()\r" << std::endl;
        Handler::handler(e);
    }

    void handler2(YACURS::Event& e) {
        std::cout << "AlrmHandler2::handler2()\r" << std::endl;

        calls_handler2++;
    }

    void handler3(YACURS::Event& e) {
        std::cout << "AlrmHandler2::handler3()\r" << std::endl;

        calls_handler3++;
    }

    int getCalls2() const { return calls_handler2; }

    int getCalls3() const { return calls_handler3; }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

    try {
        AlrmHandler ahandler;
        AlrmHandler2 ahandler2;

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        // Should be overriden by next call
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler2));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler));
        // AlrmHandler2::handler should not be called now
        YACURS::EventQueue::disconnect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGALRM, &ahandler2, &AlrmHandler2::handler));

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler2>(
                YACURS::EVT_SIGWINCH, &ahandler2, &AlrmHandler2::handler3));

        YACURS::Curses::init();
        YACURS::EventQueue::submit(YACURS::EventEx<YACURS::Size>(
            YACURS::EVT_SIGWINCH, YACURS::Area()));
        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (ahandler2.getCalls() != 0) goto _ERR;

        if (ahandler2.getCalls2() != 0) goto _ERR;

        if (ahandler2.getCalls3() != 1) goto _ERR;

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
