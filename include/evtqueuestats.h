// -*- mode: c++ -*-
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

#ifndef EVTQUEUESTATS_H
#define EVTQUEUESTATS_H

#include <stdint.h>

#include <map>
#include <list>

#include "event.h"
#include "eventconnector.h"

namespace YACURS {
    namespace INTERNAL {
        /**
         * Class maintaining EventQueue Statistics.
         */
        class EventQueueStats {
            private:
                /// Max time used for one full Event queue Iteration
                clock_t evq_max_proc_time;

                /// Min time used for one full Event queue Iteration
                clock_t evq_min_proc_time;

                /// Max time used for processessing an Event
                clock_t evt_max_proc_time;

                /// Min time used for processessing an Event
                clock_t evt_min_proc_time;

                /// Max time spent in EventConnector call
                clock_t ec_call_max_time;

                /// Min time spent in EventConnector call
                clock_t ec_call_min_time;

                /// Number of Events submitted during run time of queue
                uint32_t evt_submitted;

                /// Number of Events submitted during run time of
                /// queue by type
                std::map<EVENT_TYPE, uint32_t> evt_submitted_by_type;

                /// Events that were pending during cleanup()
                uint32_t evt_pending_cleanup;

                /// Total events processed
                uint32_t evt_proc_total;

                /// Events processed by type
                std::map<EVENT_TYPE, uint32_t> evt_proc_by_type;

                /// Max Events connected
                uint32_t ec_max;

                /// Min Events connected
                uint32_t ec_min;

                /// Events connected by type
                std::map<EVENT_TYPE, uint32_t> ec_max_by_type;

                /// Total Event Connectors calls
                uint32_t ec_calls_total;

                /// Event connector calls by type
                std::map<EVENT_TYPE, uint32_t> ec_call_by_type;

                /// Total EventConnector remove requests processed
                uint32_t ec_rm_total;

                /// Number of Cancelled EventConnector removal requests
                uint32_t ec_rm_cancelled;

                /// Max size of EventQueue
                uint16_t evq_size_max;

                /// Max Size of EventConnector Removal Queue.
                uint16_t ec_rmq_size_max;

            public:
                EventQueueStats();

                void clear();

                void update_ec_calls_by_type(EVENT_TYPE _t);

                void update_ec_call_time(clock_t t0, clock_t t1);

                void update_ec_rmq_size_max(uint16_t v);

                void update_ec_rm_total();

                void update_ec_rm_cancelled();

                void update_evt_submitted_by_type(EVENT_TYPE _t);

                void update_ec_count(const std::map<EVENT_TYPE,
                                                    std::list<EventConnectorBase*> >& _ec_map);

                void update_evq_size_max(uint16_t v);

                void update_evt_proc_by_type(EVENT_TYPE _t);

                void update_evt_pending_cleanup();

                void update_evt_proc_time(clock_t t0, clock_t t1);

                void update_evq_proc_time(clock_t t0, clock_t t1);

                void dump(std::ostream& _os) const;
        };

        inline void
        EventQueueStats::update_ec_calls_by_type(EVENT_TYPE _t) {
            ec_calls_total++;
            ec_call_by_type[_t]++;
        }

        inline void
        EventQueueStats::update_ec_call_time(clock_t t0, clock_t t1) {
            ec_call_max_time = std::max(ec_call_max_time, t1 - t0);
            ec_call_min_time = std::min(ec_call_min_time, t1 - t0);
        }

        inline void
        EventQueueStats::update_ec_rmq_size_max(uint16_t v) {
            ec_rmq_size_max = std::max(ec_rmq_size_max, v);
        }

        inline void
        EventQueueStats::update_ec_rm_total() {
            ec_rm_total++;
        }

        inline void
        EventQueueStats::update_ec_rm_cancelled() {
            ec_rm_cancelled++;
        }

        inline void
        EventQueueStats::update_evt_submitted_by_type(EVENT_TYPE _t) {
            evt_submitted++;
            evt_submitted_by_type[_t]++;
        }

        inline void
        EventQueueStats::update_ec_count(const std::map<EVENT_TYPE,
                                                        std::list<EventConnectorBase*> >& _ec_map)
        {
            std::map<EVENT_TYPE,
                     std::list<EventConnectorBase*> >::const_iterator map_it =
                _ec_map.begin();

            uint32_t overall = 0;

            while (map_it != _ec_map.end() ) {
                overall += map_it->second.size();
                ec_max_by_type[map_it->first] =
                    std::max(static_cast<std::list<EventConnectorBase*>::
                                         size_type>(ec_max_by_type[map_it->
                                                                   first]),
                             map_it->second.size() );
                map_it++;
            }
            ec_max = std::max(ec_max, overall);
            ec_min = std::min(ec_min, overall);
        }

        inline void
        EventQueueStats::update_evq_size_max(uint16_t v) {
            evq_size_max = std::max(evq_size_max, v);
        }

        inline void
        EventQueueStats::update_evt_proc_by_type(EVENT_TYPE _t) {
            evt_proc_total++;
            evt_proc_by_type[_t]++;
        }

        inline void
        EventQueueStats::update_evt_pending_cleanup() {
            evt_pending_cleanup++;
        }

        inline void
        EventQueueStats::update_evt_proc_time(clock_t t0, clock_t t1) {
            evt_max_proc_time = std::max(evt_max_proc_time, t1 - t0);
            evt_min_proc_time = std::min(evt_min_proc_time, t1 - t0);
        }

        inline void
        EventQueueStats::update_evq_proc_time(clock_t t0, clock_t t1) {
            evq_max_proc_time = std::max(evq_max_proc_time, t1 - t0);
            evq_min_proc_time = std::min(evq_min_proc_time, t1 - t0);
        }
    } // namespace INTERNAL
} // namespace YACURS

#endif // EVTQUEUESTATS_H
