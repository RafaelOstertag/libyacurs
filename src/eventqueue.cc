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

#if defined(SIGWINCH) || defined(SIGALRM) || \
    defined(SIGUSR1) || defined(SIGUSR2)
sigset_t EventQueue::block_sigmask;
sigset_t EventQueue::tmp_old_sigmask;
sigset_t EventQueue::old_sigmask;
struct sigaction EventQueue::old_winch_act;
struct sigaction EventQueue::old_alrm_act;
struct sigaction EventQueue::old_usr1_act;
struct sigaction EventQueue::old_usr2_act;		
#endif // defined(SIGWINCH) || defined(SIGALRM)
bool EventQueue::signal_blocked = false;

std::queue<EventBase*> EventQueue::evt_queue;
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
	EventBase& __eb;

    public:
	inline CallEventConnector(EventBase& _eb): __eb(_eb) {}
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
EventQueue::setupSignal() {
    int err;

    //
    // Window Size Change handler
    //
#ifdef SIGWINCH
    struct sigaction winchact;

    winchact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    winchact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    winchact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&winchact.sa_mask);

    err = sigaction(SIGWINCH, &winchact, &old_winch_act);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH

    //
    // Alarm handler
    //
#ifdef SIGALRM
    struct sigaction alrmact;

    alrmact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    alrmact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    alrmact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&alrmact.sa_mask);

    err = sigaction(SIGALRM, &alrmact, &old_alrm_act);
    if (err)
	throw SystemError(errno);
#endif // SIGALRM

    //
    // Alarm handler
    //
#ifdef SIGUSR1
    struct sigaction usr1act;

    usr1act.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    usr1act.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    usr1act.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&usr1act.sa_mask);

    err = sigaction(SIGUSR1, &usr1act, &old_usr1_act);
    if (err)
	throw SystemError(errno);
#endif // SIGUSR1

#ifdef SIGUSR2
    struct sigaction usr2act;

    usr2act.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    usr2act.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    usr2act.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&usr2act.sa_mask);

    err = sigaction(SIGUSR2, &usr2act, &old_usr2_act);
    if (err)
	throw SystemError(errno);
#endif // SIGUSR2

    sigset_t nset;
    sigemptyset(&nset);

#ifdef SIGWINCH
    sigaddset(&nset, SIGWINCH);
#endif // SIGWINCH
#ifdef SIGALRM
    sigaddset(&nset, SIGALRM);
#endif // SIGALRM
#ifdef SIGUSR1
    sigaddset(&nset, SIGUSR1);
#endif // SIGUSR1
#ifdef SIGUSR2
    sigaddset(&nset, SIGUSR2);
#endif // SIGUSR1


#if defined(SIGWINCH) || defined(SIGALRM) ||\
    defined(SIGUSR1) || defined(SIGUSR2)
    err = sigprocmask(SIG_UNBLOCK, &nset, &old_sigmask);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)

}

void
EventQueue::restoreSignal() {
    int err;

#ifdef SIGWINCH
    err = sigaction(SIGWINCH, &old_winch_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH
#ifdef SIGALRM
    err = sigaction(SIGWINCH, &old_alrm_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGALRM
#ifdef SIGUSR1
    err = sigaction(SIGUSR1, &old_usr1_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGUSR1
#ifdef SIGUSR2
    err = sigaction(SIGUSR2, &old_usr2_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGUSR2

#if defined(SIGWINCH) || defined(SIGALRM) ||\
    defined(SIGUSR1) || defined(SIGUSR2)
    err = sigprocmask(SIG_SETMASK, &old_sigmask, NULL);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)
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
#ifdef SIGALRM
    case SIGALRM:
	try {
	    evt_queue.push(new EventBase(EVT_ALARM));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif // SIGALRM
#ifdef SIGWINCH
    case SIGWINCH:
	try {
	    evt_queue.push(new EventWinCh(Curses::inquiryScreenSize()));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif // SIGWINCH
#ifdef SIGUSR1
    case SIGUSR1:
	try {
	    evt_queue.push(new EventBase(EVT_USR1));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif // SIGUSR1
#ifdef SIGUSR2
    case SIGUSR2:
	try {
	    evt_queue.push(new EventBase(EVT_USR2));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif // SIGUSR2
    }

    errno = olderrno;
}

void
EventQueue::blocksignal() {
#if defined(SIGWINCH) || defined(SIGALRM) ||\
    defined(SIGUSR1) || defined(SIGUSR2)
    if (signal_blocked) return;

    int err = sigprocmask(SIG_BLOCK, &block_sigmask, &tmp_old_sigmask);
    if (err)
	throw SystemError(errno);
    signal_blocked = true;
#endif // defined(SIGWINCH) || defined(SIGALRM)
}

void
EventQueue::unblocksignal() {
#if defined(SIGWINCH) || defined(SIGALRM) ||\
    defined(SIGUSR1) || defined(SIGUSR2)
    if (!signal_blocked) return;
    signal_blocked = false;

    int err = sigprocmask(SIG_SETMASK, &tmp_old_sigmask, NULL);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)
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
EventQueue::connectEvent(const EventConnectorBase& ec) {
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
EventQueue::disconnectEvent(const EventConnectorBase& ec) {
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
EventQueue::suspendAll(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendAll(_t, true));
}

void
EventQueue::suspendExcept(const EventConnectorBase& ec) {
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
EventQueue::unsuspendAll(EVENT_TYPE _t) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendAll(_t, false));
}

void
EventQueue::unsuspendExcept(const EventConnectorBase& ec) {
    std::for_each(evtconn_list.begin(),
		  evtconn_list.end(),
		  EvtConnSetSuspendExcept(ec, false));
}

void
EventQueue::inject(const EventBase& ev) {
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
#ifdef SIGWINCH
    sigaddset(&block_sigmask, SIGWINCH);
#endif // SIGWINCH
#ifdef SIGALRM
    sigaddset(&block_sigmask, SIGALRM);
#endif // SIGALRM

    setupSignal();

    while(true) {
	int c=getch();
	if (c != ERR)
	    inject(EventKey(c));

	blocksignal();

	// process any pending EventConnector removal requests
	proc_rem_request();

	while(!evt_queue.empty()) {

	    EventBase* evt = evt_queue.front();
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
    restoreSignal();

    std::list<EventConnectorBase*>::iterator it=evtconn_list.begin();
    while(it != evtconn_list.end()) {
	assert(*it != NULL);
	delete *it++;
    }

    evtconn_list.clear();
}
