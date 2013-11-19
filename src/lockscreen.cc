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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"

#include <stdexcept>
#include <cassert>
#include <cstdlib>

#include "lockscreen.h"
#include "eventqueue.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//

LockScreen&
LockScreen::operator=(const LockScreen&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
void
LockScreen::key_event_handler(Event& _e) {
    assert(_e == EVT_KEY);

    if (!__unlock_dialog->shown() && __msgbox == 0) {
        __unlock_dialog->clear();
        __unlock_dialog->show();
        // Set the timeout for the unlock dialog
        EventQueue::timeout(__unlock_diag_timeout);

        // The event should not be processed any further, else the key
        // might be passed on to the Unlock Dialog. Further key
        // presses must come thru, tough.
        _e.stop(true);
    }
}

void
LockScreen::window_close_event_handler(Event& _e) {
    assert(_e == EVT_WINDOW_CLOSE);

    EventEx<WindowBase*>& _evt = dynamic_cast<EventEx<WindowBase*>&>(_e);

    if (_evt.data() == __unlock_dialog) {
        if (__unlock_dialog->unlock() ) {
            close();
        } else {
            if (__unlock_dialog->dialog_state() == DIALOG_OK) {
                assert(__msgbox == 0);
                __msgbox = new MessageBox(_("Unlock Failed"),
                                          _("Wrong Password"),
                                          OK_ONLY);
                __msgbox->show();
            }
        }
        // Set the timeout back to the timeout until lock screen kicks
        // in
        EventQueue::timeout(__timeout);
        return;
    }

    if (_evt.data() == __msgbox) {
        delete __msgbox;
        __msgbox = 0;
    }
}

//
// Public
//

LockScreen::LockScreen(UnlockDialog* _unlock,
                       unsigned int timeout,
                       unsigned int ulck_timeout) : Window(Margin::zero() ),
    __timeout(timeout),
    __unlock_diag_timeout(ulck_timeout),
    __unlock_dialog(_unlock),
    __msgbox(0) {
    if (!__unlock_dialog) throw std::invalid_argument(_(
                                                          "InputDialog may not be 0") );

}

LockScreen::~LockScreen() {
}

unsigned int
LockScreen::timeout() const {
    return __timeout;
}

unsigned int
LockScreen::unlock_dialog_timeout() const {
    return __unlock_diag_timeout;
}

void
LockScreen::close_unlock_dialog() {
    if (__unlock_dialog->shown() )
        // We clear here, since the user might have entered the
        // password but not yet closed the dialog. If we simply would
        // close without reset, the Window Close handler would kick
        // in, ask the dialog if the password is OK (which we assume
        // it is) and unlock the screen.
        __unlock_dialog->clear();
    __unlock_dialog->close();
    if (__msgbox != 0 && __msgbox->shown() )
        __msgbox->close();
}

void
LockScreen::show() {
    Window::show();

    // HotKey implementation does connect this event.
    // EventQueue::connect_event(EventConnectorMethod1<LockScreen>(EVT_KEY, this, &LockScreen::event_key_handler));
    EventQueue::connect_event(EventConnectorMethod1<LockScreen>(
                                  EVT_WINDOW_CLOSE, this,
                                  &LockScreen::window_close_event_handler) );

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(
                                   EVT_FORCEREFRESH, this,
                                   &WindowBase::force_refresh_handler) );

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(
                                   EVT_REDRAW, this,
                                   &WindowBase::redraw_handler) );

    EventQueue::suspend_except(EventConnectorMethod1<LockScreen>(EVT_REFRESH,
                                                                 this,
                                                                 &WindowBase::
                                                                 refresh_handler) );
    // We don't suspend this handler, since the resize should be
    // handled by windows, but not displayed. Else we would have to
    // keep track of resizes and call resize handlers upon close() of
    // LockScreen.
    //
    // EventQueue::connect_event(EventConnectorMethod1<WindowBase>(EVT_SIGWINCH,this,&WindowBase::resize_handler));
}

void
LockScreen::close() {
    // HotKey implementation disconnects this event.
    // EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(EVT_KEY, this, &LockScreen::event_key_handler));
    EventQueue::disconnect_event(EventConnectorMethod1<LockScreen>(
                                     EVT_WINDOW_CLOSE, this,
                                     &LockScreen::window_close_event_handler) );
    EventQueue::unsuspend_all(EVT_FORCEREFRESH);
    EventQueue::unsuspend_all(EVT_REDRAW);
    EventQueue::unsuspend_all(EVT_REFRESH);

    Window::close();
}
