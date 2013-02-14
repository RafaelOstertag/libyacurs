// $Id$

#include <cassert>

#include "dialog.h"

#include "eventqueue.h"

//
// Private
//

//
// Protected
//
void
Dialog::button_press_handler(Event& _e) {
    if (realization()!=REALIZED) return;

    assert(_e==EVT_BUTTON_PRESS);
    EventEx<Button*>& evt = dynamic_cast<EventEx<Button*>&>(_e);

    assert(__bok!=NULL);

    if (evt.data()==__bok) {
	__dstate=DIALOG_OK;
	close();
	return;
    }

    if (evt.data()==__bcancel) {
	__dstate=DIALOG_CANCEL;
	close();
	return;
    }
}

void
Dialog::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Window::unrealize();

    UNREALIZE_LEAVE;
}

//
// Public
//

Dialog::Dialog(const std::string& _title, bool _ok_only): Window(),
							 __vpack(NULL),
							 __hpack(NULL),
							 __bok(NULL),
							 __bcancel(NULL),
							 __dstate(DIALOG_CANCEL),
							 __ok_only(_ok_only),
							 __title(_title) {
    __vpack=new VPack;
    __hpack=new HPack;
    __bok=new Button("OK");
    if (!__ok_only)
	__bcancel=new Button("Cancel");

    __vpack->add_back(__hpack);
    __hpack->add_back(__bok);

    if (!__ok_only)
	__hpack->add_back(__bcancel);

    // from Window
    widget(__vpack);
    frame(true);
}

Dialog::Dialog(const Dialog& _dialog): Window(_dialog),
				       __vpack(_dialog.__vpack),
				       __hpack(_dialog.__hpack),
				       __bok(_dialog.__bok),
				       __bcancel(_dialog.__bcancel),
				       __dstate(_dialog.__dstate),
				       __ok_only(_dialog.__ok_only) {
    EventQueue::connect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));
}

Dialog::~Dialog() {
    assert(__vpack!=NULL);
    assert(__hpack!=NULL);
    assert(__bok!=NULL);

    delete __vpack;
    delete __hpack;
    delete __bok;

    if (__ok_only) {
	assert(__bcancel!=NULL);
	delete __bcancel;
    }

    EventQueue::disconnect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));
}

Dialog&
Dialog::operator=(const Dialog& _dialog) {
    Window::operator=(_dialog);
    
    __vpack = _dialog.__vpack;
    __hpack = _dialog.__hpack;
    __bok = _dialog.__bok;
    __bcancel = _dialog.__bcancel;
    __dstate = _dialog.__dstate;
    __ok_only = _dialog.__ok_only;

    return *this;
}

void
Dialog::refresh(bool immediate) {
    if (realization()!=REALIZED &&
	realization()!=REALIZING) return;

    mvwaddstr(curses_window(), 0, 1, __title.c_str());

    Window::refresh(immediate);
}

void
Dialog::realize() {
    REALIZE_ENTER;

    EventQueue::connect_event(EventConnectorMethod1<Dialog>(EVT_BUTTON_PRESS, this, &Dialog::button_press_handler));

    Window::realize();
    REALIZE_LEAVE;
}

