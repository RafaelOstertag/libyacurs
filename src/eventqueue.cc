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

#include "cursex.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "curs.h"

#define STATS_EC(x) (statistics.ec_##x)
#define STATS_EC_S(x) case(EVT_##x):		\
    STATS_EC(x)++;				\
    break;
#define STATS_EC_OUT(str,stats) statsfile << str << STATS_EC(stats) << std::endl

// macro because I don't want to clutter the code
#define EVENTCONN_STATS(x) switch (x) {		\
    case EVT_SIGWINCH:				\
	statistics.ec_SIGWINCH++;		\
	break;					\
    case EVT_SIGALRM:				\
	statistics.ec_SIGALRM++;		\
    break;					\
    case EVT_SIGUSR1:				\
	statistics.ec_SIGUSR1++;		\
	break;					\
    case EVT_SIGUSR2:				\
	statistics.ec_SIGUSR2++;		\
	break;					\
    case EVT_SIGINT:				\
	statistics.ec_SIGINT++;			\
	break;					\
	STATS_EC_S(KEY)				\
	    STATS_EC_S(REFRESH)		\
	    STATS_EC_S(FORCEREFRESH)		\
	    STATS_EC_S(DOUPDATE)		\
	    STATS_EC_S(TERMRESETUP)		\
	    STATS_EC_S(FOCUS_NEXT)		\
	    STATS_EC_S(FOCUS_PREVIOUS)		\
	    STATS_EC_S(WINDOW_SHOW)		\
	    STATS_EC_S(WINDOW_CLOSE)		\
	    STATS_EC_S(BUTTON_PRESS)		\
	    STATS_EC_S(LISTBOX_ENTER)		\
	    STATS_EC_S(QUIT)			\
    default:					\
	    abort();				\
    }


#define STATS_EVT(x) (statistics.evt_##x)
#define STATS_EVT_S(x) case(EVT_##x):		\
    STATS_EVT(x)++;				\
    break;
#define STATS_EVT_OUT(str,stats) statsfile << str << STATS_EVT(stats) << std::endl

// macro because I don't want to clutter the code
#define MAINLOOP_STATS(x) switch (x) {		\
    case EVT_SIGWINCH:				\
	statistics.evt_SIGWINCH++;		\
	break;					\
    case EVT_SIGALRM:				\
	statistics.evt_SIGALRM++;		\
    break;					\
    case EVT_SIGUSR1:				\
	statistics.evt_SIGUSR1++;		\
	break;					\
    case EVT_SIGUSR2:				\
	statistics.evt_SIGUSR2++;		\
	break;					\
    case EVT_SIGINT:				\
	statistics.evt_SIGINT++;		\
	break;					\
	STATS_EVT_S(KEY)			\
	    STATS_EVT_S(REFRESH)		\
	    STATS_EVT_S(FORCEREFRESH)		\
	    STATS_EVT_S(DOUPDATE)		\
	    STATS_EVT_S(TERMRESETUP)		\
	    STATS_EVT_S(FOCUS_NEXT)		\
	    STATS_EVT_S(FOCUS_PREVIOUS)		\
	    STATS_EVT_S(WINDOW_SHOW)		\
	    STATS_EVT_S(WINDOW_CLOSE)		\
	    STATS_EVT_S(BUTTON_PRESS)		\
	    STATS_EVT_S(LISTBOX_ENTER)		\
	    STATS_EVT_S(QUIT)			\
    default:					\
	    abort();				\
    }

struct __statistics {
	// Total events processed
	unsigned long evt_total;
	unsigned long evt_SIGWINCH;
	unsigned long evt_SIGALRM;
	unsigned long evt_SIGUSR1;
	unsigned long evt_SIGUSR2;
	unsigned long evt_SIGINT;
	unsigned long evt_KEY;
	unsigned long evt_REFRESH;
	unsigned long evt_FORCEREFRESH;
	unsigned long evt_DOUPDATE;
	unsigned long evt_TERMRESETUP;
	unsigned long evt_FOCUS_NEXT;
	unsigned long evt_FOCUS_PREVIOUS;
	unsigned long evt_WINDOW_SHOW;
	unsigned long evt_WINDOW_CLOSE;
	unsigned long evt_BUTTON_PRESS;
	unsigned long evt_LISTBOX_ENTER;
	unsigned long evt_QUIT;

