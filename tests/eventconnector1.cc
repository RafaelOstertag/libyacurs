// $Id$
//
// Test handler disconnect when in handler.
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

#include "eventconnector.h"

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

class WinChHandler: public Handler {
    public:
        WinChHandler(): Handler(EVT_SIGWINCH) {}
        void handler(Event& e) {
            Handler::handler(e);
        }
};

class AlrmHandler: public Handler {
    public:
        AlrmHandler(): Handler(EVT_SIGALRM) {}
        void handler(Event& e) {
            Handler::handler(e);
        }
};

class AlrmHandler2: public AlrmHandler {
    public:
        AlrmHandler2(): AlrmHandler() {}
        void handler(Event& e) {
            return Handler::handler(e);
        }
};

class Static1 {
    public:
        static void fcthandler1(Event&) {
        }
};

class Static2 {
    public:
        static void fcthandler1(Event&) {
        }
};

int main() {

    WinChHandler winch1;
    WinChHandler winch2;
    AlrmHandler ahandler1;
    AlrmHandler ahandler2;
    AlrmHandler2 ahandler2_1;
    AlrmHandler2 ahandler2_2;

    EventConnectorMethod1<WinChHandler> wconnector1(EVT_SIGWINCH, &winch1, &WinChHandler::handler);
    EventConnectorMethod1<WinChHandler> wconnector2(EVT_SIGWINCH, &winch2, &WinChHandler::handler);

    EventConnectorMethod1<AlrmHandler> aconnector1(EVT_SIGALRM, &ahandler1, &AlrmHandler::handler);
    EventConnectorMethod1<AlrmHandler> aconnector2(EVT_SIGALRM, &ahandler2, &AlrmHandler::handler);

    EventConnectorMethod1<AlrmHandler2> aconnector2_1(EVT_SIGALRM, &ahandler2_1, &AlrmHandler2::handler);
    EventConnectorMethod1<AlrmHandler2> aconnector2_2(EVT_SIGALRM, &ahandler2_2, &AlrmHandler2::handler);
    EventConnectorMethod1<AlrmHandler2> aconnector2_2_(EVT_SIGALRM, &ahandler2_2, &AlrmHandler2::handler);
    EventConnectorMethod1<Handler> ehandler(EVT_SIGALRM, &ahandler2_2, &Handler::handler);


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

    Handler* hptr=&ahandler2_1;
    AlrmHandler* aptr=&ahandler2_1;
    AlrmHandler2* a2ptr=&ahandler2_1;

    EventConnectorMethod1<AlrmHandler2> _a1(EVT_SIGALRM, a2ptr, &AlrmHandler2::handler);
    EventConnectorMethod1<AlrmHandler> _a2(EVT_SIGALRM, aptr, &AlrmHandler::handler);
    EventConnectorMethod1<Handler> _a3(EVT_SIGALRM, hptr, &Handler::handler);

    if (!(_a1 == _a2))
        return 1;

    if (!(_a2 == _a3))
        return 1;

    EventConnectorBase *ptr1, *ptr2;

    ptr1 = &wconnector1;
    ptr2 = &wconnector2;

    if ( *ptr1 == *ptr2 )
        return 1;

    ptr1 = &aconnector2_2;
    ptr2 = &aconnector2_2_;

    if ( *ptr1 != *ptr2 )
        return 1;

    EventConnectorFunction1 efh1(EVT_SIGUSR1, &Static1::fcthandler1);
    EventConnectorFunction1 efh1_1(EVT_SIGUSR1, &Static1::fcthandler1);
    EventConnectorFunction1 efh2(EVT_SIGUSR1, &Static2::fcthandler1);

    if (efh1 != efh1)
        return 1;

    if (efh1 != efh1_1)
        return 1;

    if (efh1 == efh2)
        return 1;

    return 0;
}
