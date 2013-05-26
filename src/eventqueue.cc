//
// This file is part of libyacurs.
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <functional>
#include <algorithm>

#include <time.h>
#include <unistd.h>

#include "yacursex.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "curs.h"
#include "evtqueuestats.h"

using namespace YACURS;

#if !defined(NDEBUG) && defined(EVTQDEBUG)
#define DEBUGOUT(x) try {                                                       \
        char* __debugfile_name__;                                       \
        if ( (__debugfile_name__ = std::getenv("LIBYACURS_EVT_DBGFN") ) != \
             0) { \
            if (!__debugfile.is_open() )                                 \
                __debugfile.open(__debugfile_name__,                    \
                                 std::ios::out | std::ios::trunc);      \
            __debugfile << x << std::endl;                              \
        }                                                               \
} catch (...) {                                                     \
}
#else
#define DEBUGOUT(x)
#endif

static INTERNAL::EventQueueStats statistics;
static std::ofstream __statsfile;
static std::ofstream __debugfile;

sigset_t EventQueue::block_sigmask;
sigset_t EventQueue::tmp_old_sigmask;
sigset_t EventQueue::old_sigmask;

INTERNAL::Sigaction* EventQueue::sigwinch;
INTERNAL::Sigaction* EventQueue::sigalrm;
INTERNAL::Sigaction* EventQueue::sigusr1;
INTERNAL::Sigaction* EventQueue::sigusr2;
INTERNAL::Sigaction* EventQueue::sigint;
INTERNAL::Sigaction* EventQueue::sigterm;
INTERNAL::Sigaction* EventQueue::sigquit;
INTERNAL::Sigaction* EventQueue::sigtstp;
INTERNAL::Sigaction* EventQueue::sigcont;

bool EventQueue::signal_blocked = false;

std::queue<Event*> EventQueue::evt_queue;
std::list<EventConnectorBase*> EventQueue::evtconn_rem_request;
std::map<EVENT_TYPE, std::list<EventConnectorBase*> > EventQueue::evtconn_map;

LockScreen* EventQueue::__lockscreen = 0;
unsigned int EventQueue::__timeout = 0;

//
// Functors
//

namespace YACURS {
    namespace FUNCTORS {
        namespace EVENTQUEUE {
            /**
             * Functor for comparing EventConnectors for equality.
             */
            class EventConnectorEqual {
                private:
                    const EventConnectorBase& __eb;
                public:
                    /**
                     * @param _eb EventConnector which others are
                     * compared to.
                     */
                    EventConnectorEqual(const EventConnectorBase& _eb) : __eb(
                            _eb) {
                    }

                    /**
                     * @param eb pointer to EventConnector which is
                     * compared to __eb.
                     *
                     * @return @c true if __eb == *eb, @c false
                     * otherwise.
                     */
                    bool operator()(EventConnectorBase* eb) {
                        assert(eb != 0);
                        return *eb == __eb;
                    }
            };

            /**
             * Functor setting the suspend state.
             *
             * Functor for setting the suspend state on the given
             * EventConnector if it matches the Event.
             */
            class EvtConnSetSuspend {
                private:
                    EVENT_TYPE __evt;
                    bool __suspend;

                public:
                    /**
                     * @param _e Event the EventConnector has to
                     * connect in order to be suspended/unsuspended.
                     *
                     * @param _s suspend state to set: @c true to
                     * suspend the EventConnector, @c false to
                     * unsuspend the EventConnector.
                     */
                    EvtConnSetSuspend(EVENT_TYPE _e, bool _s) :
                        __evt(_e), __suspend(_s) {
                    }

                    void operator()(EventConnectorBase* eb) {
                        assert(eb != 0);
                        if (*eb == __evt) {
                            if (__suspend) {
                                DEBUGOUT(
                                    "Suspend: " << (void*)(eb->id() ) << ": " <<
                                    Event::evt2str(*eb) );
                                eb->suspended(true);
                            } else {
                                DEBUGOUT(
                                    "Unsuspend: " << (void*)(eb->id() ) << ": " <<
                                    Event::evt2str(*eb) );
                                eb->suspended(false);
                            }
                        }
                    }
            };