	// Event Connectors calls
	unsigned long ec_total;
	unsigned long ec_SIGWINCH;
	unsigned long ec_SIGALRM;
	unsigned long ec_SIGUSR1;
	unsigned long ec_SIGUSR2;
	unsigned long ec_SIGINT;
	unsigned long ec_KEY;
	unsigned long ec_REFRESH;
	unsigned long ec_FORCEREFRESH;
	unsigned long ec_DOUPDATE;
	unsigned long ec_TERMRESETUP;
	unsigned long ec_FOCUS_NEXT;
	unsigned long ec_FOCUS_PREVIOUS;
	unsigned long ec_WINDOW_SHOW;
	unsigned long ec_WINDOW_CLOSE;
	unsigned long ec_BUTTON_PRESS;
	unsigned long ec_LISTBOX_ENTER;
	unsigned long ec_QUIT;
	
	// EventConnector remove requests total processed
	unsigned long ec_rm_total;
	// Cancelled EventConnector removal requests
	unsigned long ec_rm_cancelled;
	// Max size of EventQueue
	unsigned short evq_size_max;
	// Max Size of EventConnector Removal Queue.
	unsigned short ec_rmq_size_max;
};
static __statistics statistics;

sigset_t EventQueue::block_sigmask;
sigset_t EventQueue::tmp_old_sigmask;
sigset_t EventQueue::old_sigmask;
struct sigaction EventQueue::old_winch_act;
struct sigaction EventQueue::old_alrm_act;
struct sigaction EventQueue::old_usr1_act;
struct sigaction EventQueue::old_usr2_act;
struct sigaction EventQueue::old_int_act;
bool EventQueue::signal_blocked = false;

std::queue<Event*> EventQueue::evt_queue;
std::list<EventConnectorBase*> EventQueue::evtconn_rem_request;
std::list<EventConnectorBase*> EventQueue::evtconn_list;

//
// Functors
//

/**
 * Functor for comparing EventConnectors for equality.
 */
class EventConnectorEqual {
    private:
	const EventConnectorBase& __eb;
    public:
	/**
	 * @param _eb EventConnector which others are compared to.
	 */
	EventConnectorEqual(const EventConnectorBase& _eb) : __eb(_eb) {}
	/**
	 * @param eb pointer to EventConnector which is compared to __eb.
	 *
	 * @return @c true if __eb == *eb, @c false otherwise.
	 */
	bool operator()(EventConnectorBase* eb) {
	    assert( eb != NULL );
	    return *eb == __eb;
	}
};

/**
 * Functor setting the suspend state.
 *
 * Functor for setting the suspend state on the given EventConnector
 * if it matches the Event.
 */
class EvtConnSetSuspend {
    private:
	EVENT_TYPE __evt;
	bool __suspend;

    public:
	/**
	 * @param _e Event the EventConnector has to connect in order
	 * to be suspended/unsuspended.
	 *
	 * @param _s suspend state to set: @c true to suspend the
	 * EventConnector, @c false to unsuspend the EventConnector.
	 */
	EvtConnSetSuspend(EVENT_TYPE _e, bool _s):
	    __evt(_e), __suspend(_s) {}
	void operator()(EventConnectorBase* eb) {
	    assert ( eb != NULL );
	    if ( *eb == __evt) {
		if (__suspend)
		    eb->suspended(true);
		else
		    eb->suspended(false);
	    }
	}
};

/**
 * Functor for setting suspend state.
 *
 * Set the suspend/unsuspend state on EventConnectors except for a
 * particular EventConnector.
 */
class EvtConnSetSuspendExcept {
    private:
	const EventConnectorBase& __evt;
	bool __suspend;

