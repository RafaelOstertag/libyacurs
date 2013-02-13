// $Id$

#include <cassert>
#include <cstdlib>

#include "button.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "colors.h"
#include "cursex.h"

//
// Private
//

//
// Protected
//
void
Button::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!__focus) return;

    EventEx<int>& ekey=dynamic_cast<EventEx<int>&>(_e);

    switch (ekey.data()) {
    case KEY_DOWN:
    case KEY_RIGHT:
    case KEY_TAB:
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_UP:
    case KEY_LEFT:
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    case KEY_ENTER:
    case KEY_RETURN:
    case KEY_RETURN2:
	EventQueue::submit(EventEx<Button*>(EVT_BUTTON_PRESS,this));
	break;
    }
}

void
Button::realize() {
    REALIZE_ENTER;

    Label::realize();

    EventQueue::connect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    FocusManager::focus_group_add(focusgroup_id(), this);

    REALIZE_LEAVE;
}

void
Button::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    FocusManager::focus_group_remove(focusgroup_id(), this);

    Label::unrealize();

    UNREALIZE_LEAVE;
}


//
// Public
//

Button::Button(const std::string& _b): Label(),
				       __focus(false) {

    // We want our label() to be called, so we don't use the Label()
    // constructor to set the label
    label(_b);
}

Button::Button(const Button& _b): Label(_b), __focus(_b.__focus) {
}

Button::~Button() {
    EventQueue::disconnect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));
}

Button&
Button::operator=(const Button& _b) {
    Label::operator=(_b);

    __focus=_b.__focus;
    return *this;
}

void
Button::label(const std::string& _l) {
    Label::label("[ " + _l + " ]");
}

std::string
Button::label() const {
    std::string tmp(Label::label());

    // get rid of the square brackets
    return tmp.substr(2, tmp.length()-4);
}


bool
Button::can_focus() const {
    return true;
}

void
Button::focus(bool _f) {
    __focus = _f;

    if (realization()==REALIZED)
	refresh(true);
}

bool
Button::focus() const {
    return __focus;
}

void
Button::refresh(bool immediate) {
    if (realization()!=REALIZED) return;

    assert(widget_subwin()!=NULL);

    if (__focus) {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::BUTTON_FOCUS);
	if (leaveok(widget_subwin(), FALSE)==ERR)
	    throw LeaveOKFailed();
    } else {
	YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::BUTTON_NOFOCUS);
	if (leaveok(widget_subwin(), TRUE)==ERR)
	    throw LeaveOKFailed();
    }

    Label::refresh(immediate);
}