            /**
             * Functor for setting suspend state.
             *
             * Set the suspend/unsuspend state on EventConnectors
             * except for a particular EventConnector.
             */
            class EvtConnSetSuspendExcept {
                private:
                    const EventConnectorBase& __evt;
                    bool __suspend;

                public:
                    /**
                     * @param _e all EventConnectors having the same
                     * EVENT_TYPE as this, will have set their suspend
                     * state. _e will not be changed, though.
                     *
                     * @param _s suspend state to set: @c true to
                     * suspend the EventConnector, @c false to
                     * unsuspend the EventConnector.
                     */
                    EvtConnSetSuspendExcept(const EventConnectorBase& _e,
                                            bool _s) : __evt(_e),
                        __suspend(_s) {
                    }

                    void operator()(EventConnectorBase* eb) {
                        assert(eb != 0);
                        if (*eb == __evt.type() &&
                            !(__evt == *eb) ) {
                            if (__suspend) {
                                DEBUGOUT("Suspend Except: " <<
                                         (void*)(eb->id() ) << ": " <<
                                         Event::evt2str(*eb) );
                                eb->suspended(true);
                            } else {
                                DEBUGOUT("Unsuspend Except: " <<
                                         (void*)(eb->id() ) << ": " <<
                                         Event::evt2str(*eb) );
                                eb->suspended(false);
                            }
                        }
                    }
            };

            /**
             * Deletes a given EventConnector.
             *
             * Functor for freeing memory used by an EventConnector.
             */
            class DestroyEventConnector {
                public:
                    /**
                     * Frees the memory of the given EventConnector.
                     *
                     * @param eb pointer to EventConnectorBase to be
                     * freed.
                     */
                    void operator()(EventConnectorBase* eb) {
                        assert(eb != 0);
                        delete eb;
                    }
            };

            /**
             * Functor for calling EventConnectors for a specific
             * event.
             */
            class CallEventConnector {
                private:
                    /**
                     * Event which is used to determine the
                     * EventConnectors to be called, and passed to
                     * them if called.
                     */
                    Event& __eb;

                public:
                    /**
                     * @param _eb Event for which EventConnectors are
                     * called.
                     */
                    CallEventConnector(Event& _eb) : __eb(_eb) {
                    }

                    /**
                     * Calls the given EventConnector conditionally.
                     *
                     * Calls the given Eventconnector only if it
                     * connects to the same EventType as __eb. __eb
                     * will be passed as argument to the
                     * EventConnector call.
                     *
                     * @param _ec EventConnector which will be called
                     * conditionally if it connects to the same
                     * EventType as __eb is.
                     */
                    void operator()(EventConnectorBase* _ec) {
                        assert(_ec != 0);
                        if (_ec->type() == __eb.type() &&
                            __eb.stop() == false) {
                            statistics.update_ec_calls_by_type(_ec->type() );

                            DEBUGOUT("Call: " <<
                                     (void*)(_ec->id() ) << ": " <<
                                     Event::evt2str(*_ec) );
                            clock_t t0 = clock();
                            _ec->call(__eb);
                            statistics.update_ec_call_time(t0, clock() );
                        }
                    }
            };
        } // namespace EVENTQUEUE
    } // namespace FUNCTORS
} // namespace YACURS

//
// Private
//

/////////////
// Signals //
/////////////