    public:
	/**
	 * @param _e all EventConnectors having the same EVENT_TYPE as
	 * this, will have set their suspend state. _e will not be
	 * changed, though.
	 *
	 * @param _s suspend state to set: @c true to suspend the
	 * EventConnector, @c false to unsuspend the EventConnector.
	 */
	EvtConnSetSuspendExcept(const EventConnectorBase& _e,
				    bool _s): __evt(_e),
					      __suspend(_s){}
	void operator()(EventConnectorBase* eb) {
	    assert ( eb != NULL );
	    if (*eb == __evt.type() &&
		! (__evt == *eb) ) {
		if (__suspend)
		    eb->suspended(true);
		else
		    eb->suspended(false);
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
	 * @param eb pointer to EventConnectorBase to be freed.
	 */
	void operator()(EventConnectorBase* eb) {
	    assert( eb != NULL );
	    delete eb;
	}
};

/**
 * Functor for calling EventConnectors for a specific event.
 */
class CallEventConnector {
    private:
	/**
	 * Event which is used to determine the EventConnectors to be
	 * called, and passed to them if called.
	 */
	Event& __eb;

    public:
	/**
	 * @param _eb Event for which EventConnectors are called.
	 */
	CallEventConnector(Event& _eb): __eb(_eb) {}

	/**
	 * Calls the given EventConnector conditionally.
	 *
	 * Calls the given Eventconnector only if it connects to the
	 * same EventType as __eb. __eb will be passed as argument to
	 * the EventConnector call.
	 *
	 * @param _ec EventConnector which will be called
	 * conditionally if it connects to the same EventType as __eb is.
	 */
	void operator()(EventConnectorBase* _ec) {
	    assert(_ec != NULL);
	    if (_ec->type() == __eb.type()) {
		statistics.ec_total++;
		EVENTCONN_STATS(_ec->type());
		_ec->call(__eb);
	    }
	}
};

//
// Private
//

/////////////
// Signals //
/////////////

void
EventQueue::setup_signal() {
    int err;

    struct sigaction sigact;
    sigact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    sigact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    sigact.sa_flags = 0;
#endif // SA_SIGINFO

    //
    // SIGWINCH
    //

    // This handler is always installed, whether or not resize_term()
    // is available. However, depending on availability of
    // resize_term(), the application will be actually resized.
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGALRM);
    sigaddset(&sigact.sa_mask, SIGUSR1);
    sigaddset(&sigact.sa_mask, SIGUSR2);
    sigaddset(&sigact.sa_mask, SIGINT);

    err = sigaction(SIGWINCH, &sigact, &old_winch_act);
    if (err)
	throw SystemError(errno);

    //
    // SIGALRM
    //
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGWINCH);
    sigaddset(&sigact.sa_mask, SIGUSR1);
    sigaddset(&sigact.sa_mask, SIGUSR2);
    sigaddset(&sigact.sa_mask, SIGINT);

    err = sigaction(SIGALRM, &sigact, &old_alrm_act);
    if (err)
	throw SystemError(errno);

    //
    // SIGUSR1
    //
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGWINCH);
    sigaddset(&sigact.sa_mask, SIGALRM);
    sigaddset(&sigact.sa_mask, SIGUSR2);
    sigaddset(&sigact.sa_mask, SIGINT);

    err = sigaction(SIGUSR1, &sigact, &old_usr1_act);
    if (err)
	throw SystemError(errno);

    //
    // SIGUSR2
    //
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGWINCH);
    sigaddset(&sigact.sa_mask, SIGALRM);
    sigaddset(&sigact.sa_mask, SIGUSR1);
    sigaddset(&sigact.sa_mask, SIGINT);

    err = sigaction(SIGUSR2, &sigact, &old_usr2_act);
    if (err)
	throw SystemError(errno);

    //
    // SIGINT
    //
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGWINCH);
    sigaddset(&sigact.sa_mask, SIGALRM);
    sigaddset(&sigact.sa_mask, SIGUSR1);
    sigaddset(&sigact.sa_mask, SIGUSR2);

    err = sigaction(SIGINT, &sigact, &old_int_act);
    if (err)
	throw SystemError(errno);

    //
    // Connect to signals
    //
    sigset_t nset;
    sigemptyset(&nset);

    sigaddset(&nset, SIGWINCH);
    sigaddset(&nset, SIGALRM);
    sigaddset(&nset, SIGUSR1);
    sigaddset(&nset, SIGUSR2);
    sigaddset(&nset, SIGINT);

    err = sigprocmask(SIG_UNBLOCK, &nset, &old_sigmask);
    if (err)
	throw SystemError(errno);
}

void
EventQueue::restore_signal() {
    int err;

    err = sigaction(SIGWINCH, &old_winch_act, NULL);
    if (err)
	throw SystemError(errno);
    err = sigaction(SIGWINCH, &old_alrm_act, NULL);
    if (err)
	throw SystemError(errno);
    err = sigaction(SIGUSR1, &old_usr1_act, NULL);
    if (err)
	throw SystemError(errno);
    err = sigaction(SIGUSR2, &old_usr2_act, NULL);
    if (err)
	throw SystemError(errno);
    err = sigaction(SIGINT, &old_int_act, NULL);
    if (err)
	throw SystemError(errno);

    err = sigprocmask(SIG_SETMASK, &old_sigmask, NULL);
    if (err)
	throw SystemError(errno);
}

