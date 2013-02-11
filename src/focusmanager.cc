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
int
FocusManager::focus_change_handler(Event& _e) {
    if (__focus_groups.empty()) return 0;
    assert(__active_focusgroup<__focus_groups.size());
    assert(__focus_groups[__active_focusgroup]!=NULL);
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

    return 0;
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
	if (__focus_groups[i]!=NULL) {
	    delete __focus_groups[i];
	    __focus_groups[i]=NULL;
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
	    if (__focus_groups[i]==NULL) {
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
    assert(__focus_groups[_id]!=NULL);

    // Destructor of FocusGroup is supposed to take care of removing
    // focus, so we simply destroy the group.
    delete __focus_groups[_id];
    __focus_groups[_id] = NULL;

    // Now, we assume that when the Focus Group has been destroyed, a
    // Window just disappeared, so a refresh will take place
    // soon. Hence we set the Active Focus Group to -1, which should
    // be changed as soon as the Window(s) do a refresh.
    __active_focusgroup=(fgid_t)-1;
}

void
FocusManager::focus_group_add(fgid_t _id, WidgetBase* _w) {
    assert(_w!=NULL);
    assert(!__focus_groups.empty());
    assert(_id < __focus_groups.size());
    assert(__focus_groups[_id]!=NULL);

    __focus_groups[_id]->add(_w);
}

void
FocusManager::focus_group_remove(fgid_t _id, WidgetBase* _w) {
    assert(_w!=NULL);
    assert(!__focus_groups.empty());
    assert(_id < __focus_groups.size());
    assert(__focus_groups[_id]!=NULL);

    __focus_groups[_id]->remove(_w);
}

void
FocusManager::focus_group_activate(fgid_t _id) {
    assert(!__focus_groups.empty());
    assert(_id<__focus_groups.size());
    assert(__focus_groups[_id]!=NULL);
    // We cannot assert this here, because fgid_t is probably of
    // unsigned type and initially set to -1, so we cannot get past
    // this assertion, since the first call which would make
    // subsequent calls pass, always fails.
    //
    //assert(__active_focusgroup<__focus_groups.size());
    
    // Deactivate the current Active Focus Group, if any.
    if (__active_focusgroup!=(fgid_t)-1 && // not initialized yet
	__focus_groups[__active_focusgroup]!=NULL /* Focus Group destroyed */ ) {
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
    // This would make many tests fail. Also, it should be no problem
    // if we don't assert.
    //
    //assert(!__focus_groups.empty());

    __focus_groups[__active_focusgroup]->refocus();
}