void
EventQueue::setup_signal() {
    sigset_t mask;

    //
    // SIGWINCH
    //

    // This handler is always installed, whether or not resize_term()
    // is available. However, depending on availability of
    // resize_term(), the application will be actually resized.
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigwinch = new INTERNAL::Sigaction(SIGWINCH, signal_handler, mask);

    //
    // SIGQUIT
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigquit = new INTERNAL::Sigaction(SIGQUIT, signal_handler, mask);

    //
    // SIGALRM
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigalrm = new INTERNAL::Sigaction(SIGALRM, signal_handler, mask);

    //
    // SIGUSR1
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigusr1 = new INTERNAL::Sigaction(SIGUSR1, signal_handler, mask);

    //
    // SIGUSR2
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigusr2 = new INTERNAL::Sigaction(SIGUSR2, signal_handler, mask);

    //
    // SIGINT
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigint = new INTERNAL::Sigaction(SIGINT, signal_handler, mask);

    //
    // SIGTERM
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGCONT);
    sigaddset(&mask, SIGTSTP);
    sigterm = new INTERNAL::Sigaction(SIGTERM, signal_handler, mask);

    //
    // SIGCONT
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGTSTP);
    sigcont = new INTERNAL::Sigaction(SIGCONT, signal_handler, mask);

    //
    // SIGTSTP
    //
    sigemptyset(&mask);
    sigaddset(&mask, SIGWINCH);
    sigaddset(&mask, SIGALRM);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGQUIT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGCONT);
    sigtstp = new INTERNAL::Sigaction(SIGTSTP, signal_handler, mask);

    //
    // Unblock signals
    //
    sigemptyset(&block_sigmask);
    sigaddset(&block_sigmask, SIGWINCH);
    sigaddset(&block_sigmask, SIGALRM);
    sigaddset(&block_sigmask, SIGUSR1);
    sigaddset(&block_sigmask, SIGUSR2);
    sigaddset(&block_sigmask, SIGINT);
    sigaddset(&block_sigmask, SIGTERM);
    sigaddset(&block_sigmask, SIGQUIT);
    sigaddset(&block_sigmask, SIGTSTP);
    sigaddset(&block_sigmask, SIGCONT);

    if (sigprocmask(SIG_UNBLOCK, &block_sigmask, &old_sigmask) != 0)
        throw EXCEPTIONS::SystemError(errno);
}

void
EventQueue::restore_signal() {
    if (sigwinch)
        delete sigwinch;

    if (sigalrm)
        delete sigalrm;

    if (sigusr1)
        delete sigusr1;

    if (sigusr2)
        delete sigusr2;

    if (sigint)
        delete sigint;

    if (sigterm)
        delete sigterm;

    if (sigquit)
        delete sigquit;

    if (sigtstp)
        delete sigtstp;

    if (sigcont)
        delete sigcont;

    if (sigprocmask(SIG_SETMASK, &old_sigmask, 0) != 0)
        throw EXCEPTIONS::SystemError(errno);
}

void
#ifdef SA_SIGINFO
EventQueue::signal_handler(int signo, siginfo_t* info, void* d)
#else
EventQueue::signal_handler(int signo)
#endif // SA_SIGINFO
{
    int olderrno = errno;

    switch (signo) {
    case SIGALRM:
        submit(EVT_SIGALRM);
        break;

    case SIGWINCH:
        submit(EVT_TERMRESETUP);
        submit(EventEx<Size>(EVT_SIGWINCH, Curses::inquiry_screensize() ) );
        submit(EVT_REFRESH);
        submit(EVT_DOUPDATE);
        break;

    case SIGUSR1:
        submit(EVT_SIGUSR1);
        break;

    case SIGUSR2:
        submit(EVT_SIGUSR2);
        break;

    case SIGINT:
        submit(EVT_SIGINT);
        break;

    case SIGTERM:
        submit(EVT_SIGTERM);
        break;

    case SIGQUIT:
        submit(EVT_SIGQUIT);
        break;

    case SIGTSTP:
        submit(EVT_SIGTSTP);
        break;

    case SIGCONT:
        submit(EVT_SIGCONT);
        break;
    }

    errno = olderrno;
}

