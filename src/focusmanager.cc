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

#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursdbg.h"
#include "yacursex.h"

using namespace YACURS;

const FocusManager::fgid_t FocusManager::nfgid = (FocusManager::fgid_t)-1;
FocusManager::fgid_t FocusManager::_active_focusgroup = FocusManager::nfgid;
std::vector<FocusGroup*> FocusManager::_focus_groups;

//
// Private
//
void FocusManager::focus_change_handler(Event& e) {
    assert(e == EVT_FOCUS_NEXT || e == EVT_FOCUS_PREVIOUS);

    if (_focus_groups.empty()) return;
    assert(_active_focusgroup < _focus_groups.size());
    assert(_focus_groups[_active_focusgroup] != 0);
    assert(_focus_groups[_active_focusgroup]->active());

    if (e.type() == EVT_FOCUS_NEXT) {
        _focus_groups[_active_focusgroup]->focus_next();
        DEBUGOUT(DBG_FOCUSMGR, "focus_next() on " << _active_focusgroup);
    } else if (e.type() == EVT_FOCUS_PREVIOUS) {
        _focus_groups[_active_focusgroup]->focus_previous();
        DEBUGOUT(DBG_FOCUSMGR, "focus_previous() on " << _active_focusgroup);
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

void FocusManager::init() {
    DEBUGOUT(DBG_FOCUSMGR, "Initialize Focus Manager");
    EventQueue::connect_event(EventConnectorFunction1(
        EVT_FOCUS_NEXT, FocusManager::focus_change_handler));
    EventQueue::connect_event(EventConnectorFunction1(
        EVT_FOCUS_PREVIOUS, FocusManager::focus_change_handler));
}

void FocusManager::uninit() {
    DEBUGOUT(DBG_FOCUSMGR, "Uninitialize Focus Manager");
    EventQueue::disconnect_event(EventConnectorFunction1(
        EVT_FOCUS_NEXT, FocusManager::focus_change_handler));
    EventQueue::disconnect_event(EventConnectorFunction1(
        EVT_FOCUS_PREVIOUS, FocusManager::focus_change_handler));

    for (fgid_t i = 0; i < _focus_groups.size(); i++)
        if (_focus_groups[i] != 0) {
            delete _focus_groups[i];
            _focus_groups[i] = 0;
        }
}

FocusManager::fgid_t FocusManager::new_focus_group() {
    fgid_t id = FocusManager::nfgid;

    if (_focus_groups.empty()) {
        id = _focus_groups.size();  // == 0
        _focus_groups.push_back(new FocusGroup);
        DEBUGOUT(DBG_FOCUSMGR, "New Focus Group (first) "
                                   << (void*)(_focus_groups[id])
                                   << " with ID: " << id);
    } else {
        // Search for a free slot in the vector
        for (fgid_t i = 0; i < _focus_groups.size(); i++) {
            if (_focus_groups[i] == 0) {
                // found free slot
                id = i;
                _focus_groups[i] = new FocusGroup;
                DEBUGOUT(DBG_FOCUSMGR, "New Focus Group (reuse slot) "
                                           << (void*)(_focus_groups[id])
                                           << " with ID: " << id);
                break;
            }
        }

        // Check if we have found a free slot, i.e. id has been set
        if (id == FocusManager::nfgid) {
            // No, no free slot, so create new slot.
            id = _focus_groups.size();
            _focus_groups.push_back(new FocusGroup);
            DEBUGOUT(DBG_FOCUSMGR, "New Focus Group (new slot) "
                                       << (void*)(_focus_groups[id])
                                       << " with ID: " << id);
        }
    }

    assert(id != FocusManager::nfgid);
    assert(id < _focus_groups.size());
    return id;
}

void FocusManager::destroy_focus_group(fgid_t id) {
    assert(!_focus_groups.empty());
    assert(id < _focus_groups.size());

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (_focus_groups[id] == 0) return;

    // Destructor of FocusGroup is supposed to take care of removing
    // focus, so we simply destroy the group.
    delete _focus_groups[id];
    _focus_groups[id] = 0;
    DEBUGOUT(DBG_FOCUSMGR, "Destroy Focus Group " << (void*)(_focus_groups[id])
                                                  << " with ID: " << id);

    if (_active_focusgroup == id) _active_focusgroup = FocusManager::nfgid;
}

void FocusManager::focus_group_add(fgid_t id, WidgetBase* w) {
    assert(w != 0);
    assert(!_focus_groups.empty());
    assert(id < _focus_groups.size());
    assert(_focus_groups[id] != 0);

    _focus_groups[id]->add(w);

    DEBUGOUT(DBG_FOCUSMGR, "Add widget " << (void*)(w) << " to Focus Group "
                                         << (void*)(_focus_groups[id])
                                         << " with ID: " << id);
}

void FocusManager::focus_group_remove(fgid_t id, WidgetBase* w) {
    assert(w != 0);
    assert(id != FocusManager::nfgid);

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (_focus_groups[id] == 0) return;

    _focus_groups[id]->remove(w);

    DEBUGOUT(DBG_FOCUSMGR, "Remove widget " << (void*)(w) << " to Focus Group "
                                            << (void*)(_focus_groups[id])
                                            << " with ID: " << id);
}

void FocusManager::focus_group_activate(fgid_t id) {
    assert(!_focus_groups.empty());
    assert(id < _focus_groups.size());
    assert(_focus_groups[id] != 0);
    // We cannot assert this here, because fgid_t is probably of
    // unsigned type and initially set to -1, so we cannot get past
    // this assertion, since the first call which would make
    // subsequent calls pass, always fails.
    //
    // assert(_active_focusgroup<_focus_groups.size());

    // Deactivate the current Active Focus Group, if any.
    if (_active_focusgroup != FocusManager::nfgid &&  // not initialized yet
        _focus_groups[_active_focusgroup] != 0 /* Focus Group destroyed */) {
        assert(_active_focusgroup < _focus_groups.size());
        _focus_groups[_active_focusgroup]->deactivate();
        DEBUGOUT(DBG_FOCUSMGR, "Deactivated Focus Group "
                                   << (void*)(_focus_groups[_active_focusgroup])
                                   << " with ID: " << _active_focusgroup);
    }

    _focus_groups[id]->activate();
    _active_focusgroup = id;
    DEBUGOUT(DBG_FOCUSMGR, "Activated Focus Group "
                               << (void*)(_focus_groups[id])
                               << " with ID: " << id);
}

void FocusManager::refocus() {
    if (_focus_groups.empty()) return;

    assert(_active_focusgroup != FocusManager::nfgid);
    assert(_focus_groups[_active_focusgroup] != 0);
    // This would make many tests fail. Also, it should be no problem
    // if we don't assert.
    //
    // assert(!_focus_groups.empty());

    _focus_groups[_active_focusgroup]->refocus();
    DEBUGOUT(DBG_FOCUSMGR, "Refocus Focus Group "
                               << (void*)(_focus_groups[_active_focusgroup])
                               << " with ID: " << _active_focusgroup);
}

void FocusManager::reset() {
    if (_focus_groups.empty()) return;
    assert(_active_focusgroup != FocusManager::nfgid);
    assert(_focus_groups[_active_focusgroup] != 0);
    _focus_groups[_active_focusgroup]->reset();
    DEBUGOUT(DBG_FOCUSMGR, "Reset Focus Group "
                               << (void*)(_focus_groups[_active_focusgroup])
                               << " with ID: " << _active_focusgroup);
}

FocusManager::fgid_t FocusManager::active_focus_group() {
    return _active_focusgroup;
}
