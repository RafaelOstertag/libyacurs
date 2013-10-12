// $Id$
//
// Test handler disconnect when in handler.
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>

#include <iostream>
#include <cstdlib>

#include "eventconnector.h"

class Handler {
    private:
        YACURS::EventType expected_evt;
        int calls;
    public:
        Handler(YACURS::EventType evt) :
            expected_evt(evt), calls(0) {
        }

        virtual ~Handler() {
        }

        virtual void handler(YACURS::Event& e) {
            if (e.type() != expected_evt) std::abort();

            calls++;
        }

        int getCalls() {
            return calls;
        }
};

class WinChHandler : public Handler {
    public:
        WinChHandler() : Handler(YACURS::EVT_SIGWINCH) {
        }

        void handler(YACURS::Event& e) {
            Handler::handler(e);
        }
};

class AlrmHandler : public Handler {
    public:
        AlrmHandler() : Handler(YACURS::EVT_SIGALRM) {
        }

        void handler(YACURS::Event& e) {
            Handler::handler(e);
        }
};

class AlrmHandler2 : public AlrmHandler {
    public:
        AlrmHandler2() : AlrmHandler() {
        }

        void handler(YACURS::Event& e) {
            return Handler::handler(e);
        }
};

class Static1 {
    public:
        static void fcthandler1(YACURS::Event&) {
        }
};

class Static2 {
    public:
        static void fcthandler1(YACURS::Event&) {
        }
};

int
main() {
    WinChHandler winch1;
    WinChHandler winch2;
    AlrmHandler ahandler1;
    AlrmHandler ahandler2;
    AlrmHandler2 ahandler2_1;
    AlrmHandler2 ahandler2_2;

    YACURS::EventConnectorMethod1<WinChHandler> wconnector1(
        YACURS::EVT_SIGWINCH,
        &winch1,
        &WinChHandler::handler);
    YACURS::EventConnectorMethod1<WinChHandler> wconnector2(
        YACURS::EVT_SIGWINCH,
        &winch2,
        &WinChHandler::handler);

    YACURS::EventConnectorMethod1<AlrmHandler> aconnector1(
        YACURS::EVT_SIGALRM,
        &ahandler1,
        &AlrmHandler::handler);
    YACURS::EventConnectorMethod1<AlrmHandler> aconnector2(
        YACURS::EVT_SIGALRM,
        &ahandler2,
        &AlrmHandler::handler);

    YACURS::EventConnectorMethod1<AlrmHandler2> aconnector2_1(
        YACURS::EVT_SIGALRM,
        &ahandler2_1,
        &AlrmHandler2::handler);
    YACURS::EventConnectorMethod1<AlrmHandler2> aconnector2_2(
        YACURS::EVT_SIGALRM,
        &ahandler2_2,
        &AlrmHandler2::handler);
    YACURS::EventConnectorMethod1<AlrmHandler2> aconnector2_2_(
        YACURS::EVT_SIGALRM,
        &ahandler2_2,
        &AlrmHandler2::handler);
    YACURS::EventConnectorMethod1<Handler> ehandler(YACURS::EVT_SIGALRM,
                                                    &ahandler2_2,
                                                    &Handler::handler);

    if (wconnector1 == wconnector2)
        return 1;

    if (wconnector1 != wconnector1)
        return 1;

    if (wconnector1 == aconnector1)
        return 1;

    if (aconnector1 == aconnector2_1)
        return 1;

    if (aconnector2_2 != aconnector2_2_)
        return 1;

    if (aconnector2_2 != ehandler)
        return 1;

    Handler* hptr = &ahandler2_1;
    AlrmHandler* aptr = &ahandler2_1;
    AlrmHandler2* a2ptr = &ahandler2_1;

    YACURS::EventConnectorMethod1<AlrmHandler2> _a1(YACURS::EVT_SIGALRM,
                                                    a2ptr,
                                                    &AlrmHandler2::handler);
    YACURS::EventConnectorMethod1<AlrmHandler> _a2(YACURS::EVT_SIGALRM,
                                                   aptr,
                                                   &AlrmHandler::handler);
    YACURS::EventConnectorMethod1<Handler> _a3(YACURS::EVT_SIGALRM,
                                               hptr,
                                               &Handler::handler);

    if (!(_a1 == _a2) )
        return 1;

    if (!(_a2 == _a3) )
        return 1;

    YACURS::EventConnectorBase* ptr1, * ptr2;

    ptr1 = &wconnector1;
    ptr2 = &wconnector2;

    if (*ptr1 == *ptr2)
        return 1;

    ptr1 = &aconnector2_2;
    ptr2 = &aconnector2_2_;

    if (*ptr1 != *ptr2)
        return 1;

    YACURS::EventConnectorFunction1 efh1(YACURS::EVT_SIGUSR1,
                                         &Static1::fcthandler1);
    YACURS::EventConnectorFunction1 efh1_1(YACURS::EVT_SIGUSR1,
                                           &Static1::fcthandler1);
    YACURS::EventConnectorFunction1 efh2(YACURS::EVT_SIGUSR1,
                                         &Static2::fcthandler1);

    if (efh1 != efh1)
        return 1;

    if (efh1 != efh1_1)
        return 1;

    if (efh1 == efh2)
        return 1;

    return 0;
}
