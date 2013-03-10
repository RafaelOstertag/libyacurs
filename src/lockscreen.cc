// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <cassert>
#include <cstdlib>


#include "lockscreen.h"
#include "eventqueue.h"

//
// Private
//

LockScreen&
LockScreen::operator=(const LockScreen&) {
    abort();
    return *this;
}

//
// Protected
//
void
LockScreen::event_key_handler(Event& _e) {
    assert(_e==EVT_KEY);

    if (!__unlock_dialog->shown()) {
	__unlock_dialog->clear();
	__unlock_dialog->show();
    }
}

void
LockScreen::event_window_close_handler(Event& _e) {
    assert(_e==EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (_evt.data() == __unlock_dialog) {
	if (__unlock_dialog->unlock()) {
	    close();
	} else {
	    if (__unlock_dialog->dialog_state() == Dialog::DIALOG_OK) {
		assert(__msgbox==NULL);
		__msgbox=new MessageBox("Unlock Failed",
					"Wrong Password",
					Dialog::OK_ONLY);
		__msgbox->show();
	    }
	}
	return;
    }

    if (_evt.data() == __msgbox) {
	delete __msgbox;
	__msgbox=NULL;
    }
}



//
// Public
//

LockScreen::LockScreen(UnlockDialog* _unlock): Window(Margin::zero()),
					       __unlock_dialog(_unlock),
					       __msgbox(NULL) {
    if (!__unlock_dialog) throw std::invalid_argument("InputDialog may not be NULL");
}

LockScreen::~LockScreen() {
}

void
LockScreen::show() {
    Window::show();

    EventQueue::connect_event(EventConnectorMethod1<LockScreen>(EVT_KEY, this, &LockScreen::event_key_handler));
    EventQueue::connect_event(EventConnectorMethod1<LockScreen>(EVT_WINDOW_CLOSE, this, &LockScreen::event_window_close_handler));

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(EVT_FORCEREFRESH,this, &WindowBase::force_refresh_handler));
    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(EVT_REFRESH,this, &WindowBase::refresh_handler));
    // We don't suspend this handler, since the resize should be
    // handled by windows, but not displayed. Else we would have to
    // keep track of resizes and call resize handlers upon close() of
    // LockScreen.
    //
    // EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this,&WindowBase::resize_handler));
}

void
LockScreen::close() {
    EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(EVT_KEY, this, &LockScreen::event_key_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(EVT_WINDOW_CLOSE, this, &LockScreen::event_window_close_handler));
    EventQueue::unsuspend_all(EVT_FORCEREFRESH);
    EventQueue::unsuspend_all(EVT_REFRESH);

    Window::close();
}