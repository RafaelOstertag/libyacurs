// -*- mode: c++ -*-
//
// This file is part of libyacurs, 
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

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include <signal.h>
#include <list>
#include <map>
#include <queue>

#include "event.h"
#include "eventconnector.h"
#include "area.h"
#include "lockscreen.h"

namespace YACURS {
    /**
     * @ingroup Event
     *
     * The Event Queue dispatches Events submitted to the respective
     * connectors.
     *
     * It is started by calling EventQueue::run(). It will then wait for
     * keyboard input or Unix Signals and dispatch, aka. `emit`, the Event
     * to all Event Connectors for the given event.
     *
     * The Event Queue will terminate upon submission of @c EVT_QUIT.
     *
     * Libyacurs counts on the order events are connected, so that the
     * EVT_REFRESH/EVT_DOUPDATE event sequence will call the last
     * connected event handler last, thus ensuring the update will not
     * suddenly display a main window before an recently opened dialog,
     * for instance.
     *
     * EventQueue will also emit events for global conditions, such as
     * screen refresh request when pressing Ctrl-L.
     *
     * @section Unix Signals
     *
     * EventQueue will emit events on following Unix Signals
     *
     * - SIGWINCH (@c EVT_SIGWINCH)
     * - SIGALRM (@c EVT_SIGALRM)
     * - SIGUSR1 (@c EVT_SIGUSR1)
     * - SIGUSR2 (@c EVT_SIGUSR2)
     * - SIGINT (@c EVT_SIGINT)
     *
     * @section Terminal Resize
     *
     * If supported by the curses implementation, it consists of following
     * event sequence:
     *
     * - EVT_TERMRESETUP (handled in Curses)
     * - EVT_SIGWINCH (only classes derived from WindowBase should connect.)
     * - EVT_REFRESH
     * - EVT_DOUPDATE
     *
     * @section Complete Refresh
     *
     * A complete refresh is initiated by pressing Ctrl-L or KEY_REFRESH
     * on the keyboard. It consists of following event sequence:
     *
     * - EVT_FORCEREFRESH
     * - EVT_REFRESH
     * - EVT_DOUPDATE
     *
     */
    class EventQueue {
	private:
	    /// Used by blocksignal()/unblocksignal()
	    static sigset_t block_sigmask;
	    /// Used by blocksignal()/unblocksignal()
	    static sigset_t tmp_old_sigmask;
	    /// Used by setup_signal()/restore_signal()
	    static sigset_t old_sigmask;

	    static struct sigaction old_winch_act;
	    static struct sigaction old_alrm_act;
	    static struct sigaction old_usr1_act;
	    static struct sigaction old_usr2_act;
	    static struct sigaction old_int_act;

	    static bool signal_blocked;
	    static std::queue<Event*> evt_queue;
	    /**
	     * EventConnectors are not removed immediately, instead remove
	     * requests are queue up for processing later.
	     */
	    static std::list<EventConnectorBase*> evtconn_rem_request;
	    static std::map<EVENT_TYPE,std::list<EventConnectorBase*> > evtconn_map;

	    /**
	     * LockScreen used on timeout
	     */
	    static LockScreen* __lockscreen;

	    /**
	     * Timeout for keyboard input.
	     *
	     * If for __timeout seconds no key (event) is pressed
	     * (received), the lock screen kicks in, if any.
	     */
	    static unsigned int __timeout;

	    static void setup_signal();
	    static void restore_signal();

#ifdef SA_SIGINFO
	    static void signal_handler(int signo, siginfo_t* info, void *d);
#else
	    static void signal_handler(int signo);
#endif // SA_SIGINFO

	    static void blocksignal();
	    static void unblocksignal();
	    static void proc_rem_request();

	    static void timeout_handler(Event& _e);

	public:
	    /**
	     * Connect an event connector.
	     *
	     * Connect a (member) function to an Event. Please note, that
	     * only one function per object and Event can be connected. If
	     * two or more member functions of the same object will be
	     * connected to a single Event, each call to connect
	     * overwrites previous connections.
	     *
	     * @note the last event connector registered will be called
	     * first.
	     *
	     * @note any pending disconnect will be cancelled and the
	     * connector unsuspended.
	     *
	     * @param ec event connector to register.
	     */
	    static void connect_event(const EventConnectorBase& ec);

	    /**
	     * Disconnect an event connector.
	     *
	     * Disconnect the specified event connector.
	     *
	     * @note disconnecting does not immediately remove the
	     * connector. Instead it will be queued up for later
	     * removal. Until it is finally removed, it will be suspended.
	     * @note any pending removal will be cancelled by a
	     * (re-)connect and the connector will be unsuspended.
	     *
	     * @param ec event connector to disconnect.
	     */
	    static void disconnect_event(const EventConnectorBase& ec);

	    /// Suspend event
	    static void suspend(const EventConnectorBase& ec);
	    /// Suspend all events equal to a given event
	    static void suspend_all(EVENT_TYPE _t);
	    /// Suspend all events except the one given
	    static void suspend_except(const EventConnectorBase& ec);
	    /// Unsuspend event
	    static void unsuspend(const EventConnectorBase& ec);
	    /// Unsuspend all events equal to a given event
	    static void unsuspend_all(EVENT_TYPE _t);
	    /// Unsuspend all events except the one given
	    static void unsuspend_except(const EventConnectorBase& ec);

	    /// Add an event to the qeue
	    static void submit(EVENT_TYPE _et);
	    static void submit(const Event& ev);

	    static void run();
	    static void cleanup();

	    static void lock_screen(LockScreen* _ls);
	    static LockScreen* lock_screen();

	    static void timeout(unsigned int _t);
	    static unsigned int timeout();
    };
}

#endif // EVENTQUEUE_H
