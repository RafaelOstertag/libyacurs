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

#include <cassert>
#include <fstream>

#include "focusmanager.h"
#include "yacursex.h"
#include "eventqueue.h"

using namespace YACURS;

#if !defined(NDEBUG) && defined(FOCUSDEBUG)
#define DEBUGOUT(x) try {                                                       \
        char* __debugfile_name__;                                       \
        if ( (__debugfile_name__ = std::getenv("LIBYACURS_FOCUSMGR_DBGFN") ) != \
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

static std::ofstream __debugfile;

const FocusManager::fgid_t FocusManager::nfgid = (FocusManager::fgid_t)-1;
FocusManager::fgid_t FocusManager::__active_focusgroup = FocusManager::nfgid;
std::vector<FocusGroup*> FocusManager::__focus_groups;

//
// Private
//
void
FocusManager::focus_change_handler(Event& _e) {
    assert(_e == EVT_FOCUS_NEXT || _e == EVT_FOCUS_PREVIOUS);

    if (__focus_groups.empty() ) return;
    assert(__active_focusgroup < __focus_groups.size() );
    assert(__focus_groups[__active_focusgroup] != 0);
    assert(__focus_groups[__active_focusgroup]->active() );

    if (_e.type() == EVT_FOCUS_NEXT) {
        __focus_groups[__active_focusgroup]->focus_next();
	DEBUGOUT("focus_next() on " << __active_focusgroup);
    } else if (_e.type() == EVT_FOCUS_PREVIOUS) {
        __focus_groups[__active_focusgroup]->focus_previous();
	DEBUGOUT("focus_previous() on " << __active_focusgroup);
    } else {
        throw EXCEPTIONS::UnexpectedEvent();
    }
}

//
// Protected
//

//
// Public
//

void
FocusManager::init() {
    DEBUGOUT("Initialize Focus Manager");
    EventQueue::connect_event(EventConnectorFunction1(EVT_FOCUS_NEXT,
                                                      FocusManager::
                                                      focus_change_handler) );
    EventQueue::connect_event(EventConnectorFunction1(EVT_FOCUS_PREVIOUS,
                                                      FocusManager::
                                                      focus_change_handler) );
}

void
FocusManager::uninit() {
    DEBUGOUT("Uninitialize Focus Manager");
    EventQueue::disconnect_event(EventConnectorFunction1(EVT_FOCUS_NEXT,
                                                         FocusManager::
                                                         focus_change_handler) );
    EventQueue::disconnect_event(EventConnectorFunction1(EVT_FOCUS_PREVIOUS,
                                                         FocusManager::
                                                         focus_change_handler) );

    for (fgid_t i = 0; i < __focus_groups.size(); i++)
        if (__focus_groups[i] != 0) {
            delete __focus_groups[i];
            __focus_groups[i] = 0;
        }
}

FocusManager::fgid_t
FocusManager::new_focus_group() {
    fgid_t _id = FocusManager::nfgid;

    if (__focus_groups.empty() ) {
        _id = __focus_groups.size(); // == 0
        __focus_groups.push_back(new FocusGroup);
	DEBUGOUT("New Focus Group (first) " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
    } else {
        // Search for a free slot in the vector
        for (fgid_t i = 0; i < __focus_groups.size(); i++) {
            if (__focus_groups[i] == 0) {
                // found free slot
                _id = i;
                __focus_groups[i] = new FocusGroup;
		DEBUGOUT("New Focus Group (reuse slot) " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
                break;
            }
        }

        // Check if we have found a free slot, i.e. _id has been set
        if (_id == FocusManager::nfgid) {
            // No, no free slot, so create new slot.
            _id = __focus_groups.size();
            __focus_groups.push_back(new FocusGroup);
	    DEBUGOUT("New Focus Group (new slot) " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
        }
    }

    assert(_id != FocusManager::nfgid);
    assert(_id < __focus_groups.size() );
    return _id;
}

void
FocusManager::destroy_focus_group(fgid_t _id) {
    assert(!__focus_groups.empty() );
    assert(_id < __focus_groups.size() );

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (__focus_groups[_id] == 0)
        return;

    // Destructor of FocusGroup is supposed to take care of removing
    // focus, so we simply destroy the group.
    delete __focus_groups[_id];
    __focus_groups[_id] = 0;
    DEBUGOUT("Destroy Focus Group " << (void*)(__focus_groups[_id]) << " with ID: " << _id);

    if (__active_focusgroup == _id)
        __active_focusgroup = FocusManager::nfgid;
}

void
FocusManager::focus_group_add(fgid_t _id, WidgetBase* _w) {
    assert(_w != 0);
    assert(!__focus_groups.empty() );
    assert(_id < __focus_groups.size() );
    assert(__focus_groups[_id] != 0);

    __focus_groups[_id]->add(_w);

    DEBUGOUT("Add widget " << (void*)(_w) << " to Focus Group " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
}

void
FocusManager::focus_group_remove(fgid_t _id, WidgetBase* _w) {
    assert(_w != 0);
    assert(_id != FocusManager::nfgid);

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (__focus_groups[_id] == 0)
        return;

    __focus_groups[_id]->remove(_w);

    DEBUGOUT("Remove widget " << (void*)(_w) << " to Focus Group " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
}

void
FocusManager::focus_group_activate(fgid_t _id) {
    assert(!__focus_groups.empty() );
    assert(_id < __focus_groups.size() );
    assert(__focus_groups[_id] != 0);
    // We cannot assert this here, because fgid_t is probably of
    // unsigned type and initially set to -1, so we cannot get past
    // this assertion, since the first call which would make
    // subsequent calls pass, always fails.
    //
    //assert(__active_focusgroup<__focus_groups.size());

    // Deactivate the current Active Focus Group, if any.
    if (__active_focusgroup != FocusManager::nfgid && // not initialized yet
        __focus_groups[__active_focusgroup] !=
        0 /* Focus Group destroyed */) {
        assert(__active_focusgroup < __focus_groups.size() );
        __focus_groups[__active_focusgroup]->deactivate();
	DEBUGOUT("Deactivated Focus Group " << (void*)(__focus_groups[__active_focusgroup]) << " with ID: " << __active_focusgroup);
    }

    __focus_groups[_id]->activate();
    __active_focusgroup = _id;
    DEBUGOUT("Activated Focus Group " << (void*)(__focus_groups[_id]) << " with ID: " << _id);
}

void
FocusManager::refocus() {
    if (__focus_groups.empty() ) return;

    assert(__active_focusgroup != FocusManager::nfgid);
    assert(__focus_groups[__active_focusgroup] != 0);
    // This would make many tests fail. Also, it should be no problem
    // if we don't assert.
    //
    //assert(!__focus_groups.empty());

    __focus_groups[__active_focusgroup]->refocus();
    DEBUGOUT("Refocus Focus Group " << (void*)(__focus_groups[__active_focusgroup]) << " with ID: " << __active_focusgroup);
}

void
FocusManager::reset() {
    if (__focus_groups.empty() ) return;
    assert(__active_focusgroup != FocusManager::nfgid);
    assert(__focus_groups[__active_focusgroup] != 0);
    __focus_groups[__active_focusgroup]->reset();
    DEBUGOUT("Reset Focus Group " << (void*)(__focus_groups[__active_focusgroup]) << " with ID: " << __active_focusgroup);
}

FocusManager::fgid_t
FocusManager::active_focus_group() {
    return __active_focusgroup;
}
