// $Id$

#include <cassert>

#include "focusmanager.h"
#include "cursex.h"
#include "eventqueue.h"

fgid_t FocusManager::__active_focusgroup=(fgid_t)-1;
std::vector<FocusGroup*> FocusManager::__focus_groups;

//
// Private
//
void
FocusManager::focus_change_handler(Event& _e) {
    if (__focus_groups.empty()) return;
    assert(__active_focusgroup<__focus_groups.size());
    assert(__focus_groups[__active_focusgroup]!=0);
    assert(__focus_groups[__active_focusgroup]->active());

    switch (_e.type()) {
    case EVT_FOCUS_NEXT:
	__focus_groups[__active_focusgroup]->focus_next();
	break;
    case EVT_FOCUS_PREVIOUS:
	__focus_groups[__active_focusgroup]->focus_previous();
	break;
    default:
	throw UnexpectedEvent();
    };
}

//
// Protected
//

//
// Public
//

void
FocusManager::init() {
    EventQueue::connect_event(EventConnectorFunction1(EVT_FOCUS_NEXT,FocusManager::focus_change_handler));
    EventQueue::connect_event(EventConnectorFunction1(EVT_FOCUS_PREVIOUS,FocusManager::focus_change_handler));
}

void
FocusManager::uninit() {
    EventQueue::disconnect_event(EventConnectorFunction1(EVT_FOCUS_NEXT,FocusManager::focus_change_handler));
    EventQueue::disconnect_event(EventConnectorFunction1(EVT_FOCUS_PREVIOUS,FocusManager::focus_change_handler));

    for(fgid_t i=0; i<__focus_groups.size(); i++)
	if (__focus_groups[i]!=0) {
	    delete __focus_groups[i];
	    __focus_groups[i]=0;
	}
}

fgid_t
FocusManager::new_focus_group() {
    fgid_t _id = (fgid_t)-1;

    if (__focus_groups.empty()) {
	_id=__focus_groups.size(); // == 0
	__focus_groups.push_back(new FocusGroup);
    } else {
	// Search for a free slot in the vector
	for (fgid_t i=0; i<__focus_groups.size(); i++) {
	    if (__focus_groups[i]==0) {
		// found free slot
		_id=i;
		__focus_groups[i]=new FocusGroup;
		break;
	    }
	}

	// Check if we have found a free slot, i.e. _id has been set
	if (_id==(fgid_t)-1) {
	    // No, no free slot, so create new slot.
	    _id=__focus_groups.size();
	    __focus_groups.push_back(new FocusGroup);
	}
    }	

    assert(_id!=(fgid_t)-1);
    assert(_id<__focus_groups.size());
    return _id;
}

void
FocusManager::destroy_focus_group(fgid_t _id) {
    assert(!__focus_groups.empty());
    assert(_id < __focus_groups.size());

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (__focus_groups[_id]==0)
	return;

    // Destructor of FocusGroup is supposed to take care of removing
    // focus, so we simply destroy the group.
    delete __focus_groups[_id];
    __focus_groups[_id] = 0;

    if (__active_focusgroup==_id)
	__active_focusgroup=(fgid_t)-1;
}

void
FocusManager::focus_group_add(fgid_t _id, WidgetBase* _w) {
    assert(_w!=0);
    assert(!__focus_groups.empty());
    assert(_id < __focus_groups.size());
    assert(__focus_groups[_id]!=0);

    __focus_groups[_id]->add(_w);
}

void
FocusManager::focus_group_remove(fgid_t _id, WidgetBase* _w) {
    assert(_w!=0);
    assert(_id!=(fgid_t)-1);

    // check if the FocusGroup still exists.
    //
    // It is possible that the Focus Groups have been destroyed by a
    // call to ::uninit() due to termination of the event loop, but
    // destruction of Windows were still not done. See the test
    // 'listbox2.cc' for an example of this bevavior
    if (__focus_groups[_id]==0)
	return;

    __focus_groups[_id]->remove(_w);
}

void
FocusManager::focus_group_activate(fgid_t _id) {
    assert(!__focus_groups.empty());
    assert(_id<__focus_groups.size());
    assert(__focus_groups[_id]!=0);
    // We cannot assert this here, because fgid_t is probably of
    // unsigned type and initially set to -1, so we cannot get past
    // this assertion, since the first call which would make
    // subsequent calls pass, always fails.
    //
    //assert(__active_focusgroup<__focus_groups.size());
    
    // Deactivate the current Active Focus Group, if any.
    if (__active_focusgroup!=(fgid_t)-1 && // not initialized yet
	__focus_groups[__active_focusgroup]!=0 /* Focus Group destroyed */ ) {
	assert(__active_focusgroup<__focus_groups.size());
	__focus_groups[__active_focusgroup]->deactivate();
    }

    __focus_groups[_id]->activate();
    __active_focusgroup=_id;
}

void
FocusManager::refocus() {
    if (__focus_groups.empty()) return;

    assert(__active_focusgroup!=(fgid_t)-1);
    assert(__focus_groups[__active_focusgroup]!=0);
    // This would make many tests fail. Also, it should be no problem
    // if we don't assert.
    //
    //assert(!__focus_groups.empty());

    __focus_groups[__active_focusgroup]->refocus();
}