void
EventQueue::blocksignal() {
    if (signal_blocked) return;

    if (sigprocmask(SIG_BLOCK, &block_sigmask, &tmp_old_sigmask) )
        throw EXCEPTIONS::SystemError(errno);

    signal_blocked = true;
}

void
EventQueue::unblocksignal() {
    if (!signal_blocked) return;

    signal_blocked = false;

    if (sigprocmask(SIG_SETMASK, &tmp_old_sigmask, 0) )
        throw EXCEPTIONS::SystemError(errno);
}

/////////////////
// End Signals //
/////////////////

void
EventQueue::proc_rem_request() {
    statistics.update_ec_rmq_size_max(evtconn_rem_request.size() );

    std::list<EventConnectorBase*>::iterator it_erq =
        evtconn_rem_request.begin();

    while (it_erq != evtconn_rem_request.end() ) {
        statistics.update_ec_rm_total();
        assert(*it_erq != 0);

        EventConnectorBase* ecb = *it_erq;

        std::list<EventConnectorBase*>& list = evtconn_map[*ecb];

        std::list<EventConnectorBase*>::iterator it =
            std::find_if(list.begin(),
                         list.end(),
                         FUNCTORS::EVENTQUEUE::EventConnectorEqual(*ecb) );

        delete ecb;
        it_erq++;

        if (it == list.end() ) continue;

        assert(*it != 0);

        delete *it;
        list.erase(it);
    }

    evtconn_rem_request.clear();
}

void
EventQueue::timeout_handler(Event& _e) {
    assert(_e == EVT_SIGALRM);

    if (__lockscreen == 0) return;

    if (__lockscreen->shown() ) {
        __lockscreen->close_unlock_dialog();
        return;
    }

    __lockscreen->show();
}

//
// Public
//
void
EventQueue::connect_event(const EventConnectorBase& ec) {
    //
    // Only one event handler per event per object can be connected
    //

    std::list<EventConnectorBase*>& list = evtconn_map[ec];

    std::list<EventConnectorBase*>::iterator it =
        std::find_if(list.begin(),
                     list.end(),
                     FUNCTORS::EVENTQUEUE::EventConnectorEqual(ec) );

    if (it != list.end() ) {
        assert(*it != 0);
        // there is already a member function registered for the
        // object on the given event. Replace that connection.

        *(*it) = ec;

        DEBUGOUT(
            "Replaced Connector: " << (void*)(*it)->id() << ": " <<
            Event::evt2str(ec) );
    } else {
        // We have to push_back(). The last registered event connector
        // has to be called last. This is required when:
        //  - Realizing
        //  - Refreshing
        //
        // It ensures proper display of overlapping windows.
        EventConnectorBase* __tmp = ec.clone();
        list.push_back(__tmp);

        DEBUGOUT("Connect: " << (void*)__tmp->id() << ": " <<
                 Event::evt2str(ec) );
    }

    //
    // Remove any pending Event connector removal requests.
    //
    // In case you wonder why we don't unsuspend, this had already
    // happended above since we overwrite the still existing
    // connector.
    //
    if (evtconn_rem_request.empty() ) return;

    std::list<EventConnectorBase*>::iterator it_erq =
        std::find_if(evtconn_rem_request.begin(),
                     evtconn_rem_request.end(),
                     FUNCTORS::EVENTQUEUE::EventConnectorEqual(ec) );

    if (it_erq != evtconn_rem_request.end() ) {
        statistics.update_ec_rm_cancelled();

        assert(*it_erq != 0);

        DEBUGOUT(
            "Cancelled Removal: " << (void*)(*it_erq)->id() << ": " <<
            Event::evt2str(ec) );

        delete *it_erq;
        evtconn_rem_request.erase(it_erq);
    }
}

