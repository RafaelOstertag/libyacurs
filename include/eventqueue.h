// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <signal.h>
#include <list>
#include <queue>

#include "event.h"
#include "eventconnector.h"
#include "area.h"

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
	static std::queue<EventConnectorBase*> evtconn_rem_request;
	static std::list<EventConnectorBase*> evtconn_list;

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

    public:
	/// Connect a member function or function to an event
	static void connect_event(const EventConnectorBase& ec);
	/// Disconnect from the event
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
	static void submit(const Event& ev);
	static void run();
	static void cleanup();
};

#endif // EVENTQUEUE_H
