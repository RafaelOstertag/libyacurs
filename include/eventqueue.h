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
#if defined(SIGWINCH) || defined(SIGALRM)
	// Used by blocksignal()/unblocksignal()
	static sigset_t block_sigmask;
	// Used by blocksignal()/unblocksignal()
	static sigset_t tmp_old_sigmask;
	// Used by setupSignal()/restoreSignal()
	static sigset_t old_sigmask;

	static struct sigaction old_winch_act;
	static struct sigaction old_alrm_act;
#endif // defined(SIGWINCH) || defined(SIGALRM)
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
	static void connectEvent(const EventConnectorBase& ec);
	static void disconnectEvent(const EventConnectorBase& ec);
	static void inject(const EventBase& ev);
	static void run();
};

#endif // EVENTQUEUE_H
