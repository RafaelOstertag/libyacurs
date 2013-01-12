// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif // HAVE_SIGNAL_H

#ifdef HAVE_LIST
#include <list>
#endif // HAVE_LIST

#ifdef HAVE_QUEUE
#include <queue>
#endif // HAVE_QUEUE

#include "event.h"
#include "eventconnector.h"
#include "rectangle.h"

class EventQueue {
    private:
	/// Used by blocksignal()/unblocksignal()
	static sigset_t block_sigmask;
	/// Used by blocksignal()/unblocksignal()
	static sigset_t tmp_old_sigmask;
	/// Used by setupSignal()/restoreSignal()
	static sigset_t old_sigmask;

	static struct sigaction old_winch_act;
	static struct sigaction old_alrm_act;
	static struct sigaction old_usr1_act;
	static struct sigaction old_usr2_act;

	static bool signal_blocked;
	static std::queue<EventBase*> evt_queue;
	/**
	 * EventConnectors are not removed immediately, instead remove
	 * requests are queue up for processing later.
	 */
	static std::queue<EventConnectorBase*> evtconn_rem_request;
	static std::list<EventConnectorBase*> evtconn_list;

	static void setupSignal();
	static void restoreSignal();

#ifdef SA_SIGINFO
	static void signal_handler(int signo, siginfo_t* info, void *d);
#else
	static void signal_handler(int signo);
#endif // SA_SIGINFO

	static void blocksignal();
	static void unblocksignal();
	static void proc_rem_request();

    public:
	/// Connect a member function or function to an event
	static void connectEvent(const EventConnectorBase& ec);
	/// Disconnect from the event
	static void disconnectEvent(const EventConnectorBase& ec);
	/// Suspend event
	static void suspend(const EventConnectorBase& ec);
	/// Suspend all events equal to a given event
	static void suspendAll(EVENT_TYPE _t);
	/// Suspend all events except the one given
	static void suspendExcept(const EventConnectorBase& ec);
	/// Unsuspend event
	static void unsuspend(const EventConnectorBase& ec);
	/// Unsuspend all events equal to a given event
	static void unsuspendAll(EVENT_TYPE _t);
	/// Unsuspend all events except the one given
	static void unsuspendExcept(const EventConnectorBase& ec);

	/// Add an event to the qeue
	static void inject(const EventBase& ev);
	static void run();
};

#endif // EVENTQUEUE_H