void
#ifdef SA_SIGINFO
EventQueue::signal_handler(int signo, siginfo_t* info, void *d)
#else
EventQueue::signal_handler(int signo)
#endif // SA_SIGINFO
{
    int olderrno = errno;

    switch (signo) {
    case SIGALRM:
	try {
	    evt_queue.push(new Event(EVT_SIGALRM));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
    case SIGWINCH:
	try {
	    evt_queue.push(new Event(EVT_TERMRESETUP));
	    evt_queue.push(new EventEx<Size>(EVT_SIGWINCH, Curses::inquiry_screensize()));
	    evt_queue.push(new Event(EVT_REFRESH));
	    evt_queue.push(new Event(EVT_DOUPDATE));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
    case SIGUSR1:
	try {
	    evt_queue.push(new Event(EVT_SIGUSR1));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
    case SIGUSR2:
	try {
	    evt_queue.push(new Event(EVT_SIGUSR2));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
    case SIGINT:
	try {
	    evt_queue.push(new Event(EVT_SIGINT));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
    }

    errno = olderrno;
}

void
EventQueue::blocksignal() {
    if (signal_blocked) return;

    if (sigprocmask(SIG_BLOCK, &block_sigmask, &tmp_old_sigmask))
	throw SystemError(errno);

    signal_blocked = true;
}

void
EventQueue::unblocksignal() {
    if (!signal_blocked) return;

    signal_blocked = false;

    if (sigprocmask(SIG_SETMASK, &tmp_old_sigmask, NULL))
	throw SystemError(errno);
}

/////////////////
// End Signals //
/////////////////

void
EventQueue::proc_rem_request() {
    statistics.ec_rmq_size_max=
	std::max(static_cast<std::queue<Event*>::size_type>(statistics.ec_rmq_size_max),
		 evtconn_rem_request.size());
		 
				     
    std::list<EventConnectorBase*>::iterator it_erq = 
	evtconn_rem_request.begin();

    while(it_erq!=evtconn_rem_request.end()) {
	statistics.ec_rm_total++;
	assert(*it_erq!=NULL);

	EventConnectorBase* ecb = *it_erq;

	std::list<EventConnectorBase*>::iterator it =
	    std::find_if(evtconn_list.begin(),
			 evtconn_list.end(),
			 EventConnectorEqual(*ecb));

	delete ecb;
	it_erq++;

	if (it == evtconn_list.end()) continue;

	assert(*it!=NULL);

	delete *it;
	evtconn_list.erase(it);
    }

    evtconn_rem_request.clear();
}

//
// Public
//
void
EventQueue::connect_event(const EventConnectorBase& ec) {
    // Only one event handler per event per object can be connectd
    std::list<EventConnectorBase*>::iterator it =
	std::find_if(evtconn_list.begin(),
		     evtconn_list.end(),
		     EventConnectorEqual(ec));
    if ( it != evtconn_list.end() ) {
	assert(*it!=NULL);
	// there is already a member function registered for the
	// object on the given event. Replace that connection.
	delete *it;
	*it = ec.clone();
    } else {
	// We have to push_back(). The last registered event connector
	// has to be called last. This is required when:
	//  - Realizing
	//  - Refreshing
	//
	// It ensures proper display of overlapping windows.
	evtconn_list.push_back(ec.clone());
    }

    //
    // Remove any pending Event connector removal requests.
    //
    // In case you wonder why we don't unsuspend, this had already
    // happended above since we overwrite the still existing
    // connector.
    //
    if (evtconn_rem_request.empty()) return;

    std::list<EventConnectorBase*>::iterator it_erq =
	std::find_if(evtconn_rem_request.begin(),
		     evtconn_rem_request.end(),
		     EventConnectorEqual(ec));

    if (it_erq != evtconn_rem_request.end()) {
	statistics.ec_rm_cancelled++;

	assert(*it_erq!=NULL);
	delete *it_erq;
	evtconn_rem_request.erase(it_erq);
    }

}

void
EventQueue::disconnect_event(const EventConnectorBase& ec) {
    // does not delete the connector, but adds it to a queue for later
    // removal. Why? What happens if an EventConnector disconnects
    // itself? std::for_each iterating over evtconn_list in run()
    // would freak out. So we wait until we're sure that nobody is
    // reading the list.
    evtconn_rem_request.push_back(ec.clone());

    // However, the event connector must not be called anymore,
    // because the object might have been destroyed meanwhile.
    std::list<EventConnectorBase*>::iterator it =
	std::find_if(evtconn_list.begin(),
		     evtconn_list.end(),
		     EventConnectorEqual(ec));
    if ( it != evtconn_list.end() ) {
	assert(*it!=NULL);
	(*it)->suspended(true);
    }
}

void
EventQueue::suspend(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>::iterator it =
	std::find_if(evtconn_list.begin(),
		     evtconn_list.end(),
		     EventConnectorEqual(ec));

    if ( it == evtconn_list.end() ) return;

    (*it)->suspended(true);
}

void
EventQueue::suspend_all(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspend(_t, true));
}

void
EventQueue::suspend_except(const EventConnectorBase& ec) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendExcept(ec, true));
}

void
EventQueue::unsuspend(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>::iterator it =
	std::find_if(evtconn_list.begin(),
		     evtconn_list.end(),
		     EventConnectorEqual(ec));

    if ( it == evtconn_list.end() ) return;

    (*it)->suspended(false);
}

void
EventQueue::unsuspend_all(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspend(_t, false));
}

void
EventQueue::unsuspend_except(const EventConnectorBase& ec) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendExcept(ec, false));
}