void
EventQueue::disconnect_event(const EventConnectorBase& ec) {
    // does not delete the connector right away, but adds it to a list
    // for later removal. Why? What happens if an EventConnector
    // disconnects itself? std::for_each iterating over the connector
    // list in run() would freak out. So we wait until we're sure that
    // nobody is reading the list.
    evtconn_rem_request.push_back(ec.clone() );

    DEBUGOUT("Disconnect: " << (void*)ec.id() << ": " << Event::evt2str(ec) );

    // However, the event connector must not be called anymore,
    // because the object might have been destroyed meanwhile.

    std::list<EventConnectorBase*>& list = evtconn_map[ec];

    std::list<EventConnectorBase*>::iterator it =
        std::find_if(list.begin(),
                     list.end(),
                     FUNCTORS::EVENTQUEUE::EventConnectorEqual(ec) );
    if (it != list.end() ) {
        assert(*it != 0);
        (*it)->suspended(true);
    }
}

void
EventQueue::suspend(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>& list = evtconn_map[ec];

    std::list<EventConnectorBase*>::iterator it =
        std::find_if(list.begin(),
                     list.end(),
                     FUNCTORS::EVENTQUEUE::EventConnectorEqual(ec) );

    if (it == list.end() ) return;

    DEBUGOUT("Suspend: " << (void*)ec.id() << ": " << Event::evt2str(ec) );

    (*it)->suspended(true);
}

void
EventQueue::suspend_all(EVENT_TYPE _t) {
    std::list<EventConnectorBase*>& list = evtconn_map[_t];

    std::for_each(list.begin(),
                  list.end(),
                  FUNCTORS::EVENTQUEUE::EvtConnSetSuspend(_t, true) );
}

void
EventQueue::suspend_except(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>& list = evtconn_map[ec];

    std::for_each(list.begin(),
                  list.end(),
                  FUNCTORS::EVENTQUEUE::EvtConnSetSuspendExcept(ec, true) );
}

void
EventQueue::unsuspend(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>& list = evtconn_map[ec];

    std::list<EventConnectorBase*>::iterator it =
        std::find_if(list.begin(),
                     list.end(),
                     FUNCTORS::EVENTQUEUE::EventConnectorEqual(ec) );

    if (it == list.end() ) return;

    DEBUGOUT("Unsuspend: " << (void*)ec.id() << ": " << Event::evt2str(ec) );

    (*it)->suspended(false);
}

void
EventQueue::unsuspend_all(EVENT_TYPE _t) {
    std::list<EventConnectorBase*>& list = evtconn_map[_t];
    std::for_each(list.begin(),
                  list.end(),
                  FUNCTORS::EVENTQUEUE::EvtConnSetSuspend(_t, false) );
}

void
EventQueue::unsuspend_except(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>& list = evtconn_map[ec];
    std::for_each(list.begin(),
                  list.end(),
                  FUNCTORS::EVENTQUEUE::EvtConnSetSuspendExcept(ec, false) );
}

void
EventQueue::submit(EVENT_TYPE _et) {
    submit(Event(_et) );
}

void
EventQueue::submit(const Event& ev) {
    blocksignal();
    try {
        DEBUGOUT("Submitted: " << Event::evt2str(ev) );
        evt_queue.push(ev.clone() );
        statistics.update_evt_submitted_by_type(ev);
#ifndef NDEBUG
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::abort();
    }
#else
    } catch (std::exception&) {
	// Intentionally empty
    }
#endif
    unblocksignal();
}

