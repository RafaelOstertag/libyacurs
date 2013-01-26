// $Id$

#include <cassert>

#include "focusmanager.h"
#include "cursex.h"
#include "eventqueue.h"

std::stack<FocusGroup*> FocusManager::__focus_groups;

//
// Private
//
int
FocusManager::focus_change_handler(Event& _e) {
    if (__focus_groups.empty()) return 0;

    switch (_e.type()) {
    case EVT_FOCUS_NEXT:
	__focus_groups.top()->focus_next();
	break;
    case EVT_FOCUS_PREVIOUS:
	__focus_groups.top()->focus_previous();
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

    while(!__focus_groups.empty())
	destroy_focus_group();
}

void
FocusManager::new_focus_group() {
    // If the stack is not empty, deactivate the top Focus Group
    if (!__focus_groups.empty())
	__focus_groups.top()->deactivate();

    __focus_groups.push(new FocusGroup);
    __focus_groups.top()->activate();
}

void
FocusManager::destroy_focus_group() {
    if (__focus_groups.empty()) return;
    // Destructor of FocusGroup is supposed to take care of removing
    // focus, so we simply destroy the group.
    delete __focus_groups.top();
    __focus_groups.pop();

    // If the stack is not empty, activate the top Focus Group which
    // is supposed to resume the focus it previously had..
    if (!__focus_groups.empty())
	__focus_groups.top()->activate();
}

void
FocusManager::current_focus_group_add(WidgetBase* _w) {
    assert(_w!=NULL);
    assert(!__focus_groups.empty());

    __focus_groups.top()->add(_w);
}

void
FocusManager::current_focus_group_remove(WidgetBase* _w) {
    assert(_w!=NULL);
    assert(!__focus_groups.empty());

    __focus_groups.top()->remove(_w);
}
