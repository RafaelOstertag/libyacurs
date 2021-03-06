// $Id$
//
// Test basic functionality of EventQueue by injecting events using
// EventQueue::submit()
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
    Handler(YACURS::EventType expected) : expected_evt(expected), calls(0) {}

    virtual void handler(YACURS::Event& e) {
        if (e.type() != expected_evt) std::abort();

        calls++;
    }

    int getCalls() { return calls; }
};

class WinChHandler : public Handler {
   public:
    inline WinChHandler() : Handler(YACURS::EVT_SIGWINCH) {}

    void handler(YACURS::Event& e) {
        Handler::handler(e);

        // Test if we received a YACURS::Size reference.
        dynamic_cast<YACURS::EventEx<YACURS::Size>&>(e).data();
        std::cout << "WinChHandler\r" << std::endl;
    }
};

class AlrmHandler : public Handler {
   public:
    inline AlrmHandler() : Handler(YACURS::EVT_SIGALRM) {}

    void handler(YACURS::Event& e) {
        std::cout << "AlrmHandler\r" << std::endl;
        Handler::handler(e);
    }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

    try {
        WinChHandler whandler;
        AlrmHandler ahandler;

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<WinChHandler>(
                YACURS::EVT_SIGWINCH, &whandler, &WinChHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));

        YACURS::EventQueue::submit(YACURS::EventEx<YACURS::Size>(
            YACURS::EVT_SIGWINCH, YACURS::Area()));
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_SIGALRM));
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));

        YACURS::Curses::init();
        // YACURS::EventQueue:run() blocks in getch().
        ungetch('a');

        YACURS::EventQueue::run();

        if (whandler.getCalls() != 1) goto _ERR;

        if (ahandler.getCalls() != 1) goto _ERR;

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