void
EventQueue::run() {
    DEBUGOUT("EventQueue::run(): --- enter");

    setup_signal();

    // since statistics is an object, its ctor has been called and we
    // don't have to initialize again.
    //
    //statistics.clear();

    for (;; ) {
        if (__lockscreen != 0) {
            // Adding lockscreen will set timeout.
            alarm(__timeout);
        }

        // This is to move the cursor to the focused widget. Before
        // adding that call tests/focus1 had the cursor always left on
        // the status line. It's sorta hack, but it works...
        FocusManager::refocus();

#ifdef ENABLE_NLS
	wint_t c;
	int retval = wget_wch(stdscr, &c);
#else
        int c = wgetch(stdscr);
#endif
        clock_t t0 = clock();

        blocksignal();

#ifdef ENABLE_NLS
	if (retval != ERR) {
#else
        if (c != ERR) {
#endif
            switch (c) {
            case KEY_REFRESH:
            case KEY_CTRL_L:
                submit(Event(EVT_FORCEREFRESH) );
                submit(Event(EVT_REFRESH) );
                submit(Event(EVT_DOUPDATE) );
                break;

            default:
#ifdef ENABLE_NLS
		submit(EventEx<wint_t>(EVT_KEY, c) );
#else
                submit(EventEx<int>(EVT_KEY, c) );
#endif
            }
        }

        statistics.update_ec_count(evtconn_map);

        // process any pending EventConnector removal requests
        proc_rem_request();

        while (!evt_queue.empty() ) {
            statistics.update_evq_size_max(evt_queue.size() );

            Event* evt = evt_queue.front();
            assert(evt != 0);
            statistics.update_evt_proc_by_type(evt->type() );

            if (evt->type() == EVT_QUIT) {
                unblocksignal();
                goto QUIT;
            }

            DEBUGOUT("Processing: " << Event::evt2str(*evt) );
            clock_t evt_t0 = clock();
            std::list<EventConnectorBase*>& list = evtconn_map[*evt];
            std::for_each(list.begin(),
                          list.end(),
                          FUNCTORS::EVENTQUEUE::CallEventConnector(*evt) );

            statistics.update_evt_proc_time(evt_t0, clock() );

            delete evt;
            evt_queue.pop();
        }

        // process any pending EventConnector removal requests
        proc_rem_request();

        statistics.update_ec_count(evtconn_map);

        unblocksignal();

        statistics.update_evq_proc_time(t0, clock() );
    }
QUIT:
    restore_signal();

    cleanup();
    DEBUGOUT("EventQueue::run(): --- leave");
}

void
EventQueue::cleanup() {
    DEBUGOUT("EventQueue::cleanup()");
    /* Cleanup event queue */
    while (!evt_queue.empty() ) {
        statistics.update_evt_pending_cleanup();
        DEBUGOUT("Discarded: " << Event::evt2str(*(evt_queue.front() ) ) );
        delete evt_queue.front();
        evt_queue.pop();
    }

    // Remove any pending removal request, so that the memory will be
    // freed properly
    proc_rem_request();

    // Free the memory occupied by remaining connectors
    std::map<EVENT_TYPE,
             std::list<EventConnectorBase*> >::iterator m_it =
        evtconn_map.begin();
    while (m_it != evtconn_map.end() ) {
        std::list<EventConnectorBase*>::iterator it = m_it->second.begin();
        while (it != m_it->second.end() ) {
            assert(*it != 0);
            delete *it++;
        }
        m_it++;
    }

    evtconn_map.clear();

    char* stats_fn;
    if ( (stats_fn = std::getenv("LIBYACURS_EVT_STATS") ) != 0) {
        try {
            if (!__statsfile.is_open() )
                __statsfile.open(stats_fn, std::ios::out | std::ios::trunc);
            statistics.dump(__statsfile);

            __statsfile.close();
        } catch (...) {
        }
    }
}

void
EventQueue::lock_screen(LockScreen* _ls) {
    __lockscreen = _ls;
    if (__lockscreen == 0) {
        alarm(0);
        disconnect_event(EventConnectorFunction1(EVT_SIGALRM,
                                                 &EventQueue::timeout_handler) );
    } else {
        timeout(__lockscreen->timeout() );
        connect_event(EventConnectorFunction1(EVT_SIGALRM,
                                              &EventQueue::timeout_handler) );
    }
}

LockScreen*
EventQueue::lock_screen() {
    return __lockscreen;
}

void
EventQueue::timeout(unsigned int _t) {
    __timeout = _t;
    alarm(__timeout);
}

unsigned int
EventQueue::timeout() {
    return __timeout;
}
