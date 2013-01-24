// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#ifdef HAVE_CERRNO
#include <cerrno>
#endif // HAVE_CERRNO_H

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#ifdef HAVE_ALGORITHM
#include <algorithm>
#endif // HAVE_ALGORITHM

#include "cursex.h"
#include "eventqueue.h"
#include "curs.h"

sigset_t EventQueue::block_sigmask;
sigset_t EventQueue::tmp_old_sigmask;
sigset_t EventQueue::old_sigmask;
struct sigaction EventQueue::old_winch_act;
struct sigaction EventQueue::old_alrm_act;
struct sigaction EventQueue::old_usr1_act;
struct sigaction EventQueue::old_usr2_act;		
bool EventQueue::signal_blocked = false;

std::queue<Event*> EventQueue::evt_queue;
std::queue<EventConnectorBase*> EventQueue::evtconn_rem_request;
std::list<EventConnectorBase*> EventQueue::evtconn_list;

//
// Functors
//

class EventConnectorEqual {
    private:
	const EventConnectorBase& __eb;
    public:
	inline EventConnectorEqual(const EventConnectorBase& _eb) : __eb(_eb) {}
	inline bool operator()(EventConnectorBase* eb) {
	    assert( eb != NULL );
	    return *eb == __eb;
	}
};

class EvtConnSetSuspendAll {
    private:
	EVENT_TYPE __evt;
	bool suspend;

    public:
	inline EvtConnSetSuspendAll(EVENT_TYPE _e, bool _s):
	    __evt(_e), suspend(_s) {}
	inline void operator()(EventConnectorBase* eb) {
	    assert ( eb != NULL );
	    if ( *eb == __evt) {
		if (suspend)
		    eb->suspend();
		else
		    eb->unsuspend();
	    }
	}
};

class EvtConnSetSuspendExcept {
    private:
	const EventConnectorBase& __evt;
	bool suspend;

    public:
	inline EvtConnSetSuspendExcept(const EventConnectorBase& _e,
				    bool _s): __evt(_e),
					      suspend(_s){}
	inline void operator()(EventConnectorBase* eb) {
	    assert ( eb != NULL );
	    if (*eb == __evt.type() &&
		! (__evt == *eb) ) {
		if (suspend)
		    eb->suspend();
		else
		    eb->unsuspend();
	    }
	}
};

class DestroyEventConnector {
    public:
	inline void operator()(EventConnectorBase* eb) {
	    assert( eb != NULL );
	    delete eb;
	}
};

class CallEventConnector {
    private:
	Event& __eb;

    public:
	inline CallEventConnector(Event& _eb): __eb(_eb) {}
	inline void operator()(EventConnectorBase* _ec) {
	    assert(_ec != NULL);
	    if (_ec->type() == __eb.type()) {
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

    //
    // Window Size Change handler
    //
    // This handler is always installed, whether or not resize_term()
    // is available. However, depending on availability of
    // resize_term(), the application will be actually resized.
    struct sigaction winchact;
    winchact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    winchact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    winchact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&winchact.sa_mask);
    sigaddset(&winchact.sa_mask, SIGALRM);
    sigaddset(&winchact.sa_mask, SIGUSR1);
    sigaddset(&winchact.sa_mask, SIGUSR2);

    err = sigaction(SIGWINCH, &winchact, &old_winch_act);
    if (err)
	throw SystemError(errno);

    //
    // Alarm handler
    //
    struct sigaction alrmact;
    alrmact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    alrmact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    alrmact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&alrmact.sa_mask);
    sigaddset(&alrmact.sa_mask, SIGWINCH);
    sigaddset(&alrmact.sa_mask, SIGUSR1);
    sigaddset(&alrmact.sa_mask, SIGUSR2);

    err = sigaction(SIGALRM, &alrmact, &old_alrm_act);
    if (err)
	throw SystemError(errno);

    //
    // Usr1 handler
    //
    struct sigaction usr1act;
    usr1act.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    usr1act.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    usr1act.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&usr1act.sa_mask);
    sigaddset(&usr1act.sa_mask, SIGWINCH);
    sigaddset(&usr1act.sa_mask, SIGALRM);
    sigaddset(&usr1act.sa_mask, SIGUSR2);

    err = sigaction(SIGUSR1, &usr1act, &old_usr1_act);
    if (err)
	throw SystemError(errno);


    struct sigaction usr2act;
    usr2act.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    usr2act.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    usr2act.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&usr2act.sa_mask);
    sigaddset(&usr2act.sa_mask, SIGWINCH);
    sigaddset(&usr2act.sa_mask, SIGALRM);
    sigaddset(&usr2act.sa_mask, SIGUSR1);

    err = sigaction(SIGUSR2, &usr2act, &old_usr2_act);
    if (err)
	throw SystemError(errno);

    sigset_t nset;
    sigemptyset(&nset);

    sigaddset(&nset, SIGWINCH);
    sigaddset(&nset, SIGALRM);
    sigaddset(&nset, SIGUSR1);
    sigaddset(&nset, SIGUSR2);

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
	    evt_queue.push(new Event(EVT_ALARM));
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
	    evt_queue.push(new EventWinCh(Curses::inquiry_screensize()));
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
	    evt_queue.push(new Event(EVT_USR1));
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
	    evt_queue.push(new Event(EVT_USR2));
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
    while(!evtconn_rem_request.empty()) {

	EventConnectorBase* ecb = evtconn_rem_request.front();

	std::list<EventConnectorBase*>::iterator it =
	    std::find_if(evtconn_list.begin(),
			 evtconn_list.end(),
			 EventConnectorEqual(*ecb));

	delete ecb;
	evtconn_rem_request.pop();

	if (it == evtconn_list.end()) continue;

	delete *it;
	evtconn_list.erase(it);
    }
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
	// there is already a member function registered for the
	// object on the given event. Replace that connection.
	delete *it;
	*it = ec.clone();
    } else {
	evtconn_list.push_front(ec.clone());
    }
}

void
EventQueue::disconnect_event(const EventConnectorBase& ec) {
    // does not delete the connector, but adds it to a queue for later
    // removal
    evtconn_rem_request.push(ec.clone());
}

void
EventQueue::suspend(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>::iterator it =
	std::find_if(evtconn_list.begin(),
		     evtconn_list.end(),
		     EventConnectorEqual(ec));

    if ( it == evtconn_list.end() ) return;

    (*it)->suspend();
}

void
EventQueue::suspend_all(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendAll(_t, true));
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

    (*it)->unsuspend();
}

void
EventQueue::unsuspend_all(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendAll(_t, false));
}

void
EventQueue::unsuspend_except(const EventConnectorBase& ec) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendExcept(ec, false));
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

    setup_signal();

    while(true) {
	int c=getch();
	if (c != ERR)
	    submit(EventKey(c));

	blocksignal();

	// process any pending EventConnector removal requests
	proc_rem_request();

	while(!evt_queue.empty()) {

	    Event* evt = evt_queue.front();
	    assert(evt != NULL);

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
    blocksignal();
    /* Cleanup event queue */
    while(!evt_queue.empty()) {
	delete evt_queue.front();
	evt_queue.pop();
    }
    unblocksignal();

    restore_signal();

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
}
