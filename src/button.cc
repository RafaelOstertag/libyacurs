// $Id$

#include <cassert>
#include <cstdlib>

#include "button.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "colors.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

Button&
Button::operator=(const Button&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
void
Button::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!focus()) return;

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

    REALIZE_LEAVE;
}

void
Button::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));

    assert(focusgroup_id()!=(fgid_t)-1);

    Label::unrealize();

    UNREALIZE_LEAVE;
}
//
// Public
//

Button::Button(const std::string& _b) {
    can_focus(true);

    // We want our label() to be called, so we don't use the Label()
    // constructor to set the label
    label(_b);
}

Button::~Button() {
    EventQueue::disconnect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));
}

void
Button::label(const std::string& _l) {
    __tmp_label = _l;
    Label::label("[ " + _l + " ]");
}

const std::string&
Button::label() const {
    return __tmp_label;
}

void
Button::refresh(bool immediate) {
    if (realization()!=REALIZED) return;

    assert(widget_subwin()!=0);

    if (focus()) {
	color(YACURS::BUTTON_FOCUS);
	widget_subwin()->leaveok(false);
    } else {
	color(YACURS::BUTTON_NOFOCUS);
	widget_subwin()->leaveok(true);
    }

    Label::refresh(immediate);
}
