// $Id$

#include <cassert>
#include <cstdlib>

#include <button.h>
#include <eventqueue.h>
#include <focusmanager.h>

//
// Private
//
void
Button::visibly_change_focus() {
#warning "Not implemented"
}
//
// Protected
//
int
Button::key_handler(Event& _e) {
    assert(_e.type()==EVT_KEY);

    if (!__focus) return 0;

    EventKey& ekey=dynamic_cast<EventKey&>(_e);

    switch (ekey.data()) {
    case KEY_DOWN:
    case KEY_RIGHT:
    case '\t':
	EventQueue::submit(EVT_FOCUS_NEXT);
	break;
    case KEY_UP:
    case KEY_LEFT:
    case KEY_BTAB:
	EventQueue::submit(EVT_FOCUS_PREVIOUS);
	break;
    case KEY_ENTER:
    case '\r':
	on_press();
	break;
    }

    return 0;
}

void
Button::realize() {
    Label::realize();

    FocusManager::current_focus_group_add(this);
}

void
Button::unrealize() {
    FocusManager::current_focus_group_remove(this);

    Label::unrealize();
}


//
// Public
//

Button::Button(const std::string& _b): Label(_b),
				       __focus(false) {
    EventQueue::connect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));
}

Button::Button(const Button& _b): Label(_b), __focus(_b.__focus) {
    EventQueue::connect_event(EventConnectorMethod1<Button>(EVT_KEY,this, &Button::key_handler));
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
		 

bool
Button::can_focus() const {
    return true;
}

void
Button::focus(bool _f) {
    __focus = _f;

    if (realized())
	visibly_change_focus();
}

bool
Button::focus() const {
    return __focus;
}