void
EventQueue::submit(EVENT_TYPE _et) {
    submit(Event(_et));
}

void
EventQueue::submit(const Event& ev) {
    blocksignal();
    try {
	evt_queue.push(ev.clone());
    } catch(std::exception& e) {
	// Intentionally empty
#ifndef NDEBUG
	std::cerr << e.what() << std::endl;
	std::abort();
#endif
    }
    unblocksignal();
}

void
EventQueue::run() {
    sigemptyset(&block_sigmask);
    sigaddset(&block_sigmask, SIGWINCH);
    sigaddset(&block_sigmask, SIGALRM);
    sigaddset(&block_sigmask, SIGUSR1);
    sigaddset(&block_sigmask, SIGUSR2);
    sigaddset(&block_sigmask, SIGINT);

    setup_signal();

    memset(&statistics, 0, sizeof(statistics));

    while(true) {
	// This is to move the cursor to the focused widget. Before
	// adding that call tests/focus1 had the cursor always left on
	// the status line. It's sorta hack, but it works...
	FocusManager::refocus();

	int c=wgetch(stdscr);

	blocksignal();

	if (c != ERR)
	    switch (c) {
	    case KEY_REFRESH:
	    case KEY_CTRL_L:
		submit(Event(EVT_FORCEREFRESH));
		submit(Event(EVT_REFRESH));
		submit(Event(EVT_DOUPDATE));
		break;
	    default:
		submit(EventEx<int>(EVT_KEY, c));
	    }
	// process any pending EventConnector removal requests
	proc_rem_request();


	while(!evt_queue.empty()) {
	    statistics.evq_size_max=
		std::max(static_cast<std::queue<Event*>::size_type>(statistics.evq_size_max),evt_queue.size());
	    statistics.evt_total++;

	    // listsize_max=max(evt_queue.size(),listsize_max)
	    // iterations_eventproc++;
	    Event* evt = evt_queue.front();
	    assert(evt != NULL);
	    MAINLOOP_STATS(evt->type());

	    if (evt->type() == EVT_QUIT) {
		unblocksignal();
		goto QUIT;
	    }

	    std::for_each(evtconn_list.begin(),
			  evtconn_list.end(),
			  CallEventConnector(*evt));
	    delete evt;
	    evt_queue.pop();
	}

	// process any pending EventConnector removal requests
	proc_rem_request();

	unblocksignal();

    }
QUIT:
    restore_signal();

    cleanup();
}

