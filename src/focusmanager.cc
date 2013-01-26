// $Id$

#include <cassert>

#include "focusmanager.h"

std::stack<FocusGroup*> FocusManager::__focus_groups;

//
// Private
//

//
// Protected
//

//
// Public
//

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

void
FocusManager::focus_next() {
    if (__focus_groups.empty()) return;

    __focus_groups.top()->focus_next();
}

void
FocusManager::focus_previous() {
    if (__focus_groups.empty()) return;

    __focus_groups.top()->focus_previous();
}
