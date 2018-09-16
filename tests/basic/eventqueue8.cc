// $Id$
//
// Test (un)suspending of events
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

    void reset() { calls = 0; }
};

class AlrmHandler : public Handler {
   public:
    AlrmHandler() : Handler(YACURS::EVT_SIGALRM) {}

    void handler(YACURS::Event& e) {
        Handler::handler(e);
        std::cout << "AlrmHandler2::handler()\r" << std::endl;
        // During processing of the queue, USR1 is blocked, so we cheat and
        // unblock it
        sigset_t _sset;

        sigemptyset(&_sset);
        sigaddset(&_sset, SIGUSR1);

        if (sigprocmask(SIG_UNBLOCK, &_sset, 0)) abort();

        raise(SIGUSR1);
    }
};

class Usr1Handler : public Handler {
   private:
    bool quit;

   public:
    Usr1Handler(bool q = false) : Handler(YACURS::EVT_SIGUSR1), quit(q) {}

    void handler(YACURS::Event& e) {
        Handler::handler(e);
        std::cout << "Usr1Handler::handler()\r" << std::endl;

        if (quit)
            YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
        else {
            // During processing of the queue, USR2 is blocked, so we cheat
            // and unblock it
            sigset_t _sset;
            sigemptyset(&_sset);
            sigaddset(&_sset, SIGUSR2);

            if (sigprocmask(SIG_UNBLOCK, &_sset, 0)) abort();

            raise(SIGUSR2);
        }
    }

    void setQuit(bool q) { quit = q; }
};

class Usr2Handler : public Handler {
   public:
    Usr2Handler() : Handler(YACURS::EVT_SIGUSR2) {}

    void handler(YACURS::Event& e) {
        Handler::handler(e);
        std::cout << "Usr2Handler::handler()\r" << std::endl;
        YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT));
    }
};

int main() {
    // test will not be run if stdout or stdin is not a tty.
    if (isatty(STDOUT_FILENO) != 1 || isatty(STDIN_FILENO) != 1) exit(77);

    try {
        AlrmHandler ahandler;
        Usr1Handler u1handler;
        Usr2Handler u2handler, u2handler2, u2handler3;

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));

        YACURS::Curses::init();
        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 1) goto _ERR;

        if (u2handler2.getCalls() != 1) goto _ERR;

        if (u2handler2.getCalls() != 1) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));
        //
        // u2handler* may not have any calls
        //
        YACURS::EventQueue::suspend_all(YACURS::EVT_SIGUSR2);

        // u1handler must terminate queue loop
        u1handler.setQuit(true);

        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 0) goto _ERR;

        if (u2handler2.getCalls() != 0) goto _ERR;

        if (u2handler3.getCalls() != 0) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));
        //
        // u2handler* must have calls
        //
        YACURS::EventQueue::unsuspend_all(YACURS::EVT_SIGUSR2);

        u1handler.setQuit(false);

        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 1) goto _ERR;

        if (u2handler2.getCalls() != 1) goto _ERR;

        if (u2handler3.getCalls() != 1) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));
        //
        // only u2handler2 must have calls
        //
        YACURS::EventQueue::suspend_except(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

        u1handler.setQuit(false);

        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 0) goto _ERR;

        if (u2handler2.getCalls() != 1) goto _ERR;

        if (u2handler3.getCalls() != 0) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));
        //
        // only u2handler2 must have no calls
        //

        YACURS::EventQueue::suspend_all(YACURS::EVT_SIGUSR2);
        YACURS::EventQueue::unsuspend_except(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 1) goto _ERR;

        if (u2handler2.getCalls() != 0) goto _ERR;

        if (u2handler3.getCalls() != 1) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<AlrmHandler>(
                YACURS::EVT_SIGALRM, &ahandler, &AlrmHandler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr1Handler>(
                YACURS::EVT_SIGUSR1, &u1handler, &Usr1Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));
        YACURS::EventQueue::connect_event(
            YACURS::EventConnectorMethod1<Usr2Handler>(
                YACURS::EVT_SIGUSR2, &u2handler3, &Usr2Handler::handler));
        //
        // only u2handler2 must have no calls
        //

        YACURS::EventQueue::suspend(YACURS::EventConnectorMethod1<Usr2Handler>(
            YACURS::EVT_SIGUSR2, &u2handler2, &Usr2Handler::handler));

        alarm(4);
        YACURS::EventQueue::run();

        if (ahandler.getCalls() != 1) goto _ERR;

        if (u1handler.getCalls() != 1) goto _ERR;

        if (u2handler.getCalls() != 1) goto _ERR;

        if (u2handler2.getCalls() != 0) goto _ERR;

        if (u2handler3.getCalls() != 1) goto _ERR;

        ahandler.reset();
        u1handler.reset();
        u2handler.reset();
        u2handler2.reset();
        u2handler3.reset();

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