void
EventQueue::cleanup() {
   /* Cleanup event queue */
    while(!evt_queue.empty()) {
	delete evt_queue.front();
	evt_queue.pop();
    }

    // Remove any pending removal request, so that the memory will be
    // freed properly
    proc_rem_request();

    // Free the memory occupied by remaining connectors
    std::list<EventConnectorBase*>::iterator it=evtconn_list.begin();
    while(it != evtconn_list.end()) {
	assert(*it != NULL);
	delete *it++;
    }

    evtconn_list.clear();

    char* stats_fn;
    if ((stats_fn = getenv("LIBYACURS_EVT_STATS"))!=NULL) {
	try {
	    std::ofstream statsfile;
	    statsfile.open(stats_fn, std::ios::out | std::ios::trunc);
	    statsfile << "EventQueue Statistics" << std::endl;
	    statsfile << "=====================" << std::endl;
	    statsfile << std::endl;
	    statsfile << "Events Processed               : " << statistics.evt_total << std::endl;
	    statsfile << "Event Connector Calls          : " << statistics.ec_total << std::endl;
	    statsfile << "Max Queue Size                 : " << statistics.evq_size_max << std::endl;
	    statsfile << "EventConnector Removal Requests: " << statistics.ec_rm_total << std::endl;
	    statsfile << "EvtConn Removal Requests Cancel: " << statistics.ec_rm_cancelled << std::endl;
	    statsfile << "Max EventConnecor Rem Queue Sz : " << statistics.ec_rmq_size_max << std::endl;
	    statsfile << "Event SIGWINCH                 : " << statistics.evt_SIGWINCH << std::endl;
	    statsfile << "Event SIGALRM                  : " << statistics.evt_SIGALRM << std::endl;
	    statsfile << "Event SIGUSR1                  : " << statistics.evt_SIGUSR1 << std::endl;
	    statsfile << "Event SIGUSR2                  : " << statistics.evt_SIGUSR2 << std::endl;
	    statsfile << "Event SIGINT                   : " << statistics.evt_SIGINT << std::endl;
	    // Events Statistics
	    STATS_EVT_OUT("Event KEY                      : ", KEY);
	    STATS_EVT_OUT("Event REFRESH                  : ", REFRESH);
	    STATS_EVT_OUT("Event FORCEREFRESH             : ", FORCEREFRESH);
	    STATS_EVT_OUT("Event DOUPDATE                 : ", DOUPDATE);
	    STATS_EVT_OUT("Event TERMRESETUP              : ", TERMRESETUP);
	    STATS_EVT_OUT("Event FOCUS_NEXT               : ", FOCUS_NEXT);
	    STATS_EVT_OUT("Event FOCUS_PREVIOUS           : ", FOCUS_PREVIOUS);
	    STATS_EVT_OUT("Event WINDOW_SHOW              : ", WINDOW_SHOW);
	    STATS_EVT_OUT("Event WINDOW_CLOSE             : ", WINDOW_CLOSE);
	    STATS_EVT_OUT("Event BUTTON_PRESS             : ", BUTTON_PRESS);
	    STATS_EVT_OUT("Event LISTBOX_ENTER            : ", LISTBOX_ENTER);
	    STATS_EVT_OUT("Event QUIT                     : ", QUIT);
	    // EventConnector Statistics
	    STATS_EC_OUT("EventConnector KEY             : ", KEY);
	    STATS_EC_OUT("EventConnector REFRESH         : ", REFRESH);
	    STATS_EC_OUT("EventConnector FORCEREFRESH    : ", FORCEREFRESH);
	    STATS_EC_OUT("EventConnector DOUPDATE        : ", DOUPDATE);
	    STATS_EC_OUT("EventConnector TERMRESETUP     : ", TERMRESETUP);
	    STATS_EC_OUT("EventConnector FOCUS_NEXT      : ", FOCUS_NEXT);
	    STATS_EC_OUT("EventConnector FOCUS_PREVIOUS  : ", FOCUS_PREVIOUS);
	    STATS_EC_OUT("EventConnector WINDOW_SHOW     : ", WINDOW_SHOW);
	    STATS_EC_OUT("EventConnector WINDOW_CLOSE    : ", WINDOW_CLOSE);
	    STATS_EC_OUT("EventConnector BUTTON_PRESS    : ", BUTTON_PRESS);
	    STATS_EC_OUT("EventConnector LISTBOX_ENTER   : ", LISTBOX_ENTER);
	    STATS_EC_OUT("EventConnector QUIT            : ", QUIT);

	    statsfile.close();
	} catch (...) {}
    }
	
}
