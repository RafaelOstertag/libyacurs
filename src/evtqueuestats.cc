// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <queue>

#include "evtqueuestats.h"

//
// Private
//

//
// Protected
//

//
// Public
//

EventQueueStats::EventQueueStats() {
    clear();
}

void
EventQueueStats::clear()  {
    evq_max_proc_time=
	evt_max_proc_time=
	evt_submitted=
	evt_pending_cleanup=
	evt_proc_total=
	ec_max=
	ec_call_max_time=
	ec_calls_total=
	ec_rm_total=
	ec_rm_cancelled=
	evq_size_max=
	ec_rmq_size_max=0;

    evq_min_proc_time=
	evt_min_proc_time=
	ec_min=
	ec_call_min_time=(unsigned int)-1;

    evt_submitted_by_type.clear();
    evt_proc_by_type.clear();
    ec_call_by_type.clear();
    ec_max_by_type.clear();
}

void
EventQueueStats::dump(std::ostream& _os) const {
#define PRINTSTATS(t,v) _os.width(40);\
    _os.fill('.');							\
    _os.setf(std::ios_base::left,std::ios_base::adjustfield);		\
    _os << t;						\
    _os.width(6);							\
    _os.fill(' ');							\
    _os.setf(std::ios_base::right,std::ios_base::adjustfield);		\
    _os << v << std::endl

    _os << "EventQueue Statistics" << std::endl;
    _os << "=====================" << std::endl;
    _os << std::endl;


    PRINTSTATS("Events submitted",evt_submitted);
    PRINTSTATS("Events processed",evt_proc_total);
    PRINTSTATS("Events pending in cleanup",evt_pending_cleanup);
    PRINTSTATS("Max Queue Size",evq_size_max);
    PRINTSTATS("EventConnector Removal Requests",ec_rm_total);
    PRINTSTATS("EvtConn Removal Requests Cancel",ec_rm_cancelled);
    PRINTSTATS("Max EventConnecor Rem Queue Size",ec_rmq_size_max);
    PRINTSTATS("Event Connector Calls",ec_calls_total);
    PRINTSTATS("EvtConn List max size",ec_max);
    PRINTSTATS("EvtConn List min size",ec_min);

    PRINTSTATS("EventQueue process time max",evq_max_proc_time);
    PRINTSTATS("EventQueue process time min",evq_min_proc_time);
    PRINTSTATS("Event Connector time spent max",ec_call_max_time);
    PRINTSTATS("Event Connector time spent min",ec_call_min_time);
    PRINTSTATS("Event Process time max", evt_max_proc_time);
    PRINTSTATS("Event Process time min", evt_min_proc_time);

    _os << std::endl << std::endl;
    _os << "Event Submitted by Type" << std::endl;
    _os << "-----------------------" << std::endl;
    _os << std::endl;

    std::map<EVENT_TYPE, unsigned int>::const_iterator map_it =
	evt_submitted_by_type.begin();

    uint32_t total=0;
    while (map_it!=evt_submitted_by_type.end()) {
	PRINTSTATS(Event::evt2str(map_it->first),map_it->second);
	total+=map_it->second;
	map_it++;
    }
    PRINTSTATS("Total",total);

    _os << std::endl << std::endl;
    _os << "Event Processed by Type" << std::endl;
    _os << "-----------------------" << std::endl;
    _os << std::endl;

    map_it = evt_proc_by_type.begin();

    total=0;
    while (map_it!=evt_proc_by_type.end()) {
	PRINTSTATS(Event::evt2str(map_it->first),map_it->second);
	total+=map_it->second;
	map_it++;
    }
    PRINTSTATS("Total",total);

    _os << std::endl << std::endl;
    _os << "Event Connector Calls by Type" << std::endl;
    _os << "-----------------------------" << std::endl;
    _os << std::endl;

    map_it = ec_call_by_type.begin();

    total=0;
    while (map_it!=ec_call_by_type.end()) {
	PRINTSTATS(Event::evt2str(map_it->first),map_it->second);
	total+=map_it->second;
	map_it++;
    }
    PRINTSTATS("Total",total);

    _os << std::endl << std::endl;
    _os << "Max Event Connector by Type" << std::endl;
    _os << "---------------------------" << std::endl;
    _os << std::endl;

    map_it = ec_max_by_type.begin();

    total=0;
    while (map_it!=ec_max_by_type.end()) {
	PRINTSTATS(Event::evt2str(map_it->first),map_it->second);
	total+=map_it->second;
	map_it++;
    }
    PRINTSTATS("Total",total);

}
